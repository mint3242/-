#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
    int i;

    if(argc < 2){
    fprintf(2, "Usage: sleep n(ticks)\n");//参数个数小于2
    exit(1);
    }
    int sleep_time=0;
    //如果有多个参数且都为数字，则睡眠时间为时间总和
    for(i = 1; i < argc; i++){
        int n=atoi(argv[i]);
        if(n<0){
            fprintf(2, "sleep: invalid time interval '%s'\n",argv[i]);//参数类型错误
            exit(1);
        }
        sleep_time+=n;
    }
    sleep(sleep_time);//调用sleep函数
    exit(0);
}