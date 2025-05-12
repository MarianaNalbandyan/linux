#include <iostream>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unisth.h>
#include <pthread.h>
#include <sstream>
#include <string>

struct Account {
	pthread_mutex_t mutex;
	int balance;
	int min_balance;
	int max_balance;
	bool freeze;
};

const char* shm_name = "/bank_open";

int main() {
	int fd = shm_open(shm_name, O_RDWR, 0666);
	if (fd == -1) {
		perror("shm_open");
		return 1;
	}

	size_t shm_size = 0;
	struct stat sb;
	if (fstat(fd, &sb) == -1) {
		perror("fstat");
		return 1;
	}
	shm_size = sb.st_size;

	void* ptr = mmap(nullptr, shm_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if (ptr == MAP_FAILED) {
		perror("mmap");
		return 1;
	}

	close(fd);

	int* counter = static_cast<int*>(ptr);
	int N = *counter;
	Account* accounts = reinterpret_cast<Account*>(counter + 1);

	std::string line;
	while (std::getline(std::cin, line)) {
		std::istringstream iss(line);
		std::string cmd;
		iss >> cmd;

		if (cmd == "PRINT") {
			std::string what;
			int a;
			iss >> what >> a;

			if (a < 0 || a >= N) {
				std::cout << "Ошибка: номер счета вне диапазона\n";
				continue;
			}
		}
		else if (cmd == "FREEZE" || cmd == "UNFREEZE") {
			int a;
			iss >> a;

			if (a < 0 || a >= N) {
				std::cout << "Ошибка: номер счёета вне диапазона\n";
				continue;
			}

			pthread_mutex_lock(&accounts[a].mutex);

			accounts[a].freeze = (cmd == "FREEZE");
			std::cout << "Счёт" << a << " " << (cmd == "FREEZE" ? "заморожен" : "разморожен") << std::endl;

			pthread_mutex_unlock(&accounts[a].mutex);
		}
		else if (cmd == "TRANSFER") {
			int from, to, x;
			iss >> from >> to >> x;

			if (from < 0 || from >= N || to < 0 || to >= N) {
				std::cout << "Ошибка: некорректный номер счёта\n";
				continue;
			}

			if (x <= 0) {
				std::cout << "Ошибка: сумма перевода должна быть положительной\n";
				continue;
			}

			if (from == to) {
				std::cout << "Ошибка: нельзя переводить на тот же счёт\n";
				continue;
			}

			int first = std::min(from, to);
			int second = std::max(from, to);

			pthread_mutex_lock(&accounts[first].mutex);
			pthread_mutex_lock(&accounts[second].mutex);

			if (accounts[from].freeze || accounts[to].freeze) {
				std::cout << "Ошибка: один из счетов заморожен\n";
			}
			else if (accounts[from].balance - x < accounts[from].min_balance) {
				std::cout << "Ошибка: недостаточно средств на счёте " << from << std::endl;
			}
			else if (accounts[to].balance + x > accounts[to].max_balance) {
				std::cout << "Ошибка: превышен лимит на счёте " << to << std::endl;
			}
			else {
				accounts[from].balance -= x;
				accounts[to].balance += x;
				std::cout << "Успешный перевод " << x << "со счёта " << from << "на счёт" << to << std::endl;
			}

			pthread_mutex_unlock(&accounts[second].mutex);
			pthread_mutex_unlock(&accounts[first].mutex);
		}
		else if (cmd == "ADDALL" || cmd == "SUBALL") {
			int x;
			iss >> x;

			if (x <= 0) {
				std::cout << "Ошибка: значение должно быть положительным\n";
				continue;
			}

			bool ok = true;

			for (int i = 0; i < N; ++i)
				pthread_mutex_lock(&accounts[i].mutex);

			for (int i = 0; i < N; ++i) {
				if (accounts[i].freeze) {
					std::cout << "Ошибка: счёт " << i << "заморожен\n";
					ok = false;
					break;
				}
				
				if (cmd == "ADDALL") {
					if (accounts[i].balance + x > accounts[i].max_balance) {
						std::cout << "Ошибка: превышен лимит на счёте " << i << "\n";
						ok = false;
						break;
					}
				}
				else {
					if (accounts[i].balance - x < accounts[i].min_balance) {
						std::cout << "Ошибка: недостаточно средств на счёте " << i << "\n";
						ok = false;
						break;
					}
				}
			}

			if (ok) {
				for (int i = 0; i < N; ++i) {
					if (cmd == "ADDALL") {
						accounts[i].balance += x;
					}
					else {
						accounts[i].balance -= x;
					}
				}
				std::cout << "Операция " << cmd << " " << x << "выполнена успешно для всех счётов\n";
			}
			for (int i = 0; i < N; ++i)
				pthread_mutex_unlock(&accounts[i].mutex);
		}
		else if (cmd == "SETMIN" || cmd == "SETMAX") {
			int a, x;
			iss >> a >> x;

			if (a < 0 || a >= N) {
				std::cout << "Ошибка: номер счёта вне диапазона\n";
				continue;
			}

			pthread_mutex_lock(&accounts[a].mutex);

			if (cmd == "SETMIN") {
				if (x > accounts[a].max_balance) {
					std::cout << "Ошибка: min больше max\n";
				}
				else if(x > accounts[a].balance) {
					std::cout << "Ошибка: текущий баланс меньше нового min\n";
				}
				else {
					accounts[a].min_balance = x;
					std::cout << "Установлен новый min для счёта " << a << ": " << x << std::endl;
				}
			}
			else {
				if (x < accounts[a].min_balance) {
					std::cout << "Ошибка: max меньше min\n";
				}
				else if (x < accounts[a].balance) {
					std::cout << "Ошибка: текущий баланс больше нового max\n";
				}
				else {
					accounts[a].max_balance = x;
					std::cout << "Установлен новый max для счёта " << a << ": " << x << std::endl;
				}
			}
			pthread_mutex_unlock(&accounts[a].mutex);
		}
	}

	return 0;
}