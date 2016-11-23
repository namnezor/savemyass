#include <stdio.h>
#include <sys/time.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>

#define NUMBER_OF_JOBS 1000

int items = 0;

int jobsp = NUMBER_OF_JOBS;
int jobsc = NUMBER_OF_JOBS;

void* producer(void* val);
void* consumer(void* val);

sem_t sync;
sem_t delay_consumer;

void * producer(void * p)
{
	while(1)
	{
		if(jobsp==0)
			pthread_exit(NULL);
		sem_wait(&sync);
		jobsp--;
		items++;
		printf("iIndex = %d\n", items);
		if (items == 1)
		{
			sem_post(&delay_consumer);
		}
		sem_post(&sync);
		
	}
}

void * consumer(void * p)
{

	sem_wait(&delay_consumer);
	while(1)
	{
		sem_wait(&sync);
		items--;
		int temp = items;
		jobsc--;
		printf("iIndex = %d\n", items);
		sem_post(&sync);
		if(jobsc==0)
			pthread_exit(NULL);
		if(temp == 0)
		{
			sem_wait(&delay_consumer);
		}
	}
}


int main(int argc, char * argv[])
{
	pthread_t thread_a;
	pthread_t thread_b;

	sem_init(&sync, 0, 1);
	sem_init(&delay_consumer, 0, 0);

	pthread_create(&thread_a, NULL, producer, NULL);
	pthread_create(&thread_b, NULL, consumer, NULL);

	pthread_join(thread_a, NULL);
	pthread_join(thread_b, NULL);

	int syncVal, delayVal;
	sem_getvalue(&sync, &syncVal);
	sem_getvalue(&delay_consumer, &delayVal);
	
	printf("sSync = %d, sDelayConsumer = %d\n", syncVal, delayVal);

	exit(0);
}