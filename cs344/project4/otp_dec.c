/*********************************************************\
* Author:       David Rigert
* Class:        CS344 Winter 2016
* Assignment:   Program 4 - OTP
* File:         otp_dec.c
* Description:  A one-time-pad decryption client written in C.
*
*               This program reads encrypted data and the corresponding
*               encryption key from the file system and sends a request
*               to a decryption server via TCP.
*
*               The command line syntax is as follows:
*
*                   otp_dec message key port
*
*               This program takes the following arguments:
*               - message   -- The file containing the text to decrypt.
*               - key       -- The file containing the decryption key.
*               - port      -- The TCP port on which the decryption server
*                              is running.
\*********************************************************/
#include "client.h"     /* verifyArgs, requestOp */
#include "socketio.h"   /* DECRYPT_REQ */

/*========================================================*
 * main function
 *========================================================*/
int main(int argc, char *argv[]) {
    verifyArgs(argc, argv);
    
    return requestOp(argv[0], argv[1], argv[2], argv[3], DECRYPT_REQ);
}
