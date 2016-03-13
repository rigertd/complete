/*********************************************************\
* Author:       David Rigert
* Class:        CS344 Winter 2016
* Assignment:   Program 4 - OTP
* File:         otp_enc.c
* Description:  A one-time-pad encryption client written in C.
*
*               This program reads plaintext data and the corresponding
*               encryption key from the file system and sends a request
*               to an encryption server via TCP.
*
*               The command line syntax is as follows:
*
*                   otp_enc message key port
*
*               This program takes the following arguments:
*               - message   -- The file containing the text to encrypt.
*               - key       -- The file containing the encryption key.
*               - port      -- The TCP port on which the encryption server
*                              is running.
\*********************************************************/
#include "client.h"     /* verifyArgs, requestOp */
#include "socketio.h"   /* ENCRYPT_REQ */

/*========================================================*
 * main function
 *========================================================*/
int main(int argc, char *argv[]) {
    verifyArgs(argc, argv);
    
    return requestOp(argv[0], argv[1], argv[2], argv[3], ENCRYPT_REQ);
}
