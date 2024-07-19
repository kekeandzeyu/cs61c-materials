#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
    // Creates an integer with value 5
    int16_t x = 5;

    // allocate an int16_t array of size 4
    int16_t some_array[4];
    printf("address of the start of the array: %p\n", some_array);

    // compute the address of the element at index 2 (0-indexed)
    int16_t* ptr_to_idx_2 = some_array + 2;
    printf("address of index 2: %p\n", ptr_to_idx_2);

    // store the value 10 at index 2, using ptr_to_idx_2
    *ptr_to_idx_2 = 10;

    // print the value at index 2
    printf("value at index 2: %d\n", *ptr_to_idx_2);

    return 0;
}
