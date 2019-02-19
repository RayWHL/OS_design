#include "header.h"


//#define _DEBUG_ 1
//#define CK_TIME 1

int semID1,semID2,shIDS1,shIDS2;
sharedBuf *sBuf1,*sBuf2;
int last_size=1;

gint print_copy(gpointer label);

void destroy_window (GtkWidget *widget, gpointer data);

int main(int argc, char **argv)
{
    //获取信号灯1
    semID1 = semget(semKey1, 2, IPC_CREAT | 0666);
    //获取信号灯2
    semID2 = semget(semKey2, 2, IPC_CREAT | 0666);

    //获取共享内存1
	shIDS1 = shmget(shKeyS1, sizeof(sharedBuf), IPC_CREAT | 0666);
	sBuf1 = (sharedBuf*)shmat(shIDS1, NULL, 0);

    //获取共享内存2
	shIDS2 = shmget(shKeyS2, sizeof(sharedBuf), IPC_CREAT | 0666);
	sBuf2 = (sharedBuf*)shmat(shIDS2, NULL, 0);

    GtkWidget *window;
    GtkWidget *vbox;
    GtkWidget *button;
    GtkWidget *label;
    gpointer data;

    gtk_set_locale(); 
    //初始化gtk库
    gtk_init (&argc, &argv);
    //新建一个顶层窗口
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    //使窗口获得伸缩功能
    gtk_window_set_resizable(GTK_WINDOW(window),TRUE);
    //窗口显示位置
    gtk_window_set_position(GTK_WINDOW(window),GTK_WIN_POS_NONE);
    //退出时调用的函数
    g_signal_connect(G_OBJECT(window),"delete_event",G_CALLBACK(destroy_window),NULL);
    //窗口标题
    gtk_window_set_title(GTK_WINDOW(window),"copy");
    //容器，这里指窗口的边框宽度
    gtk_container_set_border_width(GTK_CONTAINER(window),20);
    //新的纵向容器
    vbox = gtk_vbox_new(FALSE,10);
    //纵向容器的边框宽度
    gtk_container_border_width(GTK_CONTAINER(vbox),100);
    //将纵向容器控件加入到窗口控件中去
    gtk_container_add(GTK_CONTAINER(window),vbox);
    //使vbox能够被显示
    gtk_widget_show(vbox);
    //标签
    label = gtk_label_new("copy");
    //在vbox中从前往后进行排列,不随窗口拉伸而改变大小
    gtk_box_pack_start(GTK_BOX(vbox),label,FALSE,FALSE,10);
    //显示标签
    gtk_widget_show(label);
    //每隔1000ms进行一次刷新，运行一次sum函数
    g_timeout_add(3000,print_copy,(gpointer) label);
    //显示窗口
    gtk_widget_show(window);
    //初始化
    gtk_main();
    return 0;
}

gint print_copy(gpointer label)
{
    char string[50];
    char temp[20];
    int size;
    sleep(1);
    //从缓冲区1取数据
    if(last_size!=0)
    {
        P(semID1, 1);
        strcpy(temp,sBuf1->data);
        size=sBuf1->length;
        V(semID1, 0);

        //向缓冲区2写数据
        P(semID2, 0);
        strcpy(sBuf2->data,temp);
        sBuf2->length=size;
        V(semID2, 1);
        if(size>0)
            sprintf(string,"从缓冲区1 copy： %s 到缓冲区2",temp);
        else
            sprintf(string,"end");
        last_size=size;
    }
    else
        sprintf(string,"end");
    

    gtk_label_set_text(label,string);
   
}

void destroy_window (GtkWidget *widget, gpointer data)//退出窗口管理程序
{
    shmdt(sBuf1);
    shmdt(sBuf2);
    gtk_main_quit ();
}