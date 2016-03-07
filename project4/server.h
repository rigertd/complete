#ifndef SERVER_H
#define SERVER_H

#define MAX_CONNECTIONS 5

int acceptConnection(int fd);
void childHandler(int sig);
void registerChildHandler();
int startListening(char *port);
int validateClient(int fd, char *expected);

#endif
