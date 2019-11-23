//
// Created by LeeJoe on 23/11/2019.
//

#include "Defs.h"
#include "Pokemon.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

ERRORCODE FileReaderPokemon(FILE *file, int numberOfPokemons, char* pokemon_info){

}


ERRORCODE InitPokemonBio(PokemonBio** pPokemonBio, double height, double weight, int attack){

}

ERRORCODE InitPokemon(char** pokemon_info){
    Pokemon* ptPokemon = (Pokemon*)malloc(sizeof(Pokemon));
    if(ptPokemon == NULL){
        return ERROR_ALLOC_FAIL;
    }

    ptPokemon->Pname = pokemon_info[0];
    ptPokemon->specie = pokemon_info[1];
    double height = strtod(pokemon_info[2], NULL);  //consider using atof
    double weight = strtod(pokemon_info[3], NULL);  //consider using atof
    int attack = atoi(pokemon_info[4]);
    //init pokbio
    InitPokemonBio(&(ptPokemon->Pokemon_Bio), height, weight, attack);

    //get Pokemon Type as str and find it (if exists) in PokemonTypesList


}