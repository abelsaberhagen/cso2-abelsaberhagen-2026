#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdalign.h>
#include <math.h> 
#include <string.h>
#include <stdbool.h>
#include "tlb.h"
#include "config.h"
#include "mlpt.h"

#define NUM_SETS 16
#define NUM_WAYS 4


typedef struct {
    size_t tag;
    size_t address;
    size_t valid_bit;
} Block;

typedef struct {
    Block blocks[NUM_WAYS];
    size_t lru_value;
} Set;

Set *tlb_ptr = NULL;
size_t ptbr = 0;

int allocate_page_table() {
    size_t page_size = 1UL << POBITS;
    void *root_ptr = NULL;

    int err = posix_memalign(&root_ptr, page_size, page_size);
        
    if (err != 0) {
        return 0; 
    }
    memset(root_ptr, 0, page_size);

    ptbr = (size_t) root_ptr;
    return 1;
}

void increase_lru(int index) {
    for (int i = 0; i < NUM_SETS; i++) {
        if (i != index) {
            tlb_ptr[i].lru_value++;
        }
        else {
            tlb_ptr[i].lru_value = 0;
        }
    }
}

void tlb_clear() {
    if (tlb_ptr != NULL) {
        free(tlb_ptr);
        tlb_ptr = NULL;
    }
}

int tlb_peek(size_t va) {

    if (tlb_ptr == NULL) {
        return 0;
    }


    size_t vpn = va >> POBITS;
    size_t index = vpn & 0xF;
    size_t tag = vpn >> 4;

    if (tlb_ptr == 0) {
        return 0;
    }

    for (int block = 0; block < NUM_WAYS; block++) {
        if (tlb_ptr[index].blocks[block].tag == tag) {
            if (tlb_ptr[index].blocks[block].valid_bit == 0) {
                return 0;
            }
        }
    }
    if (tlb_ptr == 0) {
        return 0;
    }
    else {
        return (int)tlb_ptr[index].lru_value;
    }
}

size_t tlb_translate(size_t va) {

    if (ptbr == 0) {
        allocate_page_table();
    }
    
    size_t offset_bits = va & ((1UL << POBITS) - 1);
    size_t vpn = va >> POBITS;
    size_t index = vpn & 0xF;
    size_t tag = vpn >> 4;

    if (tlb_ptr == NULL) {
        void *temp_ptr = NULL;
        int err = posix_memalign(&temp_ptr, sizeof(void *), NUM_SETS * sizeof(Set));
        
        if (err != 0) {
            return 0; 
        }
        memset(temp_ptr, 0, NUM_SETS * sizeof(Set));

        tlb_ptr = (Set *) temp_ptr;
    }

    bool in_tlb = false;
    size_t translated_address = 0;
    for (int block = 0; block < NUM_WAYS; block++) {
        if (tlb_ptr[index].blocks[block].valid_bit == 0) {
            return (size_t)-1;
        } 
        else {
            if (tlb_ptr[index].blocks[block].tag == vpn) {
                in_tlb = true;
                translated_address = tlb_ptr[index].blocks[block].address + offset_bits;
                break;
            }
        }
    }

    if (!in_tlb) {
        translated_address = translate(va);
        if (translated_address == (size_t)-1) {
            return (size_t)-1;
        }

        for (int i = 0; i < NUM_SETS; i++) {
            if (tlb_ptr[index].lru_value == 0) {
                tlb_ptr[index].lru_value++;
                for (int block = 0; block < NUM_WAYS; block++) {
                    if (tlb_ptr[index].blocks[block].valid_bit == 0) {
                        tlb_ptr[index].blocks[block].valid_bit = 1;
                        tlb_ptr[index].blocks[block].tag = tag;
                        tlb_ptr[index].blocks[block].address = translated_address - offset_bits;
                        break;
                    }
                }
            }
        }
    }
    return translated_address;

}

size_t translate(size_t va) {

    size_t offset_bits = va & ((1UL << POBITS) - 1);
    size_t vpn = va >> POBITS;

    int vpn_size = POBITS - 3;
    size_t current_address = (size_t) ptbr;

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
