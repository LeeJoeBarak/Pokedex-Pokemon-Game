#include "MaxHeap.h"

/* Util macros */
#define LEFT(x) (2 * (x) + 1)
#define RIGHT(x) (2 * (x) + 2)
#define PARENT(x) ((x) / 2)

struct  MaxHeap_t{
    char* id;   //name of heap
    int size;   //actual size of heap
    int capacity;   //maximum size of heap (the amount of allocated memory for heap)
    element* data;    // an array of elements, the actual max-heap
    //funcs:
    copyFunction copyFunc;
    freeFunction freeFunc;
    printFunction printFunc;
    /* A pointer to a comparator function, used to prioritize elements */
    equalFunction equalFunc;

};

/******************      MODULE FUNCTIONS + HELPER FUNCTIONS DECLARATIONS   ***********************/
void heapifyFUNC(Heap heap, int idx);

/************************* END OF DECLARATIONS ****************************/




/******************************** CreateHeap FUNCTION ***************************************/
Heap CreateHeap(int capacity,  char* id, equalFunction equalElement, copyFunction copyElement, freeFunction freeElement, printFunction printElement)
{

    /**ALLOCATE Heap new_heap*/
    Heap new_heap = (Heap)malloc(sizeof(struct MaxHeap_t));    //MALLOC
    if(new_heap == NULL){
        return NULL;
    }
    new_heap->data = NULL;
    new_heap->id = NULL;

    /**ALLOCATE void** data*/
    new_heap->data = (element*)malloc(sizeof(element)*capacity); //the actual max heap
    if(new_heap->data == NULL){
        destroyHeap(new_heap);  ///CHECK that destroy actually worked
        new_heap = NULL;
        return NULL;
    }
    /* set all elements of heap->data array to point to NULL as a safety measure*/
    for(int j = 0 ; j < capacity; j++){
        new_heap->data[j] = NULL;
    }

    /**ALLOCATE char* copy_id*/
    char* copy_id = (char*)malloc(strlen(id) + 1);
    if(copy_id == NULL){    ///check that destroy doesn't func-up(fuck up...) !! LOL
        destroyHeap(new_heap);
        return  NULL;
    }
    strcpy(copy_id, id);
    new_heap->id = copy_id;

    /**Init basic-data-type + func-pointers attributes of heap*/
    new_heap->capacity = capacity;
    new_heap->size = 0;
    new_heap->equalFunc = equalElement;
    new_heap->copyFunc = copyElement;
    new_heap->freeFunc = freeElement;
    new_heap->printFunc = printElement;
    return new_heap;
}



/******************************** DESTROY HEAP FUNCTION ***************************************/
/***the  free() function failed when i used it on int* elements ( in freefunc(element) ) for some FUCKING reason - deal with it tomorrow!*/
status destroyHeap(Heap heap){
    status stat = failure;
    if(heap != NULL) {
        for (int i = 0; i < heap->size; i++) {
            if (heap->data[i] != NULL)
                stat = heap->freeFunc(heap->data[i]);
            if (stat == failure)//this is supposed to be indication if the User's freeFunc SUCK BALLS
                return stat;
            heap->data[i] = NULL;
        }
        if (heap->data != NULL){
            free(heap->data);
            heap->data = NULL;
        }
        if (heap->id != NULL) {
            free(heap->id);
            heap->id = NULL;
        }
        free(heap);
        return stat;
    }
    return stat;
}


/******************************** INSERT TO HEAP FUNCTION ***************************************/
//Adds a new element to the Priority Queue .
status insertToHeap(Heap heap, element elem){
    void* tmp = NULL;
    int i = 0;
    /*if heap is at FULL CAPACITY*/
    if (heap->size >= heap->capacity) { //no more space in heap
        return failure;
    }

    if (heap->size == 0) {
        heap->data[0] = heap->copyFunc(elem);
        if(heap->data[0] ==NULL) return failure;
        heap->size = 1;
        return success;
    }
    else {

        /*if heap isn't at full capacity:
         * get a copy of the element and insert to heap*/
        //heap->data[1] = Ponyta
        heap->data[heap->size] = heap->copyFunc(elem);  //MALLOC freed in destroy
        // i = 1
        i = heap->size;
        //heap->size = 2
        heap->size += 1;
    }

    //HEAPIFY
    /*The new element is swapped with its parent as long as its precedence is higher*/
    while(i > 0  && heap->equalFunc(heap->data[i], heap->data[PARENT(i)]) > 0) {
        tmp = heap->data[i];
        heap->data[i] = heap->data[PARENT(i)];
        heap->data[PARENT(i)] = tmp;
        i = PARENT(i);
    }
    return success;
}



/******************************** PRINT_HEAP FUNCTION ***************************************/
status printHeap(Heap heap){    //tricky function - probably fails - check it!
    if(heap != NULL)
    {
        if(heap->size == 0){
            printf("%s:\n", heap->id);

            printf("No elements.\n\n");
            return success;
        }
        else {
            /* create a copy of the given heap in order to pop its (the copy's) elements*/
            Heap heap_copy = deepCopyHeap(heap);
            if(heap_copy == NULL){
                return failure;
            }

            element max_elem = NULL;
            printf("%s:\n", heap->id);  //print the category name (heap id) followed by colon
            for (int i = 0; i < heap->size; i++){  //get max_elem to print: in each iteration pop max_elem and then print max_elem
                /** PRINT FORMAT OF HEAP:   */
                printf("%d. ",i + 1);

                /** get max element */
                max_elem = PopMaxHeap(heap_copy);

                /**PRINT ELEMENT --> printPokemon(pPokemon) */
                heap->printFunc(max_elem);

                /**free returned element from PopMaxHeap*/
                if(heap_copy->freeFunc(max_elem) == failure){
                    return failure;
                }
                max_elem = NULL;    //redundant?
            }
            destroyHeap(heap_copy);
        }
        return success;
    }
    return failure;
}

/**!!!! STILL NEEDS DEBUG !!!!
 *
 * (HELPER FUNCTION)*/
/*Returns NULL if MALLOC failed*/
Heap deepCopyHeap(Heap heap)
{
    if(heap != NULL)
    {
        Heap heap_copy = CreateHeap(heap->capacity, heap->id, heap->equalFunc, heap->copyFunc, heap->freeFunc, heap->printFunc);
        heap_copy->size = heap->size;
        for (int i = 0; i < heap->size; i++)
        {
            heap_copy->data[i] = heap->copyFunc(heap->data[i]); //MALLOC
        }
        return heap_copy;
    }
    return NULL;

}



/******************************** POP_MAX_HEAP FUNCTION ***************************************/
//!!!!!!!!!!the user needs to free the returned element
element PopMaxHeap(Heap heap){

    element max_elem = NULL;
    if(heap != NULL)
    {
        if (heap->size < 1)
        {
            /* Priority Queue is empty */
            return NULL;
        }
        max_elem = heap->data[0];   //max_elem points to the Max Element of the heap
        heap->data[0] = heap->data[heap->size-1];   //now our heap no longer points to the Max Element
        heap->data[heap->size-1] = NULL;
        heap->size--;
        /* Restore heap property */
        heapifyFUNC(heap, 0);
        return max_elem;
    }
    return NULL;
}

/** Turn an "almost-heap" into a heap **/
void heapifyFUNC(Heap heap, int idx) {
    /* left index, right index, largest */
    void *tmp = NULL;
    int l_idx, r_idx, lrg_idx;

    /*check that heap isn't null, and than heapify*/
    if(heap != NULL) {
        l_idx = (idx * 2);
        r_idx = (idx * 2 + 1);

        /* Left child exists, compare left child with its parent */
        if (l_idx < heap->size && heap->equalFunc(heap->data[l_idx], heap->data[idx]) >
                                  0) {/*compf: 1 if first is bigger, 0 if same, -1 if second is bigger*/
            lrg_idx = l_idx;
        } else {
            lrg_idx = idx;
        }

        /* Right child exists, compare right child with the largest element */
        if (r_idx < heap->size && heap->equalFunc(heap->data[r_idx], heap->data[lrg_idx]) > 0) {
            lrg_idx = r_idx;
        }

        /* At this point largest element was determined */
        if (lrg_idx != idx) {
            /* Swap between the index at the largest element */
            tmp = heap->data[lrg_idx];
            heap->data[lrg_idx] = heap->data[idx];
            heap->data[idx] = tmp;
            /* Heapify again */
            heapifyFUNC(heap, lrg_idx);
        }
    }
}



/******************************** TOP_MAX_HEAP FUNCTION ***************************************/
element TopMaxHeap(Heap heap)   /*in Meir's lecture he returns the actual element - Not a copy... */
{
    if (heap->size < 1) {
        return NULL;
    }
    element top_data = NULL;
    top_data = heap->data[0];
    return top_data;
}

/******************************** GET_HEAP_ID FUNCTION ***************************************/
char* getHeapId(Heap heap)
{
    return heap->id;
}



/******************************** GET_HEAP_CURRENT_SIZE FUNCTION ***************************************/

int getHeapCurrentSize(Heap heap)
{
    return heap->size;
}