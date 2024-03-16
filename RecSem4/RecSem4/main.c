#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>

typedef struct {
	int zi;
	int luna; 
	int an;
} dataPrimire;

typedef struct {
	dataPrimire data;
	char* expediator;
	float dimensiune;
} email;

typedef struct {
	email inf;
	struct nodStiva* next;
} nodStiva;

typedef struct {
	email inf;
	struct nodCoada* next;
} nodCoada;

void push(nodStiva** varf, email e) {
	nodStiva* nou = (nodStiva*)malloc(sizeof(nodStiva));
	nou->inf.data.zi = e.data.zi;
	nou->inf.data.luna = e.data.luna;
	nou->inf.data.an = e.data.an;
	nou->inf.expediator = (char*)malloc((strlen(e.expediator) + 1) *
		sizeof(char));
	strcpy(nou->inf.expediator, e.expediator);
	nou->inf.dimensiune = e.dimensiune;
	nou->next = NULL;

	if (*varf == NULL) {
		*varf = nou;
	}
	else {
		nou->next = *varf;
		*varf = nou;
	}
}

int pop(nodStiva** varf, email* e) {
	if (*varf == NULL) {
		return -1;
	}
	else {
		(*e).data.zi = (*varf)->inf.data.zi;
		(*e).data.luna = (*varf)->inf.data.luna;
		(*e).data.an = (*varf)->inf.data.an;
		(*e).expediator = (char*)malloc((strlen((*varf)->inf.expediator)
			+ 1) * sizeof(char));
		strcpy((*e).expediator, (*varf)->inf.expediator);
		(*e).dimensiune = (*varf)->inf.dimensiune;

		nodStiva* temp = *varf;
		*varf = (*varf)->next;
		free(temp->inf.expediator);
		free(temp);
		return 0;
	}
}

void afisareStiva(nodStiva* varf) {
	nodStiva* temp = varf;
	while (temp != NULL) {
		printf("\nData: %d/%d/%d, Expeditor: %s, Dimensiune: %5.2f",
			temp->inf.data.zi, temp->inf.data.luna, temp->inf.data.an,
			temp->inf.expediator, temp->inf.dimensiune);
		temp = temp->next;
	}
	printf("\n");
}

void salvareStivaInVector(nodStiva* varf, email* vect, int* nr) {
	email e;
	while (pop(&varf, &e) == 0) {
		vect[*nr] = e;
		(*nr)++;
	}
}

void put(nodCoada** prim, nodCoada** ultim, email e) {
	nodCoada* nou = (nodCoada*)malloc(sizeof(nodCoada));
	nou->inf.data.zi = e.data.zi;
	nou->inf.data.luna = e.data.luna;
	nou->inf.data.an = e.data.an;
	nou->inf.expediator = (char*)malloc((strlen(e.expediator) + 1)
		* sizeof(char));
	strcpy(nou->inf.expediator, e.expediator);
	nou->inf.dimensiune = e.dimensiune;
	nou->next = NULL;

	if(*prim == NULL || *ultim == NULL) {
		*prim = nou;
		*ultim = nou;
	}
	else {
		(*ultim)->next = nou;
		*ultim = nou;
	}
}

int get(nodCoada** prim, nodCoada** ultim, email* e) {
	if (*prim == NULL) {
		*ultim = NULL;
		return -1;
	}
	else {
		(*e).data.zi = (*prim)->inf.data.zi;
		(*e).data.luna = (*prim)->inf.data.luna;
		(*e).data.an = (*prim)->inf.data.an;
		(*e).expediator = (char*)malloc((strlen((*prim)->inf.expediator) + 1)
			* sizeof(char));
		strcpy((*e).expediator, (*prim)->inf.expediator);
		(*e).dimensiune = (*prim)->inf.dimensiune;

		nodCoada* temp = *prim;
		(*prim) = (*prim)->next;
		free(temp->inf.expediator);
		free(temp);
	}
}

void afisareCoada(nodCoada* prim) {
	nodCoada* temp = prim;
	while (temp != NULL) {
		printf("Data: %d %d %d \t Expediator: %s \t Dimensiune: %f \n", 
			temp->inf.data.zi, temp->inf.data.luna, temp->inf.data.an,
			temp->inf.expediator, temp->inf.dimensiune);
		temp = temp->next;
	}
	printf("\n");
}

void salvareStivaInCoada(nodCoada** prim, nodCoada** ultim, nodStiva* varf) {
	email e;
	while (pop(&varf, &e) == 0) {
		put(prim, ultim, e);
	}
}

void determinareEmailuriExpeditor(nodCoada* prim, nodCoada* ultim, email* vect, int* nr, char* expediator) {
	email e;
	while (get(&prim, &ultim, &e) == 0) {
		if (strcmp(e.expediator, expediator) == 0) {
			vect[*nr] = e;
			(*nr)++;
		}
	}
}

int main() {
	int nrEmailuri;
	nodStiva* varf = NULL;
	nodCoada* prim = NULL;
	nodCoada* ultim = NULL;
	email e;
	char buffer[50];
	FILE* f = fopen("input.txt", "r");
	if (f != NULL) {
		fscanf(f, "%d", &nrEmailuri);
		printf("Avem %d noduri in lista!\n", nrEmailuri);
		for (int i = 0; i < nrEmailuri; i++) {
			fscanf(f, "%d", &e.data.zi);
			fscanf(f, "%d", &e.data.luna);
			fscanf(f, "%d", &e.data.an);
			fscanf(f, "%s", buffer);
			e.expediator = (char*)malloc((strlen(buffer) + 1)
				* sizeof(char));
			strcpy(e.expediator, buffer);
			fscanf(f, "%f", &e.dimensiune);
			push(&varf, e);
			put(&prim, &ultim, e);
			free(e.expediator);
		}
		fclose(f);

		printf("\n---------stiva-----------\n");
		afisareStiva(varf);

		/*
		email* e2 = (email*)malloc(sizeof(email));
		pop(&varf, e2);
		afisareStiva(varf);
		*/

		printf("\n---------coada-----------\n");
		afisareCoada(prim);

		/*
		printf("\n---------stiva in vector-----------\n");
		int nr = 0;
		email* vect = (email*)malloc(nrEmailuri * sizeof(email));
		salvareStivaInVector(varf, vect, &nr);
		for (int i = 0; i < nr; i++) {
			printf("\nData: %d/%d/%d, Expeditor: %s, Dimensiune: %5.2f",
				vect[i].data.zi, vect[i].data.luna, vect[i].data.an,
				vect[i].expediator, vect[i].dimensiune);
		}
		printf("\n");
		*/

		/*
		for (int i = 0; i < nr; i++) {
			free(vect[i].expediator);
		}
		free(vect);
		*/

		printf("\n---------stiva in coada-----------\n");
		/*get(&prim, &ultim, &e);
		printf("%d", e.data.an);*/
		salvareStivaInCoada(&prim, &ultim, varf);
		afisareCoada(prim);

		printf("\n---------coada in vector-----------\n");
		int nrVec2 = 0;
		email* vecCoada = (email*)malloc(nrEmailuri * sizeof(email));
		determinareEmailuriExpeditor(prim, ultim, vecCoada, &nrVec2, "dorel@gmail.com");
		for (int i = 0; i < nrVec2; i++) {
			printf("\nData: %d/%d/%d, Expeditor: %s, Dimensiune: %5.2f",
				vecCoada[i].data.zi, vecCoada[i].data.luna, vecCoada[i].data.an,
				vecCoada[i].expediator, vecCoada[i].dimensiune);
		}
		printf("\n");

		for (int i = 0; i < nrVec2; i++) {
			free(vecCoada[i].expediator);
		}
		free(vecCoada);
	}
	return 0;
}