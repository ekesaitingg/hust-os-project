#include <stdio.h>
#include <stdlib.h>
int main(void)
{
    int i=syscall(333,"input.txt","output.txt");
    if(i==1)
    {
        printf("process finished\n");
        system("md5sum input.txt && md5sum output.txt");
    }
    else printf("syscall failed\n");
    return 0;
}