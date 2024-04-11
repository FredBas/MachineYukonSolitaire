#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define DECK_SIZE 52
#define CARD_SIZE 3
#define NUMBER_OF_FOUNDATIONS 4
#define NUMBER_OF_TABLEAUS 7


typedef enum { H, D, C, S } Suit;
typedef enum { A = 1, T = 10, J = 11, Q = 12, K = 13 } Rank;

typedef struct Card {
    Rank rank;
    Suit suit;
    bool isFaceUp;
    char symbol;
    struct Card* next;
} Card;

typedef struct Cardpile {
    Card* top;
    int size;
} Cardpile;


bool isInSequence(Card card1, Card card2) {
    return card1.rank == card2.rank + 1;
}

bool isSameSuit(Card card1, Card card2) {
    return card1.suit == card2.suit;
}

bool canBePlacedBottom(Card card1, Card card2) {
    return !isSameSuit(card1, card2) && isInSequence(card2, card1);
}

bool canBePlacedFoundation(Card card1, Card card2) {
    return isSameSuit(card1, card2) && isInSequence(card1, card2);
}


void shuffleDeck(Cardpile* deck, int split) {
    // Split the deck into two piles
    Card* pile1 = deck->top;
    Card* current = pile1;
    for (int i = 0; i < split - 1; ++i) {
        current = current->next;
    }
    Card* pile2 = current->next;
    current->next = NULL;

    // Shuffle the piles
    Cardpile shuffled;
    shuffled.top = NULL;
    shuffled.size = 0;
    srand(time(NULL)); // Seed for random number generation

    while (pile1 != NULL && pile2 != NULL) {
        if (rand() % 2 == 0) { // Randomly select from pile1
            Card* temp = pile1;
            pile1 = pile1->next;
            temp->next = shuffled.top;
            shuffled.top = temp;
        } else { // Randomly select from pile2
            Card* temp = pile2;
            pile2 = pile2->next;
            temp->next = shuffled.top;
            shuffled.top = temp;
        }
    }

    // Add remaining cards from the non-empty pile to the shuffled pile
    if (pile1 != NULL) {
        current = pile1;
    } else {
        current = pile2;
    }
    while (current != NULL) {
        Card* temp = current;
        current = current->next;
        temp->next = shuffled.top;
        shuffled.top = temp;
    }

    // Update the stock with the shuffled cards
    deck->top = shuffled.top;
    deck->size = DECK_SIZE;
}

int checkDuplicate(Card *deck, int numCards) {
    for (int i = 0; i < numCards - 1; i++) {
        for (int j = i + 1; j < numCards; j++) {
            if (deck[i].rank == deck[j].rank && deck[i].suit == deck[j].suit) {
                printf("Error: Duplicate card found (%d%c) on line %d.\n", deck[i].rank, deck[i].suit, j + 1);
                return 1;
            }
        }
    }
    return 0;
}

Card *readCardsFromFile(char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error: File '%s' not found\n", filename);
        return NULL;
    }

    Card *head = NULL;
    Card *prev = NULL;
    char card[CARD_SIZE];
    int i = 0;
    while (fscanf(file, "%s", card) == 1) {
        if (strlen(card) != CARD_SIZE - 1) {
            printf("Error: Invalid card format on line %d.\n", i + 1);
            fclose(file);
            return NULL;
        }
        Card *newCard = (Card *)malloc(sizeof(Card));
        if (newCard == NULL) {
            printf("Error: Memory allocation failed.\n");
            fclose(file);
            return NULL;
        }
        newCard->rank = card[0];
        newCard->suit = card[strlen(card) - 1];
        newCard->next = NULL;
        if (head == NULL) {
            head = newCard;
        } else {
            prev->next = newCard;
        }
        prev = newCard;
        i++;
    }
    fclose(file);

    // Check if the file contains the expected number of cards
    if (i != 52) {
        printf("Error: File does not contain 52 cards.\n");
        return NULL;
    }

    return head;
}

void printCardList(Card *head) {
    while (head != NULL) {
        printf("%c%c ", head->rank, head->suit);
        head = head->next;
    }
    printf("\n");
}

void printCard(Card *card) {
    if (card->isFaceUp) {
        printf("%c%c\t", card->rank, card->suit);
    } else {
        printf("[]\t");
   }
}

void freeCardList(Card *head) {
    Card *temp;
    while (head != NULL) {
        temp = head;
        head = head->next;
        free(temp);
    }
}

void loadDeck(Cardpile* tableau, Cardpile* foundation) {
    printf("C1\tC2\tC3\tC4\tC5\tC6\tC7\n\n");
    int row = 0;
    for (int i = 0; i < DECK_SIZE; i++) {
        Card *card = tableau[i % 7].top;
        if (card == NULL) {
            printf("Error: Not enough cards in the tableau.\n");
            return;
        }
        printCard(card);
        tableau[i % 7].top = card->next;
        if (i % 7 == 6) {
            if (row % 2 == 0) {
                if (foundation[row / 2].size == 0) {
                    printf("\t\t[]\tF%d\n", row / 2 + 1);
                } else {
                    foundation[row / 2].top->isFaceUp = true;
                    printCard(foundation[row / 2].top);
                    printf("\tF%d\n", row / 2 + 1);
                }
            } else {
                printf("\n");
            }
            row++;
        }
    }

    
    printf("\n\n");
    printf("LAST Command: \n");
    printf("Message: \n");
    printf("INPUT > ");
}

void startUpPopulateTableau(Cardpile* tableau, Card *head) {
    for (int i = 0; i < DECK_SIZE; i++) {
        Card *card = head;
        head = head->next;
        tableau[i % 7].top = card;
        tableau[i % 7].size++;
        printf("%c%c ", tableau[i % 7].top->rank, tableau[i % 7].top->suit);
    }
}

void initialize(Cardpile* tableau, Cardpile *foundation, Cardpile* deck) {
    // Read cards from file

    deck->top = readCardsFromFile("../unshuffledDeck.txt");

    if (deck->top == NULL) {
        printf("Error: Failed to read cards from file.\n");
        return;
    }

    // Initialize tableau and foundation as empty stacks
    for (int i = 0; i < NUMBER_OF_TABLEAUS; ++i) {
        tableau[i].top = NULL;
        tableau[i].size = 0;
    }
    for (int i = 0; i < NUMBER_OF_FOUNDATIONS; ++i) {
        foundation[i].top = NULL;
        foundation[i].size = 0;
    }
    printf("cards read, tableaus initialized, foundations initialized\n");
    startUpPopulateTableau(tableau, deck->top);
    printf("startUpPopulateTableau has occured\n");
    loadDeck(tableau, foundation);
    printf("loaddeck successful\n");

}

void displayTableau(Cardpile tableau[]) {
    // Display tableau
    for (int i = 0; i < NUMBER_OF_TABLEAUS; ++i) {
        printf("Tableau %d: ", i + 1);
        Card* current = tableau[i].top;
        while (current != NULL) {
            if (current->isFaceUp) {
                printf("%c ", current->symbol);
            } else {
                printf("* ");
            }
            current = current->next;
        }
        printf("\n");
    }
}

void cleanup(Cardpile* deck) {
    // Free the memory allocated for the cards
    Card* current = deck->top;
    while (current != NULL) {
        Card* next = current->next;
        free(current);
        current = next;
    }
}

int main() {
    // Initialize game
    Cardpile* tableau[NUMBER_OF_TABLEAUS];
    for (int i = 0; i < NUMBER_OF_TABLEAUS; ++i) {
        tableau[i] = malloc(sizeof(Cardpile));
    }
    Cardpile* foundation[NUMBER_OF_FOUNDATIONS];
    for (int i = 0; i < NUMBER_OF_FOUNDATIONS; ++i) {
        foundation[i] = malloc(sizeof(Cardpile));
    }
    Cardpile* deck;

    initialize(tableau, foundation, &deck);

    // Main game loop
    while (true) {
        // Update

        // Draw
        system("clear"); // Clear the screen (works on UNIX-like systems)

        // Draw cards
        //displayTableau(tableau);

        // Draw foundation, stock, etc.

        // Input handling and game logic here

        // Exit condition
        break; // For now, just break the loop to exit
    }

    // Cleanup
    cleanup(&deck);

    return 0;
}
