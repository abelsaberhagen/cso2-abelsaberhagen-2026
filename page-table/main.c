#include <stdio.h>
#include <math.h>
#include <stddef.h>
#include <stdalign.h>
#include <stdlib.h>
#include "mlpt.h"
#include "config.h"

alignas(4096)
static size_t page_of_data[512];

alignas(4096)
static char data_for_page_3[4096];
size_t ptbr = 0;

static void set_testing_ptbr(void) {
    ptbr = (size_t) page_of_data[3];
}

int main(int argv, char *argc[]) {

    // int page_size_bytes = pow(2.0, POBITS);
    // int page_table_entry_size_bytes = 8;
    // int num_page_table_entries = page_size_bytes/page_table_entry_size_bytes;
    
   //size_t page_table_entry_va = 0x1234567812345679;

    size_t address_of_data_for_page_3_as_integer = (size_t) &data_for_page_3[0];
    size_t physical_page_number_of_data_for_page_3 = address_of_data_for_page_3_as_integer >> 12;
    // instead of >> 12, we could have written:
        // address_of_data_for_page_3_as_integer / 4096
    size_t page_table_entry_for_page_3 = (
        // physical page number in upper (64-POBITS) bits
        (physical_page_number_of_data_for_page_3 << 12)
    |
        // valid bit in least significant bit, set to 1
        1
    );

    page_of_data[3] = page_table_entry_for_page_3;
    set_testing_ptbr();

    size_t translated = translate(ptbr, 64);

    if (translated == (size_t) &data_for_page_3[0x45]) {
        printf("True: %lx\n", translated);
    }
    else {
        printf("False: %lx\n", (size_t) &data_for_page_3[0x45]);
        printf("False: %lx\n", translated);

    }

    return 0;
}

