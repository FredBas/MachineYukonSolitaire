#include "GameInitialization.h"
#include <stdio.h>

void startupPopulateTableau(Cardpile *tableau, Card *head) {
    for (int i = 0; i < DECK_SIZE; i++) {
        Card *card = head;
        head = head->next;
        tableau[i % 7].top = card;
        tableau[i % 7].size++;
        printf("%c%c ", tableau[i % 7].top->rank, tableau[i % 7].top->suit);
    }
    printf("\n");
}

void initializeStartup(Cardpile **tableau, Cardpile **foundation, Cardpile *deck) {

    // Read cards from file
    deck->top = createDeckFromFile("../unshuffledDeck.txt");

    if (deck->top == NULL) {
        printf("Error: Failed to read cards from file.\n");
        return;
    }

    // Initialize tableau and foundation as empty stacks
    for (int i = 0; i < NUMBER_OF_TABLEAUS; ++i) {
        tableau[i]->top = NULL;
        tableau[i]->size = 0;
    }
    for (int i = 0; i < NUMBER_OF_FOUNDATIONS; ++i) {
        foundation[i]->top = NULL;
        foundation[i]->size = 0;
    }
    startupPopulateTableau(tableau, deck->top);
    printUI(tableau, foundation);
}


