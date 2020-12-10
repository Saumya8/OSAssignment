#include<iostream>
#include<cstdlib>
#include<queue>
#include<cstdio>
using namespace std;

class rr_pro{
    public:
        int p;
        int at;
        int bt;
};

void insertq(int q[],int *f,int *r,int key){
    if(*r==10)
        return;
    (*r)+=1;
    q[*r]=key;
    if(*f==-1)
        *f=0;
}

int deleteq(int q[],int *f,int *r){
    if(*f==-1)
        return -1;
    int key=q[*f];
    if(*f==0 && *r==0)
    {
        *f=*r=-1;
    }
    else if(*f==*r)
        *f=*r=-1;
    else
        (*f)+=1;
    return key;
}

int emptyq(int q[],int f,int r){
    if(f==-1)
        return 1;
    return 0;
}

int main()
{
    /* Input from the user*/
    int i;
    int f=-1,r=-1;
    int rq[10];
    int n;
    printf("Enter the number of Processes:-\t");
    scanf("%d",&n);
    rr_pro arr[n];
    printf("Enter the details\n");
    int bt_buff[30];
    printf("\nProcess no  Arrival time  Burst time\n");
    for(i=0;i<n;i++){
        scanf("%d%d%d",&arr[i].p,&arr[i].at,&arr[i].bt);
        bt_buff[i]=arr[i].bt;
    }

    int tq;
    printf("Enter the Time Quantum for each process:");
    scanf("%d",&tq);

    int k=0;
    int it=0;

    int tatime[30];
    int wtime[30];
    for(i=0;i<n;i++){
        tatime[i]=0;
        wtime[i]=0;
    }

    queue<int>q;
    it=arr[0].at;

        /** Saving the ready Queue with all the process while execution of first process*/
    for(i=0;i<n && arr[i].at<=it;i++){

        /**q.push(arr[i].p);*/
        insertq(rq,&f,&r,arr[i].p);

    }


    /** Unload the Queue one by one and then perform the quantum of time Processing on it*/
    while(!emptyq(rq,f,r)){
        k=deleteq(rq,&f,&r);

        k=k-1;
        if(arr[k].bt>tq){
            it=it+tq;
            arr[k].bt=arr[k].bt-tq;
        }
        else{
            it=it+arr[k].bt;
            arr[k].bt=0;
        }
        printf("| P%d |%d ",arr[k].p,it);
            /** Ready Queue entry will resume from the point where it first stopped during execution of First process*/
            while(i<n && arr[i].at<=it){
                insertq(rq,&f,&r,arr[i].p);
                i++;
            }
        /** put the current process if it is left with burst time*/
        if(arr[k].bt>0){
            insertq(rq,&f,&r,arr[k].p);
        }
        if(arr[k].bt<=0){
            tatime[k]=it-arr[k].at;
            wtime[k]=tatime[k]-bt_buff[k];
            if(wtime[k]<0)
                wtime[k]=0;
        }
    }

    int avg_tat=0,avg_wt=0;
    printf("\n\n");
    printf("TAT Analysis:\n");
    for(i=0;i<n;i++){
       printf("P%d: %d\n",i+1,tatime[i]);
        avg_tat+=tatime[i];
    }

    printf("\n\n");
    printf("WT Analysis:\n");
    for(i=0;i<n;i++){
       printf("P%d: %d\n",i+1,wtime[i]);
       avg_wt+=wtime[i];
    }

    printf("Avg WT: \t%g\n",(float)avg_wt/n);
    printf("Avg TAT:\t%g\n",(float)avg_tat/n);

    return 0;
}