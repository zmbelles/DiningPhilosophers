/*
This program gives an example of how threads are created and executed.
To compile: gcc -pthread philosophers.c
To run: ./a.out
*/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

//define constant PHILOSOPHERS as 7
#define PHILOSOPHERS 7

//define number position of adjacent philosophers
#define LEFT (phil_num+PHILOSOPHERS-1)%PHILOSOPHERS
#define RIGHT (phil_num+1)%PHILOSOPHERS

/*number of times each philosopher will take forks, 
eat, and put their forks down before finishing*/
#define CYCLES 100

//define status numbers
#define THINKING 2
#define HUNGRY 1
#define EATING 0

//semaphores for each philosopher
typedef int semaphore;
semaphore s[PHILOSOPHERS];
int philCycles[PHILOSOPHERS];

//i tried making it work without this but pthread_cond_wait wont work without it
pthread_cond_t cond[PHILOSOPHERS];

//a mutex
pthread_mutex_t lock;

void* test(int phil_num){
	//if i am hungry and my adjacent philosophers arent eating, i will eat
	//otherwise i will sleep/continue to sleep
	if(s[phil_num]==HUNGRY && s[LEFT]!=EATING && s[RIGHT]!=EATING){
	   s[phil_num]=EATING;
	   pthread_cond_signal(&cond[phil_num]);
	}
}

void* put_forks(int phil_num){
	//lock the critical region
	pthread_mutex_lock(&lock);

	//change the state from EATING to THINKING
	s[phil_num]=THINKING;

	/*test the left and right philosophers to see if your forks being
	put down would allow them to be awoken and eat*/
	test(LEFT);
	test(RIGHT);

	//unlock the critical region when done
	pthread_mutex_unlock(&lock);
}

void* take_forks(int phil_num){
    int status;

	//lock the critical region
    pthread_mutex_lock(&lock);
	//denote that this philosopher is hungry
	s[phil_num] = HUNGRY;
	//check if this philosopher can eat
	test(phil_num);

	//make the thread wait if it cannot.
	if(s[phil_num]!= EATING){
		pthread_cond_wait(&cond[phil_num],&lock);
	}

	//when spaghetti time, write to philosopher_output.txt the action
	char command[70];
	sprintf(command, "echo 'Spaghetti time for philosopher #%d\n' >> philosopher_output.txt", phil_num);
	status = system(command);

	//unlock the critical region
	pthread_mutex_unlock(&lock);
}
void* thinking(void * tid){
	int status;
    int phil_num = (intptr_t)tid;
	
	//think CYCLES times
	while(philCycles[phil_num]<CYCLES){
		char command[63];
		sprintf(command, "echo 'philosopher #%d is thinking\n' >> philosopher_output.txt", phil_num);
		s[phil_num]=THINKING;

		/*run the command*/
		status = system(command);

		/*after thinking, each philosopher will try to take forks, eat, then put them down
		before the cycle ended*/
		take_forks(phil_num);
		put_forks(phil_num);
		philCycles[phil_num]++;
	}
}


int main(int argc, char * argv[])
{
	//create threads for each philosopher
	pthread_t threads[PHILOSOPHERS];
	int status, i;

	//create a mutex
	if (pthread_mutex_init(&lock, NULL) != 0)
	{
		printf("\n mutex init failed\n");
		return 1;
	}
	//count through 10 threads
	for(i=0; i < PHILOSOPHERS; i++) {
		//run main thread
		printf("Main here. Creating philosopher thread %d\n", i);
		//create 10 threads
		status = pthread_create(&threads[i], NULL, thinking, (void *)(intptr_t)i);
		//check for errors with creating threads
		if (status != 0) {
			printf("Oops. pthread create returned error code %d\n", status);
			exit(-1);
		}
	}
	//have the main thread wait for all the threads created
	for (int j = 0; j < PHILOSOPHERS; j++)
	{
		pthread_join (threads[j], NULL);
	}
	//delete the mutex
	pthread_mutex_destroy(&lock);
	exit(0);
}
