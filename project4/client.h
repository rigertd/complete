#ifndef CLIENT_H
#define CLIENT_H

#include <sys/types.h>

int connectServer(char *port);
ssize_t loadData(char *path, char **data);
int openFile(char *path);
ssize_t readline(int fd, char **buf);
void verifyArgs(int argc, char *argv[]);

#endif
