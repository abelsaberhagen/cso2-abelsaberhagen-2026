#include <unistd.h>
#include <stdio.h>
#include "fork.h"

int main(int argc, char *argv[]) {
    printf("Hi!\n");
    writeoutput("echo 1 2 3; sleep 2; echo 5 5", "out.txt", "err.txt");
    printf("Bye!\n");

    const char *argv_base[] = {
        "/bin/echo", "running", NULL
    };
    parallelwriteoutput(2, argv_base, "out.txt");
}