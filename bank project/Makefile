CXX = g++ 
CXXFLAGS = -Wall -pthread -std=c++17
LDFLAGS = -ltr

TARGETS = bank_init bank_client bank_destroy

all: $(TARGETS)

bank_init: bank_init.cpp
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o bank_init bank_init.cpp

bank_client: bank_client.cpp
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o bank_client bank_client.cpp

bank_destroy: bank_destroy.cpp
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o bank_destroy bank_destroy.cpp

clean:
	rm -f $(TARGETS)