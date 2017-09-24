#include "client.h"
#define BUFLEN 4096

Client::Client() {
    // setup vars
    buf_ = new char[BUFLEN+1];
    socket_name_ = "/tmp/unix-socket";
}

Client::~Client() {
}

void Client::run() {
    // connect server and run
    create();
    echo();
}

void
Client::create() {
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

    // connect server
    if (connect(server_,(const struct sockaddr *)&server_addr,sizeof(server_addr)) < 0) {
        perror("connect");
        exit(-1);
    }
}

void
Client::close_socket() {
}

void
Client::echo() {
    string line;
    string out;
    
    // loop user interface
    while (getline(cin,line)) {
        // append newline
        line += "\n";
        // send request
        bool success = send_request(line);
        // error occurred
        if (not success)
            break;
        // get response
        out = get_response();
        // error occurred
        if (out.empty())
            break;
	cout << out << endl;
    }
    close_socket();
}

bool
Client::send_request(string request) {
    // prep send request
    const char* ptr = request.c_str();
    int nleft = request.length();
    int nwritten;
    // loop all sent
    while (nleft) {
        if ((nwritten = send(server_, ptr, nleft, 0)) < 0) {
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

string
Client::get_response() {
    string response = "";
    // read to newline
    while (response.find("\n") == string::npos) {
        int nread = recv(server_,buf_,BUFLEN,0);
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
        response.append(buf_,nread);
    }
    return response;
}
