// Client side C/C++ program to demonstrate Socket programming

#include "main.h"
int errorNull;

int main()
{
    int server_fd;
    struct sockaddr_in serv_addr;
    LogError(socket(AF_INET, SOCK_STREAM, 0), server_fd, true);
    std::memset(&serv_addr, '0', sizeof(serv_addr)); // to make sure the struct is empty. Essentially sets sin_zero as 0
                                                // which is meant to be, and rest is defined below
 
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    LogError(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr), errorNull, true);
    if(errorNull==0) return -1;
    
    LogError(connect(server_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)), errorNull, true);  // connect to the server address
    
    while(1){

        std::string comm;
        std::cout << "--> ";
        std::getline(std::cin,comm);

        char** com_ar2 = (char**) malloc(1000*sizeof(char*));
        char* tok = strtok((char*)comm.c_str(), " \n");
        int c2=0;
        while(tok!=NULL){
            com_ar2[c2] = (char*) malloc(10000);
            strcpy(com_ar2[c2], tok);
            tok = strtok(NULL, " \n");
            c2++;
            // std::cout << c2 << std::endl;
        }

        std::string command(com_ar2[0], strlen(com_ar2[0]));
        if(command == "listall" && c2!=1){
            std::cout << "listall" << std::endl;
            continue;
        }
        if(command == "send" && c2!=2){
            std::cout << "send <filename>\n";
            continue;
        }

        if(command == "listall") {
            command = "2";
            send(server_fd, (char*)command.c_str(), BLOCK_SIZE, 0);
            
            char buffer[BLOCK_SIZE] = {0};
            int valread = recv( server_fd , buffer, BLOCK_SIZE, 0);  // receive message back from server, into the buffer
           
            std::string s(buffer, valread);
            int length = std::stoi(s);
            int dl = 0;

            while(1){
                int valread = recv( server_fd , buffer, BLOCK_SIZE, 0);  
                s.assign(buffer, valread);
                std::cout << s << std::endl;
                
                dl+=valread;
                if(length == dl) break;
            }
        }
        else if(command == "send"){
            command = std::string(com_ar2[1], strlen(com_ar2[1]));
            command = "1" + command;

            LogError(send(server_fd, (char*)command.c_str(), BLOCK_SIZE, 0), errorNull, false);
            char buffer[BLOCK_SIZE] = {0};
            int valread = recv( server_fd , buffer, BLOCK_SIZE, 0);  // receive message back from server, into the buffer

            if(buffer[0] == '0') {
                std::cout << "No such file\n";
                continue;
            }
            std::string s(buffer, valread);
            long long int length = std::stoi(s);
            long long int dl = 0;

            std::cout << "[";
            FILE* fil = fopen(command.substr(1).c_str(), "wb");
            long long int prev = 0;
            while(1){
                valread = recv( server_fd , buffer, BLOCK_SIZE, 0);  
                if(buffer[0] == '0' && valread==1) {
                    std::cout << "No such file\n";
                    break;
                }
                fwrite(buffer, sizeof(char), valread, fil);
                
                dl+=(long long) valread;
                // std::cout << (dl*100)/length << " " << prev << " ";
                
                // while(prev<(dl*(long long)100)/length) {
                //     std::cout << "#";
                //     prev++;
                // }
                // std::cout << (dl*100)/length << " " << prev << "\n";
                
                if(length == dl) break;
            }
            fclose(fil);
            std::cout << "]\n";
        }
    }

    return 0;
}
