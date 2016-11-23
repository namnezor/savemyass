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
#define TIME_SLICE 25

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
	for(int i = 0; i < NUMBER_OF_JOBS; i++)
	{ 
		aiJobs[i][START_TIME] = time;
		time += aiJobs[i][BURST_TIME];
	}

	for(int i = 0; i < NUMBER_OF_JOBS; i++)
	{
		aiJobs[i][END_TIME] = aiJobs[i][START_TIME] + aiJobs[i][BURST_TIME];
	}
}

void generateAverages()
{
	float totalStartTime;
	float totalEndTime;
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

void generateRoundRobin()
{
	int time = 0;
	printf("\nROUND ROBIN; time Slice: 25,\n");

	for(int i = 0; i < NUMBER_OF_JOBS; i++)
	{
		aiJobs[i][START_TIME] = time;
		time += aiJobs[i][BURST_TIME];
	}

	for(int i = 0; i < NUMBER_OF_JOBS; i++)
	{
		aiJobs[i][END_TIME] = aiJobs[i][START_TIME] + aiJobs[i][BURST_TIME];
	}

	for (int i = 0; i < NUMBER_OF_JOBS;)
	{
		int NumSamePriority = 0;
		for(int j = i; j < NUMBER_OF_JOBS; j++)
		{
			if(aiJobs[i][PRIORITY] == aiJobs[j][PRIORITY])
				NumSamePriority++;
		}
		
		int jobsCurrentlyRunning = NumSamePriority;
		while(jobsCurrentlyRunning > 0)
		{
			for(int j = i; j < i + NumSamePriority; j++)
			{
				int jobRunTime = aiJobs[j][REMAINING_TIME];
				if(jobRunTime == 0)
					continue;
				if(jobsCurrentlyRunning > 1 && jobRunTime > TIME_SLICE)
					jobRunTime = TIME_SLICE;
				printf("JOB ID = %d, %s time = %d, %s time = %d, Priority =  %d\n",
					aiJobs[j][JOB_INDEX], 
					aiJobs[j][BURST_TIME] ==  aiJobs[j][REMAINING_TIME] ? "Start" : "re-start",
					aiJobs[j][START_TIME], 
					aiJobs[j][REMAINING_TIME] - jobRunTime == 0 ? "End" : "Remaining",
					aiJobs[j][REMAINING_TIME] - jobRunTime == 0 ? aiJobs[j][END_TIME] : aiJobs[j][REMAINING_TIME],
					aiJobs[j][PRIORITY]);
				aiJobs[j][REMAINING_TIME] -= jobRunTime;
				if(aiJobs[j][REMAINING_TIME] == 0)
					jobsCurrentlyRunning--;
			}
		}
		i += NumSamePriority;
	}
}

void printJob(int iId, int iBurstTime, int iRemainingTime, int iPriority)
{
	printf("Id = %d, Burst Time = %d, Remaining Time = %d, Priority = %d\n", iId, iBurstTime, iRemainingTime, iPriority);
}

void printJobs()
{
	printf("JOBS: \n");
	for(int i = 0; i < NUMBER_OF_JOBS; i++)
		printJob(aiJobs[i][JOB_INDEX], aiJobs[i][BURST_TIME], aiJobs[i][REMAINING_TIME], aiJobs[i][PRIORITY]);
}

int cmpfunc (const void * a, const void * b)
{
   return ((int*)a)[PRIORITY] - ((int*)b)[PRIORITY];
}

void printSortPriority()
{
	printf("\nSorting by priority:\nJOBS:\n");
	for(int i = 0; i < NUMBER_OF_JOBS; i++)
		printJob(aiJobs[i][JOB_INDEX], aiJobs[i][BURST_TIME], aiJobs[i][REMAINING_TIME], aiJobs[i][PRIORITY]);
}

void printAverages()
{
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
	generateAverages();
	qsort(aiJobs, NUMBER_OF_JOBS, 6 * sizeof(int), cmpfunc);
	printSortPriority();
	generateRoundRobin();
	printAverages();

	return 0;
}