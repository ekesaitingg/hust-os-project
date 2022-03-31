#include <unistd.h>
#include<fcntl.h>
#include <stdio.h>

char buf_read[1024];
char buf_write[1024];
char name[40]="/dev/mydriver";//设备名
int main()
{
    int file = open(name,O_RDWR|O_NONBLOCK);//打开文件
    if(file == -1)
    {
        printf("open failed!\n");
        return -1;
    }
    else
    {
        read(file, buf_read, sizeof(buf_read));//读取设备初始信息
        printf("缓冲区中已有字符串:\n%s\n",buf_read);

        printf("请输入一个字符串:\n");
        scanf("%[^\n]",buf_write);
        write(file, buf_write, sizeof(buf_write));//写入信息

        read(file, buf_read, sizeof(buf_read));//读取设备信息
        printf("输入的字符串为:\n%s\n",buf_read);

        close(file);//关闭文件
        return 0;
    }
}
