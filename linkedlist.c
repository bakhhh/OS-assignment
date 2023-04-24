#include <stdio.h>
#include <stdlib.h>
#include "linkedlist.h"

//linked list used in UCP

linkedlist * createLinkedList(){
    linkedlist * list;
    list = (linkedlist*)malloc(sizeof(linkedlist));
    list->head = NULL;
    list->size = 0;
    return list;
}

void insertLast(linkedlist *list, void * value){
    node * newNode = (node*)malloc(sizeof(node));
    node * currNode = list->head;
    newNode->value = value;
    newNode->next = NULL;
    if (list->size == 0) {
        list->head = newNode;
    }
    else{
        while(currNode->next != NULL){
            currNode = currNode->next;
        }
        currNode->next = newNode;
    }
    list->size++;
}
void deleteFirst(linkedlist *list)
{
    node * temp = list->head;
    list->head = list->head->next;
    free(temp);
}


void printList(linkedlist *list, listFunc funcPtr){
    node * pCur = list->head;
    while (pCur != NULL) {
        (*funcPtr)(pCur->value);
        pCur = pCur->next;
    }
    printf("\n");

}


void freeNode(node * listnode){
    if(listnode != NULL){
        freeNode(listnode->next);
        free(listnode);
    }
    
}
void freeLinkedList(linkedlist *list){
    freeNode(list->head);
    free(list);
}

