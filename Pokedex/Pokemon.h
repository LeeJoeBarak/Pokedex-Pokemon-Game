//
// Created by LeeJoe on 23/11/2019.
//

#ifndef POKEDEX_POKEMON_H
#define POKEDEX_POKEMON_H

#include "Defs.h"
#include <stdio.h>
#include <stdlib.h>


typedef struct PokemonType{
    char* Tname;
    int quantity;
    struct PokemonType** effective_against_me;
    struct PokemonType** effective_against_others;
} PokemonType;


typedef struct PokemonBio{
    double height;
    double weight;
    int attack;
} PokemonBio;


typedef  struct Pokemon{
    char* Pname;
    char* specie;
    PokemonType* Pokemon_type;
    PokemonBio* Pokemon_Bio;
} Pokemon;



ERRORCODE FileReaderPokemon(FILE *file, int numberOfPokemons, char* ret_str);
ERRORCODE InitPokemon(char** pokemon_info);
ERRORCODE InitPokemonBio(PokemonBio** pPokemonBio, double height, double weight, int attack);


#endif //POKEDEX_POKEMON_H
