#include <iostream>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>

int main() {
	const char* shm_name = "/bank_open";
	if (shm_unlink(shm_name) == -1) {
		perror("shm_unlink");
		return 1;
	}
	std::cout << "bank delited" << std::endl;
	return 0;
}