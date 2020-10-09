#ifndef POKEMONSBATTLES_POKEMON_H
#define POKEMONSBATTLES_POKEMON_H


#include "Defs.h"
#include "BattleByCategory.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>



typedef struct Type* pType;
typedef struct Bio* pBio;
typedef  struct Pokemon* pPokemon;
int arr_len;

/** Create Functions: */
pPokemon CreateNewPokemon(char* name, char* specie,char* height, char* weight, char* attak, char* ptype_name, pType ptype);
pBio CreateBioInfo( char* height, char* weight, char* attak);
/* Each type has a effectiveAgainstMe list and an effectiveAgainstOther list of pTypes that hold the Types that are effective me/other the source_type, respectively.
 * NOTICE: this module does not keep a copy of the Types you create. Instead, the module assumes that
 * the User is keeping a data structure of pTypes and that every Type exists in memory exactly once
 * (meaning that the module assumes EXACTLY ONE ADDRESS per Type) */
pType CreateNewType(char* name);


/** Type Functions:    */
status AddTo_EffectiveAgainst_Me(pType A, pType B);
status AddTo_EffectiveAgainst_Others(pType A, pType B);
status DeleteFrom_EffectiveAgainst_Me(pType A, char* Bname);
status DeleteFrom_EffectiveAgainst_Others(pType A, char* Bname);
char* getTypeName(element ptype);   //Input: pType , Output: the type name as char*
int getLength_EffectiveAgainstMe(element source_ptype);
int getLength_EffectiveAgainstOther(element source_ptype);
element *getEffectiveAgainstMe(element source_ptype);   //Input: pType source, Output: list of pTypes
element *getEffectiveAgainstOther(element source_ptype);   //Input: pType source, Output: list of pType




bool isInEffectiveAgainstMe(element source_pPokemon, element toFind_pPokemon); //Input: (pPokemon, pPokemo) , Output: true- if typeToFind is in source->effectiveAgainstme


bool isInEffectiveAgainstOther(element source_pPokemon, element toFind_pPokemon);// Input: getEffectiveAgainstOther[i]
pType FindpTypeInArr(char* name, pType* pType_arr, int len);
void UpdateEfMe_len(pType pok_ptype);




/** Pokemon Functions:  */
int getPokemonAttack(element p_pokemon);     //Input: pPokemon, Output: int pokAttack
/*!NOTICE: deepCopyPokemon returns a DINAMICALLY ALLOCATED pPOKEMON - make sure you free it at end of program */
element deepCopyPokemon(element p_pokemon);  //Input: pPokemon, Output: pPokemon pok_copy
char* getPokemonType(element pokemon);    //Input: pPokemon, Output: (char*)type_name


/**Free Functions: */
/*Notice: freeType should only be invoked at program finish.
 * Invoking freeType before end of program will cause corrupt the data in this module
 * and will resut in a buggy program. Don't free your Types  */
status freeType(element ptype);
status freepPokemon(element p_pokemon);
status freepBio(element p_bio);

/** Print Functions: */
status PrintPokemonInfo(element p_pokemon);
status PrintTypeInfo(int numOfPokemonsInType, element elem_ptype);

/** */


#endif //POKEMONSBATTLES_POKEMON_H