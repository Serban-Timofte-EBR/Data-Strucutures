#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

typedef struct {
	int* anFabricatie;
	char* marca;
	float consumMediu;
} masina;

typedef struct {
	masina inf;
	struct nodeLS* next;
	struct nodeLS* prev;
} nodeLS;

nodeLS* inserareNod(nodeLS* head, nodeLS** tail, masina m) {
	//create a new node
	nodeLS* nou = (nodeLS*)malloc(sizeof(nodeLS));

	//copy the information in the node inf
	nou->inf.anFabricatie = (int*)malloc(sizeof(int));
	*(nou->inf.anFabricatie) = *(m.anFabricatie);

	nou->inf.marca = (char*)malloc((strlen(m.marca) + 1) * sizeof(char));
	strcpy(nou->inf.marca, m.marca);

	nou->inf.consumMediu = m.consumMediu;

	if (head == NULL) {
		nou->next = nou;
		nou->prev = nou;
		head = nou;
		*tail = nou;
	}
	else {
		nodeLS* temp = head;
		while (temp->next != head) {
			temp = temp->next; 
		}

		temp->next = nou;
		nou->prev = temp;
		*tail = nou;
		(*tail)->next = head;
		head->prev = *tail;
	}

	printf("Node inserted!\n");
	return head;
}

void traversareLista(nodeLS* head) {
	printf("List in Normal Order: \n\n");
	nodeLS* temp = head;
	while (temp->next != head) {
		printf("An fabricatie = %d, \t Marca = %s, \t Consum Mediu = %d\n",
			*(temp->inf.anFabricatie), temp->inf.marca, temp->inf.consumMediu);
		temp = temp->next;
	}
	printf("An fabricatie = %d, \t Marca = %s, \t Consum Mediu = %d\n",
		*(temp->inf.anFabricatie), temp->inf.marca, temp->inf.consumMediu);
}

void traversareListaInvers(nodeLS* tail) {
	printf("List in Reverse Order: \n\n");
	nodeLS* temp = tail;
	while (temp->prev != tail) {
		printf("An fabricatie = %d, \t Marca = %s, \t Consum Mediu = %d\n",
			*(temp->inf.anFabricatie), temp->inf.marca, temp->inf.consumMediu);
		temp = temp->prev;
	}
	printf("An fabricatie = %d, \t Marca = %s, \t Consum Mediu = %d\n",
		*(temp->inf.anFabricatie), temp->inf.marca, temp->inf.consumMediu);
}

void saveListInVector(nodeLS* head, masina* vec, int* nrElem, int anFabricatie) {
	nodeLS* temp = head;
	*nrElem = 0;
	while (temp->next != head) {
		if (*(temp->inf.anFabricatie) >= anFabricatie) {
			vec[*nrElem].anFabricatie = temp->inf.anFabricatie;
			vec[*nrElem].marca = (char*)malloc((strlen(temp->inf.marca) + 1)
				* sizeof(char));
			strcpy(vec[*nrElem].marca, temp->inf.marca);
			vec[*nrElem].consumMediu = temp->inf.consumMediu;
			(*nrElem)++;
		}
		temp = temp->next;
	}
	if (*(temp->inf.anFabricatie) >= anFabricatie) {
		vec[*nrElem].anFabricatie = temp->inf.anFabricatie;
		vec[*nrElem].marca = (char*)malloc((strlen(temp->inf.marca) + 1)
			* sizeof(char));
		strcpy(vec[*nrElem].marca, temp->inf.marca);
		vec[*nrElem].consumMediu = temp->inf.consumMediu;
	}
}

void deleteElem(nodeLS** head, nodeLS** tail, char* marca) {
	//delete first node
	if (strcmp((*head)->inf.marca, marca) == 0) {
		nodeLS* temp = *head;
		*head = (*head)->next;
		if (*head != NULL) {
			(*head)->prev = NULL;
		}
		free(temp->inf.anFabricatie);
		free(temp->inf.marca);
		free(temp);
		return;
	}
	else {
		//delete last node
		if (strcmp((*tail)->inf.marca, marca) == 0) {
			nodeLS* temp = *tail;
			*tail = (*tail)->prev;
			if (*tail != NULL) {
				(*tail)->next = NULL;
			}
			free(temp->inf.anFabricatie);
			free(temp->inf.marca);
			free(temp);
		}
		//delete middle element
		else {
			nodeLS* temp = *head;
			while (temp != NULL) {
				if (strcmp(temp->inf.marca, marca) == 0) {
					nodeLS* anterior = temp->prev;
					nodeLS* urmator = temp->next;
					anterior->next = urmator;
					urmator->prev = anterior;
					free(temp->inf.anFabricatie);
					free(temp->inf.marca);
					free(temp);
					return;
				}
				else {
					temp = temp->next;
				}
			}
		}
	}
}

int main() {
	int nrMasini;
	nodeLS* head = NULL;
	nodeLS* tail = NULL;
	masina m;
	char buffer[20];
	FILE* fisier = fopen("input.txt", "r");

	if (fisier == NULL) {
		printf("File is not opened!");
		return -1;
	}

	fscanf(fisier, "%d", &nrMasini);
	for (int i = 0; i < nrMasini; i++) {
		m.anFabricatie = (int*)malloc(sizeof(int));
		fscanf(fisier, "%d", m.anFabricatie);
		
		fscanf(fisier, "%s", buffer);
		m.marca = (char*)malloc((strlen(buffer) + 1) * sizeof(char));
		strcpy(m.marca, buffer);

		fscanf(fisier, "%f", &m.consumMediu);

		head = inserareNod(head, &tail, m);
		free(m.anFabricatie);
		free(m.marca);
	}
	fclose(fisier);

	printf("\n------------------\n");

	traversareLista(head);

	printf("\n------------------\n");

	traversareListaInvers(tail);

	printf("\n------------------\n");

	deleteElem(&head, &tail, "Hyundai");
	traversareLista(head);

	printf("\n------------------\n");

	int nrElem;
	masina* vecMasini = (masina*)malloc(nrMasini * sizeof(masina));
	saveListInVector(head, vecMasini, &nrElem, 2020);

	printf("Cars in vector: \n\n");
	for (int i = 0; i < nrElem; i++)
		printf("\nAn fabricatie = %d, Marca = %s, Consum mediu = %5.2f",
			*(vecMasini[i].anFabricatie), vecMasini[i].marca, vecMasini[i].consumMediu);
	printf("\n");
	return 0;
}