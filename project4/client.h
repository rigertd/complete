#ifndef CLIENT_H
#define CLIENT_H

#include <sys/types.h> /* ssize_t, addrinfo, getaddrinfo */

int connectServer(const char *prog, const char *port);
int requestOp(  const char *prog,
                const char *msgpath,
                const char *keypath,
                const char *port,
                const char *type    );
ssize_t loadData(const char *path, char **data);
int openFile(const char *path);
ssize_t readline(int fd, char **buf);
void verifyArgs(int argc, char *argv[]);

#endif
