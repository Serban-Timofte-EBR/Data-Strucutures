//Linked List: Inserting a node at beginning
//Linked List: Inserting a node at nth position
//Linked List: Delete a node at nth position
#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <stdio.h>

typedef struct {
	int data;
	struct Node* next;
} Node;

struct Node* head;
struct Node* head2;

void Insert_Begin(int x) {
	Node* temp = (Node*)malloc(sizeof(Node));

	temp->data = x;	
	//temp->next = NULL;
	//To cover both scenarios: 
	temp->next = head;

	head = temp;

	//SCENARIO: List is empty
	//head = temp;

	//SCENARIO: List already has a first element
	//if (head != NULL) {
	//	temp->next = head;
	//}

	//head = temp;
}

void Print() {
	//Use reference to not use the head next element
	Node* temp = head;

	while (temp != NULL) {
		printf(" %d", temp->data);
		temp = temp->next;
	}

	printf("\n");
}

void Insert(int data, int n) {
	Node* temp1 = (Node*)malloc(sizeof(Node));

	temp1->data = data;
	temp1->next = NULL;

	if (n == 1) {
		temp1->next = head;	//if the list is not empty
		head = temp1;
		return;
	}

	Node* temp2 = head;
	for (int i = 0; i < n - 2; i++) {
		temp2 = temp2->next;
	}

	temp1->next = temp2->next;	//Elementul nou creat pointeaza catre viitorul element din lista
	temp2->next = temp1;		//Actualul element n-1 pointeaza catre noul element de la pozitia n
}

void Print2() {
	Node* temp = head;

	while (temp != NULL) {
		printf("%d ", temp->data);
		temp = temp->next;
	}

	printf("\n");
}

void Insert_End(int data) {
	Node* temp1 = (Node*)malloc(sizeof(Node));

	temp1->data = data;
	temp1->next = NULL;

	if (head2 == NULL) {
		temp1->next = head2;
		head2 = temp1;
		return;
	}

	Node* temp2 = head2;
	while (temp2->next != NULL) {
		temp2 = temp2->next;
	}

	temp2->next = temp1;
}

void Print3() {
	Node* temp = head2;

	if (head2 == NULL) { printf("List is empty"); }

	while (temp != NULL) {
		printf("%d ", temp->data);
		temp = temp->next;
	}
}

void Delete(int index) {
	Node* temp1 = head2;

	if (index == 1) {
		head2 = temp1->next;
		free(temp1);
		return;
	}

	for (int i = 0; i < index - 2; i++) {
		temp1 = temp1->next;
	}

	Node* temp2 = temp1->next;
	temp1->next = temp2->next;	//n+1 Node
	free(temp2);
}

int main() {
	head = NULL;	//empty list

	//Linked List: Inserting a node at beginning

	/*printf("How many numbers there are in the list?\n");
	int n, x;
	scanf("%d", &n);*/
	
	/*for (int i = 0; i < n; i++) {
		printf("Enter the number\t\n");
		scanf("%d", &x);
		Insert_Begin(x);
		Print();
	}*/

	//Linked List: Inserting a node at nth position
	Insert(2, 1);	//List: 2
	Insert(3, 2);	//List: 2, 3
	Insert(4, 1);	//List: 4, 2, 3
	Insert(5, 2);	//List: 4, 5, 2, 3
	Print2();

	head2 = NULL;

	Insert_End(2);
	Insert_End(4);
	Insert_End(6);
	Insert_End(5);	//List: 2,4,6,5
	Print3();

	int n;
	printf("\nEnter a position: \n");
	scanf("%d", &n);
	Delete(n);
	Print3();

	return 0;
}