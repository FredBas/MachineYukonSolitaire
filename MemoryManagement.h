#ifndef MEMORY_MANAGEMENT_H
#define MEMORY_MANAGEMENT_H
#include <stdlib.h>
#include "Card.h"
#include "Cardpile.h"

void cleanup(Cardpile **deck);

void* safeMalloc(size_t size, char* errMsg);
#endif // MEMORY_MANAGEMENT_H