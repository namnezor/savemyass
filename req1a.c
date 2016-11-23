#include <stdio.h>
#include <sys/time.h>
#include <stdlib.h>

#define NUMBER_OF_JOBS 10
#define JOB_INDEX 0
#define BURST_TIME 1
#define REMAINING_TIME 2
#define PRIORITY 3
#define START_TIME 4
#define END_TIME 5

int aiJobs[NUMBER_OF_JOBS][6];
float averageStartTime;
float averageEndTime;


void generateJobs()
{
	int i;

	for(i = 0; i < NUMBER_OF_JOBS;i++)
	{
		aiJobs[i][JOB_INDEX] = i;
		aiJobs[i][BURST_TIME] = rand() % 99 + 1;
		aiJobs[i][REMAINING_TIME] = aiJobs[i][BURST_TIME];
		aiJobs[i][PRIORITY] = rand()%10;
		aiJobs[i][START_TIME] = 0;
		aiJobs[i][END_TIME] = 0;
	}
}

void generateFcfs()
{
	int time = 0;
	float totalStartTime;
	float totalEndTime;

	for(int i = 0; i < NUMBER_OF_JOBS; i++)
	{
		aiJobs[i][START_TIME] = time;
		time += aiJobs[i][BURST_TIME];
	}

	for(int i = 0; i < NUMBER_OF_JOBS; i++)
	{
		aiJobs[i][END_TIME] = aiJobs[i][START_TIME] + aiJobs[i][BURST_TIME];
	}

	for(int i = 0; i < NUMBER_OF_JOBS; i++)
	{
		totalStartTime = totalStartTime + aiJobs[i][START_TIME];
	}
	averageStartTime = totalStartTime / NUMBER_OF_JOBS;

	for(int i = 0; i < NUMBER_OF_JOBS; i++)
	{
		totalEndTime = totalEndTime + aiJobs[i][END_TIME];
	}
	averageEndTime = totalEndTime / NUMBER_OF_JOBS;

}

void printJob(int iId, int iBurstTime, int iRemainingTime, int iPriority)
{
	printf("Id = %d, Burst Time = %d, Remaining Time = %d, Priority = %d\n", iId, iBurstTime, iRemainingTime, iPriority);
}

void printJobs()
{
	int i;
	printf("JOBS: \n");
	for(i = 0; i < NUMBER_OF_JOBS; i++)
		printJob(aiJobs[i][JOB_INDEX], aiJobs[i][BURST_TIME], aiJobs[i][REMAINING_TIME], aiJobs[i][PRIORITY]);
}

void printFcfs(int iId, int iStartTime, int iEndTime)
{
	printf("Id = %d, Start Time = %d, End Time = %d\n", iId, iStartTime, iEndTime);
}

void printFcfss()
{
	int i;
	printf("\nFCFS:\n");
	for(i = 0; i < NUMBER_OF_JOBS; i++)
		printFcfs(aiJobs[i][JOB_INDEX], aiJobs[i][START_TIME], aiJobs[i][END_TIME]);
	printf("Average Response Time: %f\n", averageStartTime);
	printf("Average Turnaround Time: %f\n", averageEndTime);
}

long int getDifferenceInMilliSeconds(struct timeval start, struct timeval end)
{
	int iSeconds = end.tv_sec - start.tv_sec;
	int iUSeconds = end.tv_usec - start.tv_usec;
 	long int mtime = (iSeconds * 1000 + iUSeconds / 1000.0);
	return mtime;
}

void simulateJob(int iTime)
{
	long int iDifference = 0;
	struct timeval startTime, currentTime;
	gettimeofday(&startTime, NULL);
	do
	{
		gettimeofday(&currentTime, NULL);
		iDifference = getDifferenceInMilliSeconds(startTime, currentTime);
	} while(iDifference < iTime);
}



int main()
{
	generateJobs();
	printJobs();
	generateFcfs();
	printFcfss();

	return 0;
}
