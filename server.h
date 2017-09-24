#pragma once

#include <errno.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include "include/BloomFilter.h"

#include <string>

using namespace std;

class Server {
public:
    Server();
    ~Server();

    void run();
    
    void create();
    void close_socket();
    static void interrupt(int);
    void serve();
    void handle(int);
    string get_request(int);
    bool send_response(int, string);

    int server_;
    int buflen_;
    char* buf_;
    char* reply;
    bool retval;
    static const char* socket_name_;
    BloomFilter *bf;
};
