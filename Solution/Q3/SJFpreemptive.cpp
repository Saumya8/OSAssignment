#include <iostream>

using namespace std;

int main(int argc, char const *argv[])
{
	double process[5] = {1,2,3,4,5};
	double arrivalTime[5]={1,1,3,2,4}, burstTime[5]={2,1,8,4,5}, temp_bt[5];
	double tat[5], wat[5], ct[5];
	double avTat=0, avWat=0;
	for(int i=0; i<5; i++){
		temp_bt[i]=burstTime[i];
	}
	int c = 0, t = 0, min = 30; 
	int minimum = 0, finishTime; 
	int flag = 0; 

	while (c!=5) { 
		for (int i = 0; i < 5; i++) { 
			if ((arrivalTime[i] <= t) && 
			(temp_bt[i] < min) && temp_bt[i] > 0) { 
				min = temp_bt[i]; 
				minimum = i; 
				flag = 1; 
			} 
		}
		if (flag == 0) { 
			t++; 
			continue; 
		} 
		temp_bt[minimum]--; 
		min = temp_bt[minimum]; 
		if (min == 0) 
			min = 30; 
 
		if (temp_bt[minimum] == 0) {  
			c++; 
			flag = 0;  
			finishTime = t + 1; 

			wat[minimum] = finishTime - 
						burstTime[minimum] - arrivalTime[minimum]; 

			if (wat[minimum] < 0) 
				wat[minimum] = 0; 
		} 
		t++; 
	}
	for (int i = 0; i < 5; i++){ 
		tat[i] = burstTime[i] + wat[i]; 
	}

	for(int i=0; i<5;i++){
		avTat = avTat + tat[i]/5;
		avWat = avWat + wat[i]/5;
	}
	int temp;
	for(int i=0;i<5;i++){
		for(int j=i+1; j<5;j++){
			if(process[i]>process[j]){
				temp=burstTime[i];
				burstTime[i]=burstTime[j];
				burstTime[j]=temp;

				temp=arrivalTime[i];
				arrivalTime[i]=arrivalTime[j];
				arrivalTime[j]=temp;

				temp=process[i];
				process[i]=process[j];
				process[j]=temp;

				temp=tat[i];
				tat[i]=tat[j];
				tat[j]=temp;


				temp=wat[i];
				wat[i]=wat[j];
				wat[j]=temp;

			}

		}
	}

	cout<<endl;
	for(int i=0;i<5;i++){
	    	cout << process[i]<< " ";
	}
	
	cout<<endl;
	cout<< "Turn Around Time\n";
	for(int i=0; i<5; i++){
		cout<< tat[i] << " ";
	};
	cout <<endl;
	cout <<"Waiting time\n";
	for(int i=0; i<5; i++){
		cout<< wat[i] << " ";
	};
	cout<<endl;
	cout<< "Average Turnaround time: " <<  avTat << endl;
	cout<<"Average waiting time: "<<  avWat << endl;
	return 0;
}
