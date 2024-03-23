#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

typedef struct {
	int isbn;
	char* titlu;
	float pret;
} carte;

typedef struct {
	carte inf;
	struct nodLS* next;
} nodLS;

typedef struct {
	nodLS* inf;
	struct nodeLP* next;
} nodLP;

void inserareLS(nodLS** cap, carte c) {
	nodLS* nou = (nodLS*)malloc(sizeof(nodLS));
	nou->inf.isbn = c.isbn;
	nou->inf.titlu = (char*)malloc((strlen(c.titlu) + 1) * sizeof(char));
	strcpy(nou->inf.titlu, c.titlu);
	nou->inf.pret = c.pret;
	nou->next = NULL;
	if (*cap == NULL) {
		*cap = nou;
	}
	else {
		nodLS* temp = *cap;
		while (temp->next != NULL) {
			temp = temp->next;
		}
		temp->next = nou;
	}
}

void inserareLP(nodLP** capLP, nodLS** capLS) {
	nodLP* nou = (nodLP*)malloc(sizeof(nodLP));
	nou->inf = capLS;
	nou->next = NULL;
	if (*capLP == NULL) {
		*capLP = nou;
	}
	else {
		nodLP* temp = *capLP;
		while (temp->next) {
			temp = temp->next;
		}
		temp->next = nou;
	}
}

void traversareLS(nodLS* cap) {
	nodLS* temp = cap;
	while (temp) {
		printf("ISBN: %d\tTitlu: %s\tPret: %f\n",
			temp->inf.isbn, temp->inf.titlu, temp->inf.pret);
		temp = temp->next;
	}
}

void traversareLP(nodLP* cap) {
	nodLP* temp = cap;
	int i = 1;
	while (temp) {
		printf("Sublista %d\n", i);
		traversareLS(temp->inf);
		temp = temp->next;
		i++;
		printf("\n");
	}
}

void dezalocareLS(nodLS* cap) {
	nodLS* temp = cap;
	while (temp) {
		nodLS* aux = temp->next;
		free(temp->inf.titlu);
		free(temp);
		temp = aux;
	}
}

void dezalocareLP(nodLP* cap) {
	nodLP* temp = cap;
	while (temp) {
		nodLP* temp2 = temp->next;
		dezalocareLS(temp->inf);
		free(temp);
		temp = temp2;
	}
}

int main() {
	nodLS* capLS1 = NULL;
	nodLS* capLS2 = NULL;
	nodLP* capLP = NULL;
	int nrCarti;
	FILE* f = fopen("input.txt", "r");
	if (!f) {
		printf("File not opened!");
		return -1;
	}

	carte c;
	char buffer[30];
	fscanf(f, "%d", &nrCarti);
	for (int i = 0; i < nrCarti; i++) {
		fscanf(f, "%d", &c.isbn);
		fscanf(f, "%s", buffer);
		c.titlu = (char*)malloc((strlen(buffer) + 1) * sizeof(char));
		strcpy(c.titlu, buffer);
		fscanf(f, "%f", &c.pret);

		if (c.pret >= 100)
			inserareLS(&capLS1, c);
		else
			inserareLS(&capLS2, c);

		free(c.titlu);
	}
	fclose(f);

	inserareLP(&capLP, capLS1);
	inserareLP(&capLP, capLS2);

	traversareLP(capLP);
	dezalocareLP(capLP);

	return 0;
}