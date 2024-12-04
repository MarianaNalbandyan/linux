#include <iostream>
#include <dirent.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>
#include <cerrno>
#include <cstdio>
#include <cstdlib>

void readFileContent(const char* filePath){
	int fd = open(filePath, O_RDONLY);
	if(fd < 0){
		std::cerr << "Error opening file:" << filePath << " - " << strerror(errno) << std::endl;
		return;
	}

	char buffer[512];
	ssize_t bytesRead;

	std::cout << "Contents of file:" << filePath << std::endl;
	while ((bytesRead = read(fd, buffer, sizeof(buffer))) > 0){
		write(STDOUT_FILENO, buffer, bytesRead);
	}

	if(bytesRead < 0){
		std::cerr << "Error reading file: " << filePath << " - " << strerror(errno) << std::endl;
	}

	close(fd);
	std::cout << "\n";
}

void traverseDirectory(const char* dirPath){
	DIR* dir = opendir(dirPath);
	if(!dir){
		std::cerr << "Error opening directory: " << dirPath << " - " << strerror(errno) << std::endl;
		return;
	}

	struct dirent* entry;
	while((entry = readdir(dir)) != nullptr){
		if(strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0){
			continue;
		}

		char fullPath[1024];
		snprintf(fullPath,  sizeof(fullPath), "%s/%s", dirPath, entry->d_name);

		struct stat fileStat;
		if(stat(fullPath, &fileStat) < 0){
			std::cerr << "Error getting file stats: " << fullPath << " - " << strerror(errno) << std::endl;
			continue;
		}

		if (S_ISDIR(fileStat.st_mode)){
			traverseDirectory(fullPath);
		} else if(S_ISREG(fileStat.st_mode)){
			readFileContent(fullPath);
		}
	}

	closedir(dir);
}

int main(int argc, char* argv[]){
	if (argc != 2){
		std::cerr << "Usage: " << argv[0] << " <directory_path>" << std::endl;
		return 1;
	}

	traverseDirectory(argv[1]);
	return 0;
}

