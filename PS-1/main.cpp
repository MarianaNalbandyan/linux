#include <iosdtrem>
#include <sys/wait.h>
#include <unistd.h>

void do_command(char **argv)
{
	pid_t pid = fork();
	if (pid < 0){
		std::cout<<"Fork failed" << std::endl;
		return;
	} else if (pid == 0){
		execvp(argv[0], argv);
		std::cerr << "Execution failed" << std::endl;
		exit(1);
	} else {
		wait(NULL);
	}
}

int main(int argc, char **argv)
{
	if(argc == 1){
		std::cerr << "Invalid arguments" << std::endl;
		return 1;
	}
	do_command(argv + 1);
	return 0;
}

