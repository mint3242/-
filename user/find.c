//查找指定目录树下的所有重名文件

#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

void
find(char *dir, char *file)
{
  char buf[512], *p;
  int fd;//文件描述符
  struct dirent de;
  struct stat st;

  if((fd = open(dir, 0)) < 0){
    fprintf(2, "find: cannot open %s\n", dir);
    return;
  }

  if(fstat(fd, &st) < 0){
    fprintf(2, "find: cannot stat %s\n", dir);
    close(fd);
    return;
  }

  if (st.type != T_DIR)
    {
        fprintf(2, "find: %s is not a directory\n", dir);//报类型不是目录错误
        close(fd);//关闭文件描述符 fd
        return;
    }

  //设置缓冲区文件路径
  if(strlen(dir) + 1 + DIRSIZ + 1 > sizeof buf){
      fprintf(2, "find: directory too long\n");
      close(fd);
      return;
    }
  strcpy(buf, dir);
  p = buf+strlen(buf);
  *p++ = '/';

  //读取路径内文件
  while(read(fd, &de, sizeof(de)) == sizeof(de)){
    if(de.inum == 0){
      continue;
    }
    if(strcmp(de.name,".")==0||strcmp(de.name,"..")==0){
        continue;
    }
    memmove(p, de.name, DIRSIZ);
    p[DIRSIZ] = 0;//文件名结束符
    if(stat(buf, &st) < 0){
      fprintf(2, "find: cannot stat %s\n", buf);
      continue;
    }
    if(st.type==T_DIR){
      find(buf,file);
    }
    else if(st.type==T_FILE&&strcmp(de.name,file)==0){
      printf("%s\n",buf);
    }
  }
  close(fd);
  return;
}
  

int
main(int argc, char *argv[])
{

  if(argc != 3){
    fprintf(2, "Usage: find path filename\n");//参数个数小于2
    exit(1);
  }
  find(argv[1],argv[2]);

  exit(0);
}
