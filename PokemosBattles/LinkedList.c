#include <stdlib.h>
#include <stdio.h>
#include "LinkedList.h"

struct node_t{
    element e;
    struct node_t* next;
};

struct LinkedList_s{
    Node head;
    Node tail;

    copyFunction copyFunc;
    freeFunction freeFunc;
    printFunction printFunc;
    equalFunction equalFunc;
};



/************************************ 1. createLinkedList ***********************************************************/
List createLinkedList(copyFunction copyf, freeFunction freef, printFunction printfunc, equalFunction equalf){
    List new_ls = (List)malloc(sizeof(struct LinkedList_s)); //MALLOC
    if(new_ls == NULL){
        return  NULL;
    }
    new_ls->head = NULL;
    new_ls->tail = NULL;
    new_ls->copyFunc = copyf;   //MALLOC
    new_ls->freeFunc = freef;
    new_ls->printFunc = printfunc;
    new_ls->equalFunc = equalf;
    return new_ls;
}//createLinkedList



/************************************ 2.destroyList ***********************************************************/

status destroyList(List list){
    status stat ;
    if(list != NULL) {
        while (list->head != NULL) {
            //each node we delete is the current list->head because deleteNode() will replace
            stat = deleteNode(list, list->head->e);
            if (stat == failure) {
                return stat;
            }
        }
        free(list);
        list=NULL;
        return stat;
    }
    return stat;
}


/************************************ 3.appendNode ***********************************************************/
status appendNode(List list, element e){
    if(list != NULL) {

        element new_e = list->copyFunc(e);  //MALLOC copyfunc
        /**if list isEmpty than init head and tail to be the same */
        if (list->head == NULL)
        {
            list->head = (Node) malloc(sizeof(struct node_t));   //MALLOC
            if (list->head == NULL) {
                //destroyList(list);  //!!!!CHECK THAT THIS WORKS
                return failure;
            }

            //*init head AND tail to point to the same node*/
            list->head->e = new_e;
            list->head->next = NULL;    //because head == tail

            list->tail = list->head;
        }
        else {   /**assuming non-empty list - append new_node to be the next node after "tail"*/
            Node temp = NULL;
            temp = list->head;

            //Parse the list until temp == tail
            while(temp->next != NULL){
                temp = temp->next;
            }

            //head->node1->node2->node3->tail->NULL
            //head->node1->node2->node3->tail->new_node->NULL
            //temp == tail
            temp->next = (Node) malloc(sizeof(struct node_t));   //MALLOC
            if (temp->next == NULL) {
                destroyList(list);  //!!!!!!CHECK THAT THIS WORKS
                return failure;
            }

            temp->next->e = new_e;
            temp->next->next = NULL;

            //make tail point to tail->next
            list->tail = temp->next;

        }
        return success;
    }
    return failure;
}//appendNode



/************************************ 4.deleteNode ***********************************************************/
status deleteNode(List list, element elem){
    if (list != NULL) {
        Node node = list->head;
        Node temp = NULL;

        bool inList = false;
        status stat;

        /** check if list_length == 1 AND if elem == head->e*/
        if (list->head->next == NULL) {
            //assumption: freeFunc did: head->e = NULL
            stat = list->freeFunc(list->head->e);   //by freeing "head" we also free tail cause they point to the same place
            if (stat == failure) {    //this is supposed to be indication if the User's freeFunc SUCK BALLS
                return failure;
            }

            free(list->head); /// new addition
            list->head->e = NULL;
            list->head = NULL;
            list->tail = NULL;
            return stat;
        }


        /** check if elem is equal to head->e assuming that list_length > 1 */
        if (list->equalFunc(list->head->e, elem)) //notice there's a "!" in if-condition
        {
            //save the second node in List for later
            temp = list->head->next;
            //free head
            stat = list->freeFunc(list->head->e);//assumption: freeFunc did: temp->e = NULL
            if (stat == failure) {//this is supposed to be indication if the User's freeFunc SUCK BALLS
                return failure;
            }
            list->head->e  = NULL;

            free(list->head);
            list->head = NULL;
            //init head to be head->next (which is temp)
            list->head = temp;
            return stat;
        }
        else
        {



            /** node == head at this point; search for the node with given elem if it exists*/
            while (node->next != NULL) {     //notice there's a "!" in if-condition
                if (!list->equalFunc(node->next->e, elem)) {
                    inList = true;
                    break;
                }
                node = node->next;
            }
            /** Delete node->next */
            if (inList == true) {
                //save node->next to delete later
                temp = node->next;
                //make node point to its next's next
                node->next = node->next->next;
                //free tmp == node->next
                stat = list->freeFunc(temp->e);
                if (stat == failure) {//this is supposed to be indication if the User's freeFunc SUCK BALLS
                    return failure;
                }
                temp->e = NULL;

                free(temp);
                temp = NULL;
                return stat;
            }
        }
    }
    return failure;
}
//deleteNode



/************************************ 5.displayList ***********************************************************/
status displayList(List list) {
    if (list != NULL) {
        status stat;
        Node tmp = list->head;
        while (tmp != NULL) {
            stat = list->printFunc(tmp->e);
            if (stat == failure) {
                return failure;
            }
            tmp = tmp->next;
        }
        return success;
    }
    return failure;
}//displayList



/************************************ 6.searchByKeyInList ***********************************************************/
element searchByKeyInList(List list, element attribute){
    if(list != NULL && attribute != NULL){
        Node node = list->head;
        while (node != NULL)
        {
            if (list->equalFunc(node->e, attribute) == 0)
            {
                return node->e;
            }
            node = node->next;
        }
        return NULL;
    }
    return NULL;
}//searchByK