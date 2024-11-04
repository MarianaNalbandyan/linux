#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <cerrno>

int main(int argc, char **argv)
{
	if(argc != 2)
	{
		std::cerr<<"Invalid arguments"<<std::endl;
		return 1;
	}
	
	int fd = open(argv[1], O_WRONLY | O_TRUNC);
	if(fd == -1)
	{
		std::cerr<<"Error:"<<std::endl;
		return errno;
	}

	if(write(fd, "first line\n", 11) < 0)
	{
		std::cerr<<"Error:"<<std::endl;
		close(fd);
		return errno;
	}

	int new_fd = dup(fd);
	if (new_fd == -1)
	{
		std::cerr<<"Error:"<<std::endl;
		return errno;
	}

	if (write(new_fd, "second line\n", 12) < 0)
	{
		std::cerr<<"Error:"<<std::endl;
		close(fd);
		close(new_fd);
		return errno;
	}

	close(new_fd);
	close(fd);
	return 0;
}

