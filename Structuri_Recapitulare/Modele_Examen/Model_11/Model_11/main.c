#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <malloc.h>
#include <string.h>

typedef struct {
	int idProiect;
	char* numeProiect;
	int idBeneficiar;
	float valoareInvestitie;
} Proiect;

typedef struct {
	Proiect inf;
	struct nodLS* next;
} nodLS;

typedef struct {
	nodLS** vector;
	int nrElem;
} hashT;

typedef struct {
	Proiect inf;
	struct nodArbBin* stanga;
	struct nodArbBin* dreapta;
} nodArbBin;

int functieHash(hashT tabela, char* nume) {
	return nume[0] % tabela.nrElem;
}

void inserareLS(nodLS** cap, Proiect pr) {
	nodLS* nou = (nodLS*)malloc(sizeof(nodLS));
	nou->inf.idBeneficiar = pr.idBeneficiar;
	nou->inf.idProiect = pr.idProiect;
	nou->inf.valoareInvestitie = pr.valoareInvestitie;
	nou->inf.numeProiect = (char*)malloc(strlen(pr.numeProiect) + 1);
	strcpy(nou->inf.numeProiect, pr.numeProiect);
	nou->next = NULL;

	if (*cap == NULL) {
		*cap = nou;
	}
	else {
		nodLS* temp = *cap;
		while (temp->next != NULL) {
			temp = temp->next;
		}
		temp->next = nou;
	}
}

void inserareHash(hashT tabela, Proiect pr) {
	int poz = functieHash(tabela, pr.numeProiect);
	inserareLS(&tabela.vector[poz], pr);
}

void afisareLS(nodLS* cap) {
	nodLS* temp = cap;
	while (temp != NULL)
	{
		printf("\tID Proiect: %d, ID Benecifiar: %d, Valoare: %5.2f, Denumire: %s\n",
			temp->inf.idProiect, temp->inf.idBeneficiar, temp->inf.valoareInvestitie, temp->inf.numeProiect);
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

void stergereProiect(hashT tabela, char* denumire) {
	int poz = functieHash(tabela, denumire);

	if (tabela.vector[poz] != NULL) {
		nodLS* temp = tabela.vector[poz];
		nodLS* prev = NULL;
		while (temp != NULL) {
			if (strcmp(temp->inf.numeProiect, denumire) == 0) {
				nodLS* deleteMe = temp;
				if (prev == NULL) {
					tabela.vector[poz] = temp->next;
				}
				else {
					prev->next = temp->next;
				}
				temp = temp->next;
				free(deleteMe->inf.numeProiect);
				free(deleteMe);
			}
			else {
				prev = temp;
				temp = temp->next;
			}
		}
	}
}

Proiect stergereProiectCuSalvare(hashT tabela, char* denumire) {
	int poz = functieHash(tabela, denumire);
	Proiect output;
	if (tabela.vector[poz] != NULL) {
		nodLS* temp = tabela.vector[poz];
		nodLS* prev = NULL;
		while (temp != NULL)
		{
			if (strcmp(temp->inf.numeProiect, denumire) == 0) {
				output.idBeneficiar = temp->inf.idBeneficiar;
				output.idProiect = temp->inf.idProiect;
				output.valoareInvestitie = temp->inf.valoareInvestitie;
				output.numeProiect = (char*)malloc(strlen(temp->inf.numeProiect) + 1);
				strcpy(output.numeProiect, temp->inf.numeProiect);

				nodLS* deleteMe = temp;
				if (prev == NULL) {
					tabela.vector[poz] = temp->next;
				}
				else {
					prev->next = temp->next;
				}
				temp = temp->next;
				free(deleteMe->inf.numeProiect);
				free(deleteMe);
				return output;
			}
			else {
				prev = temp;
				temp = temp->next;
			}
		}
	}
	output.idProiect = -1;
	return output;
}

void modificareCheie(hashT tabela, char* denumireActuala, char* denumireNoua) {
	Proiect pr = stergereProiectCuSalvare(tabela, denumireActuala);
	free(pr.numeProiect);
	pr.numeProiect = (char*)malloc(strlen(denumireNoua) + 1);
	strcpy(pr.numeProiect, denumireNoua);
	inserareHash(tabela, pr);
}

nodArbBin* creareNod(nodArbBin* stanga, nodArbBin* dreapta, Proiect pr) {
	nodArbBin* nou = (nodArbBin*)malloc(sizeof(nodArbBin));
	nou->inf.idBeneficiar = pr.idBeneficiar;
	nou->inf.idProiect = pr.idProiect;
	nou->inf.valoareInvestitie = pr.valoareInvestitie;
	nou->inf.numeProiect = (char*)malloc(strlen(pr.numeProiect) + 1);
	strcpy(nou->inf.numeProiect, pr.numeProiect);

	nou->stanga = stanga;
	nou->dreapta = dreapta;

	return nou;
}

nodArbBin* inserareArbore(nodArbBin* rad, Proiect pr) {
	if (rad != NULL) {
		if (pr.idProiect < rad->inf.idProiect) {
			rad->stanga = inserareArbore(rad->stanga, pr);
			return rad;
		}
		else if (pr.idProiect > rad->inf.idProiect) {
			rad->dreapta = inserareArbore(rad->dreapta, pr);
			return rad;
		}
		else {
			return rad;
		}
	}
	else {
		return creareNod(NULL, NULL, pr);
	}
}

nodArbBin* creareaArboreDinHash(nodArbBin* rad, hashT tabela) {
	for (int i = 0; i < tabela.nrElem; i++) {
		if (tabela.vector[i] != NULL) {
			nodLS* temp = tabela.vector[i];
			while (temp != NULL)
			{
				rad = inserareArbore(rad, temp->inf);
				temp = temp->next;
			}
		}
	}
	return rad;
}

void inordine(nodArbBin* rad) {
	if (rad != NULL) {
		inordine(rad->stanga);
		printf("ID Proiect: %d, ID Beneficiar: %d, Valoare: %5.2f, Denumire: %s\n",
			rad->inf.idProiect, rad->inf.idBeneficiar, rad->inf.valoareInvestitie, rad->inf.numeProiect);
		inordine(rad->dreapta);
	}
}

float valoareProiecteBeneficiar(nodArbBin* rad, int idBeneficiar) {
	float valTotal = 0.0;

	if (rad != NULL) {
		if (rad->inf.idBeneficiar == idBeneficiar) {
			valTotal += rad->inf.valoareInvestitie;
		}

		valTotal += valoareProiecteBeneficiar(rad->stanga, idBeneficiar);
		valTotal += valoareProiecteBeneficiar(rad->dreapta, idBeneficiar);
	}

	return valTotal;
}

int counterFrunze(nodArbBin* rad) {
	int counter = 0;

	if (rad != NULL) {
		if (rad->stanga == NULL && rad->dreapta == NULL) {
			counter++;
		}
		counter += counterFrunze(rad->stanga);
		counter += counterFrunze(rad->dreapta);
	}

	return counter;
}

void vectorFrunze(nodArbBin* rad, int* index, Proiect* vector) {
	if (rad != NULL) {
		if (rad->stanga == NULL && rad->dreapta == NULL) {
			vector[*index] = rad->inf;
			(*index)++;
		}
		vectorFrunze(rad->stanga, index, vector);
		vectorFrunze(rad->dreapta, index, vector);
	}
}

nodArbBin* findMin(nodArbBin* rad) {
	nodArbBin* temp = rad;
	while (temp->stanga != NULL)
	{
		temp = temp->stanga;
	}
	return temp;
}

nodArbBin* stergereNodCheie(nodArbBin* rad, int id) {
	if (rad == NULL) {
		return rad;
	}

	if (id < rad->inf.idProiect) {
		rad->stanga = stergereNodCheie(rad->stanga, id);
	}

	else if (id > rad->inf.idProiect) {
		rad->dreapta = stergereNodCheie(rad->dreapta, id);
	}

	else {
		if (rad->stanga == NULL) {
			nodArbBin* temp = rad->dreapta;
			free(rad->inf.numeProiect);
			free(rad);
			return temp;
		}

		else if (rad->dreapta == NULL) {
			nodArbBin* temp = rad->dreapta;
			free(rad->inf.numeProiect);
			free(rad);
			return temp;
		}

		nodArbBin* temp = findMin(rad->dreapta);
		rad->inf.idBeneficiar = temp->inf.idBeneficiar;
		rad->inf.idProiect = temp->inf.idProiect;
		rad->inf.valoareInvestitie = temp->inf.valoareInvestitie;
		free(rad->inf.numeProiect);
		rad->inf.numeProiect = (char*)malloc(strlen(temp->inf.numeProiect) + 1);
		strcpy(rad->inf.numeProiect, temp->inf.numeProiect);
		rad->dreapta = stergereNodCheie(rad->dreapta, temp->inf.idProiect);
	}
	return rad;
}

nodArbBin* stergereCuSalvare(nodArbBin* rad, int id, Proiect* pr) {
	if (rad == NULL) {
		return rad;
	}

	if (id < rad->inf.idProiect) {
		rad->stanga = stergereCuSalvare(rad->stanga, id, pr);
	}
	else if (id > rad->inf.idProiect) {
		rad->dreapta = stergereCuSalvare(rad->dreapta, id, pr);
	}
	else {
		if (rad->stanga == NULL) {
			if ((*pr).valoareInvestitie == -1.0) {
				(*pr).idBeneficiar = rad->inf.idBeneficiar;
				(*pr).idProiect = rad->inf.idProiect;
				(*pr).valoareInvestitie = rad->inf.valoareInvestitie;
				(*pr).numeProiect = (char*)malloc(strlen(rad->inf.numeProiect) + 1);
				strcpy((*pr).numeProiect, rad->inf.numeProiect);
			}
			nodArbBin* temp = rad->dreapta;
			free(rad->inf.numeProiect);
			free(rad);
			return temp;
		}

		if (rad->dreapta == NULL) {
			if ((*pr).valoareInvestitie == -1.0) {
				(*pr).idBeneficiar = rad->inf.idBeneficiar;
				(*pr).idProiect = rad->inf.idProiect;
				(*pr).valoareInvestitie = rad->inf.valoareInvestitie;
				(*pr).numeProiect = (char*)malloc(strlen(rad->inf.numeProiect) + 1);
				strcpy((*pr).numeProiect, rad->inf.numeProiect);
			}
			nodArbBin* temp = rad->stanga;
			free(rad->inf.numeProiect);
			free(rad);
			return temp;
		}

		nodArbBin* temp = findMin(rad->dreapta);
		(*pr).idBeneficiar = rad->inf.idBeneficiar;
		(*pr).idProiect = rad->inf.idProiect;
		(*pr).valoareInvestitie = rad->inf.valoareInvestitie;
		(*pr).numeProiect = (char*)malloc(strlen(rad->inf.numeProiect) + 1);
		strcpy((*pr).numeProiect, rad->inf.numeProiect);
		
		rad->inf.idBeneficiar = temp->inf.idBeneficiar;
		rad->inf.idProiect = temp->inf.idProiect;
		rad->inf.valoareInvestitie = temp->inf.valoareInvestitie;
		rad->inf.numeProiect = (char*)malloc(strlen(temp->inf.numeProiect) + 1);
		strcpy(rad->inf.numeProiect, temp->inf.numeProiect);

		rad->dreapta = stergereCuSalvare(rad->dreapta, temp->inf.idProiect, pr);
	}
	return rad;
}

nodArbBin* modificareCheieArb(nodArbBin* rad, int idNou, int idVechi) {
	Proiect pr;
	pr.valoareInvestitie = -1.0;
	rad = stergereCuSalvare(rad, idVechi, &pr);
	pr.idProiect = idNou;
	rad = inserareArbore(rad, pr);
	return rad;
}

void dezalocareLS(nodLS* prim) {
	nodLS* temp = prim;
	while (temp != NULL)
	{
		nodLS* aux = temp->next;
		free(temp->inf.numeProiect);
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

void dezalocareVector(Proiect* vector) {
	free(vector);
}

void dezalocareArbore(nodArbBin* rad) {
	if (rad != NULL) {
		dezalocareArbore(rad->stanga);
		dezalocareArbore(rad->dreapta);
		free(rad->inf.numeProiect);
		free(rad);
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

	Proiect pr;
	char buffer[256];

	while (fscanf(f, "%d", &pr.idProiect) == 1)
	{
		fscanf(f, " %[^\n]", buffer);
		pr.numeProiect = (char*)malloc(strlen(buffer) + 1);
		strcpy(pr.numeProiect, buffer);

		fscanf(f, "%d", &pr.idBeneficiar);
		fscanf(f, "%f", &pr.valoareInvestitie);

		/*printf("Proiectul a fost citit: %d \t %s \t %d \t %f\n", pr.idProiect, pr.numeProiect, pr.idBeneficiar, pr.valoareInvestitie);*/
		inserareHash(tabela, pr);

		free(pr.numeProiect);
	}
	fclose(f);

	printf("Tabela din fisier:\n");
	afisareHash(tabela);

	stergereProiect(tabela, "Creare de online chat");
	stergereProiect(tabela, "Dezvoltare Email App");
	printf("\nTabela dupa stergere:\n");
	afisareHash(tabela);

	modificareCheie(tabela, "Software pentru rezervari", "Aplicatie de rezervari");
	printf("\nTabela dupa modificare:\n");
	afisareHash(tabela);

	nodArbBin* rad = NULL;
	rad = creareaArboreDinHash(rad, tabela);
	printf("\nArborele in INORDINE:\n");
	inordine(rad);

	float val = valoareProiecteBeneficiar(rad, 60);
	printf("\nValoarea totala a proiectelor beneficiarului ales este %5.2f\n", val);
	val = 0.0;
	val = valoareProiecteBeneficiar(rad, 10);
	printf("\nValoarea totala a proiectelor beneficiarului ales este %5.2f\n", val);
	val = 0.0;
	val = valoareProiecteBeneficiar(rad, 50);
	printf("\nValoarea totala a proiectelor beneficiarului ales este %5.2f\n", val);
	val = 0.0;
	val = valoareProiecteBeneficiar(rad, 10001);
	printf("\nValoarea totala a proiectelor beneficiarului ales este %5.2f\n", val);

	int nrFrunze = counterFrunze(rad);
	Proiect* proiecteFrunze = (Proiect*)malloc(nrFrunze * sizeof(Proiect));
	int index = 0;
	vectorFrunze(rad, &index, proiecteFrunze);
	printf("\nVectorul cu proiectele de pe ultimele nivele:\n");
	for (int i = 0; i < nrFrunze; i++) {
		printf("ID Proiect: %d, ID Beneficiar: %d, Valoare: %5.2f, Denumire: %s\n",
			proiecteFrunze[i].idProiect, proiecteFrunze[i].idBeneficiar, proiecteFrunze[i].valoareInvestitie, proiecteFrunze[i].numeProiect);
	}

	rad = stergereNodCheie(rad, 421);
	printf("\nArborele in inordine dupa stergere:\n");
	inordine(rad);

	rad = modificareCheieArb(rad, 1001, 755);
	printf("\nArborele dupa modificare:\n");
	inordine(rad);

	dezalocareHash(tabela);
	dezalocareArbore(rad);
	dezalocareVector(proiecteFrunze);

	return 0;
}