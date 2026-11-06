#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdalign.h>
#include <math.h> 
#include <string.h>
#include <stdbool.h>
#include "config.h"
#include "mlpt.h"

#define NUM_SETS 16
#define NUM_WAYS 4


typedef struct {
    size_t tag;
    size_t address;
    size_t valid_bit;
    int lru_rank;
} Block;

typedef struct {
    Block blocks[NUM_WAYS];
} Set;

Set *tlb_ptr = NULL;

void increase_lru(int index, int target_block) {
    int old_rank = tlb_ptr[index].blocks[target_block].lru_rank;

    if (tlb_ptr[index].blocks[target_block].valid_bit == 0) {
        tlb_ptr[index].blocks[target_block].valid_bit = 1;
        for (int block = 0; block < NUM_WAYS; block++) {
            if (block != target_block && tlb_ptr[index].blocks[block].valid_bit == 1) {
                if (tlb_ptr[index].blocks[block].lru_rank < NUM_WAYS) {
                    tlb_ptr[index].blocks[block].lru_rank++;
                }
            }
        }
        tlb_ptr[index].blocks[target_block].lru_rank = 1; 
    } else {
        for (int block = 0; block < NUM_WAYS; block++) {
            if (tlb_ptr[index].blocks[block].valid_bit == 1) {
                if (block == target_block) {
                    tlb_ptr[index].blocks[block].lru_rank = 1; 
                } else if (tlb_ptr[index].blocks[block].lru_rank < old_rank) {
                    tlb_ptr[index].blocks[block].lru_rank++; 
                }
            }
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
        if (tlb_ptr[index].blocks[block].valid_bit == 1 && tlb_ptr[index].blocks[block].tag == tag) {
                return (int)tlb_ptr[index].blocks[block].lru_rank;
        }
    }
    
    return 0;
}

size_t tlb_translate(size_t va) {
    
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

    for (int block = 0; block < NUM_WAYS; block++) {
        if (tlb_ptr[index].blocks[block].valid_bit == 1 && tlb_ptr[index].blocks[block].tag == tag) {
            increase_lru(index, block);
            return tlb_ptr[index].blocks[block].address + offset_bits;
        }
    }

    size_t translated_address = translate(va);
    if (translated_address == (size_t)-1) {
        return (size_t)-1;
    }

    int target_block = -1;
    for (int block = 0; block < NUM_WAYS; block++) {
        if (tlb_ptr[index].blocks[block].valid_bit == 0) {
            target_block = block;
            break;
        }
    }

    if (target_block == -1) {
        for (int block = 0; block < NUM_WAYS; block++) {
            if (tlb_ptr[index].blocks[block].lru_rank == NUM_WAYS) {
                target_block = block;
                break;
            }
        }

        if (target_block == -1) {
            target_block = 0;
        }
    }
        
    tlb_ptr[index].blocks[target_block].valid_bit = 1;
    tlb_ptr[index].blocks[target_block].tag = tag;
    tlb_ptr[index].blocks[target_block].address = translated_address;
        
    increase_lru(index, target_block);
        
    
    return translated_address;

}

