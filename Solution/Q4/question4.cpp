/******************************************************************************

                              Online C++ Compiler.
               Code, Compile, Run and Debug C++ program online.
Write your code in this editor and press "Run" button to compile and execute it.

*******************************************************************************/

#include <iostream>
#include <bits/stdc++.h> 

using namespace std;

int n,maxim,minim;
int average, no;
int median, number;
double stdDev;

void *f_avg(void *arg)
{
	int *arr = (int*) arg;
	int a=0;
    for(int i=0; i < n; i++){
        a+=arr[i];
    }
	average = a / (n);
	pthread_exit(0);
}

void *f_min(void *arg)
{
	int *arr = (int*) arg;
	int a = arr[0];
    for(int i=0; i < n; i++){
        if(a > arr[i]){
            a = arr[i];
        }
    }
	minim = a;
	pthread_exit(0);
}

void *f_max(void *arg)
{
	int *arr = (int*) arg;
	int a= arr[0];
    for(int i=1; i < n; i++){
        if(a < arr[i])
            a=arr[i];
    }
	maxim = a;
	pthread_exit(0);
}

void *f_median(void *arg){
    int *arr=(int*) arg;
    sort(arr, arr+n);
    int m;
    if(n%2==0){
        m = arr[n/2]+arr[(n/2)+1];
    }else{
        m = arr[(n+1)/2];
    }
    median=m;
}
void *f_stddev(void *arg){
    int *arr=(int*) arg;
    int av=0;
    for(int i=0; i < n; i++){
        av+=arr[i];
    }
    av/=n;
    double m = 0;
    for( int i=0; i < n; i++ ){
        m+=(arr[i]-av)*(arr[i]-av);
    }
    m/=n;
    double k = sqrt(m);
    stdDev=k;
}

void *f_no(void*arg){
    int *arr=(int*) arg;
    int a=0;
    for(int i=0; i < n; i++){
        a++;
    }
    no=a;
}

int main(int argc, char *argv[])
{
    int i,j;
    int arr[argc-1];
    for(i=1 ; i<argc ; i++ ){
        arr[i-1] = atoi(argv[i]);
    }
    n = argc-1;
    
    pthread_t avg_thread;
	pthread_t min_thread;
	pthread_t max_thread;
	pthread_t med_thread;
	pthread_t std_thread;
	pthread_t no_thread;
	
	
	
	pthread_create(&avg_thread, NULL, f_avg, (void*)arr);
	pthread_create(&min_thread, NULL, f_min, (void*)arr);
	pthread_create(&max_thread, NULL, f_max, (void*)arr);
	pthread_create(&med_thread, NULL, f_median, (void*)arr);
	pthread_create(&std_thread, NULL, f_stddev, (void*)arr);
	pthread_create(&no_thread, NULL, f_no, (void*)arr);
	
	pthread_join(avg_thread, NULL);
	pthread_join(min_thread, NULL);
	pthread_join(max_thread, NULL);
	pthread_join(med_thread, NULL);
	pthread_join(std_thread, NULL);
	pthread_join(no_thread, NULL);
	
	cout<<"The average number is "<<average<<endl;
	cout<<"The maximum number is "<<maxim<<endl;
	cout<<"The minimum number is "<<minim<<endl;
	cout<<"The median number is "<<median<<endl;
	cout<<"The standard deviation is "<<stdDev<<endl;
	cout<<"The total number of inputs are "<<no<<endl;
	
    return 0;
}
