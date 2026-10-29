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

size_t translate(size_t va) {

    // If ptbr has not been allocated
    if (ptbr == 0) {
        return (size_t)-1;
    }   
   

    size_t offset_bits = va & ((1UL << POBITS) - 1);
    size_t vpn = va >> POBITS;

    int vpn_size = POBITS - 3;
    size_t current_address = ptbr;

    for (int level = 1; level <= LEVELS; ++level) {
        // Grabs the desired VPN given the VA containing a VPN for each level 
        size_t index = (vpn >> ((LEVELS - level) * vpn_size)) & ((1UL << vpn_size) - 1);

        size_t *new_pte_index = (size_t *)current_address + index;
        size_t pte = *new_pte_index;
        
        // If PTE == 0, PTE has not been allocated, meaning there is no PA.
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

    // If offset bits are not all 0, then start_va is not an address at the start of a page.
    if ((offset_bits & -1) != 0) {
        return -1;
    }

    size_t page_size = 1UL << POBITS;
    size_t vpn = start_va >> POBITS;
    int vpn_size = POBITS - 3;
    size_t current_address;
    
    // Allocate ptbr if it has not yet been allocated.
    if (ptbr == 0) {

        void *root_ptr = NULL;

        int err = posix_memalign(&root_ptr, page_size, page_size);
        
        if (err != 0) {
            return 0; 
        }
        memset(root_ptr, 0, page_size);
    
        ptbr = (size_t) root_ptr;
    }

    current_address = ptbr;

    if (vpn >= (1UL << (LEVELS * vpn_size))) {
        return 0;
    }
    else {
        for (int level = 1; level <= LEVELS; ++level) {
            size_t index = (vpn >> ((LEVELS - level) * vpn_size)) & ((1UL << vpn_size) - 1);

            size_t *new_pte_index = (size_t *)current_address + index;

            // Allocate index of a PTE if it has not yet been allocated.
            if (*new_pte_index == 0) {
                
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
                /* If PTE index has been allocated, then if this is the last level 
                the user desired, then you can't reallocate that PTE index.*/
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
    


