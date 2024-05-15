#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <malloc.h>
#include <string.h>

typedef struct {
	int zi;
	int luna;
	int an;
} dateReceptionare;

typedef struct {
	dateReceptionare data;
	int prioritate;
	char* aplicatie;
	float durata;
} notificare;

typedef struct {
	notificare inf;
	struct nodStiva* next;
} nodStiva;

typedef struct {
	notificare inf;
	struct nodCoada* next;
} nodCoada;

typedef struct {
	notificare inf;
	struct nodLS* next;
} nodLS;

typedef struct {
	nodLS* inf;
	struct nodLL* next;
} nodLL;

void push(nodStiva** varf, notificare notif) {
	nodStiva* nou = (nodStiva*)malloc(sizeof(nodStiva));
	nou->inf.data.zi = notif.data.zi;
	nou->inf.data.luna = notif.data.luna;
	nou->inf.data.an = notif.data.an;
	nou->inf.prioritate = notif.prioritate;
	nou->inf.aplicatie = (char*)malloc(strlen(notif.aplicatie) + 1);
	strcpy(nou->inf.aplicatie, notif.aplicatie);
	nou->inf.durata = notif.durata;
	nou->next = NULL;

	if (*varf == NULL) {
		*varf = nou;
	}
	else {
		nou->next = *varf;
		*varf = nou;
	}
}

int pop(nodStiva** varf, notificare* notif) {
	if (*varf == NULL) {
		return -1;
	}
	else {
		(*notif).data.zi = (*varf)->inf.data.zi;
		(*notif).data.luna = (*varf)->inf.data.luna;
		(*notif).data.an = (*varf)->inf.data.an;
		(*notif).prioritate = (*varf)->inf.prioritate;
		(*notif).aplicatie = (char*)malloc(strlen((*varf)->inf.aplicatie) + 1);
		strcpy((*notif).aplicatie, (*varf)->inf.aplicatie);
		(*notif).durata = (*varf)->inf.durata;

		nodStiva* temp = *varf;
		*varf = (*varf)->next;

		free(temp->inf.aplicatie);
		free(temp);

		return 1;
	}
}

void salvare_vector(nodStiva** varf, notificare* vector, int* nrElem) {
	notificare notif;
	while (pop(varf, &notif) == 1)
	{
		if (notif.prioritate == 1) {
			vector[*nrElem].data = notif.data;
			vector[*nrElem].durata = notif.durata;
			vector[*nrElem].prioritate = notif.prioritate;
			vector[*nrElem].aplicatie = (char*)malloc(strlen(notif.aplicatie) + 1);
			strcpy(vector[*nrElem].aplicatie, notif.aplicatie);
			(*nrElem)++;
		}
	}
}

void printare_stiva(nodStiva* varf) {
	nodStiva* temp = varf;
	while (temp != NULL)
	{
		printf("Data: %d %d %d \t Prioritate: %d \t Durata: %f \t Aplicatie: %s\n",
			temp->inf.data.zi, temp->inf.data.luna, temp->inf.data.an, temp->inf.prioritate, temp->inf.durata, temp->inf.aplicatie);
		temp = temp->next;
	}
}

void put(nodCoada** prim, nodCoada** ultim, notificare notif) {
	nodCoada* nou = (nodCoada*)malloc(sizeof(nodCoada));
	nou->inf.data = notif.data;
	nou->inf.prioritate = notif.prioritate;
	nou->inf.durata = notif.prioritate;
	nou->inf.aplicatie = (char*)malloc(strlen(notif.aplicatie) + 1);
	strcpy(nou->inf.aplicatie, notif.aplicatie);
	nou->next = NULL;

	if (*prim == NULL) {
		*prim = nou;
		*ultim = nou;
	}
	else {
		(*ultim)->next = nou;
		*ultim = nou;
	}
}

int get(nodCoada** prim, nodCoada** ultim, notificare* notif) {
	if (*prim == NULL) {
		return -1;
	}
	else {
		(*notif).data = (*prim)->inf.data;
		(*notif).durata = (*prim)->inf.durata;
		(*notif).prioritate = (*prim)->inf.prioritate;
		(*notif).aplicatie = (char*)malloc(strlen((*prim)->inf.aplicatie) + 1);
		strcpy((*notif).aplicatie, (*prim)->inf.aplicatie);

		nodCoada* temp = *prim;
		(*prim) = (*prim)->next;

		free(temp->inf.aplicatie);
		free(temp);

		return 1;
	}
}

void printare_coada(nodCoada* prim) {
	nodCoada* temp = prim;
	while (temp != NULL)
	{
		printf("Data: %d %d %d \t Prioritate: %d \t Durata: %f \t Aplicatie: %s\n",
			temp->inf.data.zi, temp->inf.data.luna, temp->inf.data.an, temp->inf.prioritate, temp->inf.durata, temp->inf.aplicatie);
		temp = temp->next;
	}
}

//cautam in lista de liste daca avem un nod cu prioritatea ceruta
nodLL* check_nod_ll(nodLL* prim, int prioritate) {
	nodLL* temp = prim;
	while (temp != NULL)
	{
		if (temp->inf->inf.prioritate == prioritate) {
			return temp;
		}
		temp = temp->next;
	}
	return NULL;
}

nodLS* inseraraLS(nodLS* prim, notificare notif) {
	nodLS* nou = (nodLS*)malloc(sizeof(nodLS));
	nou->inf.data = notif.data;
	nou->inf.prioritate = notif.prioritate;
	nou->inf.durata = notif.prioritate;
	nou->inf.aplicatie = (char*)malloc(strlen(notif.aplicatie) + 1);
	strcpy(nou->inf.aplicatie, notif.aplicatie);
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

nodLL* inserareLL(nodLL* cap, nodLS* inf) {
	nodLL* nouLL = (nodLL*)malloc(sizeof(nodLL));
	nouLL->inf = inf;
	nouLL->next = NULL;

	if (cap == NULL) {
		cap = nouLL;
	}
	else {
		nodLL* temp = cap;
		while (temp->next != NULL)
		{
			temp = temp->next;
		}
		temp->next = nouLL;
	}
	return cap;
}

void salvare_coada_lista_liste(nodCoada** prim, nodCoada** ultim, nodLL** cap) {
	notificare notif;
	while (get(prim, ultim, &notif) == 1)
	{
		nodLL* nodPrioritatedinNotif = check_nod_ll(*cap, notif.prioritate);
		if (nodPrioritatedinNotif != NULL) {
			//inserez intr-o sublista existenta
			inseraraLS(nodPrioritatedinNotif->inf, notif);
		}
		else {
			//creez un nou nod in lista de liste
			nodLS* nouLS = inseraraLS(NULL, notif);
			*cap = inserareLL(*cap, nouLS);
		}
	}
}

void printare_lista_liste(nodLL* cap) {
	nodLL* temp = cap;
	while (temp != NULL)
	{
		printf("Lista cu prioritate: \n", temp->inf->inf.prioritate);
		nodLS* temp2 = temp->inf;
		while (temp2 != NULL)
		{
			printf("\tData: %d %d %d \t Prioritate: %d \t Durata: %f \t Aplicatie: %s\n",
				temp2->inf.data.zi, temp2->inf.data.luna, temp2->inf.data.an, temp2->inf.prioritate, temp2->inf.durata, temp2->inf.aplicatie);
			temp2 = temp2->next;
		}
		temp = temp->next;
	}
}

int main() {
	int nrNotificari;
	nodStiva* varf = NULL;

	nodCoada* prim = NULL;
	nodCoada* ultim = NULL;
	
	FILE* f = fopen("fisier.txt", "r");
	if (f == NULL) {
		printf("Fisierul nu s-a deschis");
		return -1;
	}

	char buffer[128];
	notificare notif;

	fscanf(f, "%d", &nrNotificari);
	for (int i = 0; i < nrNotificari; i++) {
		fscanf(f, "%d", &notif.data.zi);
		fscanf(f, "%d", &notif.data.luna);
		fscanf(f, "%d", &notif.data.an);

		fscanf(f, "%d", &notif.prioritate);

		fscanf(f, "%s", buffer);
		notif.aplicatie = (char*)malloc(strlen(buffer) + 1);
		strcpy(notif.aplicatie, buffer);

		fscanf(f, "%f", &notif.durata);

		push(&varf, notif);
		put(&prim, &ultim, notif);

		free(notif.aplicatie);
	}

	printf("Stiva citita din fisier:\n");
	printare_stiva(varf);

	//Salvam intr-un vector notificarile cu prioritate 1
	notificare* notificariPrioritate1 = (notificare*)malloc(nrNotificari * sizeof(notificare));
	int nrElemVector = 0;
	salvare_vector(&varf, notificariPrioritate1, &nrElemVector);
	printf("\nElementele din vector sunt: \n");
	for (int i = 0; i < nrElemVector; i++) {
		printf("Data: %d %d %d \t Prioritate: %d \t Durata: %f \t Aplicatie: %s\n",
			notificariPrioritate1[i].data.zi, notificariPrioritate1[i].data.luna, notificariPrioritate1[i].data.zi,
			notificariPrioritate1[i].prioritate, notificariPrioritate1[i].durata, notificariPrioritate1[i].aplicatie);
	}

	printf("\nStiva dupa salvarea in vector: \n");
	printare_stiva(varf);


	printf("\nCoada citita din fisier: \n");
	printare_coada(prim);

	//transformam coada intr-o lista de liste pe baza de prioritate
	nodLL* capLL = NULL;
	salvare_coada_lista_liste(&prim, &ultim, &capLL);

	printf("\nLista de liste pe baza de prioritate: \n");
	printare_lista_liste(capLL);
	return 0;
}