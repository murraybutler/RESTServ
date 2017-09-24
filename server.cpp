#include "server.h"
#include "include/BloomFilter.h"

#define BUFLEN 4096

const char* Server::socket_name_ = "/tmp/unix-socket";

Server::Server() {
    // setup vars
    buf_ = new char[BUFLEN+1];
    bf = new BloomFilter();
    struct sigaction sigIntHandler;
    sigIntHandler.sa_handler = interrupt;
    sigemptyset(&sigIntHandler.sa_mask);
    sigIntHandler.sa_flags = 0;
    sigaction(SIGINT, &sigIntHandler, NULL);
}

Server::~Server() {
    delete buf_;
}

void
Server::run() {
    // create and run
    create();
    serve();
}

void
Server::create() {
    struct sockaddr_un server_addr;

    // setup socket address struct
    bzero(&server_addr,sizeof(server_addr));
    server_addr.sun_family = AF_UNIX;
    strncpy(server_addr.sun_path,socket_name_,sizeof(server_addr.sun_path) - 1);

    // create socket
    server_ = socket(PF_UNIX,SOCK_STREAM,0);
    if (!server_) {
        perror("socket");
        exit(-1);
    }

    // bind to UNIX file
    if (bind(server_,(const struct sockaddr *)&server_addr,sizeof(server_addr)) < 0) {
        perror("bind");
        exit(-1);
    }

      // convert to listen for incoming
    if (listen(server_,SOMAXCONN) < 0) {
        perror("listen");
        exit(-1);
    }
}

void
Server::close_socket() {
    unlink(socket_name_);
}

void
Server::interrupt(int) {
    unlink(socket_name_);
}

void
Server::serve() {
    // client
    int client;
    struct sockaddr_in client_addr;
    socklen_t clientlen = sizeof(client_addr);

      // accept 
    while ((client = accept(server_,(struct sockaddr *)&client_addr,&clientlen)) > 0) {

        handle(client);
    }
    close_socket();
}

void
Server::handle(int client) {
    // loop requests
    while (1) {
        // get request
        string request = get_request(client);
        string reply = "";
	bool dump = false;
        // break if done or error
        if (request.empty())
            break;
	if (request == "DumpFilter") {
	    reply = bf->DumpStr();
	} else {
            retval = bf->TestAndAdd(request);
            if (retval) {
        	reply = "String exists.\n";
            } else {
        	reply = "String added.\n";
            }
	}
        // response
        bool success = send_response(client,reply);
        // error occurred
        if (not success)
            break;
    }
    close(client);
}

string
Server::get_request(int client) {
    string request = "";
    // read to newline
    while (request.find("\n") == string::npos) {
        int nread = read(client,buf_,BUFLEN);
        if (nread < 0) {
            if (errno == EINTR)
                // interrupted -- try again
                continue;
            else
                // error occurred
                return "";
        } else if (nread == 0) {
            // socket closed
            return "";
        }
        // append in case of binary data
        request.append(buf_,nread);
    }
    return request;
}

bool
Server::send_response(int client, string response) {
    // prep response
    const char* ptr = response.c_str();
    int nleft = response.length();
    int nwritten;

    // loop sent
    while (nleft) {
        if ((nwritten = send(client, ptr, nleft, 0)) < 0) {
            if (errno == EINTR) {
                // interrupted -- try again
                continue;
            } else {
                // error occurred
                perror("write");
                return false;
            }
        } else if (nwritten == 0) {
            // socket closed
            return false;
        }
        nleft -= nwritten;
        ptr += nwritten;
    }
    return true;
}
