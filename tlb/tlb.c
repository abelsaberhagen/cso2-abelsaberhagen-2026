#include "tlb.h"
#include "config.h"

int num_sets = 16;
int num_ways = 4;

void tlb_clear() {
    tlb_ptr = 0;
}

int tlb_peek() {

}

struct {
    size_t valid_bit;
    size_t ways[num_ways];
    size_t* lru;
} Set;

size_t tlb_translate(size_t va) {
    
    size_t offset_bits = va & ((1UL << POBITS) - 1);
    size_t vpn = va >> POBITS;

    void *temp_ptr = NULL;
    int err = posix_memalign(&temp_ptr, num_set, num_sets);
    
    if (err != 0) {
        return 0; 
    }
    memset(temp_ptr, 0, num_set);

    Set tlb_ptr = (Set) temp_ptr
    Set tlb_ptr[vpn].ways[]

}

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