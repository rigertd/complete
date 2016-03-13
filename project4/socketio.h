#ifndef SOCKETIO_H
#define SOCKETIO_H

#include <sys/types.h>  /* ssize_t, size_t */

#define BUFFER_SIZE 512

ssize_t receiveAny(int fd, char *buf, size_t len);
ssize_t receiveAll(int fd, char *buf, size_t len);
ssize_t sendAll(int fd, const char *buf);

#endif
