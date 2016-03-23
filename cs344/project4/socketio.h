/*********************************************************\
* Author:       David Rigert
* Class:        CS344 Winter 2016
* Assignment:   Program 4 - OTP
* File:         socketio.h
* Description:  Defines the macros and functions used by the
*               one-time-pad clients and servers for communication
*               over sockets.
\*********************************************************/
#ifndef SOCKETIO_H
#define SOCKETIO_H

#include <sys/types.h>  /* ssize_t, size_t */

/*========================================================*
 * Macro definitions
 *========================================================*/
/* Size of buffer to use when sending/receiving data via sockets */
#define BUFFER_SIZE 512

/* The following macros define the values used for application-level
   protocol requests and acknowledgements. These are all lowercase
   to avoid being mistaken for valid message or key data. */
#define INVALID_TYPE "invalid"      /* invalid server type */
#define ENCRYPT_REQ "encrypt"       /* encryption request */
#define DECRYPT_REQ "decrypt"       /* decryption request */
#define KEY_REQ "key"               /* request key data */
#define MESSAGE_REQ "msg"           /* request message data */
#define KEY_ERROR "key_error"       /* key too short */
#define INVALID_CHAR "invalid_char" /* key or message contains invalid char */

ssize_t receiveAll(int fd, char *buf, size_t len);
ssize_t receiveAny(int fd, char *buf, size_t len);
ssize_t sendAll(int fd, const char *buf);

#endif
