#Compiler and flags
CXXFLAGS := -O3 -std=c++11 -Llib/ -Iinclude/
LINKFLAGS := -lpistache -lBloomFilter -lcityhash -lpthread
#Objects
CXX= g++ $(CXXFLAGS)

UNIX-SERVER= UNIXServ.o server.o
UNIX-CLIENT= UNIXClient.o client.o

#Main target
all: libBloomFilter.so UNIXServ UNIXClient RESTServ

libBloomFilter.so:BloomFilter.cpp
	$(CXX) -fPIC -shared $^ -o lib/$@ -lcityhash

UNIXServ:$(UNIX-SERVER)
	   $(CXX) -o UNIXServ $(UNIX-SERVER) $(LINKFLAGS)

UNIXClient:$(UNIX-CLIENT)
	   $(CXX) -o UNIXClient $(UNIX-CLIENT) $(LINKFLAGS)

RESTServ:RESTServ.cpp
	$(CXX) $^ -o RESTServ $(LINKFLAGS)
 
clean:
	rm -rf *.o lib/libBloomFilter.so UNIXClient UNIXServ RESTServ

