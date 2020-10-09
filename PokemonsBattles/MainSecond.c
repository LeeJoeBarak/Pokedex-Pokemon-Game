#include "Pokemon.h"
#include "BattleByCategory.h"
#define BUF_SIZE 300
#define TYPE_NOT_EXIST "Type name doesn't exist.\n"
#define FLUSH_STDIN(x) {if(x[strlen(x)-1]!='\n'){do fgets(Junk,16,stdin);while(Junk[strlen(Junk)-1]!='\n');}else x[strlen(x)-1]='\0';}
#define PTR_NULL(ptr) {  (ptr) = NULL; }
#define PRINT_NO_MEMORY { printf("No memory available."); exit(EXIT_FAILURE);}
#define PRNT_NO_MEM_OPTION { printf("No memory available."); }
#define VAL_NOT_NULL(ptr)   ((ptr) != NULL ? 1 : 0)  /*condition ? value_if_true : value_if_false*/
#define VAL_NULL(ptr)   ((ptr) == NULL ? 1 : 0)  /*condition ? value_if_true : value_if_false*/



/************************   Util function declarations  ***********************************************/
void freeTypeArr(pType* ptype_arr, int NumOfTypes);
status freeType(element ptype);
char *trimwhitespace(char *str);
/** funcs to pass to Battle:     */
int equalPokemon(element pokemon_A, element pokemon_B );
element copyPokemon(element pokemon);
int getAttack(element pokFromUser, element pokFromSystem, int* attackPokFromUser, int* attackPokFromSystem);
/********************************  end of function declarations  ********************************/


/***why dec + def here?*/
void *xmalloc(size_t n) {
    void *p = malloc(n);
    if (p == NULL) {
        printf("No memory available.");
        exit(EXIT_FAILURE);
    }
    return p;
}


pType* pType_arr = NULL;
/***********************************************************************************************************************/
/** getAttack  :
 *
 * Input:
 * pPokemon, pPokemon, int*, int*
 *
 *
 * Output:
 *-->direct out(return): (int) - the difference (pokFromUser - pokFromSystem)
 * -->indeirect out: (int)attackPokFromUser, (int)attackPokFromSystem
 *
 * */
int getAttack(element pokFromUser, element pokFromSystem, int* attackPokFromUser, int* attackPokFromSystem) {
    if(VAL_NOT_NULL(pokFromSystem) && VAL_NOT_NULL(pokFromUser) && VAL_NOT_NULL(attackPokFromSystem) && VAL_NOT_NULL(attackPokFromUser))
    {
        pPokemon pokemonFromUser = (pPokemon) pokFromUser;
        pPokemon pokemonFromSystem = (pPokemon) pokFromSystem;

        int attack_pokemonFromUser = getPokemonAttack((element) pokemonFromUser);
        int attack_pokemonFromSystem = getPokemonAttack((element) pokemonFromSystem);

        /*! DON'T subtract 10 points TWICE */
        //if pokemonFromSystem(toFind) isIn (source)pokemonFromUser->effectiveAgainst_ME_ then subtract 10 points from pokemonFromUser
        if (isInEffectiveAgainstMe(pokemonFromUser, pokemonFromSystem)) {
            attack_pokemonFromUser -= 10;
            (*attackPokFromUser) = attack_pokemonFromUser;
        }
            //else if pokemonFromUser isIn  pokemonFromSystem->effectiveAgainst_OTHER_ then subtract 10 points from pokemonFromUser
        else if (isInEffectiveAgainstOther(pokemonFromSystem, pokemonFromUser)) {
            attack_pokemonFromUser -= 10;
            (*attackPokFromUser) = attack_pokemonFromUser;
        }

        /*! DON'T subtract 10 points TWICE */
        //if pokemonFromUser  isIn pokemonFromSystem->effectiveAgainst_ME_ then subtract 10 points from pokemonFromSystem
        if (isInEffectiveAgainstMe(pokemonFromSystem, pokemonFromUser)) {
            attack_pokemonFromSystem -= 10;
            *attackPokFromSystem = attack_pokemonFromSystem;
        }
            //else if pokemonFromSystem isIn  pokemonFromUser->effectiveAgainst_OTHER_ then subtract 10 points from pokemonFromSystem
        else if (isInEffectiveAgainstOther(pokemonFromUser, pokemonFromSystem)) {

            attack_pokemonFromSystem -= 10;
            *attackPokFromSystem = attack_pokemonFromSystem;

        }
        *attackPokFromUser=attack_pokemonFromUser;
        *attackPokFromSystem=attack_pokemonFromSystem;
        return (attack_pokemonFromUser - attack_pokemonFromSystem);
    }
    return -1;

}


int main(int argc, char *argv[]) {

    int numberOfTypes;
    int capacityInType;
    FILE *fp;


    numberOfTypes = strtol(argv[1], NULL, 10);  /** 2nd battle param*/
    capacityInType = strtol(argv[2], NULL, 10); /** 1st battle param*/
    fp = fopen(argv[3], "r");

    /** create an array of pTypes (aka Type*) */

    if (numberOfTypes > 0)
    {
        //pType_arr = (pType *) malloc(sizeof(pType) * numberOfTypes);
        pType_arr = (pType *) xmalloc(sizeof(pType)*numberOfTypes);
        /** global */
    }

    for (int i = 0; i < numberOfTypes; i++) {
        pType_arr[i] = NULL;
    }
    int len_pType_arr = 0;  /** global */


    Battle battle = NULL;
    char BUFFER[BUF_SIZE];
    char *TypeName = NULL;
    char *List_EfAg_Is = NULL;
    char *curr = NULL;
    pType pType_tmp = NULL;
    pType pType_A = NULL;
    pType pok_type_name = NULL;
    status stat;

    /** FileReader + init Battle and pType_arr */
    if (fp != NULL) {
        while (fgets(BUFFER, BUF_SIZE, fp) != NULL) {
            if (strstr(BUFFER, "Types")) {
                int i = 0;
                fgets(BUFFER, BUF_SIZE, fp);
                trimwhitespace(BUFFER);
                char buffer[BUF_SIZE] = {0};
                int len = strlen(BUFFER);
                for(int j = 0; j < len; j++){
                    buffer[j] = BUFFER[j];
                }
                trimwhitespace(BUFFER);
                /** 3rd battle param: categories */
                //create battle
                /// !! check that last type_name in Battle wasn't deprecated
                battle = createBattleByCategory(capacityInType,
                                                numberOfTypes,
                                                buffer,
                                                equalPokemon, copyPokemon,
                                                freepPokemon, getPokemonType,
                                                getAttack,
                                                PrintPokemonInfo);               //! 3. need free
                if (battle == NULL) {
                    freeTypeArr(pType_arr, numberOfTypes);
                    PTR_NULL(pType_arr)
                    PRINT_NO_MEMORY
                }
                /*****************************************************************/

                /** init pType_array */
                TypeName = strtok(BUFFER, ",");
                while (TypeName != NULL && i < numberOfTypes) {
                    //printf ("%s\n",TypeName);
                    pType_tmp = CreateNewType(TypeName);
                    if (VAL_NULL(pType_tmp)) {
                        freeTypeArr(pType_arr, numberOfTypes);
                        PTR_NULL(pType_arr)
                        PRINT_NO_MEMORY
                    }
                    pType_arr[i] = pType_tmp;
                    len_pType_arr++;    //redundant
                    i++;
                     TypeName = strtok(NULL, ", ");
                    TypeName = trimwhitespace(TypeName);
                }
                PTR_NULL(pType_tmp)
            }//end init type_arr + create battle

            else if (strstr(BUFFER, "Pokemons")) {
                break;
            }

                /* init effective against me/other lists */
                //break each line of the format: Fire effective-against-other:Grass,Ice
                //and add the types to the source-type appropriate list
            else {
                TypeName = trimwhitespace(strtok(BUFFER, " "));   //Fire effective-against-me:Water,Ground
                pType_A = FindpTypeInArr(TypeName, pType_arr, len_pType_arr);   //you can assume that the type exists
                for (int i = 0; i < 3; i++)          //get type of effective-against-me/others list to append to
                {
                    curr = strtok(NULL, "-:");
                }
                List_EfAg_Is = curr;

                //parse the names after the ":" in line, get pType to their structs and append to correct list
                if (strcmp(List_EfAg_Is, "me") == 0)    /*effective against me*/
                {
                    curr = strtok(NULL, ",");
                    while (curr != NULL) {
                        curr = trimwhitespace(curr);
                        pType_tmp = FindpTypeInArr(curr, pType_arr, len_pType_arr);
                        if (pType_tmp != NULL && pType_A != NULL) {
                            AddTo_EffectiveAgainst_Me(pType_A, pType_tmp);
                        }
                        curr = strtok(NULL, ",");
                    }
                } else /* effective against others */
                {
                    curr = strtok(NULL, ",");
                    while (curr != NULL) {
                        curr = trimwhitespace(curr);
                        pType_tmp = FindpTypeInArr(curr, pType_arr, len_pType_arr);
                        if (pType_tmp != NULL && pType_A != NULL) {
                            AddTo_EffectiveAgainst_Others(pType_A, pType_tmp);
                        }
                        curr = strtok(NULL, ",");
                    }
                }
            }//end init effective against me/other lists
            //PTR_NULL(pType_tmp)
            //PTR_NULL(pType_A)
        }

        char *PokName = NULL;
        char *PokSpecie = NULL;
        char *PokHeight = NULL;
        char *PokWeight = NULL;
        char *PokAttack = NULL;
        pPokemon pPokemon_tmp = NULL;
        //MAKE SURE THAT THE FILE CURSOR IS EXACTLY AT FIRST LINE AFTER "Pokemon" header!!!!
        while (fgets(BUFFER, BUF_SIZE, fp) != NULL) {
            //Pikachu,MousePokemon,0.4,6.0,55,Electric
            PokName = trimwhitespace(strtok(BUFFER,
                                            ","));  //necessary because sometines the first and last words in line begin/end with a bunch of whitespace
            PokSpecie = strtok(NULL, ",");
            PokHeight = strtok(NULL, ",");
            PokWeight = strtok(NULL, ",");
            PokAttack = strtok(NULL, ",");
            TypeName = trimwhitespace(strtok(NULL, ","));
            if (PokName != NULL && PokSpecie != NULL && PokHeight != NULL && PokWeight != NULL && PokAttack != NULL &&
                TypeName != NULL) {
                /** ALLOCATION OF Pokemon */
                pok_type_name = FindpTypeInArr(TypeName, pType_arr, len_pType_arr);
                pPokemon_tmp = CreateNewPokemon(PokName, PokSpecie, PokHeight, PokWeight, PokAttack,TypeName, pok_type_name);    //! 4. need free -> freed
                if (pPokemon_tmp == NULL) {
                    freeTypeArr(pType_arr, numberOfTypes);
                    PTR_NULL(pType_arr);
                    destroyBattleByCategory(battle);
                    PTR_NULL(battle);
                    PTR_NULL(pType_tmp);
                    printf("No memory available.");
                    exit(EXIT_FAILURE);
                }

                /** ALLOCATION (but it will be freed if we freed battle correctly in destroyBattle) ! */
                stat = insertObject(battle, (element) pPokemon_tmp);   /* insert pokemon to battle */
                if (stat == failure) {
                    freeTypeArr(pType_arr, numberOfTypes);
                    PTR_NULL(pType_arr)
                    destroyBattleByCategory(battle);
                    PTR_NULL(battle)
                    freepPokemon(pPokemon_tmp);
                    PTR_NULL(pPokemon_tmp)
                    PRINT_NO_MEMORY
                }

                freepPokemon(pPokemon_tmp); /// free current pokemon allocation
                PTR_NULL(pPokemon_tmp)
            }
        }
    }//FileReader
    PTR_NULL(TypeName)
    PTR_NULL(List_EfAg_Is)
    PTR_NULL(curr)
    PTR_NULL(pType_tmp)
    PTR_NULL(pType_A)

    if (fp != NULL) {

        fclose(fp);
    }


    /*NESTED FUNCTIONS in might cause problems - all "simple" methods of Pokemon.h
                       * rely on the fact that they recieve a SPECIFIC pointer to type/pokemon
                       * check you nested if-statements !! */
/***********************************   switch-case *********************************************************************/
//BEGIN SWITCH-CASE
    char Input[300] = {0};
    int option = 0;
    char Junk[16] = {0};

    pPokemon PokeToRemove = NULL;
    pPokemon p_pokemon = NULL;
    char *PokName = NULL;
    char *PokSpecie = NULL;
    char *PokHeight = NULL;
    char *PokWeight = NULL;
    char *PokAttack = NULL;
    char *PokTypeName = NULL;

    while (option != 6)
    {

        printf("Please choose one of the following numbers:\n"
               "1 : Print all Pokemons by types\n"
               "2 : Print all Pokemons types\n"
               "3 : Insert Pokemon to battles training camp\n"
               "4 : Remove stronger Pokemon by type\n"
               "5 : Fight\n"
               "6 : Exit\n");

        fgets(Input, BUF_SIZE, stdin);
        FLUSH_STDIN(Input);    // strip newline, flush extra chars
        option = strtol(Input, NULL, 10);

        switch (option){

            case 1://1 : Print all Pokemons by types
                displayObjectsByCategories(battle);
                break;


            case 2:/* 2 : Print all Pokemons types    */
                for (int k = 0; k < len_pType_arr; k++) {

                    /* get type name for getNumberOfObjectsInType */
                    char *type_name = getTypeName(pType_arr[k]);
                    /*func def:  status PrintTypeInfo(int numOfPokemonsInType, element elem_ptype);*/
                    stat = PrintTypeInfo(getNumberOfObjectsInCategory(battle, type_name), (element) pType_arr[k]);
                    if (stat == failure) {
                        option = 6;
                    }
                }
                break;



            case 3:
                /* 3 : Insert Pokemon to battles training camp */
                printf("Please enter Pokemon type name:\n");
                fgets(Input, BUF_SIZE, stdin);
                FLUSH_STDIN(Input)
                pType_A = FindpTypeInArr(Input, pType_arr, len_pType_arr);
                if(pType_A){

                    char* type_name = getTypeName(pType_A);
                    int numOfPoks_InType = getNumberOfObjectsInCategory(battle, type_name);

                    if(capacityInType > numOfPoks_InType)
                    {


                        PokTypeName = (char *) malloc(strlen(Input) + 1);
                        if (VAL_NULL(PokTypeName)) {
                            PRNT_NO_MEM_OPTION
                            option = 6;
                            break;
                        }// 0 frees
                        strcpy(PokTypeName, Input);

                        printf("Please enter Pokemon name:\n");
                        fgets(Input, BUF_SIZE, stdin);
                        FLUSH_STDIN(Input)
                        PokName = (char *) malloc(strlen(Input) + 1);
                        if (VAL_NULL(PokName)) {
                            PRNT_NO_MEM_OPTION
                            free(PokTypeName);
                            PTR_NULL(PokTypeName)
                            option = 6;
                            break;
                        }// 1 free
                        strcpy(PokName, Input);

                        printf("Please enter Pokemon species:\n");
                        fgets(Input, BUF_SIZE, stdin);
                        FLUSH_STDIN(Input)
                        PokSpecie = (char *) malloc(strlen(Input) + 1);
                        if (VAL_NULL(PokSpecie)) {
                            PRNT_NO_MEM_OPTION
                            free(PokTypeName);
                            PTR_NULL(PokTypeName)
                            free(PokName);
                            PTR_NULL(PokName)
                            option = 6;
                            break;
                        }// 2 frees
                        strcpy(PokSpecie, Input);

                        printf("Please enter Pokemon height:\n");
                        fgets(Input, BUF_SIZE, stdin);
                        FLUSH_STDIN(Input)
                        PokHeight = (char *) malloc(strlen(Input) + 1);
                        if (VAL_NULL(PokHeight)) {
                            PRNT_NO_MEM_OPTION
                            free(PokTypeName);
                            PTR_NULL(PokTypeName)
                            free(PokName);
                            PTR_NULL(PokName)
                            free(PokSpecie);
                            PTR_NULL(PokSpecie)
                            option = 6;
                            break;
                        }// 3 frees
                        strcpy(PokHeight, Input);

                        printf("Please enter Pokemon weight:\n");
                        fgets(Input, BUF_SIZE, stdin);
                        FLUSH_STDIN(Input)
                        PokWeight = (char *) malloc(strlen(Input) + 1);
                        if (VAL_NULL(PokWeight)) {
                            PRNT_NO_MEM_OPTION
                            free(PokTypeName);
                            PTR_NULL(PokTypeName)
                            free(PokName);
                            PTR_NULL(PokName)
                            free(PokSpecie);
                            PTR_NULL(PokSpecie)
                            free(PokHeight);
                            PTR_NULL(PokHeight)
                            option = 6;
                            break;
                        }// 4 frees
                        strcpy(PokWeight, Input);

                        printf("Please enter Pokemon attack:\n");
                        fgets(Input, BUF_SIZE, stdin);
                        FLUSH_STDIN(Input)
                        PokAttack = (char *) malloc(strlen(Input) + 1);
                        if (VAL_NULL(PokAttack)) {
                            PRNT_NO_MEM_OPTION
                            free(PokTypeName);
                            PTR_NULL(PokTypeName)
                            free(PokName);
                            PTR_NULL(PokName)
                            free(PokSpecie);
                            PTR_NULL(PokSpecie)
                            free(PokHeight);
                            PTR_NULL(PokHeight)
                            free(PokWeight);
                            PTR_NULL(PokWeight)
                            option = 6;
                            break;
                        }// 5 frees
                        strcpy(PokAttack, Input);

                        /** init NEW Pokemon and free it after call to "Create" */

                        p_pokemon = CreateNewPokemon(PokName, PokSpecie, PokHeight, PokWeight,PokAttack, PokTypeName, pType_A);

                        if (VAL_NULL(p_pokemon)) {
                            free(PokTypeName);
                            PTR_NULL(PokTypeName)
                            free(PokName);
                            PTR_NULL(PokName)
                            free(PokSpecie);
                            PTR_NULL(PokSpecie)
                            free(PokHeight);
                            PTR_NULL(PokHeight)
                            free(PokWeight);
                            PTR_NULL(PokWeight)
                            free(PokAttack);
                            PTR_NULL(PokAttack)
                            PRNT_NO_MEM_OPTION
                            option = 6;
                            break;
                        }// 6 frees
                        /****************** end of new pokemon creation and checks for null pointer *********************/

                        /* INSERT POKEMON TO BATTLE */
                        stat = insertObject(battle, (element) p_pokemon);

                        if (stat == failure) {
                            freepPokemon((p_pokemon));
                            free(PokTypeName);
                            PTR_NULL(PokTypeName)
                            free(PokName);
                            PTR_NULL(PokName)
                            free(PokSpecie);
                            PTR_NULL(PokSpecie)
                            free(PokHeight);
                            PTR_NULL(PokHeight)
                            free(PokWeight);
                            PTR_NULL(PokWeight)
                            free(PokAttack);
                            PTR_NULL(PokAttack)
                            PRNT_NO_MEM_OPTION
                            option = 6;
                            break;
                        }// 6 frees
                        else {
                            printf("The Pokemon was successfully added.\n");
                            PrintPokemonInfo(p_pokemon);
                        }

                        /*********************************** end of insertToBattle **********************************************/

                        ///free() ALL 6 MALLOCS + freePokemon because battle creates a copy
                        ///frees :
                        freepPokemon(p_pokemon);
                        free(PokTypeName); //1
                        PTR_NULL(PokTypeName)
                        free(PokName);  //2
                        PTR_NULL(PokName)
                        free(PokSpecie); //3
                        PTR_NULL(PokSpecie)
                        free(PokHeight); //4
                        PTR_NULL(PokHeight)
                        free(PokWeight); //5
                        PTR_NULL(PokWeight)
                        free(PokAttack); //6
                        PTR_NULL(PokAttack)

                    }
                    else{
                        printf("Type at full capacity.\n");
                        break;
                    }

                }
                else{
                    printf(TYPE_NOT_EXIST);
                    break;
                }
                break;



            case 4:
                printf("Please enter type name:\n");
                if (fgets(Input, BUF_SIZE, stdin)) {//get type name
                    FLUSH_STDIN(Input)
                    if ((pType_tmp = FindpTypeInArr(Input, pType_arr,
                                                    len_pType_arr))) {//validate type exists in system
                        PokTypeName = getTypeName(pType_tmp);
                        if (!getNumberOfObjectsInCategory(battle, PokTypeName)) {
                            printf("There is no Pokemon to remove.\n");
                            break;
                        }
                        /** NEED FREE */
                        PokeToRemove = removeMaxByCategory(battle, PokTypeName);
                        printf("the strongest Pokemon was removed:\n");
                        PrintPokemonInfo(PokeToRemove);
                        freepPokemon(PokeToRemove); ///freed
                        PokeToRemove = NULL;
                    } else {
                        printf(TYPE_NOT_EXIST);
                        break;
                    }
                }
                break;


            case 5:

                printf("Please enter Pokemon type name:\n");
                fgets(Input, BUF_SIZE, stdin);
                FLUSH_STDIN(Input)
                pType_A = FindpTypeInArr(Input, pType_arr, len_pType_arr);
                if(pType_A)
                {


                    PokTypeName = (char *) malloc(strlen(Input) + 1);
                    if (VAL_NULL(PokTypeName)) {
                        PRNT_NO_MEM_OPTION
                        option = 6;
                        break;
                    }// 0 frees
                    strcpy(PokTypeName, Input);

                    printf("Please enter Pokemon name:\n");
                    fgets(Input, BUF_SIZE, stdin);
                    FLUSH_STDIN(Input)
                    PokName = (char *) malloc(strlen(Input) + 1);
                    if (VAL_NULL(PokName)) {
                        PRNT_NO_MEM_OPTION
                        free(PokTypeName);
                        PTR_NULL(PokTypeName)
                        option = 6;
                        break;
                    }// 1 free
                    strcpy(PokName, Input);


                    printf("Please enter Pokemon species:\n");
                    fgets(Input, BUF_SIZE, stdin);
                    FLUSH_STDIN(Input)
                    PokSpecie = (char *) malloc(strlen(Input) + 1);
                    if (VAL_NULL(PokSpecie)) {
                        PRNT_NO_MEM_OPTION
                        free(PokTypeName);
                        PTR_NULL(PokTypeName)
                        free(PokName);
                        PTR_NULL(PokName)
                        option = 6;
                        break;
                    }// 2 frees
                    strcpy(PokSpecie, Input);

                    printf("Please enter Pokemon height:\n");
                    fgets(Input, BUF_SIZE, stdin);
                    FLUSH_STDIN(Input)
                    PokHeight = (char *) malloc(strlen(Input) + 1);
                    if (VAL_NULL(PokHeight)) {
                        PRNT_NO_MEM_OPTION
                        free(PokTypeName);
                        PTR_NULL(PokTypeName)
                        free(PokName);
                        PTR_NULL(PokName)
                        free(PokSpecie);
                        PTR_NULL(PokSpecie)
                        option = 6;
                        break;
                    }// 3 frees
                    strcpy(PokHeight, Input);

                    printf("Please enter Pokemon weight:\n");
                    fgets(Input, BUF_SIZE, stdin);
                    FLUSH_STDIN(Input)
                    PokWeight = (char *) malloc(strlen(Input) + 1);
                    if (VAL_NULL(PokWeight)) {
                        PRNT_NO_MEM_OPTION
                        free(PokTypeName);
                        PTR_NULL(PokTypeName)
                        free(PokName);
                        PTR_NULL(PokName)
                        free(PokSpecie);
                        PTR_NULL(PokSpecie)
                        free(PokHeight);
                        PTR_NULL(PokHeight)
                        option = 6;
                        break;
                    }// 4 frees
                    strcpy(PokWeight, Input);

                    printf("Please enter Pokemon attack:\n");
                    fgets(Input, BUF_SIZE, stdin);
                    FLUSH_STDIN(Input)
                    PokAttack = (char *) malloc(strlen(Input) + 1);
                    if (VAL_NULL(PokAttack)) {
                        PRNT_NO_MEM_OPTION
                        free(PokTypeName);
                        PTR_NULL(PokTypeName)
                        free(PokName);
                        PTR_NULL(PokName)
                        free(PokSpecie);
                        PTR_NULL(PokSpecie)
                        free(PokHeight);
                        PTR_NULL(PokHeight)
                        free(PokWeight);
                        PTR_NULL(PokWeight)
                        option = 6;
                        break;
                    }// 5 frees
                    strcpy(PokAttack, Input);
                    //get pokemon type_name
                    //get pok ptype
                    // UpdateEfMe_len(pok_ptype);
                    /** init NEW Pokemon and free it after call to "Create" */
                    p_pokemon = CreateNewPokemon(PokName, PokSpecie, PokHeight, PokWeight,
                                                 PokAttack, PokTypeName, pType_A);

                    if (VAL_NULL(p_pokemon)) {
                        free(PokTypeName);
                        PTR_NULL(PokTypeName)
                        free(PokName);
                        PTR_NULL(PokName)
                        free(PokSpecie);
                        PTR_NULL(PokSpecie)
                        free(PokHeight);
                        PTR_NULL(PokHeight)
                        free(PokWeight);
                        PTR_NULL(PokWeight)
                        free(PokAttack);
                        PTR_NULL(PokAttack)
                        PRNT_NO_MEM_OPTION
                        option = 6;
                        break;
                    }// 6 frees
                    /****************** end of new pokemon creation and checks for null pointer *********************/


                    printf("you choose to fight with:\n");
                    PrintPokemonInfo(p_pokemon);
                    /* FIND BEST POKEMON FROM BATTLE SYSTEM TO FIGHT AGAINST P_POKEMON
                     * Notice: fight already prints the battle summary - no further prints are necessary*/
                    element ret_fight = fight(battle, p_pokemon);


                    ///free() ALL 6 MALLOCS + freePokemon because battle creates a copy
                    ///frees :
                    freepPokemon(p_pokemon);//0
                    free(PokTypeName); //1
                    PTR_NULL(PokTypeName)
                    free(PokName);  //2
                    PTR_NULL(PokName)
                    free(PokSpecie); //3
                    PTR_NULL(PokSpecie)
                    free(PokHeight); //4
                    PTR_NULL(PokHeight)
                    free(PokWeight); //5
                    PTR_NULL(PokWeight)
                    free(PokAttack); //6
                    PTR_NULL(PokAttack)

                }
                else{
                    printf(TYPE_NOT_EXIST);
                    break;
                }
                break;



            case 6:
                /** free pType_arr and ALL TYPES in program*/
                    freeTypeArr(pType_arr,numberOfTypes);   //KEEP IN MIND: if you free Types before Pokemons then all PokemonTypes will be vanished from the program
                    PTR_NULL(pType_arr)

                /** free battle */
                stat = destroyBattleByCategory(battle);
                if (stat == failure) {
                    //if stat is failure then somethings wrong with the function or param battle
                    /**!!!!! DELETE THIS PRINT!!!!!**/
                    printf("check destroyBattle.....");
                }
                PTR_NULL(battle)

                /** free switch-case variables (theoretically they all have been freed and got value NULL but should check them anyway) */
                if (VAL_NOT_NULL(PokeToRemove)) {
                    freepPokemon(PokeToRemove);
                    PTR_NULL(PokeToRemove)
                } //(freed in case 4)


                printf("All the memory cleaned and the program is safely closed.");
                break;

            default:
                printf("‫‪Please‬‬ ‫‪choose‬‬ ‫‪a‬‬ ‫‪valid‬‬ ‫‪number.\n");
                break;

        }//end switch-case

    }//END WHILE

/*********** FREE ALL FRAMEWORK VARIABLES AND EXIT ***************/



}//main





int equalPokemon(element pokemon_A, element pokemon_B ){
    pPokemon pok_A = (pPokemon)pokemon_A;
    pPokemon pok_B = (pPokemon)pokemon_B;
    int attack_A = getPokemonAttack(pok_A);
    int attack_B = getPokemonAttack(pok_B);
    return attack_A > attack_B;
}//equalPokemon


///MUST FREE THE RETURNES VALUE FROM copyFunc
element copyPokemon(element pokemon){
    pPokemon copy_pokemon = (pPokemon)deepCopyPokemon((pPokemon)pokemon);
    if(VAL_NOT_NULL(copy_pokemon)) return copy_pokemon; //VAL_NOT_NULL(copy_pokemon) returns '1' if copy_pok!=NULL
    return NULL;
}//copyPokemon


void freeTypeArr(pType* ptype_arr, int NumOfTypes)

{
    if(ptype_arr == NULL){
        return;
    }
    else
    {
        for(int i = 0; i < arr_len; i++)
        {
            freeType(ptype_arr[i]);
            ptype_arr[i] = NULL;
        }
    }
    free(ptype_arr);
}//freeTypeArr


/* Note: This function returns a pointer to a substring of the original string.
 If the given string was allocated dynamically, the caller must not overwrite
 that pointer with the returned value, since the original pointer must be
 deallocated using the same allocator with which it was allocated.  The return
 value must NOT be deallocated using free() etc.*/
char *trimwhitespace(char *str)
{
    char *end;

    if(str != NULL) {
        // Trim leading space
        while (isspace((unsigned char) *str)) str++;

        if (*str == 0)  // All spaces?
            return str;

        // Trim trailing space
        end = str + strlen(str) - 1;    // check if the LAST char is a whitespace
        while (end > str && isspace((unsigned char) *end)) end--;

        // Write new null terminator character
        end[1] = '\0';

        return str;
    }
    return  NULL;
}//trimwhitespace