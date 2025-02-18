#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <signal.h>
#include <time.h>
#include <string.h>
#include <errno.h>
#include <sys/mman.h>

#define N 3

typedef struct{
	sem_t tobacco, paper, matches;
	sem_t barmen;
	char input[100];
	pid_t smokers[N];
	int finished_smokers;
} shared_data;

void sigterm_handler(int signum){
	exit(0);
}

void smoker(shared_data *data, int id, sem_t *resource){
	signal(SIGTERM, sigterm_handler);
	while(1) {
		sem_wait(resource);
		printf("smoker %d is smoking\n", id);
		fflush(stdout);
		sleep(1);
		sem_post(&data->barmen);
	}
}

void bartender(shared_data *data){
	for(size_t i = 0; i < strlen(data->input); i++){
		sem_wait(&data->barmen);
		printf("Bartender is providing: %c\n", data->input[i]);
		switch(data->input[i]){
			case 't': sem_post(&data->tobacco); break;
		        case 'p': sem_post(&data->paper); break;
			case 'm': sem_post(&data->matches); break;
		}
	}
	sleep(1);
	for(int i = 0; i < N; i++){
		kill(data->smokers[i], SIGTERM);
		waitpid(data->smokers[i], NULL, 0);
	}
	exit(EXIT_SUCCESS);
}

int main(int argc, char *argv[]){
	if(argc != 2){
		fprintf(stderr, "Usage: %s <resource_sequence>\n", argv[0]);
		return 1;
	}

	shared_data *data = (shared_data *)mmap(NULL, sizeof(shared_data), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
	if(data == MAP_FAILED){
		perror("mmap error");
		exit(EXIT_FAILURE);
	}

	strcpy(data->input, argv[1]);
	data->finished_smokers = 0;
	sem_init(&data->tobacco, 1, 0);
	sem_init(&data->paper, 1, 0);
	sem_init(&data->matches, 1, 0);
	sem_init(&data->barmen, 1, 1);

	for(int i = 0; i < N; i++){
		data->smokers[i] = fork();
		if(data -> smokers[i] == 0){
			if (i == 0) smoker(data, 0, &data->tobacco);
			if (i == 1) smoker(data, 1, &data->paper);
			if (i == 2) smoker(data, 2, &data->matches);
			exit(EXIT_SUCCESS);
		}
	}

	bartender(data);

	for(int i = 0; i < N; i++) wait(NULL);
	sem_destroy(&data->tobacco);
	sem_destroy(&data->paper);
	sem_destroy(&data->matches);
	sem_destroy(&data->barmen);
	munmap(data, sizeof(shared_data));
	return 0;
}

