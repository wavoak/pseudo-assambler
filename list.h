#ifndef listh
#define listh
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum{False,True};

typedef struct node{
	char *data;
	char *key;
	struct node *next;
}node;

typedef struct list{
	node *head;
	node *current;
}list;

void initlist(list *ptr);
void printList(list *ptr);
node* getHead(list *l);
node* getNext(node *n);
char* getKey(node *n);
char* getData(node *n);
void insert(list *ptr, char *key, char *data);
void updateKey(list *ptr, char *key);
void updateData(node *n, char *data);
void addData(list *ptr, char *data);
void addDataTo(node *ptr, char *data);
int isEmpty(list *ptr);
int length(list *ptr);
node* find(list *ptr, char *key);
node* findData(list *l, char *data);

#endif
