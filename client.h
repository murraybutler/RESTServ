#pragma once

#include <errno.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#include <fstream>
#include <iostream>
#include <string>

using namespace std;

class Client {
public:
    Client();
    ~Client();

    void run();
    bool send_request(string);
    string get_response();
    void echo();
    void create();
    void close_socket();

    int server_;
    int buflen_;
    char* buf_;

private:
    const char* socket_name_;
};
