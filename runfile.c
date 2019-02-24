#include "monitor.h"
//#include <X11/Xlib.h>

gchar file[50];
GtkWidget *filew;
GtkWidget *entry1;


void run_a_file()
{
    gtk_widget_destroy(filew);
    //show_dialog("警告","执行功能待实现"); 
    /*
    gchar command_file[50];
    GString *string;
    int i,len,j;
    FILE *fp;

    string = g_string_new ("");//Refresh info_Num
    g_string_append (string, "cd ");

    len=strlen(file);
    for(i=len-1;i>=0;--i)
    {
        if(file[i]=='/')
        {
            for(j=0;j<i;++j)
            {
                command_file[j]=file[j];
            }
            command_file[j]='\0';
            g_string_append (string,command_file);
            fp=popen(string->str,"r");
            pclose(fp);

            g_string_free (string, TRUE);     //  清空command_file
            g_string_append (string, "./");
            for(j=i+1;j<len;++j)
            {
                command_file[j-i-1]=file[j];
            }
            command_file[j-i-1]='\0';
            g_string_append (string, command_file);
            fp=popen(string->str,"r");
            pclose(fp);

            return;
        }
    }
    */
    /*
    gchar title[20]="NB";
    gchar content[50]="...";

    GtkWidget *dialog;
    GtkWidget *label;
    dialog = gtk_dialog_new_with_buttons (title,
                                          GTK_WINDOW (window),
                                          GTK_DIALOG_DESTROY_WITH_PARENT,
                                          GTK_STOCK_CLOSE,
                                          GTK_RESPONSE_NONE,
                                          NULL);//创建一个对话框控件
    gtk_window_set_resizable (GTK_WINDOW (dialog), FALSE);//不可改变大小
    g_signal_connect_swapped (dialog,
                              "response",
                              G_CALLBACK (gtk_widget_destroy),
                              dialog);//事件回调函数为退出

    label = gtk_label_new (content);//创建一个标签,显示content的内容
    gtk_widget_show (label);
    gtk_container_add (GTK_CONTAINER (GTK_DIALOG(dialog)->vbox),
                       label);
    gtk_widget_show (dialog);
    */
    
}

void file_ok_sel(GtkWidget *filew,GtkFileSelection *fs)
{
    ///XInitThreads();
    const gchar *filename=gtk_file_selection_get_filename(GTK_FILE_SELECTION(fs));
    sprintf(file,"%s",filename);
    if(access(filename,X_OK)==0)
    {
        pthread_t id;
        int ret;
        ret=pthread_create(&id,NULL,(void *)run_a_file,NULL);
        if(ret!=0)
        {
            show_dialog("警告","执行失败");
        }
    }
    else
    {
        show_dialog("警告","该文件不可执行");
    }
    
}

void select_file()
{
     /* GtkWidget 是构件的存储类型 */     
    /*创建一个新的文件选择构件*/
    filew = gtk_file_selection_new("文件选择对话框");

    //gtk_box_pack_start (GTK_BOX (vbox), filew, FALSE, FALSE, 10);
  
    /*为ok_button按钮设置回调函数，链接到file_ok_sel 函数*/
    g_signal_connect(GTK_OBJECT(GTK_FILE_SELECTION(filew)->ok_button),"clicked",G_CALLBACK(file_ok_sel),filew);
 
    /*为cancel_button设置回调函数*/
    g_signal_connect(GTK_OBJECT(GTK_FILE_SELECTION(filew)->cancel_button),"clicked",G_CALLBACK(gtk_widget_destroy),filew);
 
    /*设置文件名，不如这个一个文件保存对话框，我们给了一个缺省文件名*/
    //gtk_file_selection_set_filename(GTK_FILE_SELECTION(filew),"1111.png");
    gtk_widget_show (filew);
}

void create_runfile_page()
{
    int i;
    GtkWidget *box1,*box2,*box3,*box4;
    GtkWidget *label1;
    GtkWidget *pid_button, *sep;


    vbox = gtk_vbox_new (FALSE, 0);//添加纵向盒子
    gtk_widget_show (vbox);
    gtk_container_add (GTK_CONTAINER (notebook), vbox);

    //scrolled_window = gtk_scrolled_window_new (NULL, NULL);//添加滚动窗口控件
    //gtk_widget_set_size_request (scrolled_window, 300, 300);
    //gtk_widget_show (scrolled_window);
    //gtk_box_pack_start (GTK_BOX (vbox), scrolled_window, TRUE, TRUE, 0);

    frame = gtk_frame_new ("run cmd:");
    gtk_widget_show (frame);//显示cmd信息边框
    gtk_box_pack_start (GTK_BOX (vbox), frame, FALSE, FALSE, 10);//加到纵向盒子里

    //info = g_string_new ("");
    //get_cpu_info (info);//用info变量存储获得的CPU信息
    //info_label = gtk_label_new (info->str);//用标签info_label显示info变量的字符串内容
    //gtk_widget_show (info_label);
    //gtk_container_add (GTK_CONTAINER(frame), info_label);//将该标签加入边框中

    box1 = gtk_hbox_new(FALSE,0);
    gtk_container_add (GTK_CONTAINER(frame), box1);
    //box2 = gtk_hbox_new(FALSE,0);
    //gtk_box_pack_start(GTK_BOX(vbox),box2,FALSE,FALSE,5);
    gtk_widget_show(box1);

    //创建选择文件按钮
    select_file_button = gtk_button_new ();
    gtk_widget_show (select_file_button);
    gtk_widget_set_size_request (select_file_button, 200, 30);//Set Button Size
    gtk_button_set_label (GTK_BUTTON (select_file_button), "select a file");//设置按钮上的文字
    g_signal_connect (G_OBJECT (select_file_button),"clicked",
                      G_CALLBACK(select_file),
                      NULL);//刷新按钮点击后执行select_file
    gtk_box_pack_start(GTK_BOX(box1),select_file_button,TRUE,FALSE,0);

    //sep = gtk_hseparator_new();//分割线
    //gtk_box_pack_start(GTK_BOX(vbox),sep,FALSE,FALSE,5);
    //gtk_widget_show(sep);

    frame = gtk_frame_new ("select a pid:");
    gtk_widget_show (frame);//显示cmd信息边框
    gtk_box_pack_start (GTK_BOX (vbox), frame, FALSE, TRUE, 10);//加到纵向盒子里

    vbox = gtk_vbox_new (FALSE, 0);//添加纵向盒子
    gtk_widget_show (vbox);
    gtk_container_add (GTK_CONTAINER(frame),vbox);

    box1 = gtk_hbox_new(FALSE,0);
    gtk_box_pack_start(GTK_BOX(vbox),box1,FALSE,FALSE,5);
    //box2 = gtk_hbox_new(FALSE,0);
    //gtk_box_pack_start(GTK_BOX(vbox),box2,FALSE,FALSE,5);
    gtk_widget_show(box1);
    //gtk_widget_show(box2);
    //sep = gtk_hseparator_new();//分割线
    //gtk_box_pack_start(GTK_BOX(box),sep,FALSE,FALSE,5);
    //box3 = gtk_hbox_new(FALSE,0);
    //gtk_box_pack_start(GTK_BOX(vbox),box3,TRUE,TRUE,5);
 
 
    label1 = gtk_label_new("PID:");
    gtk_widget_set_size_request(label1, 150, 30);
    gtk_widget_show(label1);

    entry1 = gtk_entry_new();
    gtk_widget_set_size_request(entry1, 500, 30);
    gtk_widget_show(entry1);
    gtk_box_pack_start(GTK_BOX(box1),label1,FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box1),entry1,FALSE, FALSE, 0);
 
    pid_button = gtk_button_new_with_label("确认");
    g_signal_connect(G_OBJECT(pid_button),"clicked",G_CALLBACK(pid_button_clicked),NULL);
    gtk_widget_set_size_request (pid_button, 70, 30);//Set Button Size
    //g_signal_connect_swapped(G_OBJECT(pid_button),"clicked",G_CALLBACK(gtk_widget_destroy),window);
    gtk_box_pack_start(GTK_BOX(box1),pid_button,TRUE,FALSE,0);
    gtk_widget_show(pid_button);


    //为本页添加标题
    label = gtk_label_new ("runfile");
    gtk_widget_set_size_request (label, 120, 20);
    gtk_widget_show (label);
    //将本页添加到notebook中，是第三页
    gtk_notebook_set_tab_label (GTK_NOTEBOOK (notebook),
                                gtk_notebook_get_nth_page (GTK_NOTEBOOK (notebook),2), label);
}

void pid_button_clicked (GtkWidget* button,gpointer data)
{
    gint pid_int;
    const gchar *pid_input=gtk_entry_get_text(GTK_ENTRY(entry1));
    pid_int=atoi(pid_input);
    get_pid_processes(pid_int);
}


void get_pid_processes (gint pid_int)//获取进程信息
{
    DIR *dir;
    int fd,i;
    FILE *fp;
    GtkTreeIter iter;
    struct dirent *entry;
    gdouble mem;
    gdouble srate;
    gdouble now_cutime,now_cstime,now_utime,now_stime;
    gint user_id;
    gchar *info[26];
    gchar *userinfo[5];
    gchar buffer[128];
    gchar s[1024];
    gchar dir_buf[256];
    gchar *delim = " ";
    gchar mem_buffer[16];
    gchar rate_buffer[16];
    gchar state[3][10]= {"running", "sleeping", "else"};
    struct passwd *pwd;
    gchar content[1024];
    gchar pid_temp[20];
    sprintf(pid_temp,"%d",pid_int);

    static gdouble old_cutime = 0;
    static gdouble old_cstime = 0;
    static gdouble old_utime = 0;
    static gdouble old_stime = 0;
    gint flag=0;

    //opendir打开一个与给定目录名相对应的目录流,返回指向该目录流的指针
    dir = opendir ("/proc");// 打开/proc文件目录,dir为返回指针
    while ((entry = readdir (dir)) != NULL )
    {
        if (strcmp(entry->d_name,pid_temp)==0)
        {
            flag=1;
            sprintf (dir_buf, "/proc/%s/stat", entry->d_name);//读文件到dir_buf中
            fd = open (dir_buf, O_RDONLY);//只读打开
            read (fd, buffer, sizeof (buffer));
            close (fd);

            //strtok用于分割字符串
            info[0] =  strtok (buffer, delim);//以delim分隔符切割buffer的一部分到info[0]
            for (i = 1; i < 26 ; i++){
            	     info[i] = strtok(NULL, delim);//查找整个buffer字符串
            }
            info[1]++;
            info[1] = strtok(info[1], ")");

            switch(info[2][0])
            {
            case 'R':
                info[2] = state[0];
                break;
            case 'S':
                info[2] = state[1];
                break;
            default :
                info[2] = state[2];
            }

            /*
            //该任务在用户态运行的时间
            now_utime = atoi (info[13]);
            //该任务在核心态运行的时间
            now_stime = atoi (info[14]);
            //累计的该任务的所有的waited-for进程曾经在用户态运行的时间
            now_cutime = atoi (info[15]);
            //累计的该任务的所有的waited-for进程曾经在核心态运行的时间
            now_cstime = atoi (info[16]);

            srate = ((now_utime + now_stime + now_cutime + now_cstime) - 
                     (old_utime + old_stime + old_cutime + old_cstime)) / total;

            if(srate<0||srate>1)
            {
                srate=0;
            }
            sprintf (rate_buffer, "%.2f%%",100 * srate);
            */

            mem = atoi (info[22]);//该进程的内存使用量
            mem = mem / (1024 * 1024);
            sprintf (mem_buffer, "%-.2f MB",mem);

            //获得进程所属用户
            sprintf (dir_buf, "/proc/%s/status", entry->d_name);//读文件到dir_buf中
            // printf("%s ",dir_buf);
            if((fp = fopen (dir_buf, "r")) == NULL){
                printf("error!\n");//只读打开
            }
            else{
                while(fgets(s,1024,fp) != NULL)
                {
                    i = 0;
                    if(s[0] == 'U' && s[1] == 'i' && s[2] == 'd'){
                        userinfo[0] = strtok (s,"\t");
                        for(i = 1; i < 5; i++) userinfo[i] = strtok (NULL,"\t");
                        user_id = atoi(userinfo[1]);
                        //通过获得的Uid获得进程所属的用户名
                        pwd = getpwuid(user_id);
                    }
                }
                fclose(fp);
            }
            sprintf(content,"Name:%s\nUser:%s\nPPID:%s\nStatus:%s\nMemory:%s\nPriority:%s"
                            ,info[1],pwd->pw_name,info[3],info[2],mem_buffer,info[18]);
            show_dialog(info[0],content);
            break;
        }
    }
    closedir (dir);
    if(flag==0)
    {
        show_dialog("error","the processes not exit");
    }
}

