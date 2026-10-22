#include <stdio.h>
#include <math.h>
#include <stddef.h>
#include <stdalign.h>
#include <stdlib.h>
#include "mlpt.h"
#include "config.h"



int main(int argv, char *argc[]) {

    allocate_page(0x1ff000);
    printf("%lx\n", translate(0x0));


    return 0;
}

