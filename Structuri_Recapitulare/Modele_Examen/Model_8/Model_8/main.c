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
	Magazin* inf;
	struct nodLS* next;
} nodLS;

typedef struct {
	Magazin inf;
	int ocupat;
} elementHash;

typedef struct {
	elementHash* vector;
	int nrElem;
} hashT;

nodLS* inserareLSOrdonat(nodLS* prim, Magazin mag) {
	nodLS* nou = (nodLS*)malloc(sizeof(nodLS));
	nou->inf = (Magazin*)malloc(sizeof(Magazin));
	nou->inf->id = mag.id;
	nou->inf->numar_angajati = mag.numar_angajati;
	nou->inf->suprafata = mag.suprafata;
	nou->inf->denumire = (char*)malloc(strlen(mag.denumire) + 1);
	strcpy(nou->inf->denumire, mag.denumire);
	nou->inf->localitate = (char*)malloc(strlen(mag.localitate) + 1);
	strcpy(nou->inf->localitate, mag.localitate);
	nou->next = NULL;

	if (prim == NULL) {
		prim = nou;
	}
	else {
		nodLS* temp = prim;
		nodLS* prev = NULL;
		while (temp != NULL && temp->inf->id < mag.id)
		{
			prev = temp;
			temp = temp->next;
		}

		if (temp == NULL) {
			prev->next = nou;
		}
		else if (temp->inf->id >= mag.id) {
			prev->next = nou;
			nou->next = temp;
		}
	}
	return prim;
}

void afisareaLista(nodLS* prim) {
	nodLS* temp = prim;
	while (temp != NULL)
	{
		printf("ID: %d, Nr. angajati: %d, Suprafata: %f, Nume: %s, Localitate: %s\n",
			temp->inf->id, temp->inf->numar_angajati, temp->inf->suprafata, temp->inf->denumire, temp->inf->localitate);
		temp = temp->next;
	}
}

nodLS* modificareMagazin(nodLS* prim, char* denumire, char* localitate, char* denumireNoua) {
	nodLS* temp = prim;
	while (temp != NULL)
	{
		if (strcmp(temp->inf->denumire, denumire) == 0 &&
			strcmp(temp->inf->localitate, localitate) == 0) {
			free(temp->inf->denumire);
			temp->inf->denumire = (char*)malloc(strlen(denumireNoua) + 1);
			strcpy(temp->inf->denumire, denumireNoua);
		}
		temp = temp->next;
	}
	return prim;
}

float numarMediuAngajatiLocalitate(nodLS* prim, char* localitate) {
	int nrAngajati = 0;
	int nrMagazine = 0;

	nodLS* temp = prim;
	while (temp != NULL)
	{
		if (strcmp(temp->inf->localitate, localitate) == 0) {
			nrAngajati += temp->inf->numar_angajati;
			nrMagazine++;
		}
		temp = temp->next;
	}

	return nrAngajati / (float)nrMagazine;
}

int functieHash(hashT tabela, char* localitate) {
	return localitate[0] % tabela.nrElem;
}

void inserareHash(hashT* tabela, Magazin mag) {
	int poz = functieHash(*tabela, mag.localitate);
	int index = 0;

	while (tabela->vector[(poz - index) % tabela->nrElem].ocupat == 1) {
		index++;
	}

	int pozFin = (poz - index) % tabela->nrElem;
	tabela->vector[pozFin].inf.id = mag.id;
	tabela->vector[pozFin].inf.numar_angajati = mag.numar_angajati;
	tabela->vector[pozFin].inf.suprafata = mag.suprafata;
	tabela->vector[pozFin].inf.denumire = (char*)malloc(strlen(mag.denumire) + 1);
	strcpy(tabela->vector[pozFin].inf.denumire, mag.denumire);
	tabela->vector[pozFin].inf.localitate = (char*)malloc(strlen(mag.localitate) + 1);
	strcpy(tabela->vector[pozFin].inf.localitate, mag.localitate);
	tabela->vector[pozFin].ocupat = 1;
}

hashT lista_to_hash(nodLS* prim) {
	hashT tabela;
	tabela.nrElem = 23;
	tabela.vector = (elementHash*)malloc(tabela.nrElem * sizeof(elementHash));
	for (int i = 0; i < tabela.nrElem; i++) {
		tabela.vector[i].ocupat = 0;
	}

	nodLS* temp = prim;
	while (temp != NULL)
	{
		inserareHash(&tabela, *temp->inf);
		temp = temp->next;
	}
	return tabela;
}

void afisareTabela(hashT tabela) {
	for (int i = 0; i < tabela.nrElem; i++) {
		if (tabela.vector[i].ocupat == 1) {
			printf("Pozitia %d:\n", i);
			printf("\tID: %d, Nr. angajati: %d, Suprafata: %f, Nume: %s, Localitate: %s\n",
				tabela.vector[i].inf.id, tabela.vector[i].inf.numar_angajati, tabela.vector[i].inf.suprafata,
				tabela.vector[i].inf.denumire, tabela.vector[i].inf.localitate);
		}
	}
}

void stergereHash(hashT* tabela, char* denumire, char* localitate) {
	int poz = functieHash(*tabela, localitate);
	int index = 0;
	while (tabela->vector[(poz - index + tabela->nrElem) % tabela->nrElem].ocupat != 0)
	{
		int pozVector = (poz - index) % tabela->nrElem;
		if (strcmp(tabela->vector[pozVector].inf.denumire, denumire) == 0 &&
			strcmp(tabela->vector[pozVector].inf.localitate, localitate) == 0) {
			//free(tabela->vector[pozVector].inf.denumire);
			free(tabela->vector[pozVector].inf.localitate);
			tabela->vector[pozVector].ocupat = -1;
		}
		index++;
	}
}

void modificaDenumireInHash(hashT* tabela, char* denumire, char* localitate, char* localitateNoua) {
	int poz = functieHash(*tabela, localitate);
	int index = 0;

	while (tabela->vector[(poz - index + tabela->nrElem) % tabela->nrElem].ocupat != 0) 
	{
		int pozVector = (poz - index) % tabela->nrElem;
		if (strcmp(tabela->vector[pozVector].inf.denumire, denumire) == 0 &&
			strcmp(tabela->vector[pozVector].inf.localitate, localitate) == 0) {
			Magazin mag = tabela->vector[pozVector].inf;
			mag.localitate = localitateNoua;
			stergereHash(tabela, denumire, localitate);
			inserareHash(tabela, mag);
		}
		index++;
	}
}

void dezalocareLS(nodLS* prim) {
	nodLS* temp = prim;
	while (temp != NULL)
	{
		nodLS* aux = temp->next;
		free(temp->inf->denumire);
		free(temp->inf->localitate);
		free(temp->inf);
		free(temp);
		temp = aux;
	}
}

void dezalocareHash(hashT tabela) {
	for (int i = 0; i < tabela.nrElem; i++) {
		if (tabela.vector[i].ocupat == 1) {
			free(tabela.vector[i].inf.denumire);
			free(tabela.vector[i].inf.localitate);
		}
	}
	free(tabela.vector);
}

int main() {
	nodLS* prim = NULL;

	FILE* f = fopen("fisier.txt", "r");
	if (f == NULL) {
		printf("Fisierul nu a fost deschis!");
		return -1;
	}

	int nrMagazine;
	fscanf(f, "%d", &nrMagazine);

	Magazin mag;
	char buffer[128];

	for (int i = 0; i < nrMagazine; i++) {
		fscanf(f, "%d", &mag.id);

		fscanf(f, " %[^\n]", buffer);
		mag.denumire = (char*)malloc(strlen(buffer) + 1);
		strcpy(mag.denumire, buffer);

		fscanf(f, "%s", buffer);
		mag.localitate = (char*)malloc(strlen(buffer) + 1);
		strcpy(mag.localitate, buffer);

		fscanf(f, "%f", &mag.suprafata);
		fscanf(f, "%d", &mag.numar_angajati);

		prim = inserareLSOrdonat(prim, mag);

		free(mag.denumire);
		free(mag.localitate);
	}
	fclose(f);

	printf("Lista citita din fisier:\n");
	afisareaLista(prim);

	prim = modificareMagazin(prim, "Iul Shop", "Vulturu", "La Magazinul Nostru");
	prim = modificareMagazin(prim, "Mega Image", "Corbeanca", "Corbeanca Shop");
	printf("\nLista dupa modificare:\n");
	afisareaLista(prim);

	float nrMedAng = numarMediuAngajatiLocalitate(prim, "Bucuresti");
	printf("\nNumarul mediu de angajati din localitatea ceruta este %f\n", nrMedAng);

	hashT tabela = lista_to_hash(prim);
	printf("\nTabela de dispersie:\n");
	afisareTabela(tabela);

	modificaDenumireInHash(&tabela, "La Oaie", "Fierbinti", "Las Fierbinti");
	printf("\nTabela dupa modificare:\n");
	afisareTabela(tabela);

	dezalocareLS(prim);
	dezalocareHash(tabela);

	return 0;
}