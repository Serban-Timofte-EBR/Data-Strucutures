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

typedef struct NodLS {
	ContBancar* pcb;
	struct NodLS* next;
} NodLS;

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

ListaDubla interschimbare_adiacente(ListaDubla lista, int i) {
	if (i < 0 || lista.prim == NULL) { return lista; }

	NodLD* curr = lista.prim;
	int pos = 0;
	while (curr != NULL && pos < i - 1) {
		curr = curr->next;
		pos++;
	}

	if (curr == NULL || curr->next == NULL) { return lista; }

	NodLD* nextN = curr->next;
	NodLD* prevN = curr->prev;
	NodLD* nextNnext = nextN->next;

	// daca curr nu este primul nod
	if (prevN != NULL) {	
		prevN->next = nextN;
	}
	else {
		lista.prim = nextN;
	}
	
	nextN->prev = prevN;
	nextN->next = curr;
	
	curr->prev = nextN;
	curr->next = nextNnext;

	// daca nextN nu este ultimul nod
	if (nextNnext != NULL) {
		nextNnext->prev = curr;
	}
	else {
		lista.ultim = curr;
	}

	return lista;
}

ListaDubla interschimbare_oarecare(ListaDubla lista, int i, int j) {
	// ordonare indexuri
	if (i > j) {
		int temp = i;
		i = j;
		j = temp;
	}

	if (lista.prim == NULL || i < 0 || j < 0 || i == j) { return lista; }

	NodLD* nod_i = NULL;
	NodLD* nod_j = NULL;
	int pos = 1;

	// parcurgerea listei
	NodLD* curr = lista.prim;
	while (curr != NULL) {
		if (pos == i) { nod_i = curr; }
		else if (pos == j) { nod_j = curr; }
		curr = curr->next;
		pos++;
	}

	if (nod_i == NULL || nod_j == NULL) {
		return lista;
	}

	if (j - i == 1) { interschimbare_adiacente(lista, i); }
	else {
		// avem nod_i si nod_j valizi
		NodLD* nod_i_prev = nod_i->prev;
		NodLD* nod_i_next = nod_i->next;
		NodLD* nod_j_prev = nod_j->prev;
		NodLD* nod_j_next = nod_j->next;

		nod_i->next = nod_j_next;
		nod_i->prev = nod_j_prev;
		nod_j->next = nod_i_next;
		nod_j->prev = nod_i_prev;
		if (nod_i_prev != NULL) nod_i_prev->next = nod_j;
		if (nod_i_next != NULL) nod_i_next->prev = nod_j;
		if (nod_j_prev != NULL) nod_j_prev->next = nod_i;
		if (nod_j_next != NULL) nod_j_next->prev = nod_i;

		if (i == 0) lista.prim = nod_j;
		if (j == pos - 1) lista.ultim = nod_i;

		return lista;
	}
}

void sortare_lista(ListaDubla* lista) {
	if (lista->prim == NULL || lista->prim->next == NULL) {
		return;
	}

	int swapped = 1;

	while (swapped) {
		swapped = 0;
		NodLD* current = lista->prim;

		while (current != NULL && current->next != NULL) {
			// Compar soldurile nodurilor adiacente
			if (current->pcb->sold > current->next->pcb->sold) {
				// Interschimbare noduri - de modificat cu interschimbare 
				ContBancar* temp = current->pcb;
				current->pcb = current->next->pcb;
				current->next->pcb = temp;

				swapped = 1;
			}

			// Avansăm
			current = current->next;
		}
	}
}


//extragere noduri din lista dubla pentru un titular specificat
	//ibanurile se salveaza intr-o lista simpla separata 

NodLS* extrage_noduri(ListaDubla* lista, char* titular_cautat) {
	if (lista == NULL || titular_cautat == NULL) {
		return NULL;
	}

	NodLS* lista_simpla = NULL; 
	NodLD* current = lista->prim; 

	while (current != NULL) {
		if (strcmp(current->pcb->titular, titular_cautat) == 0) {
	
			NodLS* nou = (NodLS*)malloc(sizeof(NodLS));
			nou->pcb = current->pcb; 
			nou->next = lista_simpla; 
			lista_simpla = nou;

			// Eliminăm nodul din lista dublă
			if (current->prev != NULL) {
				current->prev->next = current->next;
			}
			else {
				lista->prim = current->next;
			}
			if (current->next != NULL) {
				current->next->prev = current->prev;
			}
		

			NodLD* temp = current;
			current = current->next; 
			free(temp); 
		}
		else {
			current = current->next;
		}
	}

	return lista_simpla; 
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

	printf("\nInternschimbare adiacente nodul 3 cu 4\n");
	listaD = interschimbare_adiacente(listaD, 3);
	temp = listaD.prim;
	while (temp) {
		printf("IBAN: %s, Titular: %s\n", temp->pcb->iban, temp->pcb->titular);
		temp = temp->next;
	}

	printf("\nInternschimbare oarecare nodul 2 cu 5\n");
	listaD = interschimbare_oarecare(listaD, 2, 5);
	temp = listaD.prim;
	while (temp) {
		printf("IBAN: %s, Titular: %s\n", temp->pcb->iban, temp->pcb->titular);
		temp = temp->next;
	}

	printf("\nSortarea listei\n");
	sortare_lista(&listaD);
	temp = listaD.prim;
	while (temp) {
		printf("IBAN: %s, Titular: %s, Sold: %f\n", 
			temp->pcb->iban, temp->pcb->titular, temp->pcb->sold);
		temp = temp->next;
	}


	printf("\nSeminar 8\n");

	printf("Lista dubla dupa eliminare:\n");
	NodLS* lista_extrasa = extrage_noduri(&listaD, "Radulescu Ana-Maria");
	temp = listaD.prim;
	while (temp) {
		printf("IBAN: %s, Titular: %s, Sold: %f\n",
			temp->pcb->iban, temp->pcb->titular, temp->pcb->sold);
		temp = temp->next;
	}

	printf("Conturile extrase pentru  Radulescu Ana-Maria:\n");
	NodLS* curent = lista_extrasa;
	while (curent != NULL) {
		printf("IBAN: %s, Titular: %s, Sold: %.2f\n",
			curent->pcb->iban, curent->pcb->titular, curent->pcb->sold);
		curent = curent->next;
	}

	while (lista_extrasa != NULL) {
		NodLS* temp = lista_extrasa;
		lista_extrasa = lista_extrasa->next;
		free(temp); 
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