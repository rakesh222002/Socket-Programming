// Client side C/C++ program to demonstrate Socket programming
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <arpa/inet.h>

#define PORT 8000
void communicate(int sockfd)
{
    char cmd[1025],buffer[1025],pro[40];
    int valread,fsize,exseek,forwr;
    while(1)
    {
        //memset(&cmd, '\0', sizeof(cmd));
        fprintf(stdout,"Client>");
        fgets(cmd,1024,stdin);
        cmd[strlen(cmd)-1] = '\0';
        if(!strcmp(cmd,"exit")){
            send(sockfd, "//exit", strlen("//exit"), 0);
            break;
        }
        char *tok = strtok(cmd," \t");
        if(!strcmp(tok,"get"))
        {
            tok = strtok(NULL," \t");
            int ind=0;
            while(tok!=NULL)
            {
                send(sockfd, tok, strlen(tok), 0);
                memset(&buffer, '\0', sizeof(buffer));
                valread = read(sockfd, buffer,1024);
                if(!strcmp(buffer,"error")){
                    fprintf(stderr,"%s does not exist in server\n",tok);
                    tok = strtok(NULL," \t");
                    ind++;
                    continue;
                }
                send(sockfd, "//yes",strlen("//yes"), 0);
                //printf("buffer:%s\n",buffer);
                memset(&buffer, '\0', sizeof(buffer));
                valread = read(sockfd, buffer,1024);
                //printf("buffer:%s\n",buffer);
                fsize = atoi(buffer);
                send(sockfd, "//yes",strlen("//yes"), 0);
                //printf("%d\n",fsize);
                int presfd = open(tok,O_WRONLY | O_CREAT, 0666);
                int sum=0;
                int exfsize = fsize;
                while(fsize>0)
                {
                    memset(&buffer, '\0', sizeof(buffer));
                    valread = read(sockfd, buffer,1024);
                    forwr = write(presfd, buffer, strlen(buffer));
                    fsize = fsize - strlen(buffer);
                    sum+=strlen(buffer);
                    memset(&pro, '\0', sizeof(pro));
                    sprintf(pro,"\rProgress of %s: %0.2f",tok,(float)sum/exfsize * 100);
                    exseek = lseek(1,0,SEEK_SET); 
                    forwr = write(1,pro,40);
                }
                fprintf(stdout, "\n");;
                tok = strtok(NULL," \t");
                ind++;
            }
        }
        else fprintf(stdout,"Command does not exist\n");
    }
}
int main(int argc, char const *argv[])
{
    struct sockaddr_in address;
    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        return -1;
    }

    memset(&serv_addr, '0', sizeof(serv_addr)); // to make sure the struct is empty. Essentially sets sin_zero as 0
                                                // which is meant to be, and rest is defined below

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Converts an IP address in numbers-and-dots notation into either a 
    // struct in_addr or a struct in6_addr depending on whether you specify AF_INET or AF_INET6.
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)
    {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)  // connect to the server address
    {
        printf("\nConnection Failed \n");
        return -1;
    }
    communicate(sock);
    //send(sock , hello , strlen(hello) , 0 );  // send the message.
    //valread = read( sock , buffer, 1024);  // receive message back from server, into the buffer
    return 0;
}
