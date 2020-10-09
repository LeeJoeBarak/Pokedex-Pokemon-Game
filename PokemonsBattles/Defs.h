#ifndef POKEMONSBATTLES_DEFS_H
#define POKEMONSBATTLES_DEFS_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum e_bool {false,
    true
}bool;

typedef enum e_status {success,
    failure
}status;


typedef void* element;
typedef element (*copyFunction)(element);
typedef status (*freeFunction)(element);
typedef status (*printFunction) (element);

/*1 if first is bigger, 0 if same, -1 if second is bigger*/
typedef int(*equalFunction)(element, element);

/*get the category of element (returns the Type of a Pokemon)*/
typedef char* (*getCategoryFunction)(element);

/*return the (first attack - second attack). also produce the attack of each element.*/
typedef int(*getAttackFunction)(element firstElem, element secondElem, int* attackFirst, int* attackSecond);

/** Below definitions are necessary to create a struct Type*/
//typedef struct battle_s* Battle;    ///CHECK THAT THIS DOESN'T CREATE COLLISION WITH typedef IN BattleByCategory.h
//typedef int (*getNumOfObjInCategory)(Battle b,char* category);
#endif //POKEMONSBATTLES_DEFS_H