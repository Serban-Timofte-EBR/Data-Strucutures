#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <malloc.h>
#include <string.h>

typedef struct {
	int codRaport;
	int codPacient;
	int nrAnalize;
	float* preturiAnalize;
	char* data;
} RaportAnalizeMedicale;

typedef struct {
	RaportAnalizeMedicale inf;
	struct nodLS* next;
} nodLS;

typedef struct {
	nodLS** inf;
	int nrElem;
} hashT;

typedef struct {
	int codPacient;
	float valoare;
	char* data;
	struct nodLS2* next;
} nodLS2;

int functieHash(hashT tabela, char* data) {
	return data[0] % tabela.nrElem;
}

void inserareLS(nodLS** prim, RaportAnalizeMedicale rap) {
	nodLS* nou = (nodLS*)malloc(sizeof(nodLS));
	nou->inf.codPacient = rap.codPacient;
	nou->inf.codRaport = rap.codRaport;
	nou->inf.nrAnalize = rap.nrAnalize;
	nou->inf.preturiAnalize = (float*)malloc(rap.nrAnalize * sizeof(float));
	for (int i = 0; i < rap.nrAnalize; i++) {
		nou->inf.preturiAnalize[i] = rap.preturiAnalize[i];
	}
	nou->inf.data = (char*)malloc(strlen(rap.data) + 1);
	strcpy(nou->inf.data, rap.data);
	nou->next = NULL;

	if (*prim == NULL) {
		*prim = nou;
	}
	else {
		nodLS* temp = *prim;
		while (temp->next != NULL) {
			temp = temp->next;
		}
		temp->next = nou;
	}
}

void inserareHash(hashT tabela, RaportAnalizeMedicale rap) {
	int poz = functieHash(tabela, rap.data);
	inserareLS(&tabela.inf[poz], rap);
}

void afisareLS(nodLS* prim) {
	nodLS* temp = prim;
	while (temp != NULL) {
		printf("\tData: %s, Raport: %d, Clinet: %d, Nr. Analize: %d, Preturi: ",
			temp->inf.data, temp->inf.codRaport, temp->inf.codPacient,
			temp->inf.nrAnalize);
		for(int i=0; i<temp->inf.nrAnalize; i++) {
			printf("%5.2f ", temp->inf.preturiAnalize[i]);
		}
		printf("\n");
		temp = temp->next;
	}
}

void afisareHash(hashT tabela) {
	for (int i = 0; i < tabela.nrElem; i++) {
		if (tabela.inf[i] != NULL) {
			printf("Pozitia %d:\n", i);
			afisareLS(tabela.inf[i]);
		}
	}
}

int counterAnalizePerioadaLS(nodLS* prim, char* start, char* end) {
	int counter = 0;

	nodLS* temp = prim;
	while (temp != NULL) {
		if (strcmp(temp->inf.data, start) >= 0 && strcmp(temp->inf.data, end) <= 0) {
			counter += temp->inf.nrAnalize;
		}
		temp = temp->next;
	}

	return counter;
}

int counterAnalizePerioada(hashT tabela, char* start, char* end) {
	int counter = -1;
	for (int i = 0; i < tabela.nrElem; i++) {
		if (tabela.inf[i] != NULL) {
			counter += counterAnalizePerioadaLS(tabela.inf[i], start, end);
		}
	}
	return counter;
}

void inserareLS2(nodLS2** prim, int codPac, float val, char* data) {
	nodLS2* nou = (nodLS2*)malloc(sizeof(nodLS2));
	nou->codPacient = codPac;
	nou->valoare = val;
	nou->data = (char*)malloc(strlen(data) + 1);
	strcpy(nou->data, data);
	nou->next = NULL;

	if (*prim == NULL) {
		*prim = nou;
	}
	else {
		nodLS2* temp = *prim;
		while (temp->next != NULL) {
			temp = temp->next;
		}
		temp->next = nou;
	}
}

nodLS2* tabela_ls(hashT tabela, char* data) {
	nodLS2* prim = NULL;

	int poz = functieHash(tabela, data);
	if (tabela.inf[poz] != NULL) {
		nodLS* temp = tabela.inf[poz];
		while (temp != NULL) {
			if (strcmp(temp->inf.data, data) == 0) {
				float val = 0.0;
				for (int i = 0; i < temp->inf.nrAnalize; i++) {
					val += temp->inf.preturiAnalize[i];
				}
				inserareLS2(&prim, temp->inf.codPacient, val, temp->inf.data);
			}
			temp = temp->next;
		}
	}
	return prim;
}

void afisareLS2(nodLS2* prim) {
	nodLS2* temp = prim;
	while (temp != NULL) {
		printf("Cod Pacient: %d, Valoare: %5.2f, Data: %s\n",
			temp->codPacient, temp->valoare, temp->data);
		temp = temp->next;
	}
}

int counterPacientiLS2(nodLS2* prim) {
	int counter = 0;

	nodLS2* temp = prim;
	while (temp != NULL)
	{
		counter++;
		temp = temp->next;
	}

	return counter;
}

int counterRapoarteData(hashT tabela, char* data) {
	int couter = 0;

	int poz = functieHash(tabela, data);
	if (tabela.inf[poz] != NULL) {
		nodLS* temp = tabela.inf[poz];
		while (temp != NULL) {
			if (strcmp(temp->inf.data, data) == 0) {
				couter++;
			}
			temp = temp->next;
		}
	}

	return couter;
}

void dezalocareLS2(nodLS2* prim) {
	while (prim != NULL) {
		nodLS2* aux = prim->next;
		free(prim->data);
		free(prim);
		prim = aux;
	}
}

void dezalocareLS(nodLS* prim) {
	while (prim != NULL) {
		nodLS* aux = prim->next;
		free(prim->inf.data);
		free(prim);
		prim = aux;
	}
}

void dezalocareHash(hashT tabela) {
	for (int i = 0; i < tabela.nrElem; i++) {
		if (tabela.inf[i] != NULL) {
			dezalocareLS(tabela.inf[i]);
		}
	}
}

int main() {
	hashT tabela;
	tabela.nrElem = 23;
	tabela.inf = (nodLS**)malloc(tabela.nrElem * sizeof(nodLS*));
	for (int i = 0; i < tabela.nrElem; i++) {
		tabela.inf[i] = NULL;
	}

	FILE* f = fopen("fisier.txt", "r");
	
	RaportAnalizeMedicale rap;
	char buffer[128];

	while (fscanf(f, "%d", &rap.codRaport) == 1)
	{
		fscanf(f, "%d", &rap.codPacient);
		fscanf(f, "%d", &rap.nrAnalize);

		rap.preturiAnalize = (float*)malloc(rap.nrAnalize * sizeof(float));
		for (int i = 0; i < rap.nrAnalize; i++) {
			fscanf(f, "%f", &rap.preturiAnalize[i]);
		}

		fscanf(f, "%s", buffer);
		rap.data = (char*)malloc(strlen(buffer) + 1);
		strcpy(rap.data, buffer);

		/*printf("Data: %s, Raport: %d, Clinet: %d, Nr. Analize: %d\n", 
			rap.data, rap.codRaport, rap.codPacient, rap.nrAnalize);
		for (int i = 0; i < rap.nrAnalize; i++) {
			printf("\t%5.2f ", rap.preturiAnalize[i]);
		}
		printf("\n");*/
		inserareHash(tabela, rap);

		free(rap.data);
		free(rap.preturiAnalize);
	}
	fclose(f);

	printf("Tabela de dispersie din fisier:\n");
	afisareHash(tabela);

	int counterAnalize = counterAnalizePerioada(tabela, "21.10.2023", "31.10.2023");
	printf("\nIn perioada ceruta s-au efectuat %d analize.\n", counterAnalize);

	nodLS2* prim = tabela_ls(tabela, "21.10.2023");
	printf("\nLista simpla:\n");
	afisareLS2(prim);

	int nrPacienti = counterPacientiLS2(prim);
	printf("\nSunt %d pacienti in lista simpla\n", nrPacienti);

	int nrRapoarteMedicale = counterRapoarteData(tabela, "12.10.2023");
	printf("\nLa data ceruta sunt %d rapoarte\n", nrRapoarteMedicale);

	dezalocareLS2(prim);
	dezalocareHash(tabela);

	return 0;
}