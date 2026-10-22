#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdalign.h>
#include <stddef.h>
#include <math.h> 
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
    
    size_t* pt_ptr = (size_t*) ptbr;
    size_t index = (vpn >> ((LEVELS - 1) * vpn_size)) & ((1UL << vpn_size) - 1);
    size_t* new_pte_index = pt_ptr + index;
    *pt_ptr = *new_pte_index;

    for (int level = 1; level <= LEVELS; ++level) {
        if (level == 1) {
            index = vpn;
        } else {
            index = (vpn >> ((LEVELS - level) * vpn_size)) & ((1UL << vpn_size) - 1);
        }

        new_pte_index = pt_ptr + index;
        *pt_ptr = *new_pte_index;
    }
    size_t pte = *pt_ptr;
    if (pte == 0) {
        return (size_t)-1;
    }
    


    size_t ppn = pte >> POBITS;
    size_t ppa = (ppn << POBITS) + offset_bits;
    
    printf("offset: %lx\n", offset_bits);
    printf("ppn: %lx\n", vpn);
    printf("ppa: %lx\n", ppa);

    return ppa;
}

int allocate_page(size_t start_va) { 

    size_t page_size = 1UL << POBITS;
    int num_pt_entries = page_size/sizeof(size_t);
    size_t vpn = start_va >> POBITS;
    int vpn_size;
    if (sizeof(vpn) % LEVELS == 0) {
        vpn_size = sizeof(vpn)/LEVELS;
    }
    else {
        return 0;
    }
    
    size_t* pt_ptr = (size_t*) ptbr;
    if (ptbr == 0) {
        void *temp_ptr = NULL;

        int err = posix_memalign(&temp_ptr, sizeof(void*), num_pt_entries);
        
        if (err != 0) {
            return 0; 
        }
    
        ptbr = (size_t) temp_ptr;
        pt_ptr = (size_t*) ptbr;
    }
    if (vpn >= num_pt_entries) {
        return 0;
    }
    else {
        if (start_va % page_size == 0) { 
            size_t current_vpn = vpn >> (sizeof(vpn) - vpn_size);

            void *temp_ptr = NULL;

            int err = posix_memalign(&temp_ptr, sizeof(void*), page_size);
                
            if (err != 0) {
                return 0; 
            }
                
            pt_ptr[vpn] = (size_t) temp_ptr;

            size_t* new_pte = (size_t*) pt_ptr;
            for (int level = 1; level < LEVELS; ++level) {
                current_vpn = (vpn << (vpn_size * level)) >> ((LEVELS * 8) - vpn_size * level);
                new_pte += current_vpn;
                //posix_memalign(*new_pte, 8);
                void *temp_ptr = NULL;

                int err = posix_memalign(&temp_ptr, sizeof(void*), page_size);
                
                if (err != 0) {
                    return 0; 
                }
                
                *new_pte = (size_t) temp_ptr;
            }
       
            return 1;
        } 
        else {
            return 0;
        }
    }
    
}

