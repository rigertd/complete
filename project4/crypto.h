#ifndef CRYPTO_H
#define CRYPTO_H

enum Result {
    Result_SUCCESS,
    Result_KEY_ERROR,
    Result_INVALID_CHAR
};

enum Result encryptText(char *key, char *msg);
enum Result decryptText(char *key, char *msg);

#endif