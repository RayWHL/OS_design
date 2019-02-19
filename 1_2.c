#include "header.h"

int main(void)
{
    /*共享内存的设置*/
	int shIDS1,shIDS2;
	//分配共享内存块1
	shIDS1 = shmget(shKeyS1, sizeof(sharedBuf), IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);
    //分配共享内存块2
	shIDS2 = shmget(shKeyS2, sizeof(sharedBuf), IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);

	/*创建4个信号灯，*/
	int semId1,semId2;
	semId1 = semget(semKey1, 2, IPC_CREAT | 0666);
	union semun arg;
	arg.val = 1;
	semctl(semId1, 0, SETVAL, arg);    // 共享内存1为空
	arg.val = 0;
	semctl(semId1, 1, SETVAL, arg);     // 共享内存1满

    semId2 = semget(semKey2, 2, IPC_CREAT | 0666);
	arg.val = 1;
	semctl(semId2, 0, SETVAL, arg);    // 共享内存2为空
	arg.val = 0;
	semctl(semId2, 1, SETVAL, arg);     // 共享内存2满

    int pid1,pid2,pid3;
    if((pid1 = fork()) == 0)
    {
        execl("./1_2_get","1_2_get",NULL);
    }
    else if((pid2 = fork()) == 0)
    {
        execl("./1_2_copy","1_2_copy",NULL);
    }
    else if(pid3=fork()==0)
    {
        execl("./1_2_put","1_2_put",NULL);
    }
    else
    {
        /*等待退出*/
		waitpid(pid1, NULL, 0);
		waitpid(pid2, NULL, 0);
        waitpid(pid3, NULL, 0);

		/*删除信号灯&共享内存*/
		semctl(semId1, 1, IPC_RMID, 0);
        semctl(semId2, 1, IPC_RMID, 0);
		shmctl(shIDS1, IPC_RMID, 0);
        shmctl(shIDS1, IPC_RMID, 0);
    }
    
}