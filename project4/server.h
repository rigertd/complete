#ifndef SERVER_H
#define SERVER_H

#define MAX_CONNECTIONS 5

int acceptConnection(int fd);
void registerChildHandler();
unsigned short getRandPort();
void handleRequest(const char *prog, int fd, const char *type);
int listenPort(const char *port);
void verifyArgs(int argc, char *argv[]);

#endif
