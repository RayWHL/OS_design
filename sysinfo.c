#include "monitor.h"

void create_sysinfo_page()
{
    vbox = gtk_vbox_new (FALSE, 0);//添加纵向盒子
    gtk_widget_show (vbox);
    gtk_container_add (GTK_CONTAINER(notebook), vbox);

    frame = gtk_frame_new ("CPU Information:");
    gtk_widget_show (frame);//显示CPU信息边框
    gtk_box_pack_start (GTK_BOX (vbox), frame, TRUE, TRUE, 10);//加到纵向盒子里

    info = g_string_new ("");
    get_cpu_info (info);//用info变量存储获得的CPU信息
    info_label = gtk_label_new (info->str);//用标签info_label显示info变量的字符串内容
    gtk_widget_show (info_label);
    gtk_container_add (GTK_CONTAINER(frame), info_label);//将该标签加入边框中

    frame = gtk_frame_new ("OS Information:");//显示操作系统信息边框
    gtk_widget_show (frame);
    gtk_box_pack_start (GTK_BOX (vbox), frame, TRUE, TRUE, 10);

    info = g_string_new ("");
    get_os_info (info);//用info变量存储获得的操作系统信息
    info_label = gtk_label_new (info->str);//用标签info_label显示info变量的字符串内容
    gtk_widget_show (info_label);
    gtk_container_add (GTK_CONTAINER(frame), info_label);

    frame = gtk_frame_new ("TIME Information:");//显示系统时间信息边框
    gtk_widget_show (frame);
    gtk_box_pack_start (GTK_BOX (vbox), frame, TRUE, TRUE, 10);

    info = g_string_new ("");
    get_time_info (info);//用info变量存储获得的系统时间信息
    info_label = gtk_label_new (info->str);//用标签info_label显示info变量的字符串内容
    gtk_widget_show (info_label);
    gtk_container_add (GTK_CONTAINER(frame), info_label);


    label = gtk_label_new ("sys_information");
    gtk_widget_set_size_request (label, 120, 20);
    gtk_widget_show (label);
    //notebook标签标题
    gtk_notebook_set_tab_label (GTK_NOTEBOOK (notebook),
                                gtk_notebook_get_nth_page (GTK_NOTEBOOK (notebook),3), label);
}

void get_cpu_info(GString *string)//获得cpu信息
{
       int fd,i;
       gchar buffer[1024];								
       gchar *delim = "\n";
       gchar *cpu_info[17];
       gchar *tmp;
       fd = open ("/proc/cpuinfo", O_RDONLY);//读取cpu相关信息的文件
       //一口气将文件中所有信息读取出来
       read (fd, buffer, sizeof (buffer));
       close (fd);
       //将从cpuinfo文件中读取出来的字符串依据\n符号进行分割
       cpu_info[0] = strtok (buffer, delim);
       for (i=1; i < 17 ; i++) {
		cpu_info[i] = strtok (NULL,delim);//分割字符q串
       }	  
       for (i=0; i < 17; i++) {
  	  	tmp = strstr (cpu_info[i], ":");//忽略冒号:
		tmp += 2; 
        cpu_info[i] = tmp;	  
       }
       g_string_append (string, "CPU Model:");
       g_string_append (string, cpu_info[4]);
       g_string_append_c (string, '\n');
       g_string_append (string, "CPU cores\t:");
       g_string_append (string, cpu_info[12]);
       g_string_append_c (string, '\n');
       g_string_append (string, "CPU rate\t:");
       g_string_append (string, cpu_info[7]);
       g_string_append(string, "MHz");
       g_string_append_c (string, '\n');
       g_string_append (string, "Cache size\t:");
       g_string_append (string, cpu_info[8]);
}

void get_os_info (GString *string)//获得操作系统相关信息
{
    int fd;
    gchar buffer[128];
    // gchar *tmp,*start,
    //gchar *stop;
    gchar *info[3];
    gchar *delim = "\n";

    fd = open ("/proc/sys/kernel/hostname", O_RDONLY);//读取操作系统相关信息的文件
    read(fd, buffer, sizeof (buffer));
    close(fd);

    info[0]=strtok(buffer,delim);

    //字符串串接显示hostname
    g_string_append (string, "HOSTNAME\t:");
    g_string_append (string, info[0]);
    g_string_append (string, "\n");

    fd = open ("/proc/sys/kernel/ostype", O_RDONLY);//读取操作系统相关信息的文件
    read(fd, buffer, sizeof (buffer));
    close(fd);

    info[1]=strtok(buffer,delim);
    //字符串串接显示OS type
    g_string_append (string, "OS\t\t\t\t:");
    g_string_append (string, info[1]);
    g_string_append (string, "\n");

    fd = open ("/proc/sys/kernel/osrelease", O_RDONLY);//读取操作系统相关信息的文件
    read(fd, buffer, sizeof (buffer));
    close(fd);

    info[2]=strtok(buffer,delim);
    //字符串串接显示OS release
    g_string_append (string, "Release\t\t:");
    g_string_append (string, info[2]);
}

void get_time_info (GString *string)
{
    FILE *fd;
    double time1,time2;

    fd = fopen ("/proc/uptime", "rb");//读取系统时间相关信息的文件
    //time1为系统启动时间，time2为系统CPU总空闲时间
    fscanf(fd,"%lf %lf",&time1,&time2);
    fclose(fd);

    time2=time2/4;
    time2=time1-time2;
    //字符串串接显示
    g_string_sprintf (string, "Startup Time\t:%lf s\nRun Time\t:%lf s",time1,time2);

}