#include "main.h"
#include "File.h"
#include <fstream>

int errorNull;
void listall(){
    DIR* dir;
    struct dirent* dir_buf;
    dir = opendir(".");
    if(dir==NULL) {
        return;
    }
    std::ofstream fout(".tmp", std::ofstream::out);
    while((dir_buf = readdir(dir)) != NULL){
        if(dir_buf->d_name[0]=='.' || dir_buf->d_type==DT_DIR) continue;            
        fout << "[=] " << dir_buf->d_name << std::endl;
    }
    fout.close();
    closedir(dir);
}

int main(){
    int server_fd;
    LogError(socket(AF_INET, SOCK_STREAM, 0), server_fd, true);
    int opt = 1;
    LogError(setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR|SO_REUSEPORT, &opt, sizeof(opt)), errorNull, true)
    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(8080);
    int addrlen = sizeof(address);
    LogError(bind(server_fd, (sockaddr*)&address, sizeof(address)), errorNull, true);
    LogError(listen(server_fd, SOMAXCONN), errorNull, true);

    while(true){
        int newSocket;
        LogError(accept(server_fd, (sockaddr*)&address, (socklen_t*)&addrlen), newSocket, false);
        
        int pid = fork();
        if(pid!=0) std::cout << "new sock session:  " << pid << std::endl;
        else{
            while(1){
                char buffer[1024] = {0};              
                int readVal;
                LogError(recv( newSocket , buffer, 1024, 0), readVal, true);  // read infromation received into the buffer
                if(readVal == 0) return 0;
                *(buffer+readVal) = 0;

                if(buffer[0] == '1') {
                    std::string name = buffer+1;
                    File *fil = new File(name);
                    fil->Send(newSocket);
                    fil->Close();
                    delete fil;
                } 
                else if(buffer[0] == '2'){
                    listall();
                    File *fil = new File(".tmp");
                    fil->Send(newSocket);
                    fil->Close();
                    delete fil;
                }
                printf("server: %s\n",buffer );

            }
            return 0;
        }
    }
    return 0;
}