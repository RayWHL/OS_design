#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
// #include <time.h>
#include <sys/time.h>

#include <dirent.h>
#include <pwd.h>
 
#define READ_BUFF 10



int copy_file(char *PATH1,char *PATH2)
{
    int fp1, fp2;
    int c = 0;
    char buf[READ_BUFF] = {0};

    //打开读取的文件
    if ((fp1 = open(PATH1, O_RDONLY)) == -1) {
       printf("open fail\n");
       return -1;
    }

    //打开写入的文件，没有则创建
    if ((fp2 = open(PATH2, O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR)) == -1) {
       printf("create fail\n");
       return -1;
    }

    //循环复制，一次读入READ_BUFF
    while((c = read(fp1, buf, sizeof(buf))) > 0) 
    {
         int WriteRes = write(fp2, buf, c);
         if (WriteRes != c)
         {
            printf("write error\n");
            return -1;
         }    
    }

    //关闭文件
    if(close(fp1) != 0) {
       puts("close fp1 fail");
    }

    //关闭文件
    if(close(fp2) != 0) {
      puts("close fp2 fail");
    }
}

void copy_dir(char *PATH1,char *PATH2)
{
	if(access(PATH2,0)==-1)
		mkdir(PATH2,0755);
    DIR *dp;
    struct dirent *entry;
    struct stat statbuf;
       //打开目录
    if((dp = opendir(PATH1)) ==NULL)
    {
        printf("fail to open the dir");
        return;
    }
    chdir(PATH1);     //改变目录到dir
    while((entry = readdir(dp))!=NULL)
    {
        //以该目录项的名字为参数,调用lstat得到该目录项的相关信息;
        lstat(entry->d_name,&statbuf);
        if(S_ISDIR(statbuf.st_mode))
        {
            if(strcmp(entry->d_name,"..")==0||strcmp(entry->d_name,".")==0)
                continue;
            //mkdir(PATH2,0755);
            char temp[500];
            strcpy(temp,PATH2);
            strcat(PATH2,"/");
            strcat(PATH2,entry->d_name);
            printf("%s\n",entry->d_name );
            copy_dir(entry->d_name,PATH2);
            strcpy(PATH2,temp);
        }
        else
        {
            char temp[500];
            strcpy(temp,PATH2);
            strcat(PATH2,"/");
            strcat(PATH2,entry->d_name);
            printf("%s\n",entry->d_name );
            copy_file(entry->d_name,PATH2);
            strcpy(PATH2,temp);
        }
    }
    chdir("..");
    closedir(dp);
}

 
int main()
{
   	char PATH1[500];
   	char PATH2[500];
   	scanf("%s %s",PATH1,PATH2);


   	struct stat buf; 
   	//
	int result; 
	result = stat( PATH1, &buf ); 
	if(S_IFDIR & buf.st_mode)
	{ 
		copy_dir(PATH1,PATH2);
	}
	else if(S_IFREG & buf.st_mode)
	{ 
		copy_file(PATH1,PATH2);
	} 

   	return 0;
}