#ifndef LINKEDLIST
#define LINKEDLIST

typedef struct node{ 

    void * value;  
    struct node * next;

}node;

typedef struct {
    node * head;
    int count;

}linkedlist;

typedef void (*listFunc)(void* data);

linkedlist * createLinkedList();
void insertLast(linkedlist *list, void * value);
void *deleteFirst(linkedlist *list);
void printList(linkedlist *list,listFunc funcPtr);
void freeNode(node * listnode);
void freeLinkedList(linkedlist *list);




#endif

