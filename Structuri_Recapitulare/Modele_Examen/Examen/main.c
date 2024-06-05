#define _CRT_SECURE_NO_WARNINGS
#include  <stdio.h>
#include <malloc.h>
#include <string.h>

typedef struct {
	int cod;
	char* nume;
	char* facultate;
	float medie;
	int salariul;
} Candidat;

typedef struct {
	Candidat inf;
	struct nodLS* next;
} nodLS;

typedef struct {
	nodLS** vector;
	int nrElem;
} hashT;

typedef struct {
	Candidat* vector;
	int nrElem;
} heap;

int functieHash(hashT tabela, char* facultate) {
	return facultate[0] % tabela.nrElem;
}

void inserareLS(nodLS** prim, Candidat cand) {
	nodLS* nou = (nodLS*)malloc(sizeof(nodLS));
	nou->inf.cod = cand.cod;
	nou->inf.medie = cand.medie;
	nou->inf.salariul = cand.salariul;
	nou->inf.facultate = (char*)malloc(strlen(cand.facultate) + 1);
	strcpy(nou->inf.facultate, cand.facultate);
	nou->inf.nume = (char*)malloc(strlen(cand.nume) + 1);
	strcpy(nou->inf.nume, cand.nume);
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

void inserareHash(hashT tabela, Candidat cand) {
	int poz = functieHash(tabela, cand.facultate);
	inserareLS(&tabela.vector[poz], cand);
}

void afisareLS(nodLS* prim) {
	nodLS* temp = prim;
	while (temp != NULL) {
		printf("\tCod: %d, Medie: %5.2f, Salariul: %d, Facultatea: %s, Nume: %s\n",
			temp->inf.cod, temp->inf.medie, temp->inf.salariul, temp->inf.facultate, temp->inf.nume);
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

float medieMaxls(nodLS* prim, char* facultate) {
	float max = 0.0;
	nodLS* temp = prim;
	while (temp != NULL) {
		if (strcmp(temp->inf.facultate, facultate) == 0) {
			if (max < temp->inf.medie) {
				max = temp->inf.medie;
			}
		}
		temp = temp->next;
	}
	return max;
}

float medieMax(hashT tabela, char* facultate) {
	int poz = functieHash(tabela, facultate);
	float max = 0.0;

	if (tabela.vector[poz] != NULL) {
		max = medieMaxls(tabela.vector[poz], facultate);
	}

	return max;
}

void actualizareSalariu(hashT tabela, int cod, int salariulNou) {
	for (int i = 0; i < tabela.nrElem; i++) {
		if (tabela.vector[i] != NULL) {
			nodLS* temp = tabela.vector[i];
			while (temp != NULL) {
				if (temp->inf.cod == cod) {
					temp->inf.salariul = salariulNou;
				}
				temp = temp->next;
			}
		}
	}
}

void stergereCandidat(hashT tabela, char* nume) {
	for (int i = 0; i < tabela.nrElem; i++) {
		if (tabela.vector[i] != NULL) {
			nodLS* temp = tabela.vector[i];
			nodLS* prev = NULL;
			while (temp != NULL) {
				if (strcmp(temp->inf.nume, nume) == 0) {
					nodLS* deleteMe = temp;
					if (temp == tabela.vector[i]) {
						tabela.vector[i] = temp->next;
					}
					else {
						prev->next = temp->next;
					}
					temp = temp->next;
					free(deleteMe->inf.nume);
					free(deleteMe->inf.facultate);
					free(deleteMe);
				}
				else {
					prev = temp;
					temp = temp->next;
				}
			}
		}
	}
}

int counterFacultati(hashT tabela, char* facultate) {
	int poz = functieHash(tabela, facultate);
	int counter = 0;

	if (tabela.vector[poz] != NULL) {
		nodLS* temp = tabela.vector[poz];
		while (temp != NULL) {
			if (strcmp(temp->inf.facultate, facultate) == 0) {
				counter++;
			}
			temp = temp->next;
		}
	}

	return counter;
}

void filtareHEap(heap h, int index) {
	int indexRad = index;
	int indexS = 2 * index + 1;
	int indexD = 2 * index + 2;

	if (indexS < h.nrElem && h.vector[indexS].medie > h.vector[indexRad].medie) {
		indexRad = indexS;
	}

	if (indexD < h.nrElem && h.vector[indexD].medie > h.vector[indexRad].medie) {
		indexRad = indexD;
	}

	if (index != indexRad) {
		Candidat aux = h.vector[index];
		h.vector[index] = h.vector[indexRad];
		h.vector[indexRad] = aux;

		filtareHEap(h, indexRad);
	}
}

heap inserareHeap(heap h, Candidat cand) {
	Candidat* vectorNou = (Candidat*)malloc((h.nrElem + 1) * sizeof(Candidat));
	for (int i = 0; i < h.nrElem; i++) {
		vectorNou[i] = h.vector[i];
	}

	vectorNou[h.nrElem].cod = cand.cod;
	vectorNou[h.nrElem].medie = cand.medie;
	vectorNou[h.nrElem].salariul = cand.salariul;
	vectorNou[h.nrElem].facultate = (char*)malloc(strlen(cand.facultate) + 1);
	strcpy(vectorNou[h.nrElem].facultate, cand.facultate);
	vectorNou[h.nrElem].nume = (char*)malloc(strlen(cand.nume) + 1);
	strcpy(vectorNou[h.nrElem].nume, cand.nume);
	h.nrElem++;

	free(h.vector);
	h.vector = vectorNou;

	for (int i = (h.nrElem - 1) / 2; i >= 0; i--) {
		filtareHEap(h, i);
	}

	return h;
}

heap hash_heap(hashT tabela, char* facultate) {
	heap h;
	h.nrElem = 0;
	h.vector = (Candidat*)malloc(sizeof(Candidat));

	int nrElem = counterFacultati(tabela, facultate);
	if (nrElem > 0) {
		int poz = functieHash(tabela, facultate);

		if (tabela.vector[poz] != NULL) {
			nodLS* temp = tabela.vector[poz];
			while (temp != NULL) {
				if (strcmp(temp->inf.facultate, facultate) == 0) {
					h = inserareHeap(h, temp->inf);
				}
				temp = temp->next;
			}
		}

		return h;
	}
	else {
		free(h.vector);
		return h;
	}
}

void afisareHeap(heap h) {
	for (int i = 0; i < h.nrElem; i++) {
		printf("Cod: %d, Medie: %5.2f, Salariul: %d, Facultatea: %s, Nume: %s\n",
			h.vector[i].cod, h.vector[i].medie, h.vector[i].salariul, h.vector[i].facultate, h.vector[i].nume);
	}
}

int main() {
	hashT tabela;
	tabela.nrElem = 23;
	tabela.vector = (nodLS**)malloc(tabela.nrElem * sizeof(nodLS*));
	for (int i = 0; i < tabela.nrElem; i++) {
		tabela.vector[i] = NULL;
	}

	FILE* f = fopen("fisier.txt", "r");

	Candidat cand;
	char buffer[128];

	while (fscanf(f, "%d", &cand.cod) == 1) {
		fscanf(f, " %[^\n]", buffer);
		cand.nume = (char*)malloc(strlen(buffer) + 1);
		strcpy(cand.nume, buffer);

		fscanf(f, " %[^\n]", buffer);
		cand.facultate = (char*)malloc(strlen(buffer) + 1);
		strcpy(cand.facultate, buffer);

		fscanf(f, "%f", &cand.medie);
		fscanf(f, "%d", &cand.salariul);
		
		/*printf("Cod: %d, Medie: %5.2f, Salariul: %d, Facultatea: %s, Nume: %s\n",
			cand.cod, cand.medie, cand.salariul, cand.facultate, cand.nume);*/
		inserareHash(tabela, cand);

		free(cand.facultate);
		free(cand.nume);
	}
	fclose(f);

	printf("Tabela de dispersie din fisier:\n");
	afisareHash(tabela);

	float medieMaxC2 = medieMax(tabela, "Informatica Economica");
	if (medieMaxC2 != 0) {
		printf("\nMedia maxima din facultatea ceruta este: %5.2f\n", medieMaxC2);
	}
	else {
		printf("\nNu sunt candidati de la facultatea ceruta\n");
	}

	medieMaxC2 = medieMax(tabela, "Informatica si Programare");
	if (medieMaxC2 != 0) {
		printf("\nMedia maxima din facultatea ceruta este: %5.2f\n", medieMaxC2);
	}
	else {
		printf("\nNu sunt candidati de la facultatea ceruta\n");
	}

	medieMaxC2 = medieMax(tabela, "Harvard University");
	if (medieMaxC2 != 0) {
		printf("\nMedia maxima din facultatea ceruta este: %5.2f\n", medieMaxC2);
	}
	else {
		printf("\nNu sunt candidati de la facultatea ceruta\n");
	}

	medieMaxC2 = medieMax(tabela, "Electricitate si Telecomunicatii");
	if (medieMaxC2 != 0) {
		printf("\nMedia maxima din facultatea ceruta este: %5.2f\n", medieMaxC2);
	}
	else {
		printf("\nNu sunt candidati de la facultatea ceruta\n");
	}

	medieMaxC2 = medieMax(tabela, "Relatii Internationale");
	if (medieMaxC2 != 0) {
		printf("\nMedia maxima din facultatea ceruta este: %5.2f\n", medieMaxC2);
	}
	else {
		printf("\nNu sunt candidati de la facultatea ceruta\n");
	}

	actualizareSalariu(tabela, 45, 10000);
	actualizareSalariu(tabela, 66, 10000);
	actualizareSalariu(tabela, 9999, 10000);
	printf("\nHash table dupa actualizare\n");
	afisareHash(tabela);

	stergereCandidat(tabela, "Gabriel Octavian");
	//stergereCandidat(tabela, "Andrei Popescu");
	//stergereCandidat(tabela, "Maria Andone");
	//stergereCandidat(tabela, "Serban Timofte");
	printf("\nTabela de dispersie dupa sterger:\n");
	afisareHash(tabela);

	int nrFacultati = counterFacultati(tabela, "Informatica Economica");
	printf("\n%d\n", nrFacultati);

	heap h = hash_heap(tabela, "Informatica Economica");
	printf("\nStructura HEAP:\n");
	afisareHeap(h);

	return 0;
}