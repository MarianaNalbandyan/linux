#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <cerrno>
#include <sys/stat.h>

int main(int argc, char **argv)
{
	if(argc == 2)
	{
		struct stat fileStat;
		if(stat(argv[1], &fileStat) == -1)
		{
			std::cout<<"Error:"<<errno<<std::endl;
			return errno;
		}
		int size = fileStat.st_size;
		int fd = open(argv[1], O_WRONLY);
		if(fs == -1)
		{
			std::cout<<"Error:"<<errno<<std::endl;
			return errno;
		}
		if (lseek(fd, 0, SEEK_SET) == -1)
		{
			std::cout<<"Error:"<<errno<<std::endl;
			return errno;
		}
	       
	        if(write(fd, "\0", size) == -1)
		{
			std::cout<<"Error:"<<errno<<std::endl;
			close(fd);
			return errno;
		}
		
		close(fd);
		if(unlink(argv[1]) == -1)
		{
			std::cout<<"Error:"<<errno<<std::endl;
			return errno;
		}
	}
	else
	{
		std::cout<<"Invalid arguments"<<std::endl;
		return 1;
	}
	std::cout<<"File has been successfully deleted"<<std::endl;
	return 0;
}


