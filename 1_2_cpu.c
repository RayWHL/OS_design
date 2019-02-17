#include <gtk/gtk.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define _DEBUG_ 1
#define CK_TIME 1

gint print_cpu(gpointer label);
unsigned long GetCPUUsageRate();

void destory_progress(GtkWidget *widget)
{
    gtk_main_quit();
}
 
typedef struct cpu_info
{
	char name[8];
	unsigned long user;
	unsigned long nice;
	unsigned long system;
	unsigned long idle;
	unsigned long iowait;
	unsigned long irq;
	unsigned long softirq;
} CPUInfo;
 
unsigned long GetCPUTotalTime(CPUInfo* pstCPUInfo)
{
	return pstCPUInfo->user + pstCPUInfo->nice + pstCPUInfo->system + 
		pstCPUInfo->idle + pstCPUInfo->iowait + pstCPUInfo->irq + pstCPUInfo->softirq;
}
 
void DisplayCPUInfo(CPUInfo* pstCPUInfo)
{
	printf("%s %lu %lu %lu %lu %lu %lu %lu\n", pstCPUInfo->name, pstCPUInfo->user, pstCPUInfo->nice, pstCPUInfo->system,
		pstCPUInfo->idle, pstCPUInfo->iowait, pstCPUInfo->irq, pstCPUInfo->softirq);
}
 
int GetCPUInfo(CPUInfo *pstCPUInfo)
{
	FILE * fp = fopen("/proc/stat", "r");
 
	char buf[1024];
	fgets(buf, sizeof(buf), fp);
 
	sscanf(buf,"%s %lu %lu %lu %lu %lu %lu %lu", 
		pstCPUInfo->name, &pstCPUInfo->user, &pstCPUInfo->nice,&pstCPUInfo->system, 
		&pstCPUInfo->idle, &pstCPUInfo->iowait, &pstCPUInfo->irq,&pstCPUInfo->softirq);  
 
	fclose(fp);
 
	return 0;
}


int main(void)
{
    //窗口创建以及设置
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
    gtk_window_set_title(GTK_WINDOW(window),"CPU利用率");
    gtk_container_set_border_width(GTK_CONTAINER(window),20);
    vbox = gtk_vbox_new(FALSE,10);
    gtk_container_border_width(GTK_CONTAINER(vbox),100);
    gtk_container_add(GTK_CONTAINER(window),vbox);
    gtk_widget_show(vbox);
    label = gtk_label_new("CPU利用率");
    gtk_box_pack_start(GTK_BOX(vbox),label,FALSE,FALSE,10);
    gtk_widget_show(label);
    gtk_widget_show(window);
    g_timeout_add(2000,print_cpu,(gpointer)label);
    gtk_main();
}

gint print_cpu (gpointer label)
{
    char string[50];
    sprintf(string,"CPU利用率为：%lu%%",GetCPUUsageRate());
    gtk_label_set_text(label,string);
}


unsigned long GetCPUUsageRate()
{
	CPUInfo stCPUInfo1;
	CPUInfo stCPUInfo2;
 
	memset(&stCPUInfo1, 0, sizeof(stCPUInfo1));
	memset(&stCPUInfo2, 0, sizeof(stCPUInfo2));
    //当前CPU信息
	GetCPUInfo(&stCPUInfo1);
	DisplayCPUInfo(&stCPUInfo1);
	sleep(1);
    //一秒后CPU信息
	GetCPUInfo(&stCPUInfo2);
	DisplayCPUInfo(&stCPUInfo2);
 
	unsigned long nTotalTime = GetCPUTotalTime(&stCPUInfo2) - GetCPUTotalTime(&stCPUInfo1);     // CPU总时间
	unsigned long nIdleTime = stCPUInfo2.idle - stCPUInfo1.idle;        //CPU空闲时间
	//printf("total time: %lu\n", nTotalTime);
	//printf("idle time:  %lu\n", nIdleTime);
	if (nTotalTime == 0)
	{
		return 0;
	}
 
	unsigned long usage = (nTotalTime - nIdleTime) * 100 / nTotalTime;  //利用率
 
	return usage;
}