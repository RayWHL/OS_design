#ifndef HEADER_INCLUDED
#define HEADER_INCLUDED

#include <gtk/gtk.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <sys/stat.h>

#define get_path "1_2_read.txt"
#define put_path "1_2_write.txt"

union semun {
	int val;
	struct semid_ds *buf;
	unsigned short int *array;
	struct seminfo *info;
};

/*P操作*/
void P(int semid, int index)
{
	struct sembuf sem;
	sem.sem_num = index;	//信号量下标
	sem.sem_op = -1;	//执行操作
	sem.sem_flg = 0;	//操作标志
	semop(semid, &sem, 1);	//1为执行命令个数
	return;
}

/*V操作*/
void V(int semid, int index)
{
	struct sembuf sem;
	sem.sem_num = index;
	sem.sem_op = 1;
	sem.sem_flg = 0;
	semop(semid, &sem, 1);
	return;
}

typedef struct sharedBuf
{
	char data[10];
	int length;	
}sharedBuf;

const key_t shKeyS1 = 0x2215;	//共享内存1键值
const key_t shKeyS2=0x2216;     //共享内存2键值
key_t semKey1 = 5;		//信号量1键值
key_t semKey2=6;                //信号量2键值

#endif // HEADER_INCLUDED