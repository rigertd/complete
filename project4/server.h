#ifndef SERVER_H
#define SERVER_H

#define MAX_CONNECTIONS 5

int acceptConnection(int fd);
void registerChildHandler();
unsigned short getRandPort();
int listenPort(const char *port);

#endif
