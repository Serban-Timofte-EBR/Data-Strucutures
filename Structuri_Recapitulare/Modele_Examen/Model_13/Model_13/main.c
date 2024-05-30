#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <malloc.h>
#include <string.h>

#define APEL_TELEFONIC 1
#define APLICATIE_MOBILA 2

typedef struct {
	char* numeClient;
	char numarTelefon[11];
	char* locatie;
	int canal;
	float durataAsteptare;
	char* status;
} Comanda;

typedef struct {
	Comanda inf;
	struct nodStiva* next;
} nodStiva;

typedef struct {
	Comanda inf;
	struct nodLS* next;
} nodLS;

typedef struct {
	char* client;
	nodLS* inf;
	struct nodLL* next;
} nodLL;

nodLS* inserareLSDescrescator(nodLS* prim, Comanda com) {
	nodLS* nou = (nodLS*)malloc(sizeof(nodLS));
	nou->inf.canal = com.canal;
	nou->inf.durataAsteptare = com.durataAsteptare;
	nou->inf.numeClient = (char*)malloc(strlen(com.numeClient) + 1);
	strcpy(nou->inf.numeClient, com.numeClient);
	strcpy(nou->inf.numarTelefon, com.numarTelefon);
	nou->inf.locatie = (char*)malloc(strlen(com.locatie) + 1);
	strcpy(nou->inf.locatie, com.locatie);
	nou->inf.status = (char*)malloc(strlen(com.status) + 1);
	strcpy(nou->inf.status, com.status);
	nou->next = NULL;

	if (prim == NULL) {
		prim = nou;
	}
	else {
		nodLS* temp = prim;
		nodLS* prev = NULL;
		while (temp->next != NULL && temp->inf.durataAsteptare < com.durataAsteptare)
		{
			prev = temp;
			temp = temp->next;
		}

		if (temp->next == NULL) {
			temp->next = nou;
		}
		else if (prev == NULL) {
			nou->next = prim;
			prim = nou;
		}
		else {
			prev->next = nou;
			nou->next = temp;
		}
	}

	return prim;
}

nodLS* inserareLS(nodLS* prim, Comanda com) {
	if (strcmp(com.status, "Activa") != 0) {
		return prim;
	}

	nodLS* nou = (nodLS*)malloc(sizeof(nodLS));
	nou->inf.canal = com.canal;
	nou->inf.durataAsteptare = com.durataAsteptare;
	nou->inf.numeClient = (char*)malloc(strlen(com.numeClient) + 1);
	strcpy(nou->inf.numeClient, com.numeClient);
	strcpy(nou->inf.numarTelefon, com.numarTelefon);
	nou->inf.locatie = (char*)malloc(strlen(com.locatie) + 1);
	strcpy(nou->inf.locatie, com.locatie);
	nou->inf.status = (char*)malloc(strlen(com.status) + 1);
	strcpy(nou->inf.status, com.status);
	nou->next = NULL;

	if (prim == NULL) {
		prim = nou;
	}
	else {
		nodLS* temp = prim;
		while (temp->next != NULL)
		{
			temp = temp->next;
		}
		temp->next = nou;
	}
	return prim;
}

nodStiva* push(nodStiva* prim, Comanda com) {
	nodStiva* nou = (nodStiva*)malloc(sizeof(nodStiva));
	nou->inf.canal = com.canal;
	nou->inf.durataAsteptare = com.durataAsteptare;
	nou->inf.numeClient = (char*)malloc(strlen(com.numeClient) + 1);
	strcpy(nou->inf.numeClient, com.numeClient);
	strcpy(nou->inf.numarTelefon, com.numarTelefon);
	nou->inf.locatie = (char*)malloc(strlen(com.locatie) + 1);
	strcpy(nou->inf.locatie, com.locatie);
	nou->inf.status = (char*)malloc(strlen(com.status) + 1);
	strcpy(nou->inf.status, com.status);
	
	nou->next = prim;
	prim = nou;

	return prim;
}

int pop(nodStiva** cap, Comanda* out) {
	if (*cap == NULL) {
		return 0;
	}

	*out = (*cap)->inf;
	*cap = (*cap)->next;
	return 1;
}

nodStiva* creareaStiva(nodLS* prim, nodStiva* cap) {
	nodLS* temp = prim;
	while (temp != NULL) 
	{
		cap = push(cap, temp->inf);
		temp = temp->next;
	}
	return cap;
}

void afisareStiva(nodStiva* cap) {
	nodStiva* temp = cap;
	while (temp != NULL)
	{
		printf("Durata: %5.2f, Canal: %d, Locatie: %s, Status: %s, Telefon: %s, Client: %s\n",
			temp->inf.durataAsteptare, temp->inf.canal, temp->inf.locatie, temp->inf.status, temp->inf.numarTelefon, temp->inf.numeClient);
		temp = temp->next;
	}
}

nodLL* checkNodLL(nodLL* cap, char* client) {
	nodLL* temp = cap;
	while (temp!=NULL)
	{
		if (strcmp(temp->client, client) == 0) {
			return temp;
		}
		temp = temp->next;
	}
	return NULL;
}

nodLL* inserareLL(nodLL* cap, nodLS* inf, char* client) {
	nodLL* nou = (nodLL*)malloc(sizeof(nodLL));
	nou->inf = inf;
	nou->next = NULL;
	nou->client = (char*)malloc(strlen(client) + 1);
	strcpy(nou->client, client);

	if (cap == NULL) {
		cap = nou;
	}
	else {
		nodLL* temp = cap;
		while (temp->next != NULL)
		{
			temp = temp->next;
		}
		temp->next = nou;
	}
	return cap;
}

nodLL* listaPrintare(nodLL* prim, nodStiva** cap) {
	Comanda out;
	while (pop(cap, &out) == 1)
	{
		if (strcmp(out.status, "Activa") == 0) {
			nodLL* nodListaP = checkNodLL(prim, out.numeClient);
			if (nodListaP != NULL) {
				nodListaP->inf = inserareLS(nodListaP->inf, out);
			}
			else {
				nodLS* nouCap = inserareLSDescrescator(NULL, out);
				prim = inserareLL(prim, nouCap, out.numeClient);
			}
		}
	}
	return prim;
}

void afisareLS(nodStiva* cap) {
	nodLS* temp = cap;
	while (temp != NULL)
	{
		printf("\tDurata: %5.2f, Canal: %d, Locatie: %s, Status: %s, Telefon: %s, Client: %s\n",
			temp->inf.durataAsteptare, temp->inf.canal, temp->inf.locatie, temp->inf.status, temp->inf.numarTelefon, temp->inf.numeClient);
		temp = temp->next;
	}
}

void afisareLL(nodLL* prim) {
	nodLL* temp = prim;
	while (temp != NULL)
	{
		printf("Client %s:\n", temp->client);
		afisareLS(temp->inf);
		temp = temp->next;
	}
}

void printareRaport(nodStiva** cap) {
	nodLL* primLL = NULL;
	primLL = listaPrintare(primLL, cap);
	printf("\nLista din fisier:\n");

	FILE* f = fopen("Raport.txt", "w");

	nodLL* temp = primLL;
	while (temp != NULL)
	{
		fprintf(f, "%s:\n", temp->client);
		nodLS* tempLS = temp->inf;
		while (tempLS != NULL)
		{
			fprintf(f, "\tDurata: %5.2f, Canal: %d, Locatie: %s, Status: %s, Telefon: %s, Client: %s\n",
				tempLS->inf.durataAsteptare, tempLS->inf.canal, tempLS->inf.locatie, tempLS->inf.status, tempLS->inf.numarTelefon, tempLS->inf.numeClient);
			tempLS = tempLS->next;
		}
		temp = temp->next;
	}
}

int main() {
	nodLS* primListaInit = NULL;

	FILE* f = fopen("fisierAPEL.txt", "r");
	char buffer[128];
	Comanda com;

	while (fscanf(f, " %[^\n]", buffer) == 1)
	{
		com.numeClient = (char*)malloc(strlen(buffer) + 1);
		strcpy(com.numeClient, buffer);
		fscanf(f, "%s", com.numarTelefon);

		fscanf(f, " %[^\n]", buffer);
		com.locatie = (char*)malloc(strlen(buffer) + 1);
		strcpy(com.locatie, buffer);

		fscanf(f, "%d", &com.canal);
		fscanf(f, "%f", &com.durataAsteptare);

		fscanf(f, "%s", buffer);
		com.status = (char*)malloc(strlen(buffer) + 1);
		strcpy(com.status, buffer);

		/*printf("Durata: %5.2f, Canal: %d, Locatie: %s, Status: %s, Telefon: %s, Client: %s\n",
			com.durataAsteptare, com.canal, com.locatie, com.status, com.numarTelefon, com.numeClient);*/
		primListaInit = inserareLSDescrescator(primListaInit, com);

		free(com.numeClient);
		free(com.locatie);
		free(com.status);
	}
	fclose(f);

	FILE* f2 = fopen("fisierAPLICATIE.txt", "r");
	while (fscanf(f2, " %[^\n]", buffer) == 1)
	{
		com.numeClient = (char*)malloc(strlen(buffer) + 1);
		strcpy(com.numeClient, buffer);
		fscanf(f2, "%s", com.numarTelefon);

		fscanf(f2, " %[^\n]", buffer);
		com.locatie = (char*)malloc(strlen(buffer) + 1);
		strcpy(com.locatie, buffer);

		fscanf(f2, "%d", &com.canal);
		fscanf(f2, "%f", &com.durataAsteptare);

		fscanf(f, "%s", buffer);
		com.status = (char*)malloc(strlen(buffer) + 1);
		strcpy(com.status, buffer);

		/*printf("Durata: %5.2f, Canal: %d, Locatie: %s, Status: %s, Telefon: %s, Client: %s\n",
			com.durataAsteptare, com.canal, com.locatie, com.status, com.numarTelefon, com.numeClient);*/
		primListaInit = inserareLSDescrescator(primListaInit, com);

		free(com.numeClient);
		free(com.locatie);
		free(com.status);
	}
	fclose(f2);

	nodStiva* cap = NULL;
	cap = creareaStiva(primListaInit, cap);

	printf("Stiva:\n");
	afisareStiva(cap);

	/*nodLL* primLL = NULL;
	primLL = listaPrintare(primLL, &cap);
	printf("\nLista din fisier:\n");
	afisareLL(primLL);
	printf("\nStiva:\n");
	afisareStiva(cap);*/

	printareRaport(&cap);
	printf("\nStiva:\n");
	afisareStiva(cap);

	cap = creareaStiva(primListaInit, cap);
	printf("\nStiva refacuta:\n");
	afisareStiva(cap);

	Comanda comandaUrgenta;
	int res = pop(&cap, &comandaUrgenta);
	if (res == 1) {
		printf("\nCea mai urgenta comanda din stiva:\n");
		printf("Durata: %5.2f, Canal: %d, Locatie: %s, Status: %s, Telefon: %s, Client: %s\n",
			comandaUrgenta.durataAsteptare, comandaUrgenta.canal, comandaUrgenta.locatie, 
			comandaUrgenta.status, comandaUrgenta.numarTelefon, comandaUrgenta.numeClient);
	}

	return 0;
}