Compilation code for server.c : gcc server.c -o server
execution code for server.c   : ./server

Compilation code for client.c : gcc client.c -o client
execution code for client.c   : ./client

Note:

Server is only exitted by a sigint signal(CTRL + C).

Client supports get , exit commands only,any other command display errors.