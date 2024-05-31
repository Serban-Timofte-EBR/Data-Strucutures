#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <malloc.h>
#include <string.h>

typedef struct {
	int codClient;
	char* furnizor;
	float suma;
	char* dataScadenta;
} Factura;

typedef struct {
	Factura inf;
	struct nodLS* next;
} nodLS;

typedef struct {
	nodLS** vector;
	int nrElem;
} hashT;

typedef struct {
	nodLS* inf;
	struct nodLL* next;
} nodLL;

int functieHash(hashT tabela, char* furnizor) {
	return furnizor[0] % tabela.nrElem;
}

void inserareLS(nodLS** prim, Factura fac) {
	nodLS* nou = (nodLS*)malloc(sizeof(nodLS));
	nou->inf.codClient = fac.codClient;
	nou->inf.suma = fac.suma;
	nou->inf.furnizor = (char*)malloc(strlen(fac.furnizor) + 1);
	strcpy(nou->inf.furnizor, fac.furnizor);
	nou->inf.dataScadenta = (char*)malloc(strlen(fac.dataScadenta) + 1);
	strcpy(nou->inf.dataScadenta, fac.dataScadenta);
	nou->next = NULL;

	if (*prim == NULL) {
		*prim = nou;
	}
	else {
		nodLS* temp = *prim;
		while (temp->next != NULL)
		{
			temp = temp->next;
		}
		temp->next = nou;
	}
}

void inserareHash(hashT tabela, Factura fac) {
	int poz = functieHash(tabela, fac.furnizor);
	inserareLS(&tabela.vector[poz], fac);
}

void afisareLS(nodLS* prim) {
	nodLS* temp = prim;
	while (temp != NULL)
	{
		printf("\tClient: %d, Suma: %5.2f, Data: %s, Furnizor: %s\n",
			temp->inf.codClient, temp->inf.suma, temp->inf.dataScadenta, temp->inf.furnizor);
		temp = temp->next;
	}
}

void afisareHash(hashT tabela) {
	for (int i = 0; i < tabela.nrElem; i++) {
		if (tabela.vector[i] != NULL) {
			printf("Pozitia %d:\n", i);
			afisareLS(tabela.vector[i]);
		}
	}
}

int counterFacturiDataScadentaLS(nodLS* prim, char* data) {
	int counter = 0;

	nodLS* temp = prim;
	while (temp != NULL)
	{
		if (strcmp(temp->inf.dataScadenta, data) == 0) {
			counter++;
		}
		temp = temp->next;
	}
	return counter;
}

int counterFacturiDataScadenta(hashT tabela, char* dataScadenta) {
	int counter = 0;
	for (int i = 0; i < tabela.nrElem; i++) {
		if (tabela.vector[i] != NULL) {
			counter += counterFacturiDataScadentaLS(tabela.vector[i], dataScadenta);
		}
	}
	return counter;
}

nodLL* checkNodLL(nodLL* prim, char* furnizor) {
	nodLL* temp = prim;
	while (temp != NULL)
	{
		if (strcmp(temp->inf->inf.furnizor, furnizor) == 0) {
			return temp;
		}
		temp = temp->next;
	}
	return NULL;
}

void inserareLL(nodLL** prim, nodLS* inf) {
	nodLL* nou = (nodLL*)malloc(sizeof(nodLL));
	nou->inf = inf;
	nou->next = NULL;

	if (*prim == NULL) {
		*prim = nou;
	}
	else {
		nodLL* temp = *prim;
		while (temp->next != NULL)
		{
			temp = temp->next;
		}
		temp->next = nou;
	}
}

nodLL* salvareHashInLL(nodLL* prim, float prag, hashT tabela) {
	for (int i = 0; i < tabela.nrElem; i++) {
		if (tabela.vector[i] != NULL) {
			nodLS* temp = tabela.vector[i];
			while (temp != NULL)
			{
				if (temp->inf.suma > prag) {
					nodLL* nodLLInserare = checkNodLL(prim, temp->inf.furnizor);
					if (nodLLInserare != NULL) {
						inserareLS(&nodLLInserare->inf, temp->inf);
					}
					else {
						nodLS* nodLSCap = NULL;
						inserareLS(&nodLSCap, temp->inf);
						inserareLL(&prim, nodLSCap);
					}
				}
				temp = temp->next;
			}
		}
	}
	return prim;
}

void afisareLL(nodLL* prim) {
	nodLL* temp = prim;
	while (temp != NULL) 
	{
		printf("Furnizor %s:\n", temp->inf->inf.furnizor);
		afisareLS(temp->inf);
		temp = temp->next;
	}
}

float getSumaMaxima(nodLL* prim) {
	float sumaMax = prim->inf->inf.suma;
	nodLL* temp = prim;
	while (temp != NULL)
	{
		nodLS* temp2 = temp->inf;
		while (temp2 != NULL) {
			if (sumaMax < temp2->inf.suma) {
				sumaMax = temp2->inf.suma;
			}
			temp2 = temp2->next;
		}
		temp = temp->next;
	}
	return sumaMax;
}

nodLS* stergereNod(nodLS* prim, float suma) {
	nodLS* temp = prim;
	nodLS* prev = NULL;
	while (temp != NULL)
	{
		if (temp->inf.suma == suma) {
			nodLS* deleteMe = temp;
			if (prev == NULL) {
				prim = prim->next;
			}
			else {
				prev->next = temp->next;
			}
			temp = temp->next;
			free(deleteMe->inf.furnizor);
			free(deleteMe->inf.dataScadenta);
		}
		else {
			prev = temp;
			temp = temp->next;
		}
	}
	return prim;
}

nodLL* stergereSumeMaxime(nodLL* prim) {
	float sumaMax = getSumaMaxima(prim);
	nodLL* temp = prim;
	nodLL* prev = NULL;
	while (temp != NULL)
	{
		temp->inf = stergereNod(temp->inf, sumaMax);
		if (temp->inf == NULL) {
			nodLL* deleteMe = temp;
			if (prev == NULL) {
				prim = prim->next;
			}
			else {
				prev->next = temp->next;
			}
			temp = temp->next;
			free(deleteMe->inf);
			free(deleteMe);
		}
		else {
			prev = temp;
			temp = temp->next;
		}
	}
	return prim;
}

int main() {
	hashT tabela;
	tabela.nrElem = 23;
	tabela.vector = (nodLS**)malloc(tabela.nrElem * sizeof(nodLS*));
	for (int i = 0; i < tabela.nrElem; i++) {
		tabela.vector[i] = NULL;
	}

	FILE* f = fopen("fisier.txt", "r");

	Factura fac;
	char buffer[128];

	while (fscanf(f, "%d", &fac.codClient) == 1)
	{
		fscanf(f, " %[^\n]", buffer);
		fac.furnizor = (char*)malloc(strlen(buffer) + 1);
		strcpy(fac.furnizor, buffer);

		fscanf(f, "%f", &fac.suma);

		fscanf(f, "%s", buffer);
		fac.dataScadenta = (char*)malloc(strlen(buffer) + 1);
		strcpy(fac.dataScadenta, buffer);

		/*printf("Client: %d, Suma: %5.2f, Data: %s, Furnizor: %s\n",
			fac.codClient, fac.suma, fac.dataScadenta, fac.furnizor);*/
		inserareHash(tabela, fac);
		
		free(fac.furnizor);
		free(fac.dataScadenta);
	}
	fclose(f);

	printf("Tabela din fisier:\n");
	afisareHash(tabela);

	// char* dataCautata = "10.03.2024";
	// char* dataCautata = "21.09.2023"; 
	// char* dataCautata = "10.12.2022";
	char* dataCautata = "01.01.2024";
	int nrFacturiData = counterFacturiDataScadenta(tabela, dataCautata);
	printf("\nNumarul facturilor din data cautata este: %d\n", nrFacturiData);

	nodLL* primLL = NULL;
	primLL = salvareHashInLL(primLL, 50.0, tabela);
	printf("\nLista de Liste:\n");
	afisareLL(primLL);

	float sumaMax = getSumaMaxima(primLL);
	printf("\nSuma maxima din lista de liste: %5.2f\n", sumaMax);

	primLL = stergereSumeMaxime(primLL);
	printf("Lista de liste dupa stergeri:\n");
	afisareLL(primLL);

	return 0;
}