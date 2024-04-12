#include "MemoryManagement.h"

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