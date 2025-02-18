#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <time.h>

#define N 5

sem_t sticks[N];

void think(int id){
	printf("Philosopher %d thinking\n", id);
	sleep(rand() % 3 + 1);
}

void eat(int id){
	printf("Philosopher %d eat\n", id);
	sleep(rand() % 2 + 1);
}

void philosopher(int id){
	srand(time(NULL) ^ (getpid() << 16));

	while(1) {
		think(id);

		if(id == 0){
			sem_wait(&sticks[(id + 1) % N]);
			sem_wait(&sticks[id]);
		} else{
			sem_wait(&sticks[id]);
			sem_wait(&sticks[(id + 1) % N]);
		}

		eat(id);

		sem_post(&sticks[id]);
		sem_post(&sticks[(id + 1) % N]);
	}
}

int main(){
	pid_t pids[N];

	for (int i = 0; i < N; i++){
		sem_init(&sticks[i], 1, 1);
	}

	for (int i = 0; i < N; i++){
		pids[i] = fork();
		if (pids[i] == 0){
			philosopher(i);
			exit(0);
		}
	}

	for (int i = 0; i < N; i++){
		wait(NULL);
	}

	for (int i = 0; i < N; i++){
		sem_destroy(&sticks[i]);
	}
	return 0;
}


