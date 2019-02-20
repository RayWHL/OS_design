/usr/src/linux-4.20.10/include/asm-generic/syscalls.h 
增加新系统调用的函数定义：
#ifndef sys_mycopy
asmlinkage int sys_mycopy(char *PATH1,char *PATH2);
#endif


在/usr/src/linux-4.4.128/kernel/sys.c文件中添加sys_copy的源码

asmlinkage int sys_mycopy(char *PATH1,char *PATH2)
{
    int fp1, fp2;
    int c = 0;
    char buf[10] = {0};

    mm_segment_t fs;

    //打开读取的文件
    if ((fp1 = sys_open(PATH1, O_RDONLY,0)) == -1) {
       printk("open fail\n");
       return -1;
    }

    //打开写入的文件，没有则创建
    if ((fp2 = sys_open(PATH2, O_WRONLY | O_CREAT | O_TRUNC, 0600)) == -1) {
       printk("create fail\n");
       return -1;
    }

    fs=get_fs();
    set_fs(get_ds());

    //循环复制，一次读入READ_BUFF
    while((c = sys_read(fp1, buf, 5)) > 0) 
    {
         int WriteRes = sys_write(fp2, buf, c);
         if (WriteRes != c)
         {
            printk("write error\n");
            return -1;
         }    
    }

    //关闭文件
    if(sys_close(fp1) != 0) {
       printk("close fp1 fail");
    }

    //关闭文件
    if(sys_close(fp2) != 0) {
      printk("close fp2 fail");
    }
    set_fs(fs);
    return 1;
}


/usr/src/linux-4.4.128/arch/x86/entry/syscalls/syscall_64.tbl 系统调用表