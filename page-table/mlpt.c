#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdalign.h>
#include <stddef.h>
#include <math.h> 
#include <string.h>
#include "config.h"

size_t ptbr = 0;

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

size_t translate(size_t va) {

    if (ptbr == 0) {
        return (size_t)-1;
    }   
   

    size_t offset_bits = va & ((1UL << POBITS) - 1);
    size_t vpn = va >> POBITS;

    int vpn_size = POBITS - 3;
    size_t current_address = ptbr;

    for (int level = 1; level <= LEVELS; ++level) {
        size_t index = (vpn >> ((LEVELS - level) * vpn_size)) & ((1UL << vpn_size) - 1);

        size_t *new_pte_index = (size_t *)current_address + index;
        size_t pte = *new_pte_index;
        
        if (pte == 0) {
            return (size_t)-1;
        }
        size_t ppn = pte >> POBITS;
        current_address = ppn << POBITS;
    
    }

    return current_address + offset_bits;
}

int allocate_page(size_t start_va) { 

    size_t offset_bits = start_va & ((1UL << POBITS) - 1);

    if ((offset_bits & -1) != 0) {
        return -1;
    }

    size_t page_size = 1UL << POBITS;
    int num_pt_entries = page_size/sizeof(size_t);

    size_t vpn = start_va >> POBITS;
    int vpn_size = POBITS - 3;
    size_t current_address;
    
    if (ptbr == 0) {
        printf("made page\n");

        void *root_ptr = NULL;

        int err = posix_memalign(&root_ptr, page_size, page_size);
        
        if (err != 0) {
            return 0; 
        }
        memset(root_ptr, 0, page_size);
    
        ptbr = (size_t) root_ptr;
    }

    current_address = ptbr;

    if (vpn >= (1UL << vpn_size)) {
        return 0;
    }
    else {
        for (int level = 2; level <= LEVELS; ++level) {
            size_t index = (vpn >> ((LEVELS - level) * vpn_size)) & ((1UL << vpn_size) - 1);

            size_t *new_pte_index = (size_t *)current_address + index;

            if (*new_pte_index == 0) {
                printf("made page\n");
                void *temp_ptr = NULL;
                int err = posix_memalign(&temp_ptr, page_size, page_size);
                
                if (err != 0) {
                    return 0; 
                }
                memset(temp_ptr, 0, page_size);
                
                size_t ppn = (size_t) temp_ptr >> POBITS;
                *new_pte_index = (ppn << POBITS) | 1;
            }
            else {
                if (level == LEVELS) {
                    return 0;
                }
            }

            size_t pte = *new_pte_index;
            size_t ppn = pte >> POBITS;         
            current_address = ppn << POBITS;

        }
    
        return 1;
    } 
}
    


