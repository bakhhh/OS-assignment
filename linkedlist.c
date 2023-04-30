#include <stdio.h>
#include <stdlib.h>
#include "linkedlist.h"

//updated linked list used in UCP

linkedlist * createLinkedList(int queueSize){
    linkedlist * list;
    list = (linkedlist*)malloc(sizeof(linkedlist));
    list->head = NULL;
    list->count = 0;
    list->queueSize = queueSize;
    return list;
}

void insertLast(linkedlist *list, void * value){
    node * newNode = (node*)malloc(sizeof(node));
    node * currNode = list->head;
    newNode->value = value;
    newNode->next = NULL;
    if (list->count == list->queueSize){
        printf("Queue Full\n");
        return;
    }
    if (list->count == 0) {
        list->head = newNode;
    }
    else{
        while(currNode->next != NULL){
            currNode = currNode->next;
        }
        currNode->next = newNode;
    }
    list->count++;
}
void* deleteFirst(linkedlist *list)
{
    void *data = NULL;
    node *temp = list->head;
    if (list->count ==0){
        printf("empty\n");
    }
    else if(temp->next ==NULL){
        data = temp->value;
        list->head = NULL;
        free(temp);

    }
    else{
        data = temp->value;
        list->head = list->head->next;
        free(temp);
    }
    list->count--;
    return data;

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

