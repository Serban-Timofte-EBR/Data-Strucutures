#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>

struct ContBancar
{
	char iban[25]; // 24 bytes pt iban + 1 byte terminator string
	char moneda[4]; // 3 bytes pt cod moneda + 1 byte terminator string
	char* titular;
	float sold;
};

struct NodLD
{
	struct ContBancar *pcb;
	struct NodLD* next;
	struct NodLD* prev;
};

typedef struct ContBancar ContBancar;
typedef struct NodLD NodLD;

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

typedef struct {
	char iban[25];
} IBAN;

// salvare conturi bancare cu aceeasi moneda
// conturi bancare sunt stoate in lista 
// salvam in vector IBAN-urile 

IBAN* salveaza_conturi_vector(ListaDubla lista, char* currency, unsigned char* n) {
	*n = 0;
	NodLD* temp = lista.prim;
	while (temp) {
		if (strcmp(temp->pcb->moneda, currency) == 0) {
			*n += 1;
		}
		temp = temp->next;
	}

	IBAN* vector = (IBAN*)malloc((*n) * sizeof(IBAN));
	
	temp = lista.prim;
	unsigned char i = 0;
	while (temp) {
		if (strcmp(temp->pcb->moneda, currency) == 0) {
			strcpy(vector[i].iban, temp->pcb->iban);
			i += 1;
		}
		temp = temp->next;
	}

	return vector;
}

int main()
{
	FILE* f = fopen("Conturi.txt", "r");
	ListaDubla listaD;
	listaD.prim = listaD.ultim = NULL;	//lista dubla este empty

	char nume_buff[128];
	struct ContBancar contB;
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

	//salvare conturi bancare in vector de IBAN-uri
	unsigned char n = 0;
	IBAN* conturi = salveaza_conturi_vector(listaD, "EUR", &n);

	printf("\nConturi bancare in EUR:\n\n");
	for (unsigned char i = 0; i < n; i++) {
		printf("%s\n", conturi[i].iban);
	}

	//dezalocare vector de conturi
	free(conturi);

	conturi = NULL;

	//dezalocare lista
	while (listaD.prim) {
		temp = listaD.prim;
		listaD.prim = listaD.prim->next;
		if (listaD.prim != NULL)
			listaD.prim->prev = NULL;

		free(temp->pcb->titular);
		free(temp->pcb);
		free(temp);
	}

	return 0;
}