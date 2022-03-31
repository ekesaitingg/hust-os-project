#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>


#define OK 0
#define PARA_ERROR -1
#define OPEN_ERROR -2
#define CREAT_ERROR -3
int main(int argc, char *argv[])
{
    if(argc!=3)
    {
        printf("参数错误\n");
        return PARA_ERROR;
    }
    char copy_buf[1024];
    int fd1, fd2;
    if ((fd1 = open(argv[1], O_RDONLY, 0)) < 0)
    {
	    perror("open fd1 error");
        return OPEN_ERROR;   
    }
    if ((fd2 = open(argv[2], O_RDWR | O_CREAT , 0644)) < 0)
    {
	    perror("create fd2 error");     
	    return CREAT_ERROR; 
    }

    memset(copy_buf, 0, sizeof(copy_buf));
    int len = 0;
    while (1)
    {
        len = read(fd1, copy_buf, 1024);
        if (len <= 0) break;
        write(fd2, copy_buf, len);
        memset(copy_buf, 0, sizeof(copy_buf));
    }
    close(fd1);
    close(fd2);
    printf("复制完成\n");
    return 0;
}
