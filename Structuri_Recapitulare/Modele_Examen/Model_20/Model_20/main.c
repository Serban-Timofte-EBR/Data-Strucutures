#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <malloc.h>
#include <string.h>

typedef struct {
	int id;
	char* denumire;
	char* localitate;
	float suprafata;
	int nrAngajati;
} Magazin;

typedef struct {
	Magazin* inf;
	struct nodLD* next;
	struct nodLD* prev;
} nodLD;

typedef struct {
	Magazin inf;
	int ocupat;
} elementHash;

typedef struct {
	elementHash* vector;
	int nrElem;
} hashT;

nodLD* inserareLDCrescator(nodLD* prim, nodLD** ultim, Magazin inf) {
	nodLD* nou = (nodLD*)malloc(sizeof(nodLD));
	nou->inf = (Magazin*)malloc(sizeof(Magazin));
	nou->inf->id = inf.id;
	nou->inf->nrAngajati = inf.nrAngajati;
	nou->inf->suprafata = inf.suprafata;
	nou->inf->denumire = (char*)malloc(strlen(inf.denumire) + 1);
	strcpy(nou->inf->denumire, inf.denumire);
	nou->inf->localitate = (char*)malloc(strlen(inf.localitate) + 1);
	strcpy(nou->inf->localitate, inf.localitate);
	nou->next = NULL;
	nou->prev = NULL;

	if (prim == NULL) {
		prim = nou;
		*ultim = nou;
	}
	else {
		nodLD* temp = prim;
		while (temp != NULL && temp->inf->id < inf.id) {
			temp = temp->next;
		}

		if (temp == NULL) {
			(*ultim)->next = nou;
			nou->prev = *ultim;
			*ultim = nou;
		}
		else if (temp->inf->id >= inf.id) {
			if (temp == prim) {
				nou->next = prim;
				prim->prev = nou;
				prim = nou;
			}

			else {
				nodLD* ant = temp->prev;
				ant->next = nou;
				nou->prev = ant;
				nou->next = temp;
				temp->prev = nou;
			}
		}
	}
	return prim;
}

void afisareLDCapCoada(nodLD* prim) {
	nodLD* temp = prim;
	while (temp != NULL)
	{
		printf("ID: %d, Nr. Ang.: %d, Suprafata: %5.2f, Localitate: %s, Magazin: %s\n",
			temp->inf->id, temp->inf->nrAngajati, temp->inf->suprafata, temp->inf->localitate, temp->inf->denumire);
		temp = temp->next;
	}
}

void afisareLDCoadaCap(nodLD* ultim) {
	nodLD* temp = ultim;
	while (temp != NULL)
	{
		printf("ID: %d, Nr. Ang.: %d, Suprafata: %5.2f, Localitate: %s, Magazin: %s\n",
			temp->inf->id, temp->inf->nrAngajati, temp->inf->suprafata, temp->inf->localitate, temp->inf->denumire);
		temp = temp->prev;
	}
}

nodLD* modificareDenumire(nodLD* prim, char* denAct, char* loc, char* denNou) {
	nodLD* temp = prim;
	while (temp != NULL)
	{
		if (strcmp(temp->inf->denumire, denAct) == 0 &&
			strcmp(temp->inf->localitate, loc) == 0) {
			free(temp->inf->denumire);
			temp->inf->denumire = (char*)malloc(strlen(denNou) + 1);
			strcpy(temp->inf->denumire, denNou);
		}
		temp = temp->next;
	}
	return prim;
}

float nrMediuAng(nodLD* prim, char* localitate) {
	int nrAngLoc = 0;
	int nrMag = 0;
	nodLD* temp = prim;
	while (temp != NULL)
	{
		if (strcmp(temp->inf->localitate, localitate) == 0) {
			nrMag++;
			nrAngLoc += temp->inf->nrAngajati;
		}
		temp = temp->next;
	}

	if (nrMag > 0) {
		return nrAngLoc / (float)nrMag;
	}
	else {
		return 0.0;
	}
}

int functieHash(hashT tabela, char* localitate) {
	return localitate[0] % tabela.nrElem;
}

hashT inserareHash(hashT tabela, Magazin inf) {
	int poz = functieHash(tabela, inf.localitate);
	int index = 0;

	while (tabela.vector[(poz - index) % tabela.nrElem].ocupat != 0) {
		index++;
	}

	int pozFin = (poz - index) % tabela.nrElem;
	tabela.vector[pozFin].inf.id = inf.id;
	tabela.vector[pozFin].inf.nrAngajati = inf.nrAngajati;
	tabela.vector[pozFin].inf.suprafata = inf.suprafata;
	tabela.vector[pozFin].inf.denumire = (char*)malloc(strlen(inf.denumire) + 1);
	strcpy(tabela.vector[pozFin].inf.denumire, inf.denumire);
	tabela.vector[pozFin].inf.localitate = (char*)malloc(strlen(inf.localitate) + 1);
	strcpy(tabela.vector[pozFin].inf.localitate, inf.localitate);
	tabela.vector[pozFin].ocupat = 1;

	return tabela;
}

hashT generareTabela(nodLD* prim) {
	hashT tabela;
	tabela.nrElem = 23;
	tabela.vector = (elementHash*)malloc(tabela.nrElem * sizeof(elementHash));
	for (int i = 0; i < tabela.nrElem; i++) {
		tabela.vector[i].ocupat = 0;
	}

	nodLD* temp = prim;
	while (temp != NULL)
	{
		tabela = inserareHash(tabela, *temp->inf);
		temp = temp->next;
	}

	return tabela;
}

void afisareHash(hashT tabela) {
	for (int i = 0; i < tabela.nrElem; i++) {
		if (tabela.vector[i].ocupat == 1) {
			printf("Pozitie %d:\n", i);
			printf("\tID: %d, Nr. Ang.: %d, Suprafata: %5.2f, Localitate: %s, Magazin: %s\n",
				tabela.vector[i].inf.id, tabela.vector[i].inf.nrAngajati, tabela.vector[i].inf.suprafata,
				tabela.vector[i].inf.localitate, tabela.vector[i].inf.denumire);
		}
	}
}

Magazin stergereMagazin(hashT* tabela, char* locatitate, char* denumire) {
	int poz = functieHash(*tabela, locatitate);
	int index = 0;

	while ((*tabela).vector[(poz - index) %tabela->nrElem].ocupat == 1)
	{
		int indexC = (poz - index) % tabela->nrElem;
		if (strcmp((*tabela).vector[indexC].inf.denumire, denumire) == 0 &&
			strcmp((*tabela).vector[indexC].inf.localitate, locatitate) == 0) {
			(*tabela).vector[indexC].ocupat = -1;
			return (*tabela).vector[indexC].inf;
		}
		index++;
	}
}

hashT modificareMagazin(hashT tabela, char* localitateAct, char* den, char* locNoua) {
	Magazin mag = stergereMagazin(&tabela, localitateAct, den);
	free(mag.localitate);
	mag.localitate = (char*)malloc(strlen(locNoua) + 1);
	strcpy(mag.localitate, locNoua);
	tabela = inserareHash(tabela, mag);
	return tabela;
}

void dezalocareLD(nodLD* prim) {
	while (prim != NULL)
	{
		nodLD* aux = prim->next;
		free(prim->inf->denumire);
		free(prim->inf->localitate);
		free(prim);
		prim = aux;
	}
}

void dezalocareHash(hashT t) {
	for (int i = 0; i < t.nrElem; i++)
	{
		if (t.vector[i].ocupat == 1) {
			free(t.vector[i].inf.denumire);
			free(t.vector[i].inf.localitate);
		}
	}
	free(t.vector);
}

int main() {
	nodLD* prim = NULL;
	nodLD* ultim = NULL;

	FILE* f = fopen("fisier.txt", "r");

	Magazin mag;
	char buffer[128];

	while (fscanf(f, "%d", &mag.id) == 1)
	{
		fscanf(f, " %[^\n]", buffer);
		mag.denumire = (char*)malloc(strlen(buffer) + 1);
		strcpy(mag.denumire, buffer);

		fscanf(f, "%s", buffer);
		mag.localitate = (char*)malloc(strlen(buffer) + 1);
		strcpy(mag.localitate, buffer);

		fscanf(f, "%f", &mag.suprafata);
		fscanf(f, "%d", &mag.nrAngajati);

		/*printf("ID: %d, Nr. Ang.: %d, Suprafata: %5.2f, Localitate: %s, Magazin: %s\n",
			mag.id, mag.nrAngajati, mag.suprafata, mag.localitate, mag.denumire);*/
		prim = inserareLDCrescator(prim, &ultim, mag);

		free(mag.denumire);
		free(mag.localitate);
	}
	fclose(f);

	printf("Lista Dubla CAP -> COADA:\n");
	afisareLDCapCoada(prim);

	printf("\nLista Dubla COADA -> CAP:\n");
	afisareLDCoadaCap(ultim);

	prim = modificareDenumire(prim, "Mega Image", "Corbeanca", "Corb Market");
	printf("\nLista dupa modificare:\n");
	afisareLDCapCoada(prim);

	printf("\nLista invers:\n");
	afisareLDCoadaCap(ultim);

	float nrMed = nrMediuAng(prim, "Bucuresti");
	printf("\nNumarul mediu de angajati este: %5.2f\n", nrMed);
	nrMed = nrMediuAng(prim, "Brasov");
	printf("\nNumarul mediu de angajati este: %5.2f\n", nrMed);
	nrMed = nrMediuAng(prim, "Focsani");
	printf("\nNumarul mediu de angajati este: %5.2f\n", nrMed);
	nrMed = nrMediuAng(prim, "Mogosoaia");
	printf("\nNumarul mediu de angajati este: %5.2f\n", nrMed);

	hashT tabela = generareTabela(prim);
	printf("\nTabela de dispersie:\n");
	afisareHash(tabela);

	tabela = modificareMagazin(tabela, "Bucuresti", "Mega Image", "Focsani");
	printf("\nTabela dupa modificare:\n");
	afisareHash(tabela);

	dezalocareLD(prim);
	dezalocareHash(tabela);

	return 0;
}