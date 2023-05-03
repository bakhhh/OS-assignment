//updated linked list that i created in practical 4 COMP2002 Unix Systems Programming which was also inspired and created based of the content from COMP1000 Unix and C programming
//used as my c_queue
//Name: Sohail Bakhshi
//ID: 20605126
#include <stdio.h>
#include <stdlib.h>
#include "linkedlist.h"



linkedlist * createLinkedList(int queueSize)//create linked list data structure which i will be using as my queue
{
    linkedlist * list;
    list = (linkedlist*)malloc(sizeof(linkedlist));
    list->head = NULL;
    list->count = 0;
    list->queueSize = queueSize;
    return list;
}

void insertLast(linkedlist *list, void * value) //adding it to the end makes sense because every node will be in order of arrival meaning the first one will be at the front of the queue
{
    node * newNode = (node*)malloc(sizeof(node));
    node * currNode = list->head;
    newNode->value = value;
    newNode->next = NULL;
    if (list->count == list->queueSize) //if the number of customers reach the queue limit
    {
        printf("Queue is full of customers\n");
    }
    if (list->head == NULL) //if the queue is empty set the head to the new node/customer
    {
        list->head = newNode;
    }
    else // if theres already customers then iterate through the list and search for the last node and add the new node behind that node
    {
        while(currNode->next != NULL)
        {
            currNode = currNode->next;
        }
        currNode->next = newNode;
    }
    list->count++;
}
void* deleteFirst(linkedlist *list) //fifo queue removes the first in so this is needed to remove the first customer
{
    void *data = NULL;
    node *temp = list->head;
    
    if (list->head ==NULL) // if the linked list /queue is empty 
    {
        printf("Theres no customer in the queue\n");
    }
    else if(temp->next ==NULL) // if theres only one customer in the queue
    {
        data = temp->value; //set data to the node thats getting deleted so it can be returned
        list->head = NULL; //set the head to null then free the memory allocated
        free(temp);
        temp = NULL;
    }
    else //if theres multiple customers in the queue
    {
        data = temp->value; //set data to the node thats getting deleted so it can be returned
        list->head = list->head->next; //unlink the original head value and set the head to the next value thus deleting the original
        free(temp);
        temp = NULL;
    }
    list->count--;
    return data;

}
//recursion used to free allocated memory in the linked list
void freeNode(node * listnode)
{
    if(listnode != NULL){
        freeNode(listnode->next);
        free(listnode);
    }
    
}
void freeLinkedList(linkedlist *list){
    freeNode(list->head);
    free(list);
}

