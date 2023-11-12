#include "list.h"

void initlist(list *l){
	l->head = NULL;
	l->current = NULL;
}

void printList(list *l){
	node *ptr = l->head;
	printf("{");
	
	while(ptr != NULL){
		printf("\n[%s,%s]",ptr->key,ptr->data);
		ptr = ptr->next;
	}
	printf("\n}\n");
}

node* getHead(list *l){
	return l->head;
}

node* getNext(node *n){
	return n->next;
}

char* getKey(node *n){
	char *key = (char*)malloc(sizeof(char)*(strlen(n->key)+1));
	strcpy(key,n->key);
	return key;
}

char* getData(node *n){
	char *data = (char*)malloc(sizeof(char)*(strlen(n->data)+1));
	strcpy(data,n->data);
	return data;
}

void insert(list *l, char *key, char *data){
	node *link = (node*)malloc(sizeof(node));
	int l1 = strlen(key)+1;
	int l2 = strlen(data)+1;
	link->key = (char*)malloc(sizeof(char)*l1);
	link->data = (char*)malloc(sizeof(char)*l2);
	
	strcpy(link->key,key);
	strcpy(link->data,data);
	
	if(l->head==NULL){
		l->head = link;
		l->current = link;
	}
	else{
		l->current->next = link;
		l->current = l->current->next;
	}
}

/*updates the key of the current node*/
void updateKey(list *l, char *key){
	l->current->key = (char*)malloc(sizeof(char)*(strlen(key)+1));
	strcpy(l->current->key,key);
}

void updateData(node *n, char *data){
	int len = strlen(data);
	n->data = (char*)malloc(sizeof(char)*(len+1));
	strcpy(n->data,data);
}

/*adds data to the data of the current node*/
void addData(list *l, char *data){
	char *copy = (char*)malloc(sizeof(char)*(strlen(data)+1));
	strcpy(copy,data);
	if(strlen(l->current->data)==0){
		l->current->data = (char*)malloc(sizeof(char)*(strlen(data)+1));
		strcpy(l->current->data,copy);
	}
	else{
		strcat(l->current->data,copy);
	}
}

void addDataTo(node* n, char *data){
	char *copy = (char*)malloc(sizeof(char)*(strlen(data)+1));
	strcpy(copy,data);
	if(strlen(n->data)==0){
		n->data = (char*)malloc(sizeof(char)*(strlen(data)+1));
		strcpy(n->data,copy);
	}
	else{
		strcat(n->data,copy);
	}
}

int isEmpty(list *l){
	return l->head == NULL;
}

int length(list *l){
	int length = 0;
	node *current;
	
	for(current = l->head; current != NULL; current = current->next){
		length++;
	}
	
	return length;
}

node* find(list *l, char *key){
	node* current = l->head;
	if(l->head == NULL){
		return NULL;
	}
	while(strcmp(current->key,key)!=0){
		if(current->next == NULL){
			return NULL;
		}
		else{
			current = current->next;
		}
	}      
	return current;
}

node* findData(list *l, char *data){
	node* current = l->head;
	if(l->head == NULL){
		return NULL;
	}
	while(strcmp(current->data,data)!=0){
		if(current->next == NULL){
			return NULL;
		}
		else{
			current = current->next;
		}
	}
	return current;
}

