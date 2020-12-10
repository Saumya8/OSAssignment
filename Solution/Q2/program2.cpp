#include<iostream>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<unistd.h>
#include<cstring>
#include<signal.h>

#define READY 1
#define NotReady -1

using namespace std;
//defining structure

struct memory {
    char buffer[100];
    int status, pid1, pid2;
};

struct memory* shm;

void readData(int signal){
    int word=0,n=0;
    char a[10];
    /* if signal is equal to READER, then program 2 display
    no. of integers and words for the line written in program1*/
    if (signal == SIGUSR2) {

    cout<<"OUTPUT"<<endl;

       for(int i=0; i<strlen(shm->buffer);i++){
            if(!isspace(shm->buffer[i]) && isspace(shm->buffer[i+1]))
                        word++;
            if(isdigit(shm->buffer[i]))
                    n++;
        }
    }
    cout<<"Number of integers:"<<n<<endl;
    cout<<"Number of Words:"<<word<<endl;

}
// main function begins
int main()
{
    // process id of the reader function
    int pid = getpid();
    int shmid;
    // key value of shared memory
    int key = 12345;
    // create shared memory
    shmid = shmget(key, sizeof(struct memory), IPC_CREAT | 0666);
    // attaching to the shared memory
    shm = (struct memory*)shmat(shmid, NULL, 0);
    // store the process id of program2 in shared memory
    shm->pid2 = pid;
    shm->status = NotReady;
    // calling the signal function using signal SIGUSR2
    signal(SIGUSR2, readData);
    while (1) {
        sleep(1);
        shm->status = READY;
        while (shm->status == READY)
            continue;
    }
    shmdt((void*)shm);
    return 0;
}
