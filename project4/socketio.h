#ifndef SOCKETIO_H
#define SOCKETIO_H

#include <sys/types.h>  /* ssize_t, size_t */

#define BUFFER_SIZE 512
#define INVALID_TYPE "invalid"
#define ENCRYPT_REQ "encrypt"
#define DECRYPT_REQ "decrypt"
#define KEY_REQ "key"
#define MESSAGE_REQ "msg"
#define KEY_ERROR "key_error"
#define INVALID_CHAR "invalid_char"

ssize_t receiveAny(int fd, char *buf, size_t len);
ssize_t receiveAll(int fd, char *buf, size_t len);
ssize_t sendAll(int fd, const char *buf);

#endif
