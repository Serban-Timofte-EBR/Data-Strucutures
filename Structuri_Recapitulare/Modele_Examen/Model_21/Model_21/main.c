#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <malloc.h>
#include <string.h>

typedef struct {
	char* data;
	char* medic;
	char* specializare;
	char* diagnostic;
	int pret;
} Consultatie;

typedef struct {
	Consultatie inf;
	struct nodLS* next;
} nodLS;

typedef struct {
	nodLS* inf;
	struct nodLL* next;
} nodLL;

nodLL* checkNodLL(nodLL* prim, char* specializare) {
	nodLL* temp = prim;
	while (temp != NULL)
	{
		if (strcmp(temp->inf->inf.specializare, specializare) == 0) {
			return temp;
		}
		temp = temp->next;
	}
	return NULL;
}

void inserareLS(nodLS** prim, Consultatie cons) {
	nodLS* nou = (nodLS*)malloc(sizeof(nodLS));
	nou->inf.pret = cons.pret;
	nou->inf.data = (char*)malloc(strlen(cons.data) + 1);
	strcpy(nou->inf.data, cons.data);
	nou->inf.diagnostic = (char*)malloc(strlen(cons.diagnostic) + 1);
	strcpy(nou->inf.diagnostic, cons.diagnostic);
	nou->inf.specializare = (char*)malloc(strlen(cons.specializare) + 1);
	strcpy(nou->inf.specializare, cons.specializare);
	nou->inf.medic = (char*)malloc(strlen(cons.medic) + 1);
	strcpy(nou->inf.medic, cons.medic);
	nou->next = NULL;

	if (*prim == NULL) {
		*prim = nou;
	}
	else {
		nodLS* temp = *prim;
		while (temp->next != NULL) {
			temp = temp->next;
		}
		temp->next = nou;
	}
}

nodLL* inserareLL(nodLL* prim, nodLS* inf) {
	nodLL* nou = (nodLL*)malloc(sizeof(nodLL));
	nou->inf = inf;
	nou->next = NULL;

	if (prim == NULL) {
		prim = nou;
	}
	else {
		nodLL* temp = prim;
		while (temp->next != NULL) {
			temp = temp->next;
		}
		temp->next = nou;
	}

	return prim;
}

nodLL* inserareConsultatieLL(nodLL* prim, Consultatie cons) {
	nodLL* cap = checkNodLL(prim, cons.specializare);
	if (cap != NULL) {
		inserareLS(&cap->inf, cons);
	}
	else {
		nodLS* inf = NULL;
		inserareLS(&inf, cons);
		prim = inserareLL(prim, inf);
	}
	return prim;
}

void afisareLS(nodLS* prim) {
	nodLS* temp = prim;
	while (temp != NULL)
	{
		printf("\tPret: %d, Data: %s, Specializarea: %s, Medic: %s, Diagnostic: %s\n", 
			temp->inf.pret, temp->inf.data, temp->inf.specializare, temp->inf.medic, temp->inf.diagnostic);
		temp = temp->next;
	}
}

void afisareLL(nodLL* prim) {
	nodLL* temp = prim;
	while (temp != NULL)
	{
		printf("Specializarea %s:\n", temp->inf->inf.specializare);
		afisareLS(temp->inf);
		temp = temp->next;
	}
}

int valoareConsultatiiSpecializare(nodLL* prim, char* spec) {
	int valTotala = 0;
	nodLL* temp = prim;
	while (temp != NULL)
	{
		if (strcmp(temp->inf->inf.specializare, spec) == 0) {
			nodLS* tempLS = temp->inf;
			while (tempLS != NULL) {
				valTotala += tempLS->inf.pret;
				tempLS = tempLS->next;
			}
		}
		temp = temp->next;
	}
	return valTotala;
}

nodLL* modificarePretConsultatie(nodLL* prim, char* data, int pretNou) {
	nodLL* temp = prim;
	while (temp != NULL)
	{
		nodLS* tempLS = temp->inf;
		while (tempLS != NULL)
		{
			if (strcmp(tempLS->inf.data, data) == 0) {
				tempLS->inf.pret = pretNou;
			}
			tempLS = tempLS->next;
		}
		temp = temp->next;
	}
	return prim;
}

void stergereLS(nodLS** prim, char* diagnostic) {
	nodLS* temp = *prim;
	nodLS* prev = NULL;
	while (temp != NULL) {
		if (strcmp(temp->inf.diagnostic, diagnostic) == 0) {
			nodLS* deleteMe = temp;
			if (prev == NULL) {
				*prim = (*prim)->next;
			}
			else {
				prev->next = temp->next;
			}
			temp = temp->next;
			free(deleteMe->inf.data);
			free(deleteMe->inf.diagnostic);
			free(deleteMe->inf.medic);
			free(deleteMe->inf.specializare);
			free(deleteMe);
		}
		else {
			prev = temp;
			temp = temp->next;
		}
	}
}

nodLL* stergereDiagnosticuri(nodLL* prim, char* diag) {
	nodLL* temp = prim;
	nodLL* prev = NULL;
	while (temp != NULL)
	{
		stergereLS(&temp->inf, diag);
		if (temp->inf == NULL) {
			if (prev == NULL) {
				prim = prim->next;
			}
			else {
				prev->next = temp->next;
			}
		}
		prev = temp;
		temp = temp->next;
	}
	return prim;
}

int countConsultatiiPestePrag(nodLL* prim, int prag) {
	int counter = 0;
	nodLL* temp = prim;
	while (temp != NULL) {
		nodLS* tempLS = temp->inf;
		while(tempLS != NULL) {
			if (tempLS->inf.pret > prag) {
				counter++;
			}
			tempLS = tempLS->next;
		}
		temp = temp->next;
	}
	return counter;
}

Consultatie* vectorConsultatii(nodLL* prim, int prag, int* nrElem) {
	*nrElem = countConsultatiiPestePrag(prim, prag);
	Consultatie* vector = (Consultatie*)malloc(*nrElem * sizeof(Consultatie));
	int index = 0;

	nodLL* temp = prim;
	while (temp != NULL)
	{
		nodLS* temp2 = temp->inf;
		while (temp2 != NULL)
		{
			if (temp2->inf.pret > prag) {
				vector[index].pret = temp2->inf.pret;
				vector[index].data = (char*)malloc(strlen(temp2->inf.data) + 1);
				strcpy(vector[index].data, temp2->inf.data);
				vector[index].diagnostic = (char*)malloc(strlen(temp2->inf.diagnostic) + 1);
				strcpy(vector[index].diagnostic, temp2->inf.diagnostic);
				vector[index].medic = (char*)malloc(strlen(temp2->inf.medic) + 1);
				strcpy(vector[index].medic, temp2->inf.medic);
				vector[index].specializare = (char*)malloc(strlen(temp2->inf.specializare) + 1);
				strcpy(vector[index].specializare, temp2->inf.specializare);
				index++;
			}
			temp2 = temp2->next;
		}
		temp = temp->next;
	}

	return vector;
}

int main() {
	nodLL* prim = NULL;

	FILE* f = fopen("fisier.txt", "r");

	Consultatie cons;
	char buffer[128];

	while (fscanf(f, "%s", buffer) == 1)
	{
		cons.data = (char*)malloc(strlen(buffer) + 1);
		strcpy(cons.data, buffer);

		fscanf(f, " %[^\n]", buffer);
		cons.medic = (char*)malloc(strlen(buffer) + 1);
		strcpy(cons.medic, buffer);

		fscanf(f, "%s", buffer);
		cons.specializare = (char*)malloc(strlen(buffer) + 1);
		strcpy(cons.specializare, buffer);

		fscanf(f, " %[^\n]", buffer);
		cons.diagnostic = (char*)malloc(strlen(buffer) + 1);
		strcpy(cons.diagnostic, buffer);

		fscanf(f, "%d", &cons.pret);

		/*printf("Pret: %d, Data: %s, Specializarea: %s, Medic: %s, Diagnostic: %s\n",
			cons.pret, cons.data, cons.specializare, cons.medic, cons.diagnostic);*/
		prim = inserareConsultatieLL(prim, cons);

		free(cons.diagnostic);
		free(cons.medic);
		free(cons.specializare);
		free(cons.data);
	}
	fclose(f);

	printf("Lista de liste din fisier:\n");
	afisareLL(prim);

	int valConsult = valoareConsultatiiSpecializare(prim, "Cardiologie");
	printf("\nValoarea consultatiilor de la specializarea ceruta este: %d\n", valConsult);

	valConsult = valoareConsultatiiSpecializare(prim, "Oftalmologie");
	printf("\nValoarea consultatiilor de la specializarea ceruta este: %d\n", valConsult);

	valConsult = valoareConsultatiiSpecializare(prim, "Neurologie");
	printf("\nValoarea consultatiilor de la specializarea ceruta este: %d\n", valConsult);

	prim = modificarePretConsultatie(prim, "30.06.2023", 99);
	prim = modificarePretConsultatie(prim, "21.08.2024", 1000);
	prim = modificarePretConsultatie(prim, "01.01.2025", 1000);
	printf("\nLista de liste dupa modificare:\n");
	afisareLL(prim);

	prim = stergereDiagnosticuri(prim, "Insuficienta Cardiaca");
	prim = stergereDiagnosticuri(prim, "Miopie");
	printf("\nLista de liste dupa stergere:\n");
	afisareLL(prim);

	/*int counter = countConsultatiiPestePrag(prim, 60);
	printf("\n%d\n", counter);*/

	int nrElem = 0;
	Consultatie* vector = vectorConsultatii(prim, 60, &nrElem);
	printf("\nVectorul:\n");
	for (int i = 0; i < nrElem; i++) {
		printf("Pret: %d, Data: %s, Specializarea: %s, Medic: %s, Diagnostic: %s\n",
			vector[i].pret, vector[i].data, vector[i].specializare, vector[i].medic, vector[i].diagnostic);
	}

	return 0;
}