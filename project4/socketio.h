#ifndef SOCKETIO_H
#define SOCKETIO_H

#define BUFFER_SIZE 512


ssize_t receive(int fd, char *buf, size_t len);
ssize_t receiveAll(int fd, char *buf, size_t len);
ssize_t sendAll(int fd, const char *buf, size_t len);

#endif
