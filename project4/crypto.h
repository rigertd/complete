/*********************************************************\
* Author:       David Rigert
* Class:        CS344 Winter 2016
* Assignment:   Program 4 - OTP
* File:         crypto.h
* Description:  Defines the functions used for encrypting and
*               decrypting data for the one-time-pad programs.
\*********************************************************/
#ifndef CRYPTO_H
#define CRYPTO_H

/**
 * Enumerates the possible results of the encryption and decryption
 * functions.
 */
enum Result {
    Result_SUCCESS,     /* En/decryption succeeded */
    Result_KEY_ERROR,   /* Key was too short */
    Result_INVALID_CHAR /* Message or key contained invalid char */
};

enum Result encryptText(char *key, char *msg);
enum Result decryptText(char *key, char *msg);

#endif