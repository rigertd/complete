#include "client.h"
#include "socketio.h"

int main(int argc, char *argv[]) {
    verifyArgs(argc, argv);
    
    return requestOp(argv[0], argv[1], argv[2], argv[3], ENCRYPT_REQ);
}
