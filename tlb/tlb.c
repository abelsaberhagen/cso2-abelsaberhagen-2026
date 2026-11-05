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
} Way;

typedef struct {
    size_t valid_bit;
    Way ways[NUM_WAYS];
    size_t lru_value;
} Set;

Set *tlb_ptr = NULL;


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
    size_t set_index = vpn % NUM_SETS;

    if (tlb_ptr[set_index].valid_bit == 0) {
        return 0;
    }
    else {
        return (int)tlb_ptr[set_index].lru_value;
    }
}

size_t tlb_translate(size_t va) {
    
    size_t offset_bits = va & ((1UL << POBITS) - 1);
    size_t vpn = va >> POBITS;
    size_t set_index = vpn % NUM_SETS;

    if (tlb_ptr == NULL) {
        void *temp_ptr = NULL;
        int err = posix_memalign(&temp_ptr, sizeof(void *), NUM_SETS * sizeof(Set));
        
        if (err != 0) {
            return 0; 
        }
        memset(temp_ptr, 0, NUM_SETS * sizeof(Set));

        tlb_ptr = (Set *) temp_ptr;
    }
    
    if (tlb_ptr[set_index].valid_bit == 0) {
        return (size_t)-1;
    }

    bool in_tlb = false;
    size_t translated_address = 0;
    for (int way = 0; way < NUM_WAYS; way++) {
        if (tlb_ptr[set_index].valid_bit && tlb_ptr[set_index].ways[way].tag == vpn) {
            in_tlb = true;
            translated_address = tlb_ptr[set_index].ways[way].address + offset_bits;
            break;
        }
    }

    if (!in_tlb) {
        translated_address = translate(va);
        if (translated_address == (size_t)-1) {
            return (size_t)-1;
        }

        size_t way_to_use = tlb_ptr[set_index].lru_value % NUM_WAYS;
        tlb_ptr[set_index].valid_bit = 1;
        tlb_ptr[set_index].ways[way_to_use].tag = vpn;
        tlb_ptr[set_index].ways[way_to_use].address = translated_address - offset_bits;
        tlb_ptr[set_index].lru_value++;
    }
    return translated_address;

}

size_t translate(size_t va) {

    // If ptbr has not been allocated
    if (ptbr == 0) {
        return (size_t)-1;
    }   
   

    size_t offset_bits = va & ((1UL << POBITS) - 1);
    size_t vpn = va >> POBITS;

    int vpn_size = POBITS - 3;
    size_t current_address = (size_t) tlb_ptr;

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
