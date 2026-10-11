#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
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

size_t translate(size_t val, int size) {

    int *val_ptr = to_binary(val, size);

    size_t valid_bit = val & 0b1;
    size_t return_val;
    if (valid_bit == 0) {
        return_val = 0xFFFFFFFFFFFFFFFF;
    } 
    else {
        size_t offset_bit = val & 0xFFF;
        size_t ppn = val >> POBITS;
        size_t ppa = ppn << POBITS;
    

        
        int *offset_ptr = to_binary(offset_bit, POBITS);
        int *ppn_ptr = to_binary(ppn, size);
        
        //print_binary_value("offset: ", offset_ptr, size);
        //print_binary_value("ppn: ", ppn_ptr, size);

        printf("offset: %lx\n", offset_bit);
        printf("ppn: %lx\n", ppn);
        printf("ppa: %lx\n", ppa);

    
        free(offset_ptr);
        free(ppn_ptr);

        return_val = ppa;
    }
    
    //print_binary_value("val: ", val_ptr, size);
    printf("val: %lx\n", val);

    
    free(val_ptr);
    return return_val;
}



// size_t translate(size_t va, int size) {

//     int *binary_val_ptr = to_binary(va, size);
//     int *offset_ptr = calloc(POBITS, sizeof(int));

//     size_t return_val;
//     if (*binary_val_ptr == 0) {
//         return_val = 0b1111111111111;
//     } else {
//         for (int i = 0; i < POBITS; i++) {
//             *(offset_ptr + i) = *(binary_val_ptr + i);
//             printf("%d", *(offset_ptr + i));
//         }
//         return_val = 0b0000000000000;
//     } 

//     free(binary_val_ptr);
//     free(offset_ptr);
//     return return_val;
// }
