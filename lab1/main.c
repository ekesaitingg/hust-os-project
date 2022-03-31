#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>
int main(void)
{
    pid_t p1, p2, p3;
    if((p1=fork())==0)
    {
        execv("./process1",NULL);
        exit(0);
    }
    else if((p2=fork())==0)
    {
        execv("./process2",NULL);
        exit(0);
    }
    else if((p3=fork())==0)
    {
        execv("./process3",NULL);
        exit(0);
    }
    return 0;
}
