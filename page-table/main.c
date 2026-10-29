#include <stdio.h>
#include <math.h>
#include <stddef.h>
#include <stdalign.h>
#include <stdlib.h>
#include "mlpt.h"
#include "config.h"



int main(int argv, char *argc[]) {

    printf("%d\n", allocate_page(0x200000));
    printf("%lx\n", translate(0x200000));


    return 0;
}

