#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <malloc.h>
#include <string.h>

typedef struct {
	char* tip;
	int etaj;
	char* client;
	float pret;
	int numar;
} Camera;

typedef struct {
	Camera inf;
	struct nodArbBin* stanga;
	struct nodArbBin* dreapta;
} nodArbBin;

typedef struct {
	Camera inf;
	struct nodLD* next;
	struct nodLD* prev;
} nodLD;

nodArbBin* inserareArbore(nodArbBin* rad, Camera cam) {
	if (rad != NULL) {
		if (cam.numar < rad->inf.numar) {
			rad->stanga = inserareArbore(rad->stanga, cam);
			return rad;
		} 
		else if (cam.numar > rad->inf.numar) {
			rad->dreapta = inserareArbore(rad->dreapta, cam);
			return rad;
		}
		else {
			return rad;
		}
	}
	else {
		nodArbBin* nou = (nodArbBin*)malloc(sizeof(nodArbBin));
		nou->inf.etaj = cam.etaj;
		nou->inf.numar = cam.numar;
		nou->inf.pret = cam.pret;
		nou->inf.client = (char*)malloc(strlen(cam.client) + 1);
		strcpy(nou->inf.client, cam.client);
		nou->inf.tip = (char*)malloc(strlen(cam.tip) + 1);
		strcpy(nou->inf.tip, cam.tip);
		nou->stanga = NULL;
		nou->dreapta = NULL;
		return nou;
	}
}

void inordine(nodArbBin* rad) {
	if (rad != NULL) {
		inordine(rad->stanga);
		printf("Numar: %d, Pret: %5.2f, Etaj: %d, Client: %s, Tip: %s\n",
			rad->inf.numar, rad->inf.pret, rad->inf.etaj, rad->inf.client, rad->inf.tip);
		inordine(rad->dreapta);
	}
}

void counterEtaje(nodArbBin* rad, int* etajMax) {
	if (rad != NULL) {
		if (rad->inf.etaj > *etajMax) {
			*etajMax = rad->inf.etaj;
		}
		counterEtaje(rad->stanga, etajMax);
		counterEtaje(rad->dreapta, etajMax);
	}
}

void calculVector(nodArbBin* rad, int* vector) {
	if (rad != NULL) {
		vector[rad->inf.etaj - 1]++;
		calculVector(rad->stanga, vector);
		calculVector(rad->dreapta, vector);
	}
}

int* nrCamereEtaj(nodArbBin* rad, int* nrElem) {
	*nrElem = 0;
	counterEtaje(rad, nrElem);

	int* vector = (int*)malloc(*nrElem * sizeof(int));
	for (int i = 0; i < *nrElem; i++) {
		vector[i] = 0;
	}

	calculVector(rad, vector);

	return vector;
}

void inserareLD(nodLD** prim, nodLD** ultim, Camera cam) {
	nodLD* nou = (nodLD*)malloc(sizeof(nodLD));
	nou->inf.etaj = cam.etaj;
	nou->inf.pret = cam.pret;
	nou->inf.numar = cam.numar;
	nou->inf.client = (char*)malloc(strlen(cam.client) + 1);
	strcpy(nou->inf.client, cam.client);
	nou->inf.tip = (char*)malloc(strlen(cam.tip) + 1);
	strcpy(nou->inf.tip, cam.tip);
	nou->next = NULL;
	nou->prev = NULL;

	if (*prim == NULL) {
		*prim = nou;
		*ultim = nou;
	}
	else {
		(*ultim)->next = nou;
		nou->prev = *ultim;
		*ultim = nou;
	}
}

void arbore_list(nodArbBin* rad, nodArbBin** prim, nodLD** ultim) {
	if (rad != NULL) {
		if (rad->inf.pret > 100) {
			inserareLD(prim, ultim, rad->inf);
		}
		arbore_list(rad->stanga, prim, ultim);
		arbore_list(rad->dreapta, prim, ultim);
	}
}

void afisareLDCapCoada(nodLD* prim) {
	nodLD* temp = prim;
	while (temp != NULL)
	{
		printf("Numar: %d, Pret: %5.2f, Etaj: %d, Client: %s, Tip: %s\n",
			temp->inf.numar, temp->inf.pret, temp->inf.etaj, temp->inf.client, temp->inf.tip);
		temp = temp->next;
	}
}

void afisareLDCoadaCap(nodLD* ultim) {
	nodLD* temp = ultim;
	while (temp != NULL)
	{
		printf("Numar: %d, Pret: %5.2f, Etaj: %d, Client: %s, Tip: %s\n",
			temp->inf.numar, temp->inf.pret, temp->inf.etaj, temp->inf.client, temp->inf.tip);
		temp = temp->prev;
	}
}

void inserareLDDupaNr(nodLD** prim, nodLD** ultim, Camera cam, int nrCamIndex) {
	nodLD* nou = (nodLD*)malloc(sizeof(nodLD));
	nou->inf.etaj = cam.etaj;
	nou->inf.pret = cam.pret;
	nou->inf.numar = cam.numar;
	nou->inf.client = (char*)malloc(strlen(cam.client) + 1);
	strcpy(nou->inf.client, cam.client);
	nou->inf.tip = (char*)malloc(strlen(cam.tip) + 1);
	strcpy(nou->inf.tip, cam.tip);
	nou->next = NULL;
	nou->prev = NULL;

	if (*prim == NULL) {
		*prim = nou;
		*ultim = nou;
	}
	else {
		nodLD* temp = *prim;
		while (temp != NULL && temp->inf.numar != nrCamIndex)
		{
			temp = temp->next;
		}

		if (temp == *ultim) {
			(*ultim)->next = nou;
			nou->prev = *ultim;
			*ultim = nou;
		}
		else if (temp->inf.numar == nrCamIndex) {
			nodLD* urm = temp->next;
			temp->next = nou;
			nou->prev = temp;
			nou->next = urm;
			urm->prev = nou;
		}
	}
}

void dezalocareArbore(nodArbBin* rad) {
	if (rad != NULL) {
		dezalocareArbore(rad->stanga);
		dezalocareArbore(rad->dreapta);
		free(rad->inf.client);
		free(rad->inf.tip);
		free(rad);
	}
}

void dezalocareLD(nodLD* prim) {
	while (prim != NULL)
	{
		nodLD* aux = prim->next;
		free(prim->inf.client);
		free(prim->inf.tip);
		free(prim);
		prim = aux;
	}
}

int main() {
	nodArbBin* rad = NULL;

	FILE* f = fopen("fisier.txt", "r");

	Camera cam;
	char buffer[128];

	while (fscanf(f, " %[^\n]", buffer) == 1)
	{
		cam.tip = (char*)malloc(strlen(buffer) + 1);
		strcpy(cam.tip, buffer);

		fscanf(f, "%d", &cam.etaj);

		fscanf(f, " %[^\n]", buffer);
		cam.client = (char*)malloc(strlen(buffer) + 1);
		strcpy(cam.client, buffer);

		fscanf(f, "%f", &cam.pret);
		fscanf(f, "%d", &cam.numar);

		/*printf("Numar: %d, Pret: %5.2f, Etaj: %d, Client: %s, Tip: %s\n", 
			cam.numar, cam.pret, cam.etaj, cam.client, cam.tip);*/
		rad = inserareArbore(rad, cam);

		free(cam.client);
		free(cam.tip);
	}
	fclose(f);

	printf("Arborele din fisier:\n");
	inordine(rad);

	//int nrEtaje = 0;
	//counterEtaje(rad, &nrEtaje);
	//printf("\n%d\n", nrEtaje);

	int nrElem = 0;
	int* nrCamerePerEtaj = nrCamereEtaj(rad, &nrElem);
	printf("\n");
	for (int i = 0; i < nrElem; i++) {
		printf("Etaj %d - %d camere\n", i + 1, nrCamerePerEtaj[i]);
	}

	nodLD* prim = NULL;
	nodLD* ultim = NULL;

	arbore_list(rad, &prim, &ultim);

	printf("\nLista dubla Cap -> Coada\n");
	afisareLDCapCoada(prim);

	printf("\nLista dubla Coada -> Cap:\n");
	afisareLDCoadaCap(ultim);


	Camera camIns;
	camIns.client = "Gigel Popescu";
	camIns.tip = "Apartament cu vedere la mare";
	camIns.etaj = 4;
	camIns.pret = 321.45;
	camIns.numar = 777;

	inserareLDDupaNr(&prim, &ultim, camIns, 567);
	inserareLDDupaNr(&prim, &ultim, camIns, 999);
	inserareLDDupaNr(&prim, &ultim, camIns, 200);

	printf("\nLista dupa modificare:\n");
	afisareLDCapCoada(prim);
	printf("\nCoada Cap:\n");
	afisareLDCoadaCap(ultim);

	dezalocareArbore(rad);
	dezalocareLD(prim);

	return 0;
}