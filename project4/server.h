/*********************************************************\
* Author:       David Rigert
* Class:        CS344 Winter 2016
* Assignment:   Program 4 - OTP
* File:         server.h
* Description:  Defines functions used by the server-side programs
*               in a one-time-pad implementation.
\*********************************************************/
#ifndef SERVER_H
#define SERVER_H

int acceptConnection(int fd);
void registerChildHandler();
unsigned short getRandPort();
void handleRequest(const char *prog, int fd, const char *type);
int listenPort(const char *port);
void verifyArgs(int argc, char *argv[]);

#endif
