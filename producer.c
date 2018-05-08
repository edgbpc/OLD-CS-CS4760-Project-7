//Author: Eric Goodwin
//Project 2 - Current Unix processes
//Class: OS, Evening Section

#include "oss.h"

char * printTimeString();

FILE *fp;

time_t current_time;
struct tm * time_info;
char timeString[9];


int main (int argc, char *argv[]){
	int numProcesses = atoi(argv[1]);
	int index; //for counting

	printf("In producer\n");
	fp = fopen("producer.log", "w");


	//shared memory keys
	turnKey = 59566;
	bufferKey = 59567;
	flagKey = 59562;
	processesKey = 59564;
	//key_t keyEOFFlag = 59563;

	shmidTurn = shmget(turnKey, SHM_SIZE, 0666);
	shmidBuffer = shmget(bufferKey, 5*(sizeof(Buffer)), 0666);
	shmidFlag = shmget(flagKey, 18*SHM_SIZE, 0666);
	shmidProcesses = shmget(processesKey, 18*(sizeof(Process)), 0666);

	//int shmidEOFFlag = shmget(keyEOFFlag, SHM_SIZE, 0777);


	int * turn = (int *) (shmat(shmidTurn, 0, 0));
	//int * EOFFlag = (int *) (shmat(shmidEOFFlag, 0, 0));
	state * flag = (state * ) (shmat(shmidFlag, 0, 0));
	Buffer * bufferTable = (Buffer*)(shmat (shmidBuffer, 0, 0));
	Process * processes = (Process *)(shmat(shmidProcesses, 0, 0));

	//set each buffer flag to empty


	double time_spent;
	char str[BUFFERSIZE];


	time_t timeSeed;
	srand((int)time(&timeSeed) % getpid()); //%getpid used because children were all getting the same "random" time to run. 




	//fprintf(fp, "%s\tStarting\n", timeString = printTimeString());
	printf("Start of Produer\n");

	printf("Process %d is a producer = %d\n and consumer is %d\n", getpid(), processes[0].producer, processes[0].consumer);
	//printf("argv[1] is %s\n", argv[1]);


	//signal(SIGINT, handle);

	int i = 0;
	int j;
	int n = numProcesses;

	
	//while (fgets(str, BUFFERSIZE, stdin) != NULL){
	do {
		do{
			flag[i] = want_in;
			j = *turn;
			while (j != i)
				j = (flag[j] != idle ) ? *turn : (j + 1 ) % n;
			flag[i] = in_cs;
			for (j = 0; j < n; j++)
				if ((j != i) && (flag[j] == in_cs))
					break;
		} while ((j < n ) || ( *turn != i && flag[*turn] != idle));
		*turn = i;
		//critical section
		//
		while (fgets(str, BUFFERSIZE, stdin) != NULL){

			if (bufferTable[0].isFull == false){
				calculateTime();	
				strcpy(bufferTable[0].data, str);
				bufferTable[0].isFull = true;
				printf("Wrote to buffer 0\n");
				fprintf(fp, "%s\tWrite\t0\t%s\n", timeString, bufferTable[0].data);
				fflush(fp);
				continue;
			} else if (bufferTable[1].isFull == false) {
				calculateTime();	
	
				strcpy(bufferTable[1].data, str);
				bufferTable[1].isFull = true;
				printf("Wrote to buffer 1\n");
				fprintf(fp, "%s\tWrite\t1\t%s\n", timeString, bufferTable[1].data);
				fflush(fp);
				continue;
			} else if (bufferTable[2].isFull == false) {
				calculateTime();	

				strcpy(bufferTable[2].data, str);
				bufferTable[2].isFull = true;
				printf("Wrote to buffer 2\n");
				fprintf(fp, "%s\tWrite\t2\t%s\n", timeString, bufferTable[2].data);
				fflush(fp);
				continue;
			} else if (bufferTable[3].isFull == false) {
				calculateTime();	

				strcpy(bufferTable[3].data, str);
				bufferTable[3].isFull = true;
				printf("Wrote to buffer 3\n");
				fprintf(fp, "%s\tWrite\t3\t%s\n", timeString, bufferTable[3].data);
				fflush(fp);
				continue;
			} else if (bufferTable[4].isFull == false) {
				calculateTime();	

				strcpy(bufferTable[4].data, str);
				bufferTable[4].isFull = true;
				printf("Wrote to buffer 4\n");
				fprintf(fp, "%s\tWrite\t4\t%s\n", timeString, bufferTable[4].data);
				fflush(fp);
				continue;
			} else {
				break;
			}
		}
		//Exit section
		
		j = (*turn + 1) % n;
		while (flag[j] == idle)
			j = (j + 1) % n;

		//assign turn to next waiting process; change own flag to idle
		*turn = j; flag[i] = idle;

		//remainder_Section()
		int randSleep = rand() % 10 + 1;
		printf("Process %d is sleeping for %d\n", getpid(), randSleep);
		sleep(randSleep);
	} while(1);


}



/// functions

void calculateTime(){
	time(&current_time);
	time_info = localtime(&current_time);
	strftime(timeString, sizeof(timeString), "%H:%M:%S", time_info);
}
