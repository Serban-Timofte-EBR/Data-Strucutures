#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>


typedef struct {
	int cod;
	char* den;
	float pret;
	float cant;
} produs;

void citire_vector(produs* vp, int nr) {
	char buffer[20];
	for (int i = 0; i < nr; i++) {
		printf("Cod = ");
		scanf("%d", &vp[i].cod);

		printf("Denumire = ");
		scanf("%s", buffer);
		vp[i].den = (char*)malloc((strlen(buffer) + 1) * sizeof(char));
		strcpy(vp[i].den, buffer);

		printf("Pret = ");
		scanf("%f", &vp[i].pret);

		printf("Cantitate = ");
		scanf("%f", &vp[i].cant);
	}
}

void afisare_vector(produs* vp, int nr) {
	for (int i = 0; i < nr; i++) {
		printf("\nCod = %d \t Denumire = %s \t Pret = %f \t Cantitate = %f",
			vp[i].cod, vp[i].den, vp[i].pret, vp[i].cant);
	}
}

float valoare_totala(produs* vp, int nr) {
	float total = 0.0;
	for (int i = 0; i < nr; i++) {
		total += vp[i].cant * vp[i].pret;
	}
	return total;
}

void dezalocare_vector(produs* vp, int nr) {
	for (int i = 0; i < nr; i++) {
		free(vp[i].den);
	}
	free(vp);
}

void afisare4Vectori(int* coduri, char** denumiri, float* preturi, float* cantitati, int nr)
{
	for (int i = 0; i < nr; i++)
		printf("\nCod = %d, Denumire = %s, Pret = %5.2f, Cantitate = %5.2f",
			coduri[i], denumiri[i], preturi[i], cantitati[i]);
}

void citireMatrice(float** mat, char** denumiri, int nr)
{
	char buffer[20];
	for (int i = 0; i < nr; i++)
	{
		printf("Cod=");
		scanf("%f", &mat[i][0]);
		printf("Denumire=");
		scanf("%s", buffer);
		denumiri[i] = (char*)malloc((strlen(buffer) + 1) * sizeof(char));
		strcpy(denumiri[i], buffer);
		printf("Pret=");
		scanf("%f", &mat[i][1]);
		printf("Cantitate=");
		scanf("%f", &mat[i][2]);
	}
}

void afisareMatrice(float** mat, char** denumiri, int nr)
{
	for (int i = 0; i < nr; i++)
		printf("\nCod = %5.2f, Denumire = %s, Pret = %5.2f, Cantitate = %5.2f",
			mat[i][0], denumiri[i], mat[i][1], mat[i][2]);
}

void dezalocareMatrice(float** mat, char** denumiri, int nr)
{
	for (int i = 0; i < nr; i++)
	{
		free(mat[i]);
		free(denumiri[i]);
	}
	free(mat);
	free(denumiri);
}


int main() {
	/*
	produs* vp = nullptr;
	int nr;
	printf("Numarul de elemente din vector: ");
	scanf("%d", &nr);

	vp = (produs*)malloc(nr * sizeof(produs));

	citire_vector(vp, nr);
	afisare_vector(vp, nr);

	printf("\nValoarea totala a vectorului: %f", valoare_totala(vp, nr));

	dezalocare_vector(vp, nr);
	*/

	FILE* f; 
	f = fopen("date.txt", "r");
	
	if (f == NULL) {
		printf("No such file or directory!");
		return -1;
	}

	produs* vpDoc = nullptr;
	int nrDoc;
	fscanf(f, "%d", &nrDoc);

	vpDoc = (produs*)malloc(nrDoc * sizeof(produs));

	char buffer[20];
	for (int i = 0; i < nrDoc; i++) {
		fscanf(f, "%d", &vpDoc[i].cod);
		fscanf(f, "%s", buffer);
		vpDoc[i].den = (char*)malloc((strlen(buffer) + 1) * sizeof(char));
		strcpy(vpDoc[i].den, buffer);
		fscanf(f, "%f", vpDoc[i].pret);
		fscanf(f, "%f", vpDoc[i].cant);
	}

	int nr;
	printf("Numarul de elemente din vector: ");
	scanf("%d", &nr);

	int* coduri = (int*)malloc(nr * sizeof(int));
	char** denumiri = (char**)malloc(nr * sizeof(char*));
	float* preturi = (float*)malloc(nr * sizeof(float));
	float* cantitate = (float*)malloc(nr * sizeof(float));

	char buffer2[20];
	for (int i = 0; i < nr; i++) {
		fscanf(f, "%d", &coduri[i]);
		fscanf(f, "%s", buffer2);
		denumiri[i] = (char*)malloc((strlen(buffer) + 1) * sizeof(char));
		strcpy(denumiri[i], buffer2);
		fscanf(f, "%f", &preturi[i]);
		fscanf(f, "%f", cantitate[i]);
	}

	float** mat = (float**)malloc(nr * sizeof(float*));
	for (int i = 0; i < nr; i++)
		mat[i] = (float*)malloc(3 * sizeof(float));

	citireMatrice(mat, denumiri, nr);
	afisareMatrice(mat, denumiri, nr);
	dezalocareMatrice(mat, denumiri, nr);

	fclose(f); 
	return 0;
}