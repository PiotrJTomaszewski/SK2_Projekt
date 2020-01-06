#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include "server.h"


int main(int argc, char **argv) {
    srand((unsigned) time(NULL));
    int port;
    if (argc < 2) {
        printf("Port not specified. Using the default value of 1234\n");
        port = 1234;
    } else {
        port = atoi(argv[1]);
    }
    server_run(port);
    return 0;
}
