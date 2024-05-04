#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <malloc.h>
#include <string.h>

typedef struct {
	int cod;
	char* denumire;
	float pret;
} produs;

typedef struct {
	produs inf;
	struct nodLS* next;
} nodLS;

typedef struct {
	nodLS* inf;
	struct nodLL* next;
} nodLL;

void inserareLS(nodLS** prim, produs prod) {
	nodLS* nou = (nodLS*)malloc(sizeof(nodLS));
	nou->inf.cod = prod.cod;
	nou->inf.pret = prod.pret;
	nou->inf.denumire = (char*)malloc(strlen(prod.denumire) + 1);
	strcpy(nou->inf.denumire, prod.denumire);
	nou->next = NULL;
	if (*prim == NULL) {
		*prim = nou;
	}
	else {
		nodLS* temp = *prim;
		while (temp->next != NULL)
		{
			temp = temp->next;
		}
		temp->next = nou;
	}
}

nodLL* inserareLL(nodLL* cap, nodLS* nodNou) {
	nodLL* nou = (nodLL*)malloc(sizeof(nodLL));
	nou->inf = nodNou;
	nou->next = NULL;

	if (cap == NULL) {
		cap = nou;
	}
	else {
		nodLL* temp = cap;
		while (temp->next != NULL)
		{
			temp = temp->next;
		}
		temp->next = nou;
	}
	return cap;
}

void traversareLS(nodLS* prim) {
	nodLS* temp = prim;
	while (temp != NULL)
	{
		printf("\Cod: %d\n\t\t Denumire: %s \n\t\t Pret: %f\n",
			temp->inf.cod, temp->inf.denumire, temp->inf.pret);
		temp = temp->next;
	}
}

void traversareLL(nodLL* prim) {
	nodLL* temp = prim;
	int index = 1;
	while (temp!=NULL)
	{
		printf("\nNodul %d:\n", index);
		traversareLS(temp->inf);
		temp = temp->next;
		index++;
	}
}

void dezalocareLS(nodLS* prim) {
	nodLS* temp = prim;
	while (temp != NULL) 
	{
		nodLS* aux = temp->next;
		free(temp->inf.denumire);
		free(temp);
		temp = aux;
	}
}

void dezalocareLL(nodLL* prim) {
	nodLL* temp = prim;
	while (temp!= NULL)
	{
		nodLL* aux = temp->next;
		dezalocareLS(temp->inf);
		free(temp);
		temp = aux;
	}
}

int main() {
	nodLL* primLL = NULL;
	nodLS* primLS1 = NULL;
	nodLS* primLS2 = NULL;

	FILE* f = fopen("produse.txt", "r");
	if (f == NULL) {
		printf("Fisierul nu a fost deschis!");
		return -1;
	}

	produs prod;
	char buffer[128];

	while (fscanf(f, "%d", &prod.cod) == 1) 
	{
		fscanf(f, " %s", buffer);
		prod.denumire = (char*)malloc(strlen(buffer) + 1);
		strcpy(prod.denumire, buffer);

		fscanf(f, "%f", &prod.pret);

		if (prod.cod % 2 == 0) {
			inserareLS(&primLS1, prod);
		}
		else {
			inserareLS(&primLS2, prod);
		}

		free(prod.denumire);
	}

	primLL = inserareLL(primLL, primLS1);
	primLL = inserareLL(primLL, primLS2);

	traversareLL(primLL);

	dezalocareLL(primLL);
	return 0;
}