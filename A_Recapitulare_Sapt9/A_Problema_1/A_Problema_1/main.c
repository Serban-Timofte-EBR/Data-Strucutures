#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>

typedef struct {
	char titlu[100];
	char* autor;
	int anPublicare;
} Carte;

typedef struct {
	Carte carte;
	struct NodLS* next;
} NodLS;

typedef struct {
	Carte* carte;
	struct NodLD* next;
	struct NodLD* prev;
} NodLD;

typedef struct {
	NodLS** vector;
	int nrElem;
} hashT;

NodLS* inserare_lista_simpla_sfarsit(NodLS* prim, Carte carte) {
	NodLS* nou = (NodLS*)malloc(sizeof(NodLS));
	strcpy(nou->carte.titlu, carte.titlu);
	nou->carte.autor = (char*)malloc(strlen(carte.autor) + 1);
	strcpy(nou->carte.autor, carte.autor);
	nou->carte.anPublicare = carte.anPublicare;
	nou->next = NULL;

	if (prim == NULL)
		prim = nou;
	else
	{
		NodLS* temp = prim;
		while (temp->next != NULL)
		{
			temp = temp->next;
		}
		temp->next = nou;
	}

	return prim;
}

void printare_lists_simpla(NodLS* prim) {
	if (prim == NULL) {
		printf("Lista este goala\n");
	}
	else
	{
		NodLS* temp = prim;
		while (temp != NULL)
		{
			printf("Autor: %s \t Titul: %s \t An: %d\n", temp->carte.autor, temp->carte.titlu, temp->carte.anPublicare);
			temp = temp->next;
		}
	}
}

NodLS* stegere_titlu_lista_simpla(NodLS* prim, char* titlu) {
	if (prim == NULL)
		return prim;
	else
	{
		//stergem primul nod
		if (strcmp(prim->carte.titlu, titlu) == 0) {
			NodLS* deleteMe = prim;
			prim = prim->next;
			free(deleteMe->carte.autor);
			free(deleteMe);
		}

		NodLS* temp = prim;
		NodLS* prev = NULL;
		while (temp != NULL)
		{
			if (strcmp(temp->carte.titlu, titlu) == 0) {
				NodLS* deleteMe = temp;
				prev->next = temp->next;
				free(deleteMe->carte.autor);
				free(deleteMe);
				temp = prev->next;
			}
			else {
				prev = temp;
				temp = temp->next;
			}
		}
	}
	return prim;
}

NodLD* inserare_lista_dubla(NodLD* prim, NodLD** ultim, Carte* carte) {
	NodLD* nou = (NodLD*)malloc(sizeof(NodLD));
	nou->carte = carte;
	nou->next = NULL;
	nou->prev = NULL;

	if (prim == NULL) {
		prim = nou;
		*ultim = nou;
	}
	else
	{
		NodLD* temp = prim;
		while (temp != NULL && strcmp(temp->carte->titlu, carte->titlu) == -1)
		{
			temp = temp->next;
		}
		// am iesit din loop
		if (temp == NULL) {
			//suntem la sfarsitul listei
			(*ultim)->next = nou;
			nou->prev = *ultim;
			*ultim = nou;
		}
		else if (temp == prim) {
			// inserarea se face la inceputul listei
			nou->next = prim;
			prim->prev = nou;
			prim = nou;
		}
		else
		{
			//inserarea se face inainte de temp
			NodLD* prev = temp->prev;
			nou->next = temp;
			nou->prev = prev;
			prev->next = nou;
			temp->prev = nou;
		}
	}
	return prim;
}

NodLD* inserare_lista_dubla_alfabetic(NodLD* prim, NodLD** ultim, NodLS* capLS, char* titlu) {
	NodLS* temp = capLS;
	while (temp != NULL)
	{
		if (strcmp(temp->carte.titlu, titlu) == 0) {
			//adaug in lista dubla
			prim = inserare_lista_dubla(prim, ultim, &temp->carte);
		}
		temp = temp->next;
	}
	return prim;
}

void printare_lista_dubla(NodLD* prim) {
	NodLD* temp = prim;
	while (temp != NULL)
	{
		printf("Autor: %s \t Titul: %s \t An: %d\n", temp->carte->autor, temp->carte->titlu, temp->carte->anPublicare);
		temp = temp->next;
	}
}

int functie_hash(char* autor, hashT tabela) {
	return autor[0] % tabela.nrElem;
}

void inserare_lista_simpla(NodLS** prim, Carte carte) {
	NodLS* nou = (NodLS*)malloc(sizeof(NodLS));
	strcpy(nou->carte.titlu, carte.titlu);
	nou->carte.autor = (char*)malloc(strlen(carte.autor) + 1);
	strcpy(nou->carte.autor, carte.autor);
	nou->carte.anPublicare = carte.anPublicare;
	nou->next = NULL;

	if (*prim == NULL) {
		*prim = nou;
	}
	else
	{
		NodLS* temp = *prim;
		while (temp->next != NULL)
		{
			temp = temp->next;
		}
		temp->next = nou;
	}
}

void inserare_tabela(hashT tabela, Carte carte) {
	int poz = functie_hash(carte.autor, tabela);
	inserare_lista_simpla(&tabela.vector[poz], carte);
}

void traversare_hash_table(hashT tabela) {
	for (unsigned char i = 0; i < tabela.nrElem; i++)
	{
		if (tabela.vector[i] != NULL) {
			printf("\nPozitie = %d\n\t", i);
			printare_lists_simpla(tabela.vector[i]);
		}
	}
}

Carte* carti_autor_hash(hashT tabela, char* autor, Carte* vector, int* nrElem) {
	int poz = functie_hash(autor, tabela);
	if (tabela.vector[poz] == NULL) {
		return vector;
	}
	else
	{
		NodLS* temp = tabela.vector[poz];
		while (temp != NULL)
		{
			if (strcmp(temp->carte.autor, autor) == 0) {
				vector[*nrElem] = temp->carte;
				(*nrElem)++;
			}
			temp = temp->next;
		}
	}
	return vector;
}

int main() {
	NodLS* primLS = (NodLS*)malloc(sizeof(NodLS));
	primLS = NULL;

	FILE* f = fopen("Carti.txt", "r");
	if (f == NULL) {
		printf("Fisierul nu a fost deschis");
		return -1;
	}

	char buffer[128];
	Carte carte;
	while (fscanf(f, " %[^\n]", carte.titlu) == 1)
	{
		fscanf(f, " %[^\n]", buffer);
		carte.autor = (char*)malloc(strlen(buffer) + 1);
		strcpy(carte.autor, buffer);
		fscanf(f, "%d", &carte.anPublicare);

		primLS = inserare_lista_simpla_sfarsit(primLS, carte);
		
		free(carte.autor);
	}
	fclose(f);

	printf("Lista simpla citita din fisier\n");
	printare_lists_simpla(primLS);

	//primLS = stegere_titlu_lista_simpla(primLS, "Ferma Animalelor");
	printf("\nLista simpla dupa stegere\n");
	printare_lists_simpla(primLS);

	NodLD* primLD = (NodLD*)malloc(sizeof(NodLD));
	NodLD* ultimLD = (NodLD*)malloc(sizeof(NodLD));
	primLD = NULL;
	ultimLD = NULL;

	primLD = inserare_lista_dubla_alfabetic(primLD, &ultimLD, primLS, "Enigma Otiliei");
	primLD = inserare_lista_dubla_alfabetic(primLD, &ultimLD, primLS, "Amintiri din Copilarie");
	primLD = inserare_lista_dubla_alfabetic(primLD, &ultimLD, primLS, "Ion");
	primLD = inserare_lista_dubla_alfabetic(primLD, &ultimLD, primLS, "Ferma Animalelor");
	printf("\nLista dubla dupa inserare\n");
	printare_lista_dubla(primLD);

	//inseram toate cartile din lista simpla intr-o tabela
	hashT tabela;
	tabela.nrElem = 13;
	tabela.vector = (NodLS**)malloc(tabela.nrElem * sizeof(NodLS*));
	for (unsigned char i = 0; i < tabela.nrElem; i++) {
		tabela.vector[i] = NULL;
	}

	NodLS* temp = primLS;
	while (temp != NULL)
	{
		inserare_tabela(tabela, temp->carte);
		temp = temp->next;
	}

	traversare_hash_table(tabela);

	int nrElem = 0;
	Carte* vectorCartiAutor = (Carte*)malloc(5 * sizeof(Carte));
	vectorCartiAutor = carti_autor_hash(tabela, "G. Orwell", vectorCartiAutor, &nrElem);
	printf("\nCartile din vector\n");
	for (unsigned char i = 0; i < nrElem; i++)
	{
		printf("Autor: %s \t Titul: %s \t An: %d\n", vectorCartiAutor[i].autor, vectorCartiAutor[i].titlu, vectorCartiAutor[i].anPublicare);
	}

	return 0;
}