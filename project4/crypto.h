#ifndef CRYPTO_H
#define CRYPTO_H

enum Result {
    Result_SUCCESS = 0,
    Result_KEY_ERROR,
    Result_INVALID_CHAR
};

Result encryptText(char *key, char *msg);
Result decryptText(char *key, char *msg);

#endif