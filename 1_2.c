#include <gtk/gtk.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void)
{
    int pid1,pid2,pid3;
    if((pid1 = fork()) == 0)
    {
        execv("./1_2_time",NULL);
    }
    else if((pid2 = fork()) == 0)
    {
        execv("./1_2_cpu",NULL);
    }
    else
    {
        execv("./1_2_sum",NULL);
    }
    
}