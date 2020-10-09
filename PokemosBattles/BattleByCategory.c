#include "BattleByCategory.h"
#include "MaxHeap.h"
#include "LinkedList.h"
#include <ctype.h>

struct battle_s {
    int numOfCategories;
    int capacity;
    List elems_by_cats_list;    //the "System": a LinkedList where each Node points to a MaxHeap
    ////FUNCS:
    equalFunction equalElement;
    copyFunction copyElement;
    freeFunction freeElement;
    getCategoryFunction getCategory;
    getAttackFunction getAttack;
    printFunction printElement;
    ////additional data:
    char** categories_strArr;
};

/******************      MODULE FUNCTIONS + HELPER FUNCTIONS DECLARATIONS   ***********************/
static char *trimwhitespace(char *str);
bool CategoryIsInBattle(char* category, Battle b);

///notice that heap_id in this module is always a category
int equalFunc_HeapId(element heap, element category);
void printFight(Battle  b, element eFromUser, int attack_FromUser, int attack_ChosenElem, element eChosen, bool isTie, element eWinner);
/************************* END OF DECLARATIONS ****************************/





/******************************** 1.createBattleByCategory FUNCTION ***************************************/

Battle createBattleByCategory(int capacity,int numberOfCategories,char* categories,equalFunction equalElement
        ,copyFunction copyElement,freeFunction freeElement,getCategoryFunction getCategory,getAttackFunction getAttack,printFunction printElement)
{

    /**ALLOCATE Battle new_battle*/
    Battle  new_battle = (Battle)malloc(sizeof(struct battle_s));
    if(new_battle == NULL){
        return NULL;
    }
    new_battle->elems_by_cats_list = NULL;
    new_battle->categories_strArr = NULL;

    /**Init basic-data-type + func-pointers attributes of Battle*/
    new_battle->numOfCategories = numberOfCategories;
    new_battle->capacity = capacity;
    //FUNCS: (these funcs work on BattleElements/Pokemons)
    new_battle->equalElement = equalElement;
    new_battle->copyElement = copyElement;
    new_battle->freeElement = freeElement;
    new_battle->getCategory = getCategory;
    new_battle->getAttack = getAttack;
    new_battle->printElement = printElement;

    /**ALLOCATE List new_battle->elems_by_cats_list */
    //the "System"(aka Battle): a LinkedList where each Node points to a MaxHeap [=Category]
    //ListElements are: Heap (aka struct MaxHeap_t*);  do: cast to (element)Heap
    new_battle->elems_by_cats_list = createLinkedList((element)deepCopyHeap, (element)destroyHeap, (element)printHeap, equalFunc_HeapId );
    if(new_battle->elems_by_cats_list == NULL){
        /*free(new_battle);*/
        return NULL;
    }

    /**ALLOCATE (char**) categories_strArr */
    new_battle->categories_strArr = (char**)malloc(sizeof(char*)*numberOfCategories);
    if(new_battle->categories_strArr == NULL) return NULL;
    /*set all (char*) elements of (char**)categories_srtArr to NULL */
    for(int i = 0; i < numberOfCategories; i++){
        new_battle->categories_strArr[i] = NULL;
    }


    /**ALLOCATE (char*)x(numOfCategories) */
    //init heap_arr AND (char**) categories_strArr
    Heap heap_ptr = NULL;
    char* CategoryName = NULL;  //a temp to hold the Current_CategoryName in each iteration
    int i = 0;
    if(categories != NULL)
    {
        CategoryName = strtok(categories, ","); //get first category (as Cstring)
        trimwhitespace(CategoryName);
        while (CategoryName != NULL && i < numberOfCategories)  //NOTICE that (numberOfCategories == heap_arr_length)
        {
            /**ALLOCATE new Cstring for each category_name (we'll use it in Fight function)*/
            new_battle->categories_strArr[i] = (char*)malloc(strlen(CategoryName) + 1);
            if(new_battle->categories_strArr[i] == NULL) return NULL;

            strcpy(new_battle->categories_strArr[i], CategoryName);
            /**ALLOCATE new heap to represent current Category such that: heap->id == CategoryName*/ //-->>freed inside this loop
            heap_ptr = CreateHeap(capacity, CategoryName, new_battle->equalElement, new_battle->copyElement,
                                  new_battle->freeElement, new_battle->printElement);   /*the "elements" in each heap is of type: battleElement (aka Pokemon)
                                                                                            therefore we init each heap with funcs that work on BattleElements(aka Pokemons))*/
            if(heap_ptr == NULL) return NULL;
            /*! the function appenedNode creates a DEEP COPY of the heap it's given, so we must free the heaps that we've allocated here
             * because we only use the copied heaps that are in the List */
            if(appendNode(new_battle->elems_by_cats_list, (element)heap_ptr) == failure)
                return NULL;
            destroyHeap(heap_ptr);  /* free the heap we just created */

            i++;
            CategoryName = strtok(NULL, ", ");
            //CategoryName = trimwhitespace(CategoryName);
        }
    }//end_while
    return  new_battle;
}//createBattleByCategory


/******************************** 2.destroyBattleByCategory FUNCTION ***************************************/

status destroyBattleByCategory(Battle b){
    status stat;
    int i;
    if(b != NULL) {
        if (b->elems_by_cats_list != NULL) {
            stat = destroyList(b->elems_by_cats_list);
            b->elems_by_cats_list =NULL;
            if (stat == failure) {//this is supposed to be indication if the ADT's destroyFunc SUCK BALLS
                return failure;
            }
        }
        if(b->categories_strArr != NULL){
            for(i=0; i<b->numOfCategories; i++)
            {
                if(b->categories_strArr[i] != NULL){
                    free(b->categories_strArr[i]);
                    b->categories_strArr[i] = NULL;
                }
            }
            free(b->categories_strArr);
            b->categories_strArr = NULL;
        }
        free(b);
        return success;
    }
    return failure;
}//destroyBattleByCategory



/******************************** 3.insertObject FUNCTION ***************************************/

status insertObject(Battle b, element elem)
{
    //each elem has a category - use getCategory() to get it
    char* category = b->getCategory(elem);
    bool isInBattle = CategoryIsInBattle(category, b);
    if(isInBattle == false){    //if: Category doesn't exist in b then return failure
        return failure;
    }
    else   //else: return insertToHeap(Heap, elem) - will return failure in case Heap is at FULL CAPACITY
    {
        //get ListElement(->Heap) with heap->id == getCategory(elem);
        Heap heap = (Heap)searchByKeyInList(b->elems_by_cats_list , category); //list is going to compare TypeName to heap.id (which is also "TypeName")
        /*********** TILL THIS LINE ALREADY DEBUGGED **********/

        return insertToHeap(heap,elem); //returns failure if heap is at full capacity
    }
}//insertObject



/******************************** 4.displayObjectsByCategories FUNCTION ***************************************/
void displayObjectsByCategories(Battle b)
{
    if(b != NULL) {
        displayList(b->elems_by_cats_list);
    }


}//displayObjectsByCategories



/******************************** 5.removeMaxByCategory FUNCTION ***************************************/
//!!! User Must Free The Returned Element !!!
element removeMaxByCategory(Battle b, char* category)
{
    if(b != NULL && category != NULL) {
        //check if category exists in b
        bool categoryExists = CategoryIsInBattle(category, b);
        //if category doesn't exist return NULL
        if (categoryExists == false) {
            return NULL;
        }
        else {   //category exists in Battle b

            Heap heap = (Heap)searchByKeyInList(b->elems_by_cats_list, (element) category); //get heap with heap->id == category
            return PopMaxHeap(heap);    //PopMaxHeap returns NULL if the heap is empty
        }
    }
    return NULL;
}//removeMaxByCategory



/******************************** 6.getNumberOfObjectsInCategory FUNCTION ***************************************/

int getNumberOfObjectsInCategory(Battle b,char* category){
    if(b != NULL && category != NULL) {
        //check if category exists in b
        bool categoryExists = CategoryIsInBattle(category, b);
        //if category doesn't exist return NULL
        if (categoryExists == false) {
            return -1;
        }
        else {   //category exists in Battle b
            Heap heap = (Heap)searchByKeyInList(b->elems_by_cats_list, (element) category); //get heap with heap->id == category
            return getHeapCurrentSize(heap);
        }
    }
    return -2;  //false == 0
}//getNumberOfObjectsInCategory


/******************************** 7.fight FUNCTION ***************************************/
element fight(Battle b , element elemFromUser)
{
    if(b != NULL && elemFromUser != NULL) {
        //for(int j = 0; j < b->)
        int i;
        element e_currCandidate = NULL; ///DO NOT FREE
        char* currCategory = NULL;
        Heap currHeap = NULL;
        element chosenElement = NULL;
        int iAttackChosenElement;
        int iAttackDiffFinal=1000;    //saves the attack difference between the ChosenElement and the elemFromUser
        int iAttackelemFromUser;    //FirstAttack
        int icurrCandidateAttack;   //SecondAttack
        int attak_diff;      //saves the returned value from getAttack in each iteration

        /**iterate over all categories in Battle and choose the Element with maximal Attack against elemFromUser*/
        for (i = 0; i < b->numOfCategories; i++)
        {
            currCategory = b->categories_strArr[i];
            currHeap = (Heap)searchByKeyInList(b->elems_by_cats_list, (element)currCategory);   //equalFunc compares heap->id with (char*)category
            /*!DO NOT FREE the returned element from TopMaxHeap*/
            e_currCandidate = TopMaxHeap(currHeap);
            if(e_currCandidate!=NULL) {

                /**find the BattleElement with Largest Attack against elemFromUser*/
                /*getAttack returns the diff:(first attack(OtherPokemon) - second attack(UserPokemon)). also produce the attack of each element.*/
                attak_diff = b->getAttack(elemFromUser, e_currCandidate, &iAttackelemFromUser, &icurrCandidateAttack);
                /* OtherPokemon(1st) - UserPokemon(2nd) = k
                 * FromUser: pikachu = 30
                 * squirttle = 20
                 * balbazor = 20
                 * Ponyta = 50
                 *
                 * Calc:   Other - Pikachu = k
                 * balbazor - lost 10 points: 10 - 30 = -20
                 * squirttle - pikachu lost 10 points: 20 - 20 = 0 --> squirtlle is better for the fight
                 * Ponyta - lost 10 points: 40 - 30 = 10 --> Ponyta is better than squirttle */
                /*if this is the first candidate we check*/
                if (iAttackDiffFinal == -1 && chosenElement == NULL) {
                    chosenElement = e_currCandidate;
                    iAttackChosenElement = icurrCandidateAttack;

                    iAttackDiffFinal = attak_diff;
                }
                    /*else make current element the chosen one if it has larger attack_diff in respect to the elemFromUser*/
                else if (attak_diff < iAttackDiffFinal) {
                    chosenElement = e_currCandidate;
                    iAttackChosenElement = icurrCandidateAttack;

                    iAttackDiffFinal = attak_diff;
                }
            }
        }//for_loop

        bool isTie; //indicates whether the Fight ended with a tie
        if(chosenElement == NULL){
            printf("There are no Pokemons to fight against you.\n");
            return NULL;
        }
        else if (iAttackDiffFinal > 0) {//winner is useer
            isTie = false;
            printFight(b, elemFromUser, iAttackelemFromUser, iAttackChosenElement, chosenElement, isTie, elemFromUser);
            return chosenElement;
        }
        else if(iAttackDiffFinal == 0){//it's a tie - no winner
            isTie = true;
            printFight(b, elemFromUser, iAttackelemFromUser, iAttackChosenElement, chosenElement, isTie,NULL );
            return chosenElement;
        }
        else{//winner is chosen
            isTie = false;
            printFight(b, elemFromUser, iAttackelemFromUser, iAttackChosenElement, chosenElement, isTie, chosenElement);

            return elemFromUser;
        }
    }
    return NULL;
}//fight


/******************************** HELPER FUNCTIONS ***************************************/

// Note: This function returns a pointer to a substring of the original string.
// If the given string was allocated dynamically, the caller must not overwrite
// that pointer with the returned value, since the original pointer must be
// deallocated using the same allocator with which it was allocated.  The return
// value must NOT be deallocated using free() etc.
static char *trimwhitespace(char *str)
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
}


/******************************** equalFunc_HeapId FUNCTION ***************************************/

/**compares (char*)heap_id to given (char*)category
 * returns: 1 if first is bigger,
 *          0 if heap_id == id,
 *          -1 if second is bigger*/
int equalFunc_HeapId(element heap, element category){
    char* heap_id = getHeapId((Heap)heap);
    return strcmp(heap_id, (char*)category);
}//equalFuncHeap


/**
 * returns:
 * true - if category is in Battle
 * false - else.
 */
bool CategoryIsInBattle(char* category, Battle b){
    for(int i = 0; i < b->numOfCategories; i++){
        if(searchByKeyInList(b->elems_by_cats_list, category) != NULL){
            return true;
        }
    }
    return false;
}




void printFight(Battle  b, element eFromUser, int attack_FromUser, int attack_ChosenElem, element eChosen, bool isTie, element eWinner){
    if(!isTie) {//if it's NOT a tie (there's a winner...)
        printf("The final battle between:\n");
        b->printElement(eFromUser);
        printf("In this battle his attack is :%d\n\n",attack_FromUser);



        printf("against ");
        b->printElement(eChosen);
        printf("In this battle his attack is :%d\n\n",attack_ChosenElem);
        printf("THE WINNER IS:\n");
        b->printElement(eWinner);
    }
    else{
        printf("The final battle between:\n");
        b->printElement(eFromUser);
        printf("In this battle his attack is :%d\n\n",attack_FromUser);


        printf("against ");
        b->printElement(eChosen);
        printf("In this battle his attack is :%d\n\n",attack_ChosenElem);
        printf("IT IS A DRAW.\n");
        b->printElement(eWinner);
    }

}