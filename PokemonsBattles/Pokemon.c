#include "Pokemon.h"
#define PTR_NULL(ptr) {  (ptr) = NULL; }
#define VAL_NOT_NULL(ptr)   ((ptr) != NULL ? 1 : 0)  /*condition ? value_if_true : value_if_false*/


struct Type{
    char* name; /*type name*/
    /* for quantity -> Use: getNumberOfObjectsInCategory(Battle b,char* category) [in MainSecond] */
    pType* effective_against_me;
    pType* effective_against_others;
    int len_EfAgMe;
    int len_EfAgOthers;
};

struct Bio{
    double height;
    double  weight;
    int attack;
};

struct Pokemon{
    char* name;
    char* specie;
    char* type_name;
    pType ptype;
    pBio p_Bio;
};

/******************************* Util Functions Declarations   *******************************/
status ShiftDynamicListLeftFromIndex(pType** pEf_Ag_List, int Index, int* len);
/**********************************************************************************************/

/// Create Functions:
pType CreateNewType(char* name)
{
    if(VAL_NOT_NULL(name)) {
        /*ALLOCATE struct Type */
        pType new_pType = (pType) malloc(sizeof(struct Type));  //DON'T FORGET TO FREE: freeType()
        if (new_pType == NULL) return NULL;
        /*ALLOCATE (CHAR*)  */
        new_pType->name = (char *) malloc(strlen(name) + 1);
        if (new_pType->name == NULL) return NULL;
        strcpy(new_pType->name, name);


        new_pType->effective_against_me = NULL;
        new_pType->effective_against_others = NULL;
        new_pType->len_EfAgMe = 0;
        new_pType->len_EfAgOthers = 0;
        return new_pType;
    }
    return NULL;
}//CreateNewType


pPokemon CreateNewPokemon(char* name, char* specie,char* height, char* weight, char* attak, char* ptype_name, pType ptype)
{
    if( VAL_NOT_NULL(name) && VAL_NOT_NULL(specie) && VAL_NOT_NULL(height) && VAL_NOT_NULL(weight) && VAL_NOT_NULL(attak) && VAL_NOT_NULL(ptype_name)
        && VAL_NOT_NULL(ptype))
    {
        /*ALLOCATE struct pokemon */
        pPokemon new_pPokemon = (pPokemon) malloc(sizeof(struct Pokemon));
        if (new_pPokemon == NULL) return NULL;

        /*ALLOCATE struct Bio */
        new_pPokemon->p_Bio = CreateBioInfo(height, weight, attak);     //MALLOC was invoked
        if (new_pPokemon->p_Bio == NULL) return NULL;

        /*ALLOCATE (char*) */
        new_pPokemon->name = (char *) malloc(strlen(name) + 1);     //MUST malloc+strcpy ALL Cstrings because the BUFFER in the fileReader will erase it in next iteration
        if (new_pPokemon->name == NULL) return NULL;
        strcpy(new_pPokemon->name, name);

        /*ALLOCATE (char*) */
        new_pPokemon->specie = (char *) malloc(strlen(specie) + 1);
        if (new_pPokemon->specie == NULL) return NULL;
        strcpy(new_pPokemon->specie, specie);

        /*ALLOCATE (char*) */
        new_pPokemon->type_name = (char *) malloc(strlen(ptype_name) + 1); // Check if problematic
        if (new_pPokemon->type_name == NULL) return NULL;
        strcpy(new_pPokemon->type_name, ptype_name);

        new_pPokemon->ptype = ptype;
        return new_pPokemon;
    }
    return NULL;
}//CreateNewPokemon


pBio CreateBioInfo( char* height, char* weight, char* attak)
{
    pBio new_pBio = (pBio)malloc(sizeof(struct Bio));
    if(new_pBio == NULL)    return  NULL;
    new_pBio->height = strtod(height, NULL);
    new_pBio->weight = strtod(weight, NULL);
    new_pBio->attack = strtol(attak, NULL, 10);
    return new_pBio;
}//CreateBioInfo


/*****************************************************************************************************************/
///Free Functions:
status freeType(element ptype)
{
    if(ptype != NULL) {
        pType ptype_ref = (pType)ptype;
        if(ptype_ref->effective_against_me != NULL) {
            for (int i = 0;i < ptype_ref->len_EfAgMe; i++)  ///NOTICE: by freeing the pointed Types  in this list we will actually
                // BE ERASING THE ENTIRE EXISTENCE OF THESE TYPES IN MEMORY
                //THAT'S WHY WE DON'T FREE THEM!
            {
                ptype_ref->effective_against_me[i] = NULL;
            }

            free(ptype_ref->effective_against_me);
            ptype_ref->effective_against_me = NULL;
        }
        if(ptype_ref->effective_against_others != NULL) {
            for (int i = 0; i < ptype_ref->len_EfAgOthers; i++) {
                ptype_ref->effective_against_others[i] = NULL;
            }
            free(ptype_ref->effective_against_others);
            ptype_ref->effective_against_others = NULL;
        }

        if(ptype_ref->name !=NULL) free(ptype_ref->name);
        free(ptype);    //freeing ptype_ref is the same probably, but make sure
        return success;
    }
    return  failure;
}//freeType


status freepPokemon(element p_pokemon)
{
    if(p_pokemon != NULL) {
        pPokemon pkmn = (pPokemon)p_pokemon;
        if (pkmn->name != NULL) {free(pkmn->name); PTR_NULL(pkmn->name)}

        if (pkmn->specie != NULL) {free(pkmn->specie); PTR_NULL(pkmn->specie)}

        if (pkmn->type_name != NULL) {free(pkmn->type_name);PTR_NULL(pkmn->type_name)}

        if (pkmn->p_Bio != NULL) {freepBio(pkmn->p_Bio); PTR_NULL(pkmn->p_Bio)}

        free(pkmn->p_Bio);
        PTR_NULL(pkmn->p_Bio)

        pkmn->ptype=NULL;

        free(p_pokemon);
        PTR_NULL(p_pokemon)
        return success;
    }
    return failure;
}//freepPokemon


status freepBio(element p_bio)
{
    if(p_bio == NULL) return failure;
    free(p_bio);
    return success;
}//freeBio


/*****************************************************************************************************************/
/// Type Functions:
status AddTo_EffectiveAgainst_Me(pType A, pType B)
{
    if(A->len_EfAgMe == 0 && A->effective_against_me == NULL)
    {
        A->effective_against_me = (pType*)malloc(sizeof(pType));
        if(A->effective_against_me == NULL)
        {
            //   printf("Overflow!\n");
            return failure;
        }
        A->len_EfAgMe++;
        A->effective_against_me[0] = B; //CHECK THAT INIT REALLY WORKED - don't forget B is a pointer to Type so no freeing !
    }
    else        //len_EfAgMe >= 1
    {
        A->effective_against_me = (pType*)realloc(A->effective_against_me, sizeof(pType)*(A->len_EfAgMe + 1));  //CONSIDER: using a tmp (in case realloc fails...)
        if(A->effective_against_me == NULL && A->len_EfAgMe >= 1){  //realloc will return a NULL pointer if the requested size is 0
            //* No memory available *//
            return failure;

        }
        A->len_EfAgMe++;
        A->effective_against_me[A->len_EfAgMe - 1] = B;
    }
    return success;
}//AddTo_EffectiveAgainst_Me


status AddTo_EffectiveAgainst_Others(pType A, pType B)
{
    if(A->len_EfAgOthers == 0 && A->effective_against_others == NULL)
    {
        A->effective_against_others = (pType*)malloc(sizeof(pType));
        if(A->effective_against_others == NULL)
        {
            printf("Overflow!\n");
            return failure;
        }
        A->len_EfAgOthers++;
        A->effective_against_others[0] = B; //CHECK THAT INIT REALLY WORKED - don't forget B is a pointer to Type so no freeing !
    }
    else        //len_EfAgOthers >= 1
    {

        A->effective_against_others = (pType*)realloc(A->effective_against_others, sizeof(pType)*(A->len_EfAgOthers + 1));  //CONSIDER: using a tmp (in case realloc fails...)
        if(A->effective_against_others == NULL && A->len_EfAgOthers >= 1){  //realloc will return a NULL pointer if the requested size is 0
            //* No memory available *//
            return failure;
        }
        A->len_EfAgOthers++;
        A->effective_against_others[A->len_EfAgOthers - 1] = B;
    }
    return success;
}//AddTo_EffectiveAgainst_Others



status DeleteFrom_EffectiveAgainst_Me(pType A, char* Bname) //MAKE SURE she really meant recieving a Cstring and not the actual pType
{
    status sttus = success;
    if(A != NULL && Bname != NULL && A->len_EfAgMe > 0)
    {
        int i;

        //trimwhitespace(Bname);        //uncomment if necessary
        for( i = 0; i < A->len_EfAgMe; i++)
        {

            if(strcmp(A->effective_against_me[i]->name, Bname) == 0)
            {
                A->effective_against_me[i] = NULL;      //turn B to NULL (same as "deleting"
                //shift effective_against_me from the i'th pos 1 cell to the left + SHRINK by Address
                sttus = ShiftDynamicListLeftFromIndex(&(A->effective_against_me), i, &(A->len_EfAgMe));     //REALLOC IS CURRENTLY PART OF ShiftDynamic..... CONSIDER SEPARATING TO 2 DIFFERENT FUNCTIONS
                if(sttus == failure){
                    return failure;
                }
                return  success;
            }
        }
        return  failure;
    }
    printf("Delete From Effective Against Me received a NULL pointer or ef_ag_list length is 0 ");
    return failure;
}//DeleteFrom_EffectiveAgainst_Me


status DeleteFrom_EffectiveAgainst_Others(pType A, char* Bname)
{
    status sttus = success;
    if(A != NULL && Bname != NULL && A->len_EfAgOthers > 0)
    {
        int i;
        //trimwhitespace(Bname);        //uncomment if necessary
        for( i = 0; i < A->len_EfAgOthers; i++)
        {
            if(strcmp(A->effective_against_others[i]->name, Bname) == 0)
            {
                A->effective_against_others[i] = NULL;      //turn B to NULL (same as "deleting"

                //shift effective_against_me from the i'th pos 1 cell to the left + SHRINK by Address
                sttus = ShiftDynamicListLeftFromIndex(&(A->effective_against_others), i, &(A->len_EfAgOthers));     //REALLOC IS CURRENTLY PART OF ShiftDynamic..... CONSIDER SEPARATING TO 2 DIFFERENT FUNCTIONS
                if(sttus == failure){
                    return failure;
                }
                return  success;
            }
        }
        return  failure;
    }
    printf("Delete From Effective Against Me received a NULL pointer or ef_ag_list length is 0 ");
    return failure;

}//DeleteFrom_EffectiveAgainst_Others

char* getTypeName(element ptype){
    pType p_type = (pType)ptype;
    return p_type->name;
}//getTypeNmae


bool isInEffectiveAgainstMe(element source_pPokemon, element toFind_pPokemon){//Input: (pType, pType) , Output: true- if typeToFind is in source->effectiveAgainstme
    if(VAL_NOT_NULL(source_pPokemon) && VAL_NOT_NULL(toFind_pPokemon)){
        pType this_source_pType = ((pPokemon)(source_pPokemon))->ptype;
        pType this_toFind_pType =  ((pPokemon)(toFind_pPokemon))->ptype;
        for(int i = 0; i < this_source_pType->len_EfAgMe ; i++){
            if(this_toFind_pType == this_source_pType->effective_against_me[i]) return true;
        }
        return false;
    }
    return false;

}

bool isInEffectiveAgainstOther(element source_pPokemon, element toFind_pPokemon){//Input: (pType, pType) , Output: true- if typeToFind is in source->effectiveAgainstme
    if(VAL_NOT_NULL(source_pPokemon) && VAL_NOT_NULL(toFind_pPokemon)){
        pType this_source_pType = ((pPokemon)(source_pPokemon))->ptype;
        pType this_toFind_pType =  ((pPokemon)(toFind_pPokemon))->ptype;
        for(int i = 0; i<this_source_pType->len_EfAgOthers; i++){
            if(this_toFind_pType == this_source_pType->effective_against_others[i]) return true;
        }
        return false;
    }
    return false;

}//isInEffectiveAgainstOther



element *getEffectiveAgainstMe(element source_ptype){ //Input: pType source, Output: list of pTypes
    if(VAL_NOT_NULL(source_ptype)) {
        pType this_source_pType = (pType) source_ptype;
        return (element *)this_source_pType->effective_against_me;
    }
    return NULL;
}

element *getEffectiveAgainstOther(element source_ptype) { //Input: pType source, Output: list of pTypes
    if (VAL_NOT_NULL(source_ptype)) {
        pType this_source_pType = (pType) source_ptype;
        return (element *) this_source_pType->effective_against_others;
    }
    return NULL;
}




pType FindpTypeInArr(char* name, pType* pType_arr, int len)
{
    int i = 0;
    while (i < len && strcmp(getTypeName(pType_arr[i]), name) != 0)
        // indx i range should be validated before accessing memory block otherwise we'll get SIGSEGV
    {
        i++;
    }
    if(i < len){
        return pType_arr[i];
    }
    return NULL;
}//FindpTypeInArr

int getLength_EffectiveAgainstMe(element source_ptype)
{
    pType PokeType=(pType)source_ptype;
    return PokeType->len_EfAgMe;
}


int getLength_EffectiveAgainstOther(element source_ptype) {
    pType PokeType = (pType) source_ptype;
    return PokeType->len_EfAgOthers;
}
/**********************************************************************************************************************/
///Print Functions:
status PrintTypeInfo(int numOfPokemonsInType, element elem_ptype)
{
    if(VAL_NOT_NULL(elem_ptype)) {
        pType ptype = (pType) elem_ptype;
        printf("Type %s -- %d pokemons\n", ptype->name, numOfPokemonsInType);
        if (ptype->len_EfAgOthers == 0 && ptype->len_EfAgMe == 0) {
            printf("\n");
        }
        if (ptype->len_EfAgMe > 0) {
            printf("\tThese types are super-effective against %s:", ptype->name);
            for (int i = 0; i < ptype->len_EfAgMe - 1; i++) {
                printf("%s ,", ptype->effective_against_me[i]->name);
            }
            printf("%s\n", ptype->effective_against_me[ptype->len_EfAgMe - 1]->name);
        }

        if (ptype->len_EfAgOthers > 0) {
            printf("\t%s moves are super-effective against:", ptype->name);
            for (int i = 0; i < ptype->len_EfAgOthers - 1; i++) {
                printf("%s ,", ptype->effective_against_others[i]->name);
            }
            printf("%s\n\n", ptype->effective_against_others[ptype->len_EfAgOthers - 1]->name);
        }

        return success;
    }
    return failure;
}


status PrintPokemonInfo(element p_pokemon)      //Input: pPokemon
{
    if(VAL_NOT_NULL(p_pokemon)) {
        pPokemon pkmn = (pPokemon) p_pokemon;
        printf("%s :\n", pkmn->name);
        char *pok_typeName = (pkmn)->ptype->name;  ///Input: pPokemon, Output: (char*)type_name
        printf("%s, %s Type.\n", pkmn->specie, pok_typeName);
        printf("Height: %0.2f m    Weight: %0.2f kg    Attack: %d\n\n", pkmn->p_Bio->height, pkmn->p_Bio->weight,
               pkmn->p_Bio->attack);
        return success;
    }
    return  failure;
}//PrintPokemonInfo


/**********************************************************************************************************************/
///Pokemon functions:

int getPokemonAttack(element p_pokemon){
    if(VAL_NOT_NULL(p_pokemon)) {
        pPokemon pkmn = (pPokemon) p_pokemon;
        return pkmn->p_Bio->attack;
    }
    return -1;
}//getPokemoAttack

element deepCopyPokemon(element p_pokemon){

    if(VAL_NOT_NULL(p_pokemon)) {
        pPokemon orig_pokemon = (pPokemon) p_pokemon;    //get a specific pointer instead of the element (aka void*)
        /*ALLOCATE new struct Pokemon*/
        pPokemon copy_pokemon = (pPokemon) malloc(sizeof(struct Pokemon));
        if (copy_pokemon == NULL) return NULL;

        /*ALLOCATE new struct Bio*/
        pBio copy_pBio = (pBio) malloc(sizeof(struct Bio));
        if (copy_pBio == NULL) return NULL;

        copy_pBio->height = orig_pokemon->p_Bio->height;
        copy_pBio->weight = orig_pokemon->p_Bio->weight;
        copy_pBio->attack = orig_pokemon->p_Bio->attack;
        copy_pokemon->p_Bio = copy_pBio;

        /*ALLOCATE (char*) name */
        copy_pokemon->name = (char *) malloc(strlen(orig_pokemon->name) + 1);     //MUST malloc+strcpy ALL Cstrings
        if (copy_pokemon->name == NULL) return NULL;
        strcpy(copy_pokemon->name, orig_pokemon->name);

        /*ALLOCATE (char*)specie */
        copy_pokemon->specie = (char *) malloc(strlen(orig_pokemon->specie) + 1);
        if (copy_pokemon->specie == NULL) return NULL;
        strcpy(copy_pokemon->specie, orig_pokemon->specie);


        /*ALLOCATE (char*) */
        copy_pokemon->type_name = (char *) malloc(strlen(orig_pokemon->type_name) + 1);
        if (copy_pokemon->type_name == NULL) return NULL;
        strcpy(copy_pokemon->type_name, orig_pokemon->type_name);

        copy_pokemon->ptype = orig_pokemon->ptype;

        return (element) copy_pokemon;
    }
    return  NULL;
}//deepcopy_pokemon


char* getPokemonType(element pokemon){
    if(VAL_NOT_NULL(pokemon)) {
        pPokemon pkmn = (pPokemon) pokemon;
        return pkmn->type_name;
    }
    return NULL;
}

/**********************************************************************************************************************/
///Util Functions:

status ShiftDynamicListLeftFromIndex(pType** pEf_Ag_List, int Index, int* len)

{
    //Index is where pType elem == NULL (after deletion)
    int i = 0;
    for(i = Index; i < (*len) - 1; i++)
    {
        (*pEf_Ag_List)[i] = (*pEf_Ag_List)[i + 1];
    }
    (*len)--;
    pType* tmp = NULL;
    if(*len > 0){
        tmp = (pType*)realloc((*pEf_Ag_List),sizeof(pType)*(*len));
        if(tmp == NULL){
            printf("ShiftDynamicList: realloc failed");
            return failure;
        }
        (*pEf_Ag_List) = tmp;
    }

    return success;
}//ShiftDynamicListLeftFromIndex