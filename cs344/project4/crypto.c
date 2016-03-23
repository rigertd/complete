/*********************************************************\
* Author:       David Rigert
* Class:        CS344 Winter 2016
* Assignment:   Program 4 - OTP
* File:         crypto.c
* Description:  Implementation file for crypto.h
\*********************************************************/
#include "crypto.h"

/**
 * Encrypts the specified message using the specified key.
 *
 * This function uses the one-time-pad encryption technique to encrypt
 * the specified message using the specified key.
 *
 *  key     The key to use for encryption.
 *  msg     The message to encrypt.
 *
 * Returns a Result enumerator indicating whether the operation was successful.
 */
enum Result encryptText(char *key, char *msg) {
    int i = 0;
    while (msg[i] != '\0') {
        /* Check for errors and return result accordingly.
           Caller prints the error messages. */
        if (key[i] == '\0') {
            return Result_KEY_ERROR;
        } else if (
            (msg[i] != ' ' && (msg[i] < 'A' || msg[i] > 'Z')) ||
            (key[i] != ' ' && (key[i] < 'A' || key[i] > 'Z'))
            ) {
            return Result_INVALID_CHAR;
        }

        /* Convert spaces to '@' symbol */
        if (msg[i] == ' ') msg[i] = '@';
        if (key[i] == ' ') key[i] = '@';

        /* Encrypt the text */
        msg[i] = ((msg[i] - '@' + key[i] - '@') % 27) + '@';

        /* Convert '@' symbol back to space */
        if (msg[i] == '@') msg[i] = ' ';
        if (key[i] == '@') key[i] = ' ';

        ++i;
    }
    return Result_SUCCESS;
}

/**
 * Decrypts the specified message using the specified key.
 *
 * This function uses the one-time-pad encryption technique to decrypt
 * the specified message using the specified key.
 *
 *  key     The key to use for decryption.
 *  msg     The message to decrypt.
 *
 * Returns a Result enumerator indicating whether the operation was successful.
 */
enum Result decryptText(char *key, char *msg) {
    int i = 0;
    while (msg[i] != '\0') {
        /* Check for errors and return result accordingly.
           Caller prints the error messages. */
        if (key[i] == '\0') {
            return Result_KEY_ERROR;
        } else if (msg[i] != ' ' && (msg[i] < 'A' || msg[i] > 'Z')) {
            return Result_INVALID_CHAR;
        }

        /* Convert spaces to '@' symbol */
        if (msg[i] == ' ') msg[i] = '@';
        if (key[i] == ' ') key[i] = '@';

        /* Decrypt the text */
        msg[i] = ((msg[i] - '@') - (key[i] - '@')) % 27;
        msg[i] = msg[i] < 0 ? msg[i] + 27 : msg[i];
        msg[i] += '@';

        /* Convert '@' symbol back to space */
        if (msg[i] == '@') msg[i] = ' ';
        if (key[i] == '@') key[i] = ' ';

        ++i;
    }
    return Result_SUCCESS;
}
