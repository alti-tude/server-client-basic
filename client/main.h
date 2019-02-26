#ifndef MAIN_H
#define MAIN_H

#include <sys/socket.h>
#include <iostream>
#include <cstring>
#include <string>
#include <netinet/in.h>
#include <unistd.h>
#include <vector>
#include <arpa/inet.h>
#define PORT 8080


#define BLOCK_SIZE 1024
#define LogError(x, b, EXIT) (b) = x;\
    if((b)<0) std::cerr << __FILE__ << " " << __LINE__ << ":: ", perror(#x);\
    if(EXIT && (b)<0) exit(1);

enum Except{NOFILE};
#endif