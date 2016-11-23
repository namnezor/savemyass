#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <semaphore.h>
#include <pthread.h>

#define NUMBER_OF_JOBS 50
#define BUFFER_SIZE 10
#define NUMBER_OF_CONSUMERS 2

sem_t sync;
sem_t delay_consumer;
sem_t delay_producer;

char buffer[BUFFER_SIZE];

int currentBuffer()
{
	for(int i=0; i<BUFFER_SIZE; i++)
	{
		if (buffer[i] == ' ')
		{
			return i;
		}
	}
	return BUFFER_SIZE;
}

void printBuffer()
{
	int items = currentBuffer();
	for(int i=0; i<items; i++)
	{
		printf("*");
	}
	printf("\n");
}

void * producer(void * p)
{
	int items;
	for(int i=0; i<NUMBER_OF_JOBS; i++)
	{
		sem_wait(&sync);

		items = currentBuffer();
		buffer[items++] = '*';
		printf("iIndex = %d ", items);
		printBuffer();
		sem_post(&sync);

		if (items == 1)
		{
			sem_post(&delay_consumer);
		}
		else if(items == BUFFER_SIZE)
		{
			sem_wait(&delay_producer);
		}
	}
	sem_post(&delay_consumer);
}

void * consumer(void * p)
{
	int items;
	sem_wait(&delay_consumer);
	while(1)
	{
		sem_wait(&sync);
		
		items = currentBuffer();
		buffer[--items] = ' ';
		printf("iIndex = %d ", items);
		printBuffer();
		sem_post(&sync);

		if(items == 0)
		{
			sem_wait(&delay_consumer);
		}
		else if(items == BUFFER_SIZE -1)
		{
			sem_post(&delay_producer);
		}
	}
}


int main(int argc, char * argv[])
{
	pthread_t thread_a;
	pthread_t thread_b[NUMBER_OF_CONSUMERS];

	for(int i=0; i<BUFFER_SIZE; i++)
	{
		buffer[i] = ' ';
	}

	sem_init(&sync, 0, 1);
	sem_init(&delay_consumer, 0, 0);
	sem_init(&delay_producer, 0, 0);

	pthread_create(&thread_a, NULL, producer, NULL);
	for(int i=0; i<NUMBER_OF_CONSUMERS; i++)
	{
	pthread_create(&thread_b[i], NULL, consumer, NULL);
	}

	pthread_join(thread_a, NULL);
	for(int i=0; i<NUMBER_OF_CONSUMERS; i++)
	{
	pthread_join(thread_b[i], NULL);
	}

	int syncVal, delaycVal, delaypVal;
	sem_getvalue(&sync, &syncVal);
	sem_getvalue(&delay_consumer, &delaycVal);
	sem_getvalue(&delay_producer, &delaypVal);
	
	printf("Sync = %d, DelayConsumer = %d, DelayProducer = %d\n", syncVal, delaycVal, delaypVal);
}