#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

#define HASHTABLE_SIZE 200

typedef struct 
{
	char iban[25]; // 24 bytes pt iban + 1 byte terminator string
	char moneda[4]; // 3 bytes pt cod moneda + 1 byte terminator string
	char* titular;
	float sold;
} ContBancar;

typedef struct {
	ContBancar* pcb;
	struct NodLS* next;
} NodLS;

typedef struct {
	ContBancar* pcb;
	struct NodLD* next;
	struct NodLD* prev;
} NodLD;

typedef struct {
	NodLD* prim;
	NodLD* ultim;
} ListaDubla;

ListaDubla inserare_sfarsit(ListaDubla lista, ContBancar* pcont)
{
	NodLD* nou = (NodLD*)malloc(sizeof(NodLD));
	nou->pcb = pcont;
	nou->next = NULL;

	if (lista.prim == NULL) {
		//empty list
		nou->prev = NULL;
		lista.prim = lista.ultim = nou;
	}
	else {
		//lista contine cel putin un nod
		nou->prev = lista.ultim;
		lista.ultim->next = nou;
		lista.ultim = nou;
	}

	return lista;
}

int main() {
	FILE* f = fopen("Conturi.txt", "r");
	ListaDubla listaD;
	listaD.prim = listaD.ultim = NULL;	//lista dubla este empty

	NodLS** hashTable = NULL; //variabila pentru gestionarea tabelei de dispersie
	hashTable = (NodLS**)malloc(HASHTABLE_SIZE * sizeof(NodLS*));
	for (unsigned char i = 0; i < HASHTABLE_SIZE; i++) {
		hashTable[i] = NULL;	//lista htabel este empty inainte de inserare 
	}

	char nume_buff[128];
	ContBancar contB;
	fscanf(f, "%s\n", contB.iban);
	while (strlen(contB.iban))
	{
		fscanf(f, "%[^\n]", nume_buff);
		contB.titular = (char*)malloc(strlen(nume_buff) + 1);
		strcpy(contB.titular, nume_buff);

		fscanf(f, "%s", contB.moneda);
		fscanf(f, "%f", &contB.sold);

		ContBancar* pContB = (ContBancar*)malloc(sizeof(ContBancar));
		*pContB = contB;

		//inserare cont Bancar in list dubla

		inserare_tabela_hash(hashTable, HASHTABLE_SIZE, contB);

		listaD = inserare_sfarsit(listaD, pContB);

		contB.titular = NULL; // pentru a elimina partajarea dintre ultimul nod inserat si contB
		contB.iban[0] = 0;
		fscanf(f, "%s\n", contB.iban);
	}

	//traversare lista dubla in ambele sensuri

	printf("Lista cap -> coada:\n\n");
	NodLD* temp = listaD.prim;
	while (temp) {
		printf("IBAN: %s, Titular: %s\n", temp->pcb->iban, temp->pcb->titular);
		temp = temp->next;
	}

	printf("\nLista coada -> cap\n\n");
	temp = listaD.ultim;
	while (temp) {
		printf("IBAN: %s, Titular: %s\n", temp->pcb->iban, temp->pcb->titular);
		temp = temp->prev;
	}

	listaD.ultim = NULL;

	fclose(f);

	//HASH TABLE

	return 0;
}