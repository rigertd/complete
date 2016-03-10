#ifndef SOCKETIO_H
#define SOCKETIO_H

int getData(int fd, const char *request, char **buf);
ssize_t receiveAll(int fd, char **buf);
ssize_t sendAll(int fd, const char *buf);

#endif
