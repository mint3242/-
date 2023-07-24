//从标准输入中读取行并为每一行运行一个命令，将该行作为命令的参数提供
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"
#define MAXN 1024


int
main(int argc, char *argv[])
{

    if(argc < 2){
        fprintf(2, "Usage: xargs command\n");//参数个数小于2
        exit(1);
    }
    char * argvs[MAXARG];
    int index=0;
    for (int i = 1; i < argc; ++i) {
        argvs[index++] = argv[i];//保存命令行参数
    }
    //设立缓冲区，存放从管道读出的数据
    char buf[MAXN] = {"\0"};
    int stat = 1;   // 从标准输入read返回的状态
    char c=0;
    while(stat){
        int buf_n=0,arg_begin=0;
        index=argc-1;
        //读取一行
        while(1) {
            stat = read(0, &c, 1); //从标准输入中读取.0代表管道，从管道读取数据
            if (stat == 0){ 
                exit(0);// 标准输入中没有数据，exit
            }
            else if (c==' '||c=='\n'){
                buf[buf_n++] = 0; //最后一个参数必需为0，否则会执行失败
                argvs[index++] = &buf[arg_begin];
                arg_begin = buf_n; 
                if (c == '\n')
                    break;
            }
            else{
                buf[buf_n++] = c;
            }
        }
        argvs[index] = 0; //结束标志
        //创建子线程执行命令
        if (fork() == 0) {
            exec(argv[1], argvs);
        }
        else {
            wait(0); // 等待子线程执行完毕
        }
    }
    
  exit(0);
}
