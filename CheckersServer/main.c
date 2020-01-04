#include <stdlib.h>
#include <time.h>
#include "server.h"


int main() {
    srand((unsigned) time(NULL));
    server_run(1234);
    return 0;
}
