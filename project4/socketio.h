#ifndef SOCKETIO_H
#define SOCKETIO_H

int getData(int fd, const char *request, char **buf);
int receiveAll(int fd, char **buf);
int sendAll(int fd, char *buf);

#endif
