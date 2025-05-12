#include <iostream>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include <cstring>

struct Account {
	pthread_mutex_t mutex;
	int balance;
	int min_balance;
	int max_balance;
	bool freeze;
};
	
const char* shm_name = "/bank_open";

int main(int argc, char** argv) {
	if (argc != 2) {
		std::cerr << "Usage: " << argv[0] << "<number_of_accounts>\n";
		return 1;
	}

	int fd = shm_open(shm_name, O_CREAT | O_RDWR, 0666);
	if (fd == -1) {
		perror("shm_open");
		return 1;
	}

	int N = std::atoi(argv[1]);
	size_t shm_size = sizeof(int) + sizeof(Account) * N;

	if (ftruncate(fd, shm_size) == -1) {
		perror("ftruncate");
		return 1;
	}

	void* ptr = mmap(nullptr, shm_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if (ptr == MAP_FAILED) {
		perror("mmap");
		return 1;
	}

	int* counter = static_cast<int*>(ptr);
	*counter = N;
	
	Account* accounts = reinterpret_cast<Account*>(counter + 1);
	
	pthread_mutexattr_t attr;
	pthread_mutexattr_init(&attr);
	pthread_mutexattr_setpshared(&attr, PTHREAD_PROCESS_SHARED);
	
	for (int i = 0; i < N; ++i) {
		pthread_mutex_init(&accounts[i].mutex, &attr);

		accounts[i].balance = 0;
		accounts[i].min_balance = 0;
		accounts[i].max_balance = 10000000;
		accounts[i].freeze = false;
	}

	pthread_mutexattr_destroy(&attr);
	close(fd);
	return 0;
}