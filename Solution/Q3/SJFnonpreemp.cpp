#include <iostream>

using namespace std;

int main(int argc, char const *argv[])
{	double process[5] = {1,2,3,4,5};
	double temp;
	double arrivalTime[5]={1,1,3,2,4}, burstTime[5]={2,1,8,4,5};
	double tat[5], wat[5], ct[5];
	double avTat=0, avWat=0;
	for(int i=0;i<5;i++){
		for(int j=i+1; j<5;j++){
			if(burstTime[i]>burstTime[j]){
				temp=burstTime[i];
				burstTime[i]=burstTime[j];
				burstTime[j]=temp;

				temp=arrivalTime[i];
				arrivalTime[i]=arrivalTime[j];
				arrivalTime[j]=temp;

				temp=process[i];
				process[i]=process[j];
				process[j]=temp;
			}

		}
	}
	for(int i;i<5;i++){
		ct[i]=0;
	}

	double min = arrivalTime[0];
	int a=0;
	for(int i=0;i<5;i++){
		if(min>arrivalTime[i]){
			min=arrivalTime[i];
			a=i;
		}
	}

	ct[a]=min+burstTime[a];
	cout<<endl;
	for(int i=a+1;i<5;i++){
		ct[i]=ct[i-1] + burstTime[i];
	}

	for(int i=0; i<5;i++){
		tat[i] = ct[i] - arrivalTime[i];
		wat[i] = tat[i] - burstTime[i];
		avTat = avTat + tat[i]/5;
		avWat = avWat + wat[i]/5;
	}
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
