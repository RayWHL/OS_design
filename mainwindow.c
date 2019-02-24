#include "monitor.h"

int main(int argc, char **argv)
{
    gtk_set_locale();         //为本地环境读入本地化数据库,必须在gtk_init前调用
    gtk_init (&argc, &argv);  //初始化
    
    //新建主窗口
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    //使窗口获得伸缩功能
    gtk_window_set_resizable(GTK_WINDOW(window),TRUE);
    //设置窗口大小
    gtk_window_set_default_size (GTK_WINDOW (window), 800, 500);
    //窗口显示位置
    gtk_window_set_position(GTK_WINDOW(window),GTK_WIN_POS_CENTER_ALWAYS);
    //退出时调用的函数
    g_signal_connect(G_OBJECT(window), "delete_event",G_CALLBACK(destroy_window), NULL);
    //窗口标题
    gtk_window_set_title(GTK_WINDOW(window),"system monitor");

    //添加纵向盒子
    vbox = gtk_vbox_new (FALSE, 0);
    gtk_widget_show (vbox);
    gtk_container_add (GTK_CONTAINER(window), vbox);


    //设置分窗口
    notebook = gtk_notebook_new ();
    //显示notebook
    gtk_widget_show (notebook);
    // 笔记本放进窗口
    gtk_container_add(GTK_CONTAINER(vbox), notebook); 
    //笔记本标签的位置，上方
    gtk_notebook_set_tab_pos(GTK_NOTEBOOK (notebook), GTK_POS_TOP);

    frame = gtk_frame_new ("");
    gtk_widget_show (frame);//显示边框
    gtk_box_pack_start (GTK_BOX (vbox), frame, TRUE, TRUE, 1);//加到纵向盒子里

    time_t t;
    time(&t);
    time_info_label = gtk_label_new(ctime(&t));//用标签time_info_label显示time
    //gtk_widget_set_size_request (time_info_label, 120, 30);
    gtk_timeout_add (1000, (GtkFunction)print_time, (gpointer)time_info_label);
    gtk_widget_show (time_info_label);
    gtk_container_add (GTK_CONTAINER(frame), time_info_label);//将该标签加入边框中

    gtk_timeout_add (2000, (GtkFunction)handle_timeout, NULL);
    gtk_timeout_add (1000, (GtkFunction)load_graph_refresh, NULL);
    gtk_timeout_add (2000, (GtkFunction)get_cpu_rate, NULL); //CPUrate refresh per second
    //gtk_timeout_add (1000, (GtkFunction)get_mem_rate, NULL); //memrate refresh per second

    //创建进程页面
    create_process_page();
    //创建资源页面
    create_resource_page();
    //创建文件系统页面
    create_runfile_page();
    //创建系统信息页面
    create_sysinfo_page();

    //显示窗口
    gtk_widget_show(window);

    //初始化
    gtk_main();
    return 0;
}

//点击关闭窗口后执行
void destroy_window (GtkWidget *widget, gpointer data)//退出窗口管理程序
{
    gtk_main_quit ();
}