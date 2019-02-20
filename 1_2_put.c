#include "header.h"


//#define _DEBUG_ 1
//#define CK_TIME 1

int semID2,shIDS2;
sharedBuf *sBuf2;
FILE *fp2;
int last_size=1;

gint print_put (gpointer label);

void destory_progress(GtkWidget *widget)
{
    fclose(fp2);
    shmdt(sBuf2);
    gtk_main_quit();
}

int main(void)
{
    //获取信号灯2
    semID2 = semget(semKey2, 2, IPC_CREAT | 0666);

    //获取共享内存2
    shIDS2 = shmget(shKeyS2, sizeof(sharedBuf), IPC_CREAT | 0666);
    sBuf2 = (sharedBuf*)shmat(shIDS2, NULL, 0);

    //打开get的文件
    fp2 = fopen(put_path, "wb");

    GtkWidget *window;
    GtkWidget *vbox;
    GtkWidget *button;
    GtkWidget *label;
    gpointer data;
    gtk_init(NULL,NULL);
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_resizable(GTK_WINDOW(window),TRUE);
    gtk_window_set_position(GTK_WINDOW(window),GTK_WIN_POS_NONE);
    g_signal_connect(G_OBJECT(window),"delete_event",G_CALLBACK(destory_progress),NULL);
    gtk_window_set_title(GTK_WINDOW(window),"put");
    gtk_container_set_border_width(GTK_CONTAINER(window),20);
    vbox = gtk_vbox_new(FALSE,10);
    gtk_container_border_width(GTK_CONTAINER(vbox),100);
    gtk_container_add(GTK_CONTAINER(window),vbox);
    gtk_widget_show(vbox);
    label = gtk_label_new("put");
    gtk_box_pack_start(GTK_BOX(vbox),label,FALSE,FALSE,10);
    gtk_widget_show(label);
    gtk_widget_show(window);
    g_timeout_add(3000,print_put,(gpointer)label);
    gtk_main();
}

gint print_put (gpointer label)
{
    sleep(2);
    //从缓冲区2将数据put到文件
    char temp[20];
    char string[50];
    if(last_size!=0)
    {
        P(semID2, 1);
        int size=fwrite(sBuf2->data, sizeof(char), sBuf2->length, fp2);
        strcpy(temp,sBuf2->data);
        V(semID2, 0);

        if(size>0)
        {
            sprintf(string,"从缓冲区2 put :%s 到文件",temp);
        }
        else
        {
            sprintf(string,"结束");
        }
        last_size=size;

    }
    else
        sprintf(string,"结束");
    
    
    gtk_label_set_text(label,string);
}