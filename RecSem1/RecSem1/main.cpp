#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <malloc.h>
#include <string>


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

int main() {
	produs* vp = nullptr;
	int nr;
	printf("Numarul de elemente din vector: ");
	scanf("%d", &nr);

	vp = (produs*)malloc(nr * sizeof(produs));

	citire_vector(vp, nr);
	afisare_vector(vp, nr);

	printf("\nValoarea totala a vectorului: %f", valoare_totala(vp, nr));

	dezalocare_vector(vp, nr);

	return 0;
}