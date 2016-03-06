#include "crypto.h"

Result encryptText(char *key, char *msg) {
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

        /* Encrypt the text */
        msg[i] = ((msg[i] + key[i]) % 27) + '@';
        
        /* Convert '@' symbol back to space */
        if (msg[i] == '@') msg[i] = ' ';
        if (key[i] == '@') key[i] = ' ';
        
        ++i;
    }
    return Result_SUCCESS;
}

Result decryptText(char *key, char *msg) {
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
        msg[i] = ((msg[i] - key[i]) % 27) + '@';
        
        /* Convert '@' symbol back to space */
        if (msg[i] == '@') msg[i] = ' ';
        if (key[i] == '@') key[i] = ' ';
        
        ++i;
    }
    return Result_SUCCESS;
}
