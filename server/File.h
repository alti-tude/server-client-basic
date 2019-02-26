#ifndef FILE_H
#define FILE_H

#include "main.h"
#include <fstream>

class File{

public:
    File(std::string name);
    ~File(){};
    void Send(int sock_fd);
    void Close();

private:
    std::string name;
    std::ifstream fin;
    int errorNull;
    int length;
public:
    std::vector<Except> errors;
};
#endif