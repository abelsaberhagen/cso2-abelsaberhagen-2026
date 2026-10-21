#include <stdio.h>
#include <math.h>
#include <stddef.h>
#include <stdalign.h>
#include <stdlib.h>
#include "mlpt.h"
#include "config.h"



int main(int argv, char *argc[]) {

    int success = allocate_page(0x1000);
    size_t translated = translate(0x1);


    return 0;
}

