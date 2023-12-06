#include <stdio.h>
#include "algorithms.h"  // Include your array and heap definitions
#include "algorithms.c"

#define BIG_SIZE 10 

void array_print(struct array *arr) {
    for (size_t i = 0; i < arr->size; ++i) {
        printf("%d ", arr->data[i]);
    }
    printf("\n");
}

int main() {
    struct array a;
    array_create(&a);

    for (int i = 0; i < BIG_SIZE; ++i) {
        array_heap_add(&a, i);
    }

    printf("Initial array: ");
    array_print(&a);

    for (int i = 0; i < BIG_SIZE; ++i) {
        printf("\nIteration %d:\n", i);
        printf("Before removal: ");
        array_print(&a);
        printf("Top value expected: %d\n", BIG_SIZE - i - 1);
        printf("Top value actual: %d\n", array_heap_top(&a));

        // Add a check for heap property before removal
        printf("Is heap before removal: %s\n", array_is_heap(&a) ? "Yes" : "No");

        array_heap_remove_top(&a);

        printf("After removal: ");
        array_print(&a);

        // Add a check for heap property after removal
        printf("Is heap after removal: %s\n", array_is_heap(&a) ? "Yes" : "No");
    }

    array_destroy(&a);

    return 0;
}

