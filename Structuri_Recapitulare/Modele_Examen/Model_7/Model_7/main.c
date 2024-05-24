#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <malloc.h>
#include <string.h>

typedef struct {
	unsigned int id;
	char* denumireHotel;
	unsigned char nrCamereHotel;
	unsigned char nrCamereRezervare;
	char* numeClient;
	char* perioadaRezevarii;
	float pret;
} Rezervare;

typedef struct {
	Rezervare inf;
	int ocupat;
} elementTabela;

typedef struct {
	elementTabela* vector;
	int nrElem;
} hashT;

int functieHash(hashT tabela, char* denumire) {
	return denumire[0] % tabela.nrElem;
}

void inserareHash(hashT tabela, Rezervare rez) {
	int poz = functieHash(tabela, rez.denumireHotel);
	int index = 0;

	while (tabela.vector[(poz + index) % tabela.nrElem].ocupat == 1)
	{
		index++;
	}

	tabela.vector[(poz + index) % tabela.nrElem].inf.id = rez.id;
	tabela.vector[(poz + index) % tabela.nrElem].inf.nrCamereHotel = rez.nrCamereHotel;
	tabela.vector[(poz + index) % tabela.nrElem].inf.nrCamereRezervare = rez.nrCamereRezervare;
	tabela.vector[(poz + index) % tabela.nrElem].inf.pret = rez.pret;
	tabela.vector[(poz + index) % tabela.nrElem].inf.denumireHotel = (char*)malloc(strlen(rez.denumireHotel) + 1);
	strcpy(tabela.vector[(poz + index) % tabela.nrElem].inf.denumireHotel, rez.denumireHotel);
	tabela.vector[(poz + index) % tabela.nrElem].inf.numeClient = (char*)malloc(strlen(rez.numeClient) + 1);
	strcpy(tabela.vector[(poz + index) % tabela.nrElem].inf.numeClient, rez.numeClient);
	tabela.vector[(poz + index) % tabela.nrElem].inf.perioadaRezevarii = (char*)malloc(strlen(rez.perioadaRezevarii) + 1);
	strcpy(tabela.vector[(poz + index) % tabela.nrElem].inf.perioadaRezevarii, rez.perioadaRezevarii);
	tabela.vector[(poz + index) % tabela.nrElem].ocupat = 1;
}

float determinareGradDeOcupare(hashT tabela, char* denumire, char* perioada) {
	int nrCamereHotel = 0;
	int camereOcupate = 0;

	int poz = functieHash(tabela, denumire);
	int index = 0;

	while (tabela.vector[(poz + index) % tabela.nrElem].ocupat == 1 && (poz + index) < tabela.nrElem )
	{
		if (strcmp(tabela.vector[(poz + index) % tabela.nrElem].inf.denumireHotel, denumire) == 0 &&
			strcmp(tabela.vector[(poz + index) % tabela.nrElem].inf.perioadaRezevarii, perioada) == 0) {
			nrCamereHotel = tabela.vector[(poz + index) % tabela.nrElem].inf.nrCamereHotel;
			camereOcupate += tabela.vector[(poz + index) % tabela.nrElem].inf.nrCamereRezervare;
		}
		index++;
	}

	return camereOcupate / (float)nrCamereHotel * 100;
}

void afisareHash(hashT tabela) {
	for (int i = 0; i < tabela.nrElem; i++) {
		if (tabela.vector[i].ocupat == 1) {
			printf("Pozitia %d:\n", i);
			printf("\tID: %d, Camere Hotel: %d, Camere Rez: %d, Pret: %5.2f, Perioada: %s, Hotel: %s, Client: %s\n",
				tabela.vector[i].inf.id, tabela.vector[i].inf.nrCamereHotel, tabela.vector[i].inf.nrCamereRezervare,
				tabela.vector[i].inf.pret, tabela.vector[i].inf.perioadaRezevarii, tabela.vector[i].inf.denumireHotel, tabela.vector[i].inf.numeClient);
		}
	}
}

int counterRezervariClient(hashT tabela, char* client) {
	int counter = 0;

	for (int i = 0; i < tabela.nrElem; i++) {
		if (strcmp(tabela.vector[i].inf.numeClient, client) == 0) {
			counter += 1;
		}
	}

	return counter;
}

Rezervare* vectorRezervariClinet(hashT tabela, char* client, Rezervare* vector) {
	int indexElem = 0;

	for (int i = 0; i < tabela.nrElem; i++) {
		if (strcmp(tabela.vector[i].inf.numeClient, client) == 0) {
			vector[indexElem] = tabela.vector[i].inf;
			indexElem++;
		}
	}

	return vector;
}

float calculIncasari(Rezervare* vector, int nrElem, char* perioada, int nrZilePerioada) {
	float valoareaIncasare = 0.0;
	for (int i = 0; i < nrElem; i++) {
		if (strcmp(vector[i].perioadaRezevarii, perioada) == 0) {
			valoareaIncasare += vector[i].nrCamereRezervare * vector[i].pret * nrZilePerioada;
		}
	}
	return valoareaIncasare;
}

void dezalocareHash(hashT tabela) {
	for (int i = 0; i < tabela.nrElem; i++) {
		if (tabela.vector[i].ocupat != 0) {
			free(tabela.vector[i].inf.denumireHotel);
			free(tabela.vector[i].inf.numeClient);
			free(tabela.vector[i].inf.perioadaRezevarii);
		}
	}
	free(tabela.vector);
}

void dezalocareVector(Rezervare* vector) {
	free(vector);
}

int main() {
	FILE* f = fopen("fisier.txt", "r");

	int nrRezervari;
	fscanf(f, "%d", &nrRezervari);

	hashT tabela;
	tabela.nrElem = nrRezervari;
	tabela.vector = (elementTabela*)malloc(tabela.nrElem * sizeof(elementTabela));
	for (int i = 0; i < tabela.nrElem; i++) {
		tabela.vector[i].ocupat = 0;
	}

	Rezervare rez;
	char buffer[128];
	for (int i = 0; i < tabela.nrElem; i++) {
		fscanf(f, "%d", &rez.id);

		fscanf(f, "%s", buffer);
		rez.denumireHotel = (char*)malloc(strlen(buffer) + 1);
		strcpy(rez.denumireHotel, buffer);

		fscanf(f, "%d", &rez.nrCamereHotel);
		fscanf(f, "%d", &rez.nrCamereRezervare);

		fscanf(f, "%s", buffer);
		rez.numeClient = (char*)malloc(strlen(buffer) + 1);
		strcpy(rez.numeClient, buffer);

		fscanf(f, "%s", buffer);
		rez.perioadaRezevarii = (char*)malloc(strlen(buffer) + 1);
		strcpy(rez.perioadaRezevarii, buffer);

		fscanf(f, "%f", &rez.pret);

		inserareHash(tabela, rez);

		free(rez.denumireHotel);
		free(rez.numeClient);
		free(rez.perioadaRezevarii);
	}
	fclose(f);

	printf("Tabela din fisier:\n");
	afisareHash(tabela);

	float gradOcupare = determinareGradDeOcupare(tabela, "Poiana", "V-D");
	printf("\nGradul de ocupare este %5.1f%s\n", gradOcupare, "%");

	int nrRezerveriClient = counterRezervariClient(tabela, "Mihai");
	Rezervare* vectoriRezervariClinet = (Rezervare*)malloc(nrRezerveriClient * sizeof(Rezervare));
	vectoriRezervariClinet = vectorRezervariClinet(tabela, "Mihai", vectoriRezervariClinet);
	printf("\nVectorul de rezervari:\n");
	for (int i = 0; i < nrRezerveriClient; i++) {
		printf("ID: %d, Camere Hotel: %d, Camere Rez: %d, Pret: %5.2f, Perioada: %s, Hotel: %s, Client: %s\n",
			vectoriRezervariClinet[i].id, vectoriRezervariClinet[i].nrCamereHotel, vectoriRezervariClinet[i].nrCamereRezervare,
			vectoriRezervariClinet[i].pret, vectoriRezervariClinet[i].perioadaRezevarii, vectoriRezervariClinet[i].denumireHotel,
			vectoriRezervariClinet[i].numeClient);
	}


	float incasariVector = calculIncasari(vectoriRezervariClinet, nrRezerveriClient, "V-D", 2);
	printf("\nValoarea incasarilor pentru perioada ceruta este: %5.2f\n", incasariVector);

	dezalocareHash(tabela);
	dezalocareVector(vectoriRezervariClinet);

	return 0;
}