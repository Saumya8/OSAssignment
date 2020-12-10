#include<iostream>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include<unistd.h>
#include<cstring>
#include<signal.h>

#define FULL 0
#define READY 1
#define NotReady -1

using namespace std;
struct memory {
    char buffer[100];
    int status, pid1, pid2;
};
struct memory* shm;

int main()
{
    // process id of writer program
    int pid = getpid();
    int shmid;
    // key value of shared memory
    int key = 12345;
    // create shared memory
    shmid = shmget(key, sizeof(struct memory), IPC_CREAT | 0666);
    // attaching the shared memory
    shm = (struct memory*)shmat(shmid, NULL, 0);
    // store the process id of user1 in shared memory
    shm->pid1 = pid;
    shm->status = NotReady;
    // calling the signal function using signal type SIGUSER1
    while (1) {

        while (shm->status != READY)
            continue;
        sleep(1);
        // taking input from writer
        cout<<"Enter the data"<<endl;
        fgets(shm->buffer, 100, stdin);
        shm->status =FULL;
        // sending the message to user2 using kill function
        kill(shm->pid2, SIGUSR2);

    }
    shmdt((void*)shm);

    shmctl(shmid, IPC_RMID, NULL);

    return 0;
}
