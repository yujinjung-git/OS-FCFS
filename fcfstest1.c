#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#define MAXSIZE 15
int numofpro;
int width;
int gaussianRandom(double average, double stdev) {
	double v1, v2, s, temp;
	srand(time(NULL));
	do {
		v1 =  2 * ((double) rand() / RAND_MAX) - 1;      // -1.0 ~ 1.0 ÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩ ÔøΩÔøΩ
		v2 =  2 * ((double) rand() / RAND_MAX) - 1;      // -1.0 ~ 1.0 ÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩ ÔøΩÔøΩ
		s = v1 * v1 + v2 * v2;
	} while (s >= 1 || s == 0);
	
	s = sqrt( (-2 * log(s)) / s );
	temp = v1 * s;
	temp = (stdev * temp) + average;
	return (int)temp;
}

typedef struct _process {
    int pronum, arrT, burstT, waitT, startT, taT, remainT;
}
Process;

void setProcess2(Process pr[]){
	memset(pr,0,sizeof(pr));
	srand(time(NULL));
	numofpro=(rand()%11)+1;
	int i;
	
	for(i=0; i<numofpro; i++){
		
		pr[i].pronum=i+1;
		srand(time(NULL)+i);
		pr[i].arrT=abs(gaussianRandom(fmod(rand(), 10.0), fmod(rand(), 3.0)));
		srand(i);
		pr[i].burstT=abs(gaussianRandom(fmod(rand(), 10.0), fmod(rand(), 3.0)))+1;
	}
}

void setProcess1(Process pr[]) {
	printf("\n- How many process (1 to 50) -> ");
	scanf("%d", &numofpro);

	int i;

	for (i = 0; i < numofpro; i++) {
		pr[i].pronum = 1 + i;
		printf("\n\n*--- P# %d ---*\n", pr[i].pronum);
		printf("- Arrival time -> "); scanf("%d", &pr[i].arrT);
		printf("- Service time -> "); scanf("%d", &pr[i].burstT);
	}
}


Process fcfssortProcess(Process pr[]){
	int i,j;
	Process temp;
	for(i=0; i<numofpro; i++){
		for(j=0; j<numofpro-1; j++)
			if(pr[j].arrT>pr[j+1].arrT){
				temp=pr[j];
				pr[j]=pr[j+1];
				pr[j+1]=temp;
			}
	}
	return *pr;
}

Process fcfsResortProcess(Process pr[]){
	int i,j;
	Process temp;
	for(i=0; i<numofpro; i++){
		for(j=0; j<numofpro-1; j++)
			if(pr[j].pronum>pr[j+1].pronum){
				temp=pr[j];
				pr[j]=pr[j+1];
				pr[j+1]=temp;
			}
	}
	return *pr;
}


void waitandturnaroundtime(Process pr[]){
	int i;
	int temptaT=0;
	pr[0].waitT=0;
	pr[0].taT=pr[0].arrT+pr[0].burstT;
	for(i=1; i<numofpro; i++){
		temptaT=pr[i-1].taT;
		pr[i].waitT=temptaT-pr[i].arrT;
		if(pr[i].waitT<0){
			pr[i].waitT=0;
			pr[i].taT=pr[i].arrT+pr[i].burstT;
		}
		else{
			pr[i].taT=pr[i-1].taT+pr[i].burstT;
		}
		
	}
}

void avaerageandtotalwait(Process pr[]){
	int i;
	double sum=0.0;
	for(i=0; i<numofpro; i++){
		sum+=pr[i].waitT;
	}
	printf("Total wait time : %d \n", (int)sum);
	printf("Average wait time : %f \n", sum/(double)numofpro);
}


void totalturnaroundtime(Process pr[]){
	int i;
	double sum=width;
	printf("Total turnaround time : %d \n", (int)sum);
	
}

void sheet(int numofpro, Process pr[]){
	int i,j;
	int height=numofpro;
	width=pr[numofpro-1].taT;
	printf(". . . . . FCFS(FIFO) . . . . . (-->Time Quantum,  °ÈProcess Number) \n\n");
	fcfsResortProcess(pr);
	for(i=0; i<height; i++){
		if(pr[i].pronum<10)
			printf("P# %d  ", pr[i].pronum);
		else
			printf("P# %d ", pr[i].pronum);
	    for(j=1; j<=width; j++){
	    	if(j<=pr[i].arrT+pr[i].waitT){
	         	printf("°‡");
			}
			else if((j>pr[i].arrT+pr[i].waitT) && j<=pr[i].taT){
				printf("°·");
			}
			else if(j>pr[i].taT && j<=width){
				printf("°‡");
			}
	    }
	    printf("\n");  
   }

}

void main(){
	Process sjf[MAXSIZE], fcfs[MAXSIZE];
	int i,totalturn;
	int c;
	while(c!=1){
		re:
		printf("Set up a process?(1) / Random process?(2)");
		scanf("%d", &c);
		switch(c){
			case 1 :
				setProcess1(fcfs);
				break;
			case 2 :
				setProcess2(fcfs);
				break;
			default :
				printf("error!\n");
				goto re;
		}
		fcfssortProcess(fcfs);
		waitandturnaroundtime(fcfs);
		totalturn=fcfs[numofpro-1].taT;
		printf("\nProcess Order : ");
		for(i=0; i<numofpro-1; i++){
			printf("%d -> ", fcfs[i].pronum);
		}
		printf("%d\n\n", fcfs[numofpro-1].pronum);
		sheet(numofpro, fcfs);
		
		printf("\n\n");
		for(i=0; i<numofpro; i++){
			printf("P#%d , arr= %d wait= %d burst=%d turn=%d \n", fcfs[i].pronum, fcfs[i].arrT, fcfs[i].waitT, fcfs[i].burstT, fcfs[i].taT);
		}
		printf("\n\n");
		avaerageandtotalwait(fcfs);
		printf("Total Turnaround time : %d\n", totalturn);
		printf("Continue? (y==0/n==1) ");
		scanf("%d", &c);
		printf("---------------------------------------\n");
	}
}
