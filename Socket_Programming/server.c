#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#define PORT 8000
void communicate(int new_socket)
{
    char filename[1025],buffer[1025];    
    int end,start,counter,forread,valread,fd;
    while(1)
    {
        memset(&filename, '\0', sizeof(filename));
        valread = read(new_socket , filename, 1024);
        if(!strcmp(filename,"//exit"))break;
        fd = open(filename,O_RDONLY);
        //printf("%d\n",fd);
        if(fd==-1){
            memset(&buffer, '\0', sizeof(buffer));
            sprintf(buffer,"error");
            send(new_socket, buffer, strlen(buffer), 0);
            continue;
        }
        else{
            memset(&buffer, '\0', sizeof(buffer));
            sprintf(buffer,"fine");
            send(new_socket, buffer, strlen(buffer), 0);            
        }
        valread = read(new_socket , buffer, 1024);
        end = lseek(fd,0,SEEK_END);
        start = lseek(fd,0,SEEK_SET);
        counter = end;
        memset(&buffer, '\0', sizeof(buffer));
        sprintf(buffer,"%d",end);
        //printf("end = %s\n",buffer);
        send(new_socket, buffer, strlen(buffer), 0);
        valread = read(new_socket , buffer, 1024);
        while(counter > 0)
        {
            memset(&buffer, '\0', sizeof(buffer));
            forread = read(fd,buffer,1024);
            send(new_socket, buffer, strlen(buffer), 0);
            //printf("%s\n",buffer );
            counter = counter - strlen(buffer);
        }
    }
}
int main(int argc, char const *argv[]   )
{
    int server_fd, new_socket;
    struct sockaddr_in address;  
    int opt = 1;
    int addrlen = sizeof(address);
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET; 
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }


    if (listen(server_fd, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    while(1)
    {
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
                       (socklen_t*)&addrlen))<0)
        {
            perror("accept");
            exit(EXIT_FAILURE);
        }
        communicate(new_socket);
    }
    return 0;
}