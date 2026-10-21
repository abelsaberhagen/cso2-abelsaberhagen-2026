#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdalign.h>
#include <stddef.h>
#include <math.h> 
#include "config.h"


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

    size_t valid_bit = va & 0b1;
    size_t return_val;
    if (valid_bit == 0) {
        return_val = -1;
    } 
    else {

        size_t offset_bits = va << (sizeof(va) - POBITS);
        size_t vpn = va >> POBITS;
        size_t pte_index = vpn;

        int vpn_size;
        if (sizeof(vpn) % LEVELS == 0) {
            vpn_size = sizeof(vpn)/LEVELS;
        }
        else {
            return -1;
        }
        size_t* pt_ptr = (size_t*) ptbr;
        size_t new_vpn = vpn >> (sizeof(vpn) - vpn_size);
        size_t new_pte_index = pt_ptr + vpn;
        size_t* new_pt_ptr = (size_t*) new_pte_index;
        for (int i = 1; i < LEVELS; ++i) {
            current_vpn = (vpn << (vpn_size * i)) >> (LEVELS - vpn_size * i);
            new_pte_index = new_pte_index + current_vpn;
            new_pt_ptr = (size_t *) new_pte_index;
        }
        pte = *new_pt_ptr;
        size_t ppn = pte >> POBITS;
        size_t ppa = (ppn << POBITS) + offset_bits;
        
        printf("offset: %lx\n", offset_bits);
        printf("ppn: %lx\n", vpn);
        printf("ppa: %lx\n", ppa);

    
        free(offset_ptr);
        free(ppa_ptr);

        return_val = ppa;
    }
    

    
    free(val_ptr);
    return return_val;
}

int allocate_page(size_t start_va) { 
    int num_pt_entries = pow(2, POBITS)/8
    size_t vpn = va >> POBITS;
    int vpn_size;
    if (sizeof(vpn) % LEVELS == 0) {
        vpn_size = sizeof(vpn)/LEVELS;
    }
    else {
        return 0;
    }
    
    size_t* pt_ptr = (size_t*) ptbr;
    if (pt_ptr == 0) {
        void *result;
        int error_code = posix_memalign(&result, pt_ptr, num_pt_entries);
    }
    if (pt_ptr + start_va > num_pt_entries) {
        return 0;
    }
    else {
        if (start_va % (1 << POBITS) == 0) { 
            void *result;
            current_vpn = vpn >> sizeof(vpn) - vpn_size;
            int error_code = posix_memalign(&result, pt_ptr[vpn], 8);
            size_t* new_pte = *pt_ptr
            for (int i = 1; i < LEVELS; ++i) {
                current_vpn = (vpn << (vpn_size * i)) >> (LEVELS - vpn_size * i);
                new_pte += current_vpn;
                int error_code = posix_memalign(&result, *new_pte, 8);
            }
            return 1;
        } 
        else {
            return 0;
        }
    }
    
}

