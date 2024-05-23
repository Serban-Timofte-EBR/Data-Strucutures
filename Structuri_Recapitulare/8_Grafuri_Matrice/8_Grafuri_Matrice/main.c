#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <malloc.h>
#include <string.h>

typedef struct {
	int inf;
	struct nodStiva* next;
} nodStiva;

typedef struct {
	int inf;
	struct nodCoada* next;
} nodCoada;

void push(nodStiva** varf, int idNod) {
	nodStiva* nou = (nodStiva*)malloc(sizeof(nodStiva));
	nou->inf = idNod;
	nou->next = NULL;

	if (*varf == NULL) {
		*varf = nou;
	}
	else {
		nou->next = *varf;
		*varf = nou;
	}
 }

int pop(nodStiva** varf, int* idNodExtras) {
	if (*varf == NULL) {
		return 0;
	}
	else {
		*idNodExtras = (*varf)->inf;
		nodStiva* deleteMe = *varf;
		*varf = (*varf)->next;
		free(deleteMe);
		return 1;
	}
}

void put(nodCoada** prim, nodCoada** ultim, int idNod) {
	nodCoada* nou = (nodCoada*)malloc(sizeof(nodCoada));
	nou->inf = idNod;
	nou->next = NULL;

	if (*prim == NULL) {
		*prim = nou;
		*ultim = nou;
	}
	else {
		(*ultim)->next = nou;
		*ultim = nou;
	}
}

int get(nodCoada** prim, nodCoada** ultim, int* idExtras) {
	if (*prim == NULL && *ultim == NULL) {
		return 0;
	}

	else {
		nodCoada* deleteMe = *prim;
		*idExtras = (*prim)->inf;
		*prim = (*prim)->next;
		return 1;
	}
}

void parcurgereLatime(int** mat, int* vizitate, int nrNoduri, int idStart) {
	for (int i = 0; i < nrNoduri; i++) {
		vizitate[i] = 0;
	}

	nodCoada* prim = NULL;
	nodCoada* ultim = NULL;

	put(&prim, &ultim, idStart);
	vizitate[idStart] = 1;

	while (prim != NULL) 
	{
		get(&prim, &ultim, &idStart);
		printf("%d ", idStart + 1);
		for (int i = 0; i < nrNoduri; i++) {
			if (mat[idStart][i] == 1 && vizitate[i] == 0) {
				put(&prim, &ultim, i);
				vizitate[i] = 1;
			}
		}
	}
}

void parcurgereInAdancime(int** mat, int* vizitate, int nrNoduri, int idStart) {
	for (int i = 0; i < nrNoduri; i++) {
		vizitate[i] = 0;
	}

	nodStiva* varf = NULL;

	push(&varf, idStart);
	vizitate[idStart] = 1;

	while (varf != NULL) {
		pop(&varf, &idStart);
		printf("%d ", idStart + 1);
		for (int i = 0; i < nrNoduri; i++) {
			if (mat[idStart][i] == 1 && vizitate[i] == 0) {
				push(&varf, i);
				vizitate[i] = 1;
			}
		}
	}
}
 
int main() {
	FILE* f = fopen("fisier.txt", "r");

	int nrNoduri;
	fscanf(f, "%d", &nrNoduri);

	int** mat = (int**)malloc(nrNoduri * sizeof(int*));
	for (int i = 0; i < nrNoduri; i++) {
		mat[i] = (int*)malloc(nrNoduri * sizeof(int));
	}

	for (int i = 0; i < nrNoduri; i++) {
		for (int j = 0; j < nrNoduri; j++) {
			mat[i][j] = 0;
		}
	}

	int nrArce, nodStart, nodEnd;
	fscanf(f, "%d", &nrArce);
	for (int i = 0; i < nrArce; i++) {
		fscanf(f, "%d", &nodStart);
		fscanf(f, "%d", &nodEnd);

		if (nodStart < nrNoduri && nodEnd < nrNoduri) {
			mat[nodStart - 1][nodEnd - 1] = 1;
			mat[nodEnd - 1][nodStart - 1] = 1;
		}
	}

	printf("Matricea grafului:\n");
	for (int i = 0; i < nrNoduri; i++) {
		for (int j = 0; j < nrNoduri; j++) {
			printf("%d ", mat[i][j]);
		}
		printf("\n");
	}

	printf("\nParcurgerea in adancime:\n");
	int* vizitate = (int*)malloc(nrNoduri * sizeof(int));
	parcurgereInAdancime(mat, vizitate, nrNoduri, 2);

	printf("\nParcurgerea in adancime:\n");
	parcurgereLatime(mat, vizitate, nrNoduri, 2);

	fclose(f);
}