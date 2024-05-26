#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <malloc.h>
#include <string.h>

typedef struct {
	int id;
	char* denumire;
	char* localitate;
	float suprafata;
	int numar_angajati;
} Magazin;

typedef struct {
	int id;
	float distanta;
	struct nodLS* next;
} nodLS;

typedef struct {
	Magazin inf;
	nodLS* adiacenta;
	struct nodListaPrincipala* next;
} nodListaPrincipala;

typedef struct {
	nodListaPrincipala* capListaNoduri;
	int nrNoduri;
} Graf;

typedef struct {
	int id;
	struct nodStiva* next;
} nodStiva;

void inserareLL(nodListaPrincipala** cap, Magazin mag) {
	nodListaPrincipala* nou = (nodListaPrincipala*)malloc(sizeof(nodListaPrincipala));
	nou->inf.id = mag.id;
	nou->inf.numar_angajati = mag.numar_angajati;
	nou->inf.suprafata = mag.suprafata;
	nou->inf.denumire = (char*)malloc(strlen(mag.denumire) + 1);
	strcpy(nou->inf.denumire, mag.denumire);
	nou->inf.localitate = (char*)malloc(strlen(mag.localitate) + 1);
	strcpy(nou->inf.localitate, mag.localitate);

	nou->adiacenta = NULL;
	nou->next = NULL;

	if (*cap == NULL) {
		*cap = nou;
	}
	else {
		nodListaPrincipala* temp = *cap;
		while (temp->next != NULL) {
			temp = temp->next;
		}
		temp->next = nou;
	}
}

void inserareLS(nodLS** cap, int id, float distanta) {
	nodLS* nou = (nodLS*)malloc(sizeof(nodLS));
	nou->id = id;
	nou->distanta = distanta;
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

Graf citesteGraf(char* denumire) {
	Graf graf;
	graf.nrNoduri = -1;
	graf.capListaNoduri = NULL;

	FILE* f = fopen(denumire, "r");
	if (f == NULL) {
		return graf;
	}

	fscanf(f, "%d", &graf.nrNoduri);

	Magazin mag;
	char buffer[128];

	for (int i = 0; i < graf.nrNoduri; i++) {
		fscanf(f, "%d", &mag.id);

		fscanf(f, "%s", buffer);
		mag.denumire = (char*)malloc(strlen(buffer) + 1);
		strcpy(mag.denumire, buffer);

		fscanf(f, "%s", buffer);
		mag.localitate = (char*)malloc(strlen(buffer) + 1);
		strcpy(mag.localitate, buffer);

		fscanf(f, "%f", &mag.suprafata);
		fscanf(f, "%d", &mag.numar_angajati);

		inserareLL(&graf.capListaNoduri, mag);

		free(mag.denumire);
		free(mag.localitate);
	}

	int nrArce;

	fscanf(f, "%d", &nrArce);
	for (int i = 0; i < nrArce; i++) {
		int indexStart, indexEnd;
		float distanta;

		fscanf(f, "%d", &indexStart);
		fscanf(f, "%d", &indexEnd);
		fscanf(f, "%f", &distanta);

		nodListaPrincipala* temp = graf.capListaNoduri;
		for (int i = 1; i < indexStart; i++) {
			temp = temp->next;
		}
		inserareLS(&temp->adiacenta, indexEnd, distanta);
	}

	return graf;
}

void afisareLS(nodLS* cap) {
	nodLS* temp = cap;
	while (temp != NULL)
	{
		printf("\t->ID: %d, Distanta: %5.2f\n",
			temp->id, temp->distanta);
		temp = temp->next;
	}
}

void afiseazaGraf(Graf graf) {
	nodListaPrincipala* temp = graf.capListaNoduri;
	while (temp != NULL) {
		printf("Magazin ID: %d, Denumire: %s, Localitate: %s, Suprafata: %.2f, Angajati: %d\n",
			temp->inf.id, temp->inf.denumire, temp->inf.localitate, temp->inf.suprafata, temp->inf.numar_angajati);
		afisareLS(temp->adiacenta);
		temp = temp->next;
	}
}

void afisareArce(Graf graf) {
	nodListaPrincipala* tempListaPrinc = graf.capListaNoduri;
	while (tempListaPrinc != NULL)
	{
		int indexStart = tempListaPrinc->inf.id;
		nodLS* tempAdia = tempListaPrinc->adiacenta;
		while (tempAdia != NULL)
		{
			int indexEnd = tempAdia->id;
			float cost = tempAdia->distanta;
			printf("%d, %d, %f\n", indexStart, indexEnd, cost);
			tempAdia = tempAdia->next;
		}
		tempListaPrinc = tempListaPrinc->next;
	}
}

float suprafataTotala(Graf graf) {
	float suprafataTotala = 0;
	nodListaPrincipala* temp = graf.capListaNoduri;
	while (temp != NULL)
	{
		suprafataTotala += temp->inf.suprafata;
		temp = temp->next;
	}
	return suprafataTotala;
}

float* vectorChirii(Graf graf, int* nrElem) {
	*nrElem = graf.nrNoduri;
	float* chirii = (int*)malloc(*nrElem * sizeof(float));
	int index = 0;
	
	nodListaPrincipala* temp = graf.capListaNoduri;
	while (temp != NULL)
	{
		chirii[index] = temp->inf.suprafata * 9;
		index++;
		temp = temp->next;
	}

	return chirii;
}

void stergereLS(nodLS** cap, int id) {
	nodLS* temp = *cap;
	nodLS* prev = NULL;
	while (temp != NULL)
	{
		if (temp->id == id) {
			nodLS* deleteMe = temp;
			if (prev == NULL) {
				*cap = temp->next;
			}
			else if (temp->next == NULL) {
				prev->next = NULL;
			}
			else {
				prev->next = temp->next;
			}
			temp = temp->next;
			free(deleteMe);
		}
		else {
			prev = temp;
			temp = temp->next;
		}
	}
}

void dezalocareLS(nodLS* prim) {
	nodLS* temp = prim;
	while (temp != NULL) {
		nodLS* aux = temp->next;
		free(temp);
		temp = aux;
	}
}

void stergereLL(Graf* graf, int id) {
	nodListaPrincipala* temp = graf->capListaNoduri;
	nodListaPrincipala* prev = NULL;

	while (temp != NULL)
	{
		stergereLS(&temp->adiacenta, id);
		if (temp->inf.id == id) {
			nodListaPrincipala* deleteMe = temp;
			if (prev == NULL) {
				graf->capListaNoduri = temp->next;
			}
			else if (temp->next == NULL) {
				prev->next = NULL;
			}
			else {
				prev->next = temp->next;
			}
			free(deleteMe->inf.denumire);
			free(deleteMe->inf.localitate);
			dezalocareLS(deleteMe->adiacenta);
			temp = temp->next;
		}
		else {
			prev = temp;
			temp = temp->next;
		}
	}
}

void modificaAng(Graf* graf, int noulNrAng, int id) {
	nodListaPrincipala* temp = (*graf).capListaNoduri;
	while (temp != NULL)
	{
		if (temp->inf.id == id) {
			temp->inf.numar_angajati = noulNrAng;
		}
		temp = temp->next;
	}
}

void push(nodStiva** cap, int id) {
	nodStiva* nou = (nodStiva*)malloc(sizeof(nodStiva));
	nou->id = id;
	nou->next = *cap;
	*cap = nou;
}

int pop(nodStiva** cap, int* id) {
	if (*cap == NULL) {
		return 0;
	}
	
	*id = (*cap)->id;
	nodStiva* aux = *cap;
	*cap = (*cap)->next;
	free(aux);
	return 1;
}

int* parcurgeraInAdancime(Graf graf, int indexStart, int* nrNoduriParcurse) {
	int* vizitat = (int*)malloc((graf.nrNoduri + 1) * sizeof(int));
	for (int i = 0; i < graf.nrNoduri; i++) {
		vizitat[i] = 0;
	}

	int* rezultat = (int*)malloc(graf.nrNoduri * sizeof(int));

	*nrNoduriParcurse = 0;
	nodStiva* cap = NULL;
	push(&cap, indexStart);
	vizitat[indexStart] = 1;

	while (cap != NULL)
	{
		int currentId;
		pop(&cap, &currentId);
		rezultat[*nrNoduriParcurse] = currentId;
		(*nrNoduriParcurse)++;

		nodListaPrincipala* temp = graf.capListaNoduri;
		while (temp != NULL && temp->inf.id != currentId) {
			temp = temp->next;
		}

		if (temp != NULL) {
			nodLS* tempAdia = temp->adiacenta;
			while (tempAdia != NULL)
			{
				if (vizitat[tempAdia->id] != 1) {
					push(&cap, tempAdia->id);
					vizitat[tempAdia->id] = 1;
				}
				tempAdia = tempAdia->next;
			}
		}

	}
	free(vizitat);
	return rezultat;
}

int main() {
    Graf graf = citesteGraf("fisier.txt");
    if (graf.nrNoduri == -1) {
        printf("Fisierul nu a fost deschis\n");
        return -1;
    }

    printf("Graf initial:\n");
    afiseazaGraf(graf);

	printf("\nMuchiile sunt:\n");
	afisareArce(graf);

	float suprafataTotalaGraf = suprafataTotala(graf);
	printf("\nSuprafata totala este: %5.2f\n", suprafataTotalaGraf);

	int nrChirii = 0;
	float* chirii = vectorChirii(graf, &nrChirii);
	printf("\nVectorul de chirii:\n");
	for (int i = 0; i < nrChirii; i++) {
		printf("Magazinul de pe pozitia %d are chiria %5.2f\n",
			i, chirii[i]);
	}

	stergereLL(&graf, 2);
	printf("\nGraful dupa stergere\n");
	afiseazaGraf(graf);

	modificaAng(&graf, 100, 3);
	printf("\nGraful dupa modificarea numarului de angajati:\n");
	afiseazaGraf(graf);

	int nrNoduriParcurse = 0;
	int* rezultatDFS = parcurgeraInAdancime(graf, 1, &nrNoduriParcurse);
	printf("\nParcurgerea in adancime:\n");
	for (int i = 0; i < nrNoduriParcurse; i++) {
		printf("%d ", rezultatDFS[i]);
	}
	printf("\n");

	free(rezultatDFS);

	return 0;
}