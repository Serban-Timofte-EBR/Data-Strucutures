#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

typedef struct {
	int data;
	struct node* next;
} node;

typedef struct {
	int data;
	struct node* next;
	struct node* prev;
} nodeD;

/*
node* reverse(node* head) {
	node* prev, *current, *next;
	current = head;
	prev = NULL;
	while (current != NULL) {
		next = current->next;
		current->next = prev;
		//stepping forward
		prev = current;
		current = next;
	}
	head = prev;
	return head;
}
*/

nodeD* getNewNode(int info) {
	nodeD* newNode = (nodeD*)malloc(sizeof(nodeD));
	newNode->data = info;
	newNode->next = NULL;
	newNode->prev = NULL;
	return newNode;
}

nodeD* insertHead(nodeD* head, int info) {
	nodeD* newNode = getNewNode(info);
	if (head == NULL) {
		head = newNode;
	}
	else {
		head->prev = newNode;
		newNode->next = head;
		head = newNode;
	}
	return head;
}

int main() {
	//node* head;
	nodeD* headD = NULL;

	headD = insertHead(headD, 2);
	headD = insertHead(headD, 4);
	headD = insertHead(headD, 6);

	nodeD* temp = headD;
	while (temp != NULL) {
		printf("%d, ", temp->data);
		temp = temp->next;
	}

	return 0;
}

//STACK

//A list with the restriction that insertion and deletion 
//can be performed only from one end, called the top

//LIFO - LAST IN FIRST OUT

//OPERTION
/*
PUSH(x)
POP
TOP
ISEMPTY
*/

/*
|			|
|			|
|			|
|			|
|			|
|___________|

PUSH(2)

|			|
|			|
|			|
|___________|
|	 2		|
|___________|

PUSH(10)

|			|
|___________|
|	 10		|
|___________|
|	 2		|
|___________|

POP

|			|
|			|
|			|
|___________|
|	 2		|
|___________|

PUSH(7)

|			|
|___________|
|	 7		|
|___________|
|	 2		|
|___________|

PUSH(5)

|			|
|	 5		|
|___________|
|	 7		|
|___________|
|	 2		|
|___________|

TOP => 5
ISEMPTY => false

POP
|			|
|___________|
|	 7		|
|___________|
|	 2		|
|___________|

POP

|			|
|			|
|			|
|___________|
|	 2		|
|___________|

POP

|			|
|			|
|			|
|			|
|	 		|
|___________|
*/

//APPLICATIONS

//Functions Calls/ Recursions
//Undo Function
//Balanced Parenthesis