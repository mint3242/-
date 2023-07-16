// 打印2-35间的素数
#include "kernel/types.h"
#include "kernel/stat.h"
#include "stddef.h"
#include "user/user.h"

void mapping(int n,int pd[])
{
    //重定向函数
    close(n);//关闭文件描述符n
    dup(pd[n]);//将管道的读/写端口复制给文件描述符n
    close(pd[0]);
    close(pd[1]);//将管道端口关闭
}

void primes()
{
    //递归函数 
    int fd[2];
    pipe(fd);//创建管道
    //读入第一个数字
    int num, base;
    if(read(0,&base,sizeof(int))==0){
        return;
    }
    //打印第一个数字
    printf("prime %d\n",base);
    if(fork()==0){
        //子进程
        mapping(1,fd);//写端口重定向
        //筛选写入数字
        while(read(0,&num,sizeof(int))){
        if(num%base){
            write(1,&num,sizeof(int));
        }
    }
    }
    else{
        //父进程
        mapping(0,fd);//读端口重定向
        wait(NULL);//等待子进程退出
        primes();
    }
}

int
main(int argc, char *argv[])
{
    //检查程序参数
    if(argc != 1){
    fprintf(2, "Usage: primes\n");//参数个数不等于2
    exit(1);
    }
    //创建管道
    int fd[2];
    pipe(fd);
    if(fork()==0){
        //子进程
        mapping(1,fd);//写端口重定向
        //将2-35写入写端口
        int i=2;
        for(i=2;i<=35;i++){
            write(1,&i,sizeof(int));
        }
    }
    else{
        //父进程
        mapping(0,fd);//读端口重定向
        wait(NULL);//等待子进程退出
        primes();
    }
 
    exit(0);
}