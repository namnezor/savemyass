#include <stdio.h>
#include <sys/time.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>

sem_t s;
int sum = 0;
void * calc(void * number_of_increments)
{ int i;
for(i = 0; i < *((int*) number_of_increments);i++)
{ sem_wait(&s);
sum++;
sem_post(&s);
}
}
void main()
{ int iterations = 50000000;
pthread_t tid1,tid2;
sem_init(&s,0,1);
// no error checking for clarity/brevity
pthread_create(&tid1, NULL, calc, (void *) &iterations);
pthread_create(&tid2, NULL, calc, (void *) &iterations);
pthread_join(tid1,NULL);
pthread_join(tid2,NULL);
printf("The value of sum is: %d\n", sum);
}