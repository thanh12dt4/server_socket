#include <stdio.h>
#include "server.h"

int main(int argc , char *argv[])
{
    int ret = initServer("127.0.0.1", 8888);
    if (ret) {
        printf("Init server: failed");
        return 1;
    }
    waitUserInput();

    return 0;
}

