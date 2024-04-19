#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>

typedef struct {
	char id[10];
	char* nume;
	int salariul;
} Angajat;

typedef struct {
	Angajat ang;
	struct NodLD* next;
	struct NodLD* prev;
} NodLD;

typedef struct {
	char* nume;
	struct NodLS* next;
} NodLS;

typedef struct {
	NodLS** vector;
	int nrElem;
} hashT;

NodLD* inserare_LD_alfabetic(NodLD* prim, NodLD** ultim, Angajat ang) {
	NodLD* nou = (NodLD*)malloc(sizeof(NodLD));
	strcpy(nou->ang.id, ang.id);
	nou->ang.nume = (char*)malloc(strlen(ang.nume) + 1);
	strcpy(nou->ang.nume, ang.nume);
	nou->ang.salariul = ang.salariul;
	nou->next = NULL;
	nou->prev = NULL;

	if (prim == NULL) {
		prim = nou;
		*ultim = nou;
	}
	else
	{
		NodLD* temp = prim;
		while (temp != NULL && strcmp(temp->ang.nume, ang.nume) < 0) {
			temp = temp->next;
		}
		if (temp == NULL) {
			(*ultim)->next = nou;
			nou->prev = *ultim;
			*ultim = nou;
		}
		else if (temp == prim) {
			nou->next = prim;
			prim->prev = nou;
			prim = nou;
		}
		else {
			NodLD* ant = temp->prev;
			nou->next = temp;
			nou->prev = ant;
			temp->prev = nou;
			ant->next = nou;
		}
	}
	return prim;
}

NodLD* inserare_LD(NodLD* prim, NodLD** ultim, Angajat ang) {
    NodLD* nou = (NodLD*)malloc(sizeof(NodLD));
    strcpy(nou->ang.id, ang.id);
    nou->ang.nume = (char*)malloc(strlen(ang.nume) + 1);
    strcpy(nou->ang.nume, ang.nume);
    nou->ang.salariul = ang.salariul;
    nou->next = NULL;
    nou->prev = NULL;

    if (prim == NULL) {
        prim = nou;
        *ultim = nou;
    }
	else {
		NodLD* temp = prim;
		while (temp->next != NULL) {
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
		printf("ID: %s \t Nume: %s \t Salariu: %d\n", temp->ang.id, temp->ang.nume, temp->ang.salariul);
		temp = temp->next;
	}
 }

void printare_ld_invers(NodLD* prim) {
	NodLD* temp = prim;
	while (temp != NULL)
	{
		printf("ID: %s \t Nume: %s \t Salariu: %d\n", temp->ang.id, temp->ang.nume, temp->ang.salariul);
		temp = temp->prev;
	}
}

NodLS* inserare_ls_500(NodLS* prim, char* nume) {
	NodLS* nou = (NodLS*)malloc(sizeof(NodLS));
	nou->nume = (char*)malloc(strlen(nume) + 1);
	strcpy(nou->nume, nume);
	nou->next = NULL;

	if (prim == NULL) {
		prim = nou;
	}
	else
	{
		NodLS* temp = prim;
		while (temp->next != NULL) {
			temp = temp->next;
		}
		temp->next = nou;
	}
	return prim;
}

NodLD* sterge_ang_5000(NodLD* prim, NodLD** ultim, NodLS** primLS) {
	NodLD* temp = prim;
	while (temp != NULL)
	{
		if (temp->ang.salariul < 5000) {
			NodLD* deleteME = temp;
			*primLS = inserare_ls_500(*primLS, temp->ang.nume);
			if (temp == prim) {
				prim = prim->next;
				prim->prev = NULL;
			}
			else if (temp == *ultim) {
				NodLD* ant = (*ultim)->prev;
				ant->next = NULL;
				*ultim = ant;
			}
			else {
				NodLD* ant = temp->prev;
				NodLD* urm = temp->next;
				ant->next = urm;
				urm->prev = ant;
			}
			temp = temp->next;
			free(deleteME->ang.nume);
			free(deleteME);
		}
		else {
			temp = temp->next;
		}
	}
	return prim;
}

int functie_hash(hashT tabela, char* nume) {
	return nume[0] % tabela.nrElem;
}

void inserare_ls_hash(NodLS** prim, char* nume) {
	NodLS* nou = (NodLS*)malloc(sizeof(NodLS));
	nou->nume = (char*)malloc(strlen(nume) + 1);
	strcpy(nou->nume, nume);
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

void inserare_hash(hashT tabela, char* nume) {
	int poz = functie_hash(tabela, nume);
	inserare_ls_hash(&tabela.vector[poz], nume);
}

NodLD* sterge_angajati_peste_10000(NodLD* prim, NodLD** ultim, hashT tabela) {
	if (prim == NULL) {
		return prim;
	}
	else
	{
		NodLD* temp = prim;
		while (temp != NULL)
		{
			if (temp->ang.salariul > 10000) {
				NodLD* deleteMe = temp;
				inserare_hash(tabela, deleteMe->ang.nume);
				if (temp == prim) {
					prim = prim->next;
					prim->prev = NULL;
				}
				else if (temp == *ultim) {
					NodLD* ant = temp->prev;
					ant->next = NULL;
					*ultim = ant;
				}
				else {
					NodLD* ant = temp->prev;
					NodLD* urm = temp->next;
					ant->next = urm;
					urm->prev = ant;
				}
				temp = temp->next;
				free(deleteMe->ang.nume);
				free(deleteMe);
			}
			else
			{
				temp = temp->next;
			}
		}
	}
	return prim;
}

void printare_hah_ls(NodLS* prim) {
	NodLS* temp = prim;
	while (temp != NULL)
	{
		printf("\tNume: %s\n", temp->nume);
		temp = temp->next;
	}
}

void printare_hash(hashT tablela) {
	for (int i = 0; i < tablela.nrElem; i++)
	{
		if (tablela.vector[i] != NULL) {
			printf("Pozitie = %d\n", i);
			printare_hah_ls(tablela.vector[i]);
		}
	}
}

void sterge_nume_hash(hashT tabela, char* nume) {
	int poz = functie_hash(tabela, nume);
	if (tabela.vector[poz] == NULL)
		return;
	else {
		NodLS* temp = tabela.vector[poz];
		NodLS* prev = NULL;
		while (temp != NULL) {
			if (strcmp(temp->nume, nume) == 0) {
				NodLS* deleteMe = temp;
				if (prev == NULL) {
					//stergem primul element
					tabela.vector[poz] = temp->next;
				}
				else if (temp->next == NULL) {
					//stergem ultimul element
					prev->next = NULL;
				}
				else {
					prev->next = temp->next;
				}
				temp = temp->next;
			}
			else {
				prev = temp;
				temp = temp->next;
			}
		}
	}
}

void dezalocareLD(NodLD* prim) {
	NodLD* temp = prim;
	while (temp != NULL)
	{
		NodLD* aux = temp->next;
		free(temp->ang.nume);
		free(temp);
		temp = aux;
	}
}

void dezalocareLS(NodLS* prim) {
	NodLS* temp = prim;
	while (temp != NULL)
	{
		NodLS* aux = temp->next;
		free(temp->nume);
		free(temp);
		temp = aux;
	}
}

void dezalocareHash(hashT tabela) {
	for (int i = 0; i < tabela.nrElem; i++)
	{
		if (tabela.vector[i] != NULL) {
			dezalocareLS(tabela.vector[i]);
		}
	}
}

int main() {
	NodLD* prim = NULL;
	NodLD* ultim = NULL;

	FILE* f = fopen("Angajati.txt", "r");
	if (f == NULL) {
		printf("Fisierul nu a fost deschis");
		return -1;
	}

	char buffer[128];
	Angajat ang;
	while (fscanf(f, "%s", ang.id) == 1) 
	{
		fscanf(f, " %[^\n]", buffer);
		ang.nume = (char*)malloc(strlen(buffer) + 1);
		strcpy(ang.nume, buffer);

		fscanf(f, "%d", &ang.salariul);

		//prim = inserare_LD(prim, &ultim, ang);
		prim = inserare_LD_alfabetic(prim, &ultim, ang);

		free(ang.nume);
	}

	printf("\nLista simpal sortata alfabetic din fisier\n");
	printare_ld(prim);

	printf("\nLista simpal sortata alfabetic din fisier invers\n");
	printare_ld_invers(ultim);

	//sterg angajatii cu salarii sub 5000 si stochez numele intr-o lista simpal separata
	NodLS* prim_ang5000 = NULL;
	prim = sterge_ang_5000(prim, &ultim, &prim_ang5000);
	printf("\nLista dubla dupa stergerea salariilor mici\n");
	printare_ld(prim);

	printf("\nLista dubla dupa stergerea salariilor mici invers\n");
	printare_ld_invers(ultim);

	//sterg angajatii cu salarii peste 10000 si stochez numele intr-o tabela de variatie
	hashT table;
	table.nrElem = 13;
	table.vector = (NodLS**)malloc(table.nrElem * sizeof(NodLS*));
	for (int i = 0; i < table.nrElem; i++)
	{
		table.vector[i] = NULL;
	}
	
	prim = sterge_angajati_peste_10000(prim, &ultim, table);

	printf("\nLista dubla dupa stergerea salariilor mari\n");
	printare_ld(prim);

	printf("\nLista dubla dupa stergerea salariilor mari invers\n");
	printare_ld_invers(ultim);

	printf("\nTabela de dispersie\n");
	printare_hash(table);

	sterge_nume_hash(table, "John Deel");
	sterge_nume_hash(table, "Maria Popescu"); 
	printf("\nTabela de dispersie dupa stergere\n");
	printare_hash(table);

	dezalocareLD(prim);
	dezalocareLS(prim_ang5000);
	dezalocareHash(table);

	return 0;
}