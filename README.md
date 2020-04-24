# linux_command
2020年 《Linux程序设计》

---
# 使用前 ：
```
cd [.c file path]
gcc -o ls ./myls.c
gcc -o wc ./mywc.c
```

# 使用命令的格式：
## ls命令 :
```
./ls [-options]
```
* 可用的option : -l, -i, -a, -d 或者没有 (옵션으로 한개든 여러개든 l,i,a,d 순서 상관없이 선택 가능함)
* 多个option的循序随便都可以
* 输出的内容是根据从目前编译之后的ls文件位置开始的 (컴파일한 ls파일 현재 위치 기준으로 ls결과가 출력됨 )

## wc命令 :
```
./wc [filename]
```
---
### myls.c的设计：
![mylsDiagram](https://user-images.githubusercontent.com/44460142/80183518-ee781780-8643-11ea-8cbd-346318fc2ffa.jpg)

### myls.c的结果对比 ：
<img width="666" alt="ls_result_" src="https://user-images.githubusercontent.com/44460142/80183520-ef10ae00-8643-11ea-9aae-92dfb1a6674a.png">

<img width="953" alt="ls_result_2" src="https://user-images.githubusercontent.com/44460142/80183515-ec15bd80-8643-11ea-8323-342a3fe241ce.png">

### mywc.c的设计：
![mywc](https://user-images.githubusercontent.com/44460142/80183508-ea4bfa00-8643-11ea-9b93-77a82086a98e.jpg)

### mywc.c的结果对比：
<img width="710" alt="wc_result" src="https://user-images.githubusercontent.com/44460142/80183528-f20b9e80-8643-11ea-9df4-5afbdb503b5a.png">
