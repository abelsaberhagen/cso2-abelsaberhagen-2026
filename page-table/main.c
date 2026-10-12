#include <stdio.h>
#include <math.h>
#include <stddef.h>
#include <stdalign.h>
#include <stdlib.h>
#include "mlpt.h"
#include "config.h"



int main(int argv, char *argc[]) {

    size_t translated = translate(0x3045, 64);

    printf("%lx\n", translated);


    return 0;
}

