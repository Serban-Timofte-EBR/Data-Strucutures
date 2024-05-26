#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <malloc.h>
#include <string.h>

typedef struct {
	char* denumireExamen;
	char* numeStudent;
	int semestru;
	float nota;
	int anUniversitar;
	int sala;
} Evaluare;

typedef struct {
	Evaluare* inf;
	struct nodLS* next;
} nodLS;

typedef struct {
	nodLS** vector;
	int nrElem;
} hashT;

typedef struct {
	int pozTabela;
	char elementCluster;
	int nrStudentiPromovati;
} elementVector;

typedef struct {
	Evaluare** eval;
	int nrElem;
} MatriceEvaluari; 

int functieHash(hashT tabela, char* denumire) {
	return denumire[0] % tabela.nrElem;
}

void inserareLS(nodLS** prim, Evaluare eval) {
	nodLS* nou = (nodLS*)malloc(sizeof(nodLS));
	nou->inf = (Evaluare*)malloc(sizeof(Evaluare));
	nou->inf->anUniversitar = eval.anUniversitar;
	nou->inf->nota = eval.nota;
	nou->inf->sala = eval.sala;
	nou->inf->semestru = eval.semestru;
	nou->inf->denumireExamen = (char*)malloc(strlen(eval.denumireExamen) + 1);
	strcpy(nou->inf->denumireExamen, eval.denumireExamen);
	nou->inf->numeStudent = (char*)malloc(strlen(eval.numeStudent) + 1);
	strcpy(nou->inf->numeStudent, eval.numeStudent);
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

void inserareHash(hashT tabela, Evaluare eval) {
	int poz = functieHash(tabela, eval.denumireExamen);
	inserareLS(&tabela.vector[poz], eval);
}

void afisareLS(nodLS* prim) {
	nodLS* temp = prim;
	while (temp != NULL)
	{
		printf("\tAn: %d, Semestru: %d, Sala: %d, Nota: %5.2f, Examen: %s, Student: %s\n",
			temp->inf->anUniversitar, temp->inf->semestru, temp->inf->sala, temp->inf->nota,
			temp->inf->denumireExamen, temp->inf->numeStudent);
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

int counterMateriiAn2(hashT tabela, int an) {
	int counter = 0;
	for (int i = 0; i < tabela.nrElem; i++) {
		if (tabela.vector[i] != NULL) {
			nodLS* temp = tabela.vector[i];
			while (temp != NULL)
			{
				if (temp->inf->anUniversitar == an) { counter++; }
				temp = temp->next;
			}
		}
	} 
	return counter;
}

Evaluare* vectorEvaluariAn2(hashT tabela, int* nrElemVector, int an) {
	*nrElemVector = counterMateriiAn2(tabela, an);
	Evaluare* vector = (Evaluare*)malloc(*nrElemVector * sizeof(Evaluare));
	int indexVector = 0;

	for (int i = 0; i < tabela.nrElem; i++) {
		if (tabela.vector[i] != NULL) {
			nodLS* temp = tabela.vector[i];
			while (temp != NULL)
			{
				if (temp->inf->anUniversitar == an) {
					vector[indexVector].anUniversitar = temp->inf->anUniversitar;
					vector[indexVector].nota = temp->inf->nota;
					vector[indexVector].sala = temp->inf->sala;
					vector[indexVector].semestru = temp->inf->semestru;
					vector[indexVector].denumireExamen = (char*)malloc(strlen(temp->inf->denumireExamen) + 1);
					strcpy(vector[indexVector].denumireExamen, temp->inf->denumireExamen);
					vector[indexVector].numeStudent = (char*)malloc(strlen(temp->inf->numeStudent) + 1);
					strcpy(vector[indexVector].numeStudent, temp->inf->numeStudent);
					indexVector++;
				}
				temp = temp->next;
			}
		}
	}

	return vector;
}

int counterPozTabela(hashT tabela) {
	int counter = 0;
	for (int i = 0; i < tabela.nrElem; i++) {
		if (tabela.vector[i] != NULL) {
			counter++;
		}
	}
	return counter;
}

elementVector* vectorTabStudProm(hashT tabela, int* nrElem) {
	*nrElem = counterPozTabela(tabela);
	elementVector* vector = (elementVector*)malloc(*nrElem * sizeof(elementVector));
	int indexVector = 0;

	for (int i = 0; i < tabela.nrElem; i++) {
		if (tabela.vector[i] != NULL) {
			vector[indexVector].pozTabela = i;
			vector[indexVector].elementCluster = tabela.vector[i]->inf->denumireExamen[0];
			
			int counter = 0;
			nodLS* temp = tabela.vector[i];
			while (temp != NULL)
			{
				if (temp->inf->nota >= 5) {
					counter++;
				}
				temp = temp->next;
			}
			vector[indexVector].nrStudentiPromovati = counter;
			indexVector++;
		}
	}
}

void afisareMatriceEvaluari(MatriceEvaluari* matrice) {
	for (int i = 0; i < 2; i++) {
		printf("Semestrul %d:\n", i + 1);
		for (int j = 0; j < matrice[i].nrElem; j++) {
			printf("\tAn: %d, Semestru: %d, Sala: %d, Nota: %5.2f, Examen: %s, Student: %s\n",
				matrice[i].eval[j]->anUniversitar, matrice[i].eval[j]->semestru,
				matrice[i].eval[j]->sala, matrice[i].eval[j]->nota,
				matrice[i].eval[j]->denumireExamen, matrice[i].eval[j]->numeStudent);
		}
	}
}

MatriceEvaluari* matriceaEvaluarilorPeSemestre(hashT tabela) {
	MatriceEvaluari* matrice = (MatriceEvaluari*)malloc(2 * sizeof(MatriceEvaluari));
	for (int i = 0; i < 2; i++) {
		matrice[i].eval = NULL;
		matrice[i].nrElem = 0;
	}

	for (int i = 0; i < tabela.nrElem; i++) {
		if (tabela.vector[i] != NULL) {
			nodLS* temp = tabela.vector[i];
			while (temp != NULL)
			{
				int index = temp->inf->semestru - 1;
				matrice[index].nrElem++;
				temp = temp->next;
			}
		}
	}

	for (int i = 0; i < 2; i++) {
		matrice[i].eval = (Evaluare**)malloc(matrice[i].nrElem * sizeof(Evaluare*));
		matrice[i].nrElem = 0;
	}

	for (int i = 0; i < tabela.nrElem; i++) {
		if (tabela.vector[i] != NULL) {
			nodLS* temp = tabela.vector[i];
			while (temp != NULL)
			{
				int index = temp->inf->semestru - 1;
				matrice[index].nrElem++;
				matrice[index].eval[matrice[index].nrElem - 1] = temp->inf;
				temp = temp->next;
			}
		}
	}

	return matrice;
}

void dezalocareLS(nodLS* prim) {
	nodLS* temp = prim;
	while (temp != NULL) {
		nodLS* aux = temp->next;
		free(temp->inf->denumireExamen);
		free(temp->inf->numeStudent);
		free(temp->inf);
		free(temp);
		temp = aux;
	}
}

void dezalocareHash(hashT tabela) {
	for (int i = 0; i < tabela.nrElem; i++) {
		if (tabela.vector[i] != NULL) {
			dezalocareLS(tabela.vector[i]);
		}
	}
}

void dezalocareVector(Evaluare* vector, int nrElem) {
	for (int i = 0; i < nrElem; i++) {
		free(vector[i].denumireExamen);
		free(vector[i].numeStudent);
	}
	free(vector);
}

void dezalocareMatrice(MatriceEvaluari* matrice) {
	for (int i = 0; i < 2; i++) {
		free(matrice[i].eval);
	}
	free(matrice);
}

int main() {
	hashT tabela;
	tabela.nrElem = 23;
	tabela.vector = (nodLS**)malloc(tabela.nrElem * sizeof(nodLS*));
	for (int i = 0; i < tabela.nrElem; i++) {
		tabela.vector[i] = NULL;
	}

	FILE* f = fopen("fisier.txt", "r");
	if (f == NULL) {
		printf("Fisierul nu a fost deschis\n");
		return -1;
	}

	int nrEvaluari;
	fscanf(f, "%d", &nrEvaluari);

	Evaluare eval;
	char buffer[128];

	for (int i = 0; i < nrEvaluari; i++) {
		fscanf(f, " %[^\n]", buffer);
		eval.denumireExamen = (char*)malloc(strlen(buffer) + 1);
		strcpy(eval.denumireExamen, buffer);

		fscanf(f, " %[^\n]", buffer);
		eval.numeStudent = (char*)malloc(strlen(buffer) + 1);
		strcpy(eval.numeStudent, buffer);

		fscanf(f, "%d", &eval.semestru);
		fscanf(f, "%f", &eval.nota);
		fscanf(f, "%d", &eval.anUniversitar);
		fscanf(f, "%d", &eval.sala);

		inserareHash(tabela, eval);

		free(eval.denumireExamen);
		free(eval.numeStudent);
	}
	fclose(f);

	printf("Tabela din fisier:\n");
	afisareHash(tabela);

	// Vector cu toate evaluarile din anul 2
	int nrElem = 0;
	Evaluare* vector = vectorEvaluariAn2(tabela, &nrElem, 2);
	printf("\nVectorul:\n");
	for (int i = 0; i < nrElem; i++) {
		printf("\tAn: %d, Semestru: %d, Sala: %d, Nota: %5.2f, Examen: %s, Student: %s\n",
			vector[i].anUniversitar, vector[i].semestru, vector[i].sala, vector[i].nota,
			vector[i].denumireExamen, vector[i].numeStudent);
	}

	printf("\nVectorul are dimensiunea %d\n", nrElem);

	// Vector cu numarul de examene promovate pe pozitie din tabela de dispersie [indexTabela, cluster, nrExamene]
	int nrElem2 = 0;
	elementVector* vectorTabelaStudProm = vectorTabStudProm(tabela, &nrElem2);
	printf("\nVectorul 2:\n");
	for (int i = 0; i < nrElem2; i++) {
		printf("Pozitie Tabela: %d, Cheie: %c, Nr. Stud. Promovati: %d\n",
			vectorTabelaStudProm[i].pozTabela, vectorTabelaStudProm[i].elementCluster, vectorTabelaStudProm[i].nrStudentiPromovati);
	}

	MatriceEvaluari* matrice = matriceaEvaluarilorPeSemestre(tabela);
	printf("\nMatricea de evaluari:\n");
	afisareMatriceEvaluari(matrice);

	dezalocareVector(vector, nrElem);
	dezalocareHash(tabela);
	dezalocareMatrice(matrice);

	return 0;
}