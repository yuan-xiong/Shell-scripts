#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "dump.h"

int main() {

    printf("This is main - mknod\n");
    
    dump_host_devices();

    return 0;
}
