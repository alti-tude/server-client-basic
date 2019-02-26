#include "File.h"
#include "main.h"
#include <fstream>
#include <cstring>

File::File(std::string name){
    fin.open(name, std::fstream::binary|std::fstream::in);
    if(!fin.is_open()){
        std::cerr << "No suh file\n";
        errors.push_back(NOFILE);
    }
    else{
        fin.seekg(0, std::fstream::end);
        length = fin.tellg();
        fin.seekg(0, std::fstream::beg);
    }
}

void File::Send(int sock_fd){
    do{
        if(errors.size()!=0) {
            std::string b = "0";
            send(sock_fd, (char*)b.c_str(), 4, 0);
            return;
        }
        if(!fin.is_open()){
            std::cerr << "No such file\n";
            errors.push_back(NOFILE);
            continue;
        }

        if(fin.tellg()==0){
            std::string b = std::to_string(length);
            send(sock_fd, (char*)b.c_str(), b.size(), 0);            
        }

        char* buf = (char*)malloc(BLOCK_SIZE*sizeof(char));
        fin.read(buf, BLOCK_SIZE);
        int valread = fin.gcount();

        std::string sBuf(buf, valread);
        std::cout << "File Pos = " << fin.tellg() << " Bytes Read = " << fin.gcount() << std::endl;
        LogError(send(sock_fd, (char*)sBuf.c_str(), valread, 0), errorNull, true);

    }while(fin.tellg()!=length && !fin.eof());

    Close();
}

void File::Close(){
    if(fin.is_open())
        fin.close();
}

