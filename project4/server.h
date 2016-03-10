#ifndef SERVER_H
#define SERVER_H

#define MAX_CONNECTIONS 5

int acceptConnection(int fd);
void registerChildHandler();
int listenPort(const char *port);
int validateClient(int fd, char *expected);

#endif
