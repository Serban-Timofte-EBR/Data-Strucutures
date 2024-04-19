#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>

typedef struct {
	char nume[50];
	char cnp[14];
	int varsta;
	char* diagnostic;
} Pacient;

typedef struct {
	Pacient pacient;
	struct NodLS* next;
} NodLS;

typedef struct {
	NodLS** vector;
	int nrElem;
} hashT;

typedef struct {
	Pacient pacient;
	struct NodLD* next;
	struct NodLD* prev;
} NodLD;

typedef struct {
	char* diagnostic;
	struct NodLS2* next;
} NodLS2;

int functie_hash(char* cnp, hashT tabela) {
	return cnp[0] % tabela.nrElem;
}

void inserare_lista_simpla(NodLS** prim, Pacient pacient) {
	NodLS* nou = (NodLS*)malloc(sizeof(NodLS));
	strcpy(nou->pacient.cnp, pacient.cnp);
	strcpy(nou->pacient.nume, pacient.nume);
	nou->pacient.varsta = pacient.varsta;
	nou->pacient.diagnostic = (char*)malloc(strlen(pacient.diagnostic) + 1);
	strcpy(nou->pacient.diagnostic, pacient.diagnostic);
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

void inserare_hash_table(hashT tabela, Pacient pacient) {
	int poz = functie_hash(pacient.cnp, tabela);
	inserare_lista_simpla(&tabela.vector[poz], pacient);
}

void printare_lista_simpla_tabela(NodLS* prim) {
	NodLS* temp = prim;
	while (temp != NULL)
	{
		printf("\tNume: %s \t CNP: %s \t Varsta: %d \t Diagnostic: %s\n", temp->pacient.nume, temp->pacient.cnp, temp->pacient.varsta, temp->pacient.diagnostic);
		temp = temp->next;
	}
}

void printare_tabela(hashT tabela) {
	for (int i = 0; i < tabela.nrElem; i++)
	{
		if (tabela.vector[i] != NULL) {
			printf("Pozitie = %d\n", i);
			printare_lista_simpla_tabela(tabela.vector[i]);
		}
	}
}

hashT sterge_cnp_tabela(hashT tabela, char* CNP) {
	int poz = functie_hash(CNP, tabela);
	if (tabela.vector[poz] == NULL) {
		return;
	}
	else
	{
		NodLS* temp = tabela.vector[poz];
		NodLS* prev = NULL;
		while (temp != NULL)
		{
			if (strcmp(temp->pacient.cnp, CNP) == 0) {
				if (prev == NULL) {
					tabela.vector[poz] = temp->next;
				}
				else {
					prev->next = temp->next;
				}
				NodLS* deleteMe = temp;
				temp = temp->next;
				free(deleteMe->pacient.diagnostic);
				free(deleteMe);
			}
			else
			{
				prev = temp;
				temp = temp->next;
			}
		}
	}
	return tabela;
}

NodLD* inserare_ld(NodLD* prim, NodLD** ultim, Pacient pacient) {
	NodLD* nou = (NodLD*)malloc(sizeof(NodLD));
	strcpy(nou->pacient.cnp, pacient.cnp);
	strcpy(nou->pacient.nume, pacient.nume);
	nou->pacient.varsta = pacient.varsta;
	nou->pacient.diagnostic = (char*)malloc(strlen(pacient.diagnostic) + 1);
	strcpy(nou->pacient.diagnostic, pacient.diagnostic);

	if (prim == NULL) {
		nou->next = NULL;
		nou->prev = NULL;
		prim = nou;
		*ultim = nou;
	}
	else
	{
		NodLD* temp = prim;
		while (temp->next != NULL)
		{
			temp = temp->next;
		}
		temp->next = nou;
		nou->next = NULL;
		nou->prev = temp;
		*ultim = nou;
	}

	return prim;
}

void printare_ld(NodLD* prim) {
	NodLD* temp = prim;
	while (temp != NULL)
	{
		printf("Nume: %s \t CNP: %s \t Varsta: %d \t Diagnostic: %s\n", temp->pacient.nume, temp->pacient.cnp, temp->pacient.varsta, temp->pacient.diagnostic);
		temp = temp->next;
	}
}

void printare_ld_invers(NodLD* ultim) {
	NodLD* temp = ultim;
	while (temp != NULL)
	{
		printf("Nume: %s \t CNP: %s \t Varsta: %d \t Diagnostic: %s\n", temp->pacient.nume, temp->pacient.cnp, temp->pacient.varsta, temp->pacient.diagnostic);
		temp = temp->prev;
	}
}

NodLS2* inserare_ls2(NodLS2* prim, char* diag) {
	NodLS2* nou = (NodLS2*)malloc(sizeof(NodLS2));
	nou->diagnostic = (char*)malloc(strlen(diag) + 1);
	strcpy(nou->diagnostic, diag);
	nou->next = NULL;

	if (prim == NULL)
		prim = nou;
	else
	{
		NodLS2* temp = prim;
		while (temp->next != NULL)
		{
			temp = temp->next;
		}
		temp->next = nou;
	}
	return prim;
}

NodLD* sterge_cnp_ld(NodLD* prim, NodLD** ultim, char* cnp, NodLS2** primLS2) {
	if (prim == NULL) {
		return prim;
	}
	else
	{
		NodLD* temp = prim;
		while (temp != NULL)
		{
			if (strcmp(temp->pacient.cnp, cnp) == 0) {
				NodLD* anterior = temp->prev;
				NodLD* urmator = temp->next;
				if (anterior == NULL) {
					//stergem primul nod din lista
					temp->prev = NULL;
					if (urmator == NULL) {
						//am un singur nod
						prim = NULL;
						*ultim = NULL;
					}
					else
					{
						prim = temp->next;
					}
				}
				else if(urmator == NULL)
				{
					//sterg ultimul nod
					*ultim = anterior;
					if (anterior == NULL) {
						prim = NULL;
						*ultim = NULL;
					}
					else
					{
						anterior->next = NULL;
					}
				}
				else
				{
					//sterg din mijloc
					anterior->next = urmator;
					urmator->prev = anterior;
				}
				NodLD* deleteMe = temp;
				temp = temp->next;

				*primLS2 = inserare_ls2(*primLS2, deleteMe->pacient.diagnostic);

				free(deleteMe->pacient.diagnostic);
				free(deleteMe);
			}
			else {
				temp = temp->next;
			}
		}
	}
	return prim;
}

void dezalocareLS(NodLS* prim) {
	NodLS* temp = prim;
	while (temp != NULL)
	{
		NodLS* aux = temp->next;
		free(temp->pacient.diagnostic);
		free(temp);
		temp = aux;
	}
}

void dezalocareHashTable(hashT tabela) {
	for (int i = 0; i < tabela.nrElem; i++) {
		if (tabela.vector[i] != NULL) {
			dezalocareLS(tabela.vector[i]);
		}
	}
}

void dezalocareLD(NodLD* prim) {
	NodLD* temp = prim;
	while (temp != NULL)
	{
		NodLS* aux = temp->next;
		free(temp->pacient.diagnostic);
		free(temp);
		temp = aux;
	}
}

void dezalocareLS2(NodLS2* prim) {
	NodLS2* temp = prim;
	while (temp != NULL)
	{
		NodLS* aux = temp->next;
		free(temp->diagnostic);
		free(temp);
		temp = aux;
	}
}

int main() {
	hashT tabela;
	tabela.nrElem = 13;
	tabela.vector = (NodLS**)malloc(tabela.nrElem * sizeof(NodLS*));
	for (unsigned char i = 0; i < tabela.nrElem; i++)
		tabela.vector[i] = NULL;

	FILE* f = fopen("Pacienti.txt", "r");
	if (f == NULL) {
		printf("Fisierul nu s-a deschis");
		return -1;
	}

	char buffer[128];
	Pacient pacient;
	while (fscanf(f, "%s", pacient.nume) == 1) 
	{
		fscanf(f, "%s", pacient.cnp);
		fscanf(f, "%d", &pacient.varsta);

		fscanf(f, " %[^\n]", buffer);
		pacient.diagnostic = (char*)malloc(strlen(buffer) + 1);
		strcpy(pacient.diagnostic, buffer);

		inserare_hash_table(tabela, pacient);

		free(pacient.diagnostic);
	}

	printf("\nTabela citita din fisier\n");
	printare_tabela(tabela);

	tabela = sterge_cnp_tabela(tabela, "5020910394431");
	printf("\nTabela dupa stergere\n");
	printare_tabela(tabela);


	NodLD* primLD = (NodLD*)malloc(sizeof(NodLD));
	NodLD* ultimLD = (NodLD*)malloc(sizeof(NodLD));
	primLD = NULL;
	ultimLD = NULL;
	for (int i = 0; i < tabela.nrElem; i++)
	{
		if (tabela.vector[i] != NULL) {
			NodLS* temp = tabela.vector[i];
			while (temp != NULL)
			{
				if (strcmp(temp->pacient.nume, "Ioanovici") == 0) {
					primLD = inserare_ld(primLD, &ultimLD, temp->pacient);
				}
				temp = temp->next;
			}
		}
	}

	printf("\nLista pacientului Ioanovici\n");
	printare_ld(primLD);

	printf("\nSi invers\n");
	printare_ld_invers(ultimLD);

	//sterg din lista dubla cnp 5000910394431 si salvez diagnosticul intr-o lista simpla
	NodLS2* primLS2 = (NodLS2*)malloc(sizeof(NodLS2));
	primLS2 = NULL;
	primLD = sterge_cnp_ld(primLD, &ultimLD, "5000910394431", &primLS2);
	printf("\nLista pacientului Ioanovici dupa stergere de CNP\n");
	printare_ld(primLD);

	printf("\nLista Diagnosticurilor sterse\n");
	NodLS2* temp = primLS2;
	while (temp != NULL)
	{
		printf("Diagnostic: %s\n", temp->diagnostic);
		temp = temp->next;
	}

	dezalocareHashTable(tabela);
	dezalocareLD(primLD);
	dezalocareLS2(primLS2);

	return 0;
}