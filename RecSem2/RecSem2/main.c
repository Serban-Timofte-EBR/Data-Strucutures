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
	struct nodeLs* next;
} nodeLs;

nodeLs* inserareNod(nodeLs* cap, masina m) {
	//alocare nod nou
	nodeLs* nou = (nodeLs*)malloc(sizeof(nodeLs));
	nou->inf.anFabricatie = (int*)malloc(sizeof(int));
	*(nou->inf.anFabricatie) = *(m.anFabricatie);

	nou->inf.marca = (char*)malloc((strlen(m.marca) + 1) * sizeof(char));
	strcpy(nou->inf.marca, m.marca);

	nou->inf.consumMediu = m.consumMediu;

	nou->next = NULL;

	//inserarea nodului
	if (cap == NULL) {
		cap = nou;
	}

	else {
		nodeLs* temp = cap;
		while (temp->next != NULL) {
			temp = temp->next;
		}
		temp->next = nou;
	}

	return cap;
}

void traversareLista(nodeLs* head) {
	nodeLs* temp = head;
	while (temp != NULL) {
		printf("An fabricatie: %d, \t Marca = %s, \t Consum mediu = %5.2f", *(temp->inf.anFabricatie), temp->inf.marca, temp->inf.consumMediu);
		printf("\n");
		temp = temp->next;
	}
}

//functie care stocheaza elementele listei intr-un vector, daca respecta o conditie
void salvareMasiniInVector(nodeLs* head, masina* vector, int* nrElem, int anFabricatie) {
	nodeLs* temp = head;
	while (temp != NULL) {
		if (*(temp->inf.anFabricatie) > anFabricatie) {
			vector[*nrElem] = temp->inf;
			(*nrElem)++;
		}
		temp = temp->next;
	}
}

void dezalocareLs(nodeLs* head) {
	nodeLs* temp = head;
	while (temp != NULL) {
		nodeLs* aux = temp->next;
		free(temp->inf.anFabricatie);
		free(temp->inf.marca);
		free(temp);
		temp = aux;
	}
}

int main() {
	int nrMasini;
	nodeLs* head = NULL;
	masina m;
	char buffer[20];

	FILE* f = fopen("input.txt", "r");

	if (!f) {
		printf("Fisierul nu a fost deschis!\n");
	}
	else {
		fscanf(f, "%d", &nrMasini);
		for (int i = 0; i < nrMasini; i++) {
			m.anFabricatie = (int*)malloc(sizeof(int));
			fscanf(f, "%d", m.anFabricatie);

			fscanf(f, "%s", buffer);
			m.marca = (char*)malloc((strlen(buffer) + 1) * sizeof(char));
			strcpy(m.marca, buffer);

			fscanf(f, "%f", &m.consumMediu);

			//adaugarea nodului citit in lista
			head = inserareNod(head, m);
			free(m.marca);
			free(m.anFabricatie);
		}
		fclose(f);
	}

	traversareLista(head);

	printf("\n\n------------------------------------------------------\n\n");

	int nrElem = 0;
	masina* vectorMasini = (masina*)malloc(nrElem * sizeof(masina));
	salvareMasiniInVector(head, vectorMasini, &nrElem, 2020);
	for (int i = 0; i < nrElem; i++) {
		printf("\nAn fabricatie = %d, Marca = %s, Consum mediu = %5.2f",
			*(vectorMasini[i].anFabricatie), vectorMasini[i].marca, vectorMasini[i].consumMediu);
	}

	free(vectorMasini);

	dezalocareLs(head);
	return 0;
}
