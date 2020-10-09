#ifndef POKEMONSBATTLES_LINKEDLIST_H
#define POKEMONSBATTLES_LINKEDLIST_H

#include "Defs.h"
typedef struct LinkedList_s* List;

typedef struct node_t* Node;


List createLinkedList(copyFunction copyf, freeFunction freef, printFunction printfunc, equalFunction equalf);
status destroyList(List list);
status appendNode(List list, element e);
status deleteNode(List list, element e);
status displayList(List list);
element searchByKeyInList(List list, element attribute);    //returns the element with key equal to given key

#endif //POKEMONSBATTLES_LINKEDLIST_H