//
// Created by jayson on 20. 4. 23..
//
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>


#define FILE_NAME_MAX 40


void do_wc(char *file_dir){
    FILE *file = fopen(file_dir, "r");
    struct stat buf;
    char ch, prev_ch = ' ';
    int file_line=0, file_word=0, file_char=0;

    if(lstat(file_dir, &buf)==0){
        if(S_ISDIR(buf.st_mode)){
            printf("./wc: %s: Is a directory\n",file_dir);
            printf("\t0\t0\t0 %s\n",file_dir);
            exit(0);
        }
    }else{
        printf("./wc: %s: No such file or directory\n",file_dir);
        exit(0);
    }

    while((ch=fgetc(file))!=EOF){
        if(ch=='\n' || file_line==0){
            file_line++;
        }
        if((prev_ch=='\n' || prev_ch=='\t' || prev_ch==' ') && !(ch=='\n' || ch=='\t' || ch==' ')){
            file_word++;
        }
        file_char++;

        prev_ch=ch;
    }

    printf(" %d %d %d %s\n", file_line-1, file_word, file_char, file_dir);
    fclose(file);
}

int main(int argc, char *argv[]){
    char file_dir[FILE_NAME_MAX];

    if(argc==1){
        perror("Command Incorrect : ./wc [filename]");
        exit(2);
    }else{
        strcpy(file_dir, argv[1]);
    }

    do_wc(file_dir);
    exit(0);
}
