#include <stddef.h>
#include "tlb.h"

int main(void) {
    tlb_translate(0x12345);
    return 0;
}