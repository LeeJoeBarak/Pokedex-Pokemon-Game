#ifndef BATTLEBYCATEGORY_H_
#define BATTLEBYCATEGORY_H_
#include "Defs.h"

/*Observations:
 * 1. A Users that want to use this module must provide:
 * Attack, Category attributes -> for every BattleElement they insert to Battle
 * 2.
 * the Users of this module has no idea how it is implemented,
 * especially, the Users don't know that this module uses LinkedList ADT or MaxHeap ADT
 * therefore, it makes no sense to ask the Users to send */
typedef struct battle_s* Battle;

Battle createBattleByCategory(int capacity,int numberOfCategories,char* categories,equalFunction equalElement,
                              copyFunction copyElement,freeFunction freeElement,getCategoryFunction getCategory,getAttackFunction getAttack,
                              printFunction printElement);
status destroyBattleByCategory(Battle b);
status insertObject(Battle b, element elem);
void displayObjectsByCategories(Battle b);

//!!! User Must Free The Returned Element from removeMaxByCategory!!!
element removeMaxByCategory(Battle b,char* category);

int getNumberOfObjectsInCategory(Battle b,char* category); //get current size of Type ->aka updated numOfPokemons in Type

//!!! User Must Free The Returned Element from fight!!!
element fight(Battle b,element elemFromUser);


#endif /* BATTLEBYCATEGORY_H_ */