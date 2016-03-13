#ifndef CLIENT_H
#define CLIENT_H

#include <sys/types.h>

int connectServer(char *port);
int openFile(char *path);
ssize_t readline(int fd, char **buf);

#endif
