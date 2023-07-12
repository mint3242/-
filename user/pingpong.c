#include "kernel/types.h"
#include "kernel/stat.h"
#include "stddef.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
    //检查程序参数
    if(argc != 1){
    fprintf(2, "Usage: pingpong\n");//参数个数不等于2
    exit(1);
    }
    int fds1[2],fds2[2];
    char buf[8];//缓冲区
    //创建管道
    pipe(fds1);
    pipe(fds2);
    int pid=fork();
    //创建子进程
    if(pid<0){
        printf("fork failed\n");
        exit(1);
    }
    else if(pid==0){
        //子进程
        read(fds1[0],buf,4);
        printf("%d: received %s\n", getpid(), buf);
        write(fds2[1], "pong", strlen("pong"));
    }
    else{
        //父进程
        write(fds1[1], "ping", strlen("ping"));
        wait(NULL);//等待子进程结束
        read(fds2[0],buf,4);
        printf("%d: received %s\n", getpid(), buf);
        
    }
    exit(0);
}