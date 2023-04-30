#ifndef LINKEDLIST
#define LINKEDLIST

typedef struct node{ 

    void * value;  
    struct node * next;

}node;

typedef struct linkedlist{
    node * head;
    int count;
    int queueSize;

}linkedlist;

typedef void (*listFunc)(void* data);

linkedlist * createLinkedList(int queueSize);
void insertLast(linkedlist *list, void * value);
void *deleteFirst(linkedlist *list);
void printList(linkedlist *list,listFunc funcPtr);
void freeNode(node * listnode);
void freeLinkedList(linkedlist *list);




#endif

