#include <stdio.h>
#include <stdlib.h>
#include <stdalign.h>
#include <stddef.h>
#include "config.h"

alignas(4096);
static size_t page_of_data[512];

alignas(4096);
static char data_for_page_3[4096];
size_t ptbr = 0;

static void set_testing_ptbr(void) {
    ptbr = (size_t) page_of_data[3];
}



int* to_binary(int num, int size) {
    int *binary_value = calloc(size, sizeof(int));
    
    int i = 0;
    while (num > 0){
        int remainder = num % 2;
        *(binary_value + i) = remainder;
        num = num / 2;
        i++;
    }

    return binary_value;

}

void print_binary_value(char *label, int *value, int limit) {
    printf("%s", label);
    for (int i = limit; i >= 0; --i){
        printf("%d", *(value + i));
    }
    printf("\n");
}

size_t translate(size_t val, int size) {

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

    int *val_ptr = to_binary(val, size);

    size_t valid_bit = val & 0b1;
    size_t return_val;
    if (valid_bit == 0) {
        return_val = 0xFFFFFFFFFFFFFFFF;
    } 
    else {
        size_t offset_bit = val & 0xFFF;
        size_t vpn = val >> POBITS;
        size_t pte_index = vpn;
        printf("hello");
        size_t pte = page_of_data[pte_index];
        printf("hello");
        size_t ppn = pte >> POBITS;
        size_t ppa = (ppn << POBITS) + offset_bit;
    

        
        int *offset_ptr = to_binary(offset_bit, POBITS);
        int *ppa_ptr = to_binary(ppa, size);
        
        //print_binary_value("offset: ", offset_ptr, size);
        //print_binary_value("ppn: ", ppn_ptr, size);

        printf("offset: %lx\n", offset_bit);
        printf("ppn: %lx\n", vpn);
        printf("ppa: %lx\n", ppa);

    
        free(offset_ptr);
        free(ppa_ptr);

        return_val = ppa;
    }
    
    //print_binary_value("val: ", val_ptr, size);
    printf("val: %lx\n", val);

    
    free(val_ptr);
    return return_val;
}

