#ifndef POKEMONSBATTLES_MAXHEAP_H
#define POKEMONSBATTLES_MAXHEAP_H
#include "Defs.h"

typedef struct  MaxHeap_t* Heap;

Heap CreateHeap(int capacity, char* id, equalFunction equalElement,copyFunction copyElement,
                freeFunction freeElement, printFunction printElement);
status destroyHeap(Heap heap);
status insertToHeap(Heap heap, element elem);
status printHeap(Heap heap);

/** NOTICE!!!
 * function "PopMaxHeap"
 * @return: Dynamically Allocated Pointer to the max_elem
 * therefore the User should FREE this returned pointer to element. */
element PopMaxHeap(Heap heap); /* the user needs to free the returned element */

// Don't free
element TopMaxHeap(Heap heap);  /* !Don't free the returned value! */
char* getHeapId(Heap heap);
int getHeapCurrentSize(Heap heap);

Heap deepCopyHeap(Heap heap); ///returns a deep copy of heap


#endif //POKEMONSBATTLES_MAXHEAP_H