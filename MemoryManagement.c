#include "MemoryManagement.h"
#include <stdio.h>
void cleanup(Cardpile **deck) {
    // Free the memory allocated for the cards
    Card *current = (*deck)->top;
    while (current != NULL) {
        Card *next = current->next;
        free(current);
        current = next;
    }
    // Free the memory allocated for the deck
    free(*deck);
    *deck = NULL;
}

void* safeMalloc(size_t size, char* errMsg) {
    void* ptr = malloc(size);
    if (ptr == NULL) {
        printf("%s\n", errMsg);
        exit(1);
    }
    return ptr;
}