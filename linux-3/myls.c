//
// Created by jayson on 20. 4. 21..
//
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>


#define FILE_NAME_MAX 40

typedef struct file_Info {
    char permission[11];
    int link_count;
    char user_id[20];
    char group_id[20];
    int size;
    int date_n_time[4];
    char file_name[255];
    int inode_num;
}file_Info;


char *checkFileMode(struct stat *buf){
    char *permission = malloc(1024);
    //is dir or file?
    if(S_ISDIR(buf->st_mode))
        strcat(permission, "d");
    else if(S_ISLNK(buf->st_mode))
        strcat(permission, "l");
    else if(S_ISCHR(buf->st_mode))
        strcat(permission, "c");
    else if(S_ISBLK(buf->st_mode))
        strcat(permission, "b");
    else if(S_ISSOCK(buf->st_mode))
        strcat(permission, "s");
    else if(S_ISFIFO(buf->st_mode))
        strcat(permission, "P");
    else
        strcat(permission, "-");

    //user auth
    if(buf->st_mode & S_IRUSR)
        strcat(permission, "r");
    else
        strcat(permission, "-");
    if(buf->st_mode & S_IWUSR)
        strcat(permission, "w");
    else
        strcat(permission, "-");
    if(buf->st_mode & S_IXUSR)
        strcat(permission, "x");
    else if(buf->st_mode & S_ISUID)
        strcat(permission, "s");
    else
        strcat(permission, "-");

    //group auth
    if(buf->st_mode & S_IRGRP)
        strcat(permission, "r");
    else
        strcat(permission, "-");
    if(buf->st_mode & S_IWGRP)
        strcat(permission, "w");
    else
        strcat(permission, "-");
    if(buf->st_mode & S_IXGRP)
        strcat(permission, "x");
    else if(buf->st_mode & S_ISGID)
        strcat(permission, "s");
    else
        strcat(permission, "-");

    //user
    if(buf->st_mode & S_IROTH)
        strcat(permission, "r");
    else
        strcat(permission, "-");
    if(buf->st_mode & S_IWOTH)
        strcat(permission, "w");
    else
        strcat(permission, "-");

    if(buf->st_mode & S_IXOTH) //stiky bit
    {
        if(buf->st_mode & S_ISVTX)
            strcat(permission, "t");
        else
            strcat(permission, "x");
    }
    else
    {
        if(buf->st_mode & S_ISVTX)
            strcat(permission, "T");
        else
            strcat(permission, "-");
    }

    return permission;
}

void print_l_option(file_Info *info, int i){
    if(i==1){
        printf("%d %s %d %s %s %d  %d月 %d日 %d:%d %s\n",
                info->inode_num,
                info->permission,
                info->link_count,
                info->user_id,
                info->group_id,
                info->size,
                info->date_n_time[0], info->date_n_time[1], info->date_n_time[2], info->date_n_time[3],
                info->file_name);
    }else{
        printf("%s %d %s %s %d  %d月 %d日 %d:%d %s\n",
               info->permission,
               info->link_count,
               info->user_id,
               info->group_id,
               info->size,
               info->date_n_time[0], info->date_n_time[1], info->date_n_time[2], info->date_n_time[3],
               info->file_name);
    }

}

void stat2fileInfo(file_Info* info, struct stat buf, int i){
    struct group *grp;
    struct passwd *pwd;
    struct tm *time;
    char *permission;
    char *usr_name;
    char *grp_name;

    permission = checkFileMode(&buf);
    pwd = getpwuid(buf.st_uid);
    grp = getgrgid(buf.st_gid);
    time = localtime(&buf.st_mtime);
    usr_name = pwd->pw_name;
    grp_name = grp->gr_name;

    info->date_n_time[0] = (time->tm_mon)+1;
    info->date_n_time[1] = time->tm_mday;
    info->date_n_time[2] = time->tm_hour;
    info->date_n_time[3] = time->tm_min;
    strcpy(info->user_id, pwd->pw_name);
    strcpy(info->group_id, grp->gr_name);
    strcpy(info->permission, permission);
    info->size = buf.st_size;
    info->link_count = buf.st_nlink;

    if(i==1){
        info->inode_num = buf.st_ino;
    }

    print_l_option(info, i);
    free(permission);
    free(info);
}

void do_l_option(char *current_dir, int a, int i, int d){
    int is_no_Option = a+i+d;
    file_Info *info;
    struct dirent *item;
    struct stat buf;
    DIR *dp;
    char filename[FILE_NAME_MAX];

    if(d==1){
        if(lstat(".", &buf)==0){
            info = (file_Info*)malloc(sizeof(file_Info));
            strcpy(info->file_name, ".");
            stat2fileInfo(info, buf, i);
            exit(0);
        }
    }else{
        dp = opendir(current_dir);
        while((item=readdir(dp))!=NULL){
            strcpy(filename, current_dir);
            strcat(filename, item->d_name);

            if(lstat(filename, &buf)==0){
                if(a==0){
                    char *check_file_name = item->d_name;
                    if(check_file_name[0]=='.'){
                        continue;
                    }
                }

                info = (file_Info*)malloc(sizeof(file_Info));
                strcpy(info->file_name, item->d_name);
                stat2fileInfo(info, buf, i);
            }
        }
        closedir(dp);
    }


}

void do_non_l_option(char *current_dir, int a, int i, int d){
    file_Info *info;
    struct dirent *item;
    struct stat buf;
    DIR *dp;
    char filename[FILE_NAME_MAX];

    if(d==1){
        if(lstat(".", &buf)==0){
            printf(".\n");
            if(i==1){
              info = (file_Info *) malloc(sizeof(file_Info));
              info->inode_num = buf.st_ino;
              printf("%d .\n", info->inode_num);
            }
            exit(0);
        }
    }else {
        dp = opendir(current_dir);
        while ((item = readdir(dp)) != NULL) {
            strcpy(filename, current_dir);
            strcat(filename, item->d_name);

            if (lstat(filename, &buf) == 0) {
                if (a == 0) {
                    char *check_file_name = item->d_name;
                    if (check_file_name[0] == '.') {
                        continue;
                    }
                }

                info = (file_Info *) malloc(sizeof(file_Info));
                strcpy(info->file_name, item->d_name);

                if (i == 1) {
                    info->inode_num = buf.st_ino;
                    printf("%d %s ", info->inode_num, info->file_name);
                } else {
                    printf("%s ", info->file_name);
                }
            }

        }
        closedir(dp);
        printf("\n");
    }

}


int main(int argc, char *argv[]){
    int option_Index;
    int l=0, d=0, R=0, a=0, i=0;

    if(argc==1){
        do_non_l_option("./", a, i, d);
        exit(0);
    }else if(argv[1][0] == '-'){
        for(option_Index=1; option_Index<strlen(argv[1]); option_Index++){
            if(argv[1][option_Index] == 'l'){
                l=1;
            }else if(argv[1][option_Index] == 'd'){
                d=1;
            }else if(argv[1][option_Index] == 'R'){
                R=1;
            }else if(argv[1][option_Index] == 'a'){
                a=1;
            }else if(argv[1][option_Index] == 'i'){
                i=1;
            }
        }
    }

    if(l==0){
        do_non_l_option("./", a, i, d);
    }else{
        do_l_option("./", a, i, d);
    }


    exit(0);
}
