#Compiler and flags
CXXFLAGS := -O3 -std=c++11 -Llib/ -Iinclude/
LINKFLAGS := -lpistache -lBloomFilter -lcityhash -lpthread
#Objects
CXX= g++ $(CXXFLAGS)

UNIX-ECHO-SERVER= unix-echo-server.o server.o
UNIX-ECHO-CLIENT= unix-echo-client.o client.o

#Main target
all: libBloomFilter.so unix-echo-server unix-echo-client RESTServ

libBloomFilter.so:BloomFilter.cpp
	$(CXX) -fPIC -shared $^ -o lib/$@ -lcityhash

unix-echo-server:$(UNIX-ECHO-SERVER)
	   $(CXX) -o server $(UNIX-ECHO-SERVER) $(LINKFLAGS)

unix-echo-client:$(UNIX-ECHO-CLIENT)
	   $(CXX) -o client $(UNIX-ECHO-CLIENT) $(LINKFLAGS)

RESTServ:RESTServ.cpp
	$(CXX) $^ -o RESTServ $(LINKFLAGS)
 
clean:
	rm -rf *.o lib/libBloomFilter.so client server RESTServ

