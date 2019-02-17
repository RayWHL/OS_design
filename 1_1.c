#include<stdio.h>
 
#define PATH1 "1_read.txt"
#define PATH2 "1_write.txt"
 
#define READ_BUFF 10
 
int main()
{
    FILE *fp1, *fp2;
    int c = 0;
    char buf[READ_BUFF] = {0};

    //打开读取的文件
    if ((fp1 = fopen(PATH1, "rb")) == NULL) {
       printf("open 1_read.txt fail\n");
       return -1;
    }

    //打开写入的文件
    if ((fp2 = fopen(PATH2, "wb")) == NULL) {
       printf("open 1_write.txt fail\n");
       return -1;
    }

    //循环复制，一次读入READ_BUFF
    while((c = fread(buf, sizeof(char), READ_BUFF, fp1)) > 0) {
       fwrite(buf, sizeof(char), c, fp2);
    }

    //关闭文件
    if(fclose(fp1) != 0) {
       puts("close fp1 fail");
    }

    //关闭文件
    if(fclose(fp2) != 0) {
      puts("close fp2 fail");
    }
    return 0;
}
