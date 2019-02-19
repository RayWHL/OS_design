#include "header.h"


//#define _DEBUG_ 1
//#define CK_TIME 1

int semID1,shIDS1;
sharedBuf *sBuf;
FILE *fp;

gint print_get(gpointer label);

void destory_progress(GtkWidget *widget)
{
    fclose(fp);
    shmdt(sBuf);
    gtk_main_quit();
}

int main(int argc, char **argv)
{
    //获取信号灯1
    semID1 = semget(semKey1, 2, IPC_CREAT | 0666);

    //获取共享内存1
	shIDS1 = shmget(shKeyS1, sizeof(sharedBuf), IPC_CREAT | 0666);
	sBuf = (sharedBuf*)shmat(shIDS1, NULL, 0);

    //打开get的文件
    fp = fopen(get_path, "rb");

    //窗口创建以及设置
    GtkWidget *window;
    GtkWidget *vbox;
    GtkWidget *button;
    GtkWidget *label;
    gpointer data;
   //初始化gtk库
    gtk_init (&argc, &argv);
    //新建一个顶层窗口
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    //使窗口获得伸缩功能
    gtk_window_set_resizable(GTK_WINDOW(window),TRUE);
    //窗口显示位置
    gtk_window_set_position(GTK_WINDOW(window),GTK_WIN_POS_NONE);
    //退出时调用的函数
    g_signal_connect(G_OBJECT(window),"delete_event",G_CALLBACK(destory_progress),NULL);
    //窗口标题
    gtk_window_set_title(GTK_WINDOW(window),"get");
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
    label = gtk_label_new("get");
    //在vbox中从前往后进行排列,不随窗口拉伸而改变大小
    gtk_box_pack_start(GTK_BOX(vbox),label,FALSE,FALSE,10);
    //显示标签
    gtk_widget_show(label);
    //显示窗口
    gtk_widget_show(window);
    //每隔1000ms进行一次刷新，运行一次函数
    g_timeout_add(3000,print_get,(gpointer)label);
    gtk_main();
}

gint print_get (gpointer label)
{
    P(semID1, 0);
    int size = fread(sBuf->data, sizeof(char), 10, fp);
    sBuf->length=size;
    V(semID1, 1);

    char string[50];
    if(size==0)
    {
        sprintf(string,"get:结束");
    }
    else
    {
        sprintf(string,"从文件get：%s 到缓冲区1",sBuf->data);
    }
    
    gtk_label_set_text(label,string);
}
