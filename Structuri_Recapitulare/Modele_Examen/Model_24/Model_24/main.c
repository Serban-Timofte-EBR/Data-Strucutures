#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <malloc.h>
#include <string.h>

typedef struct {
	int id;
	char* titlu;
	char* autor;
	int anPublicare;
} Carte;

typedef struct {
	Carte* inf;
	struct nodLC* next;
} nodLC;

typedef struct {
	Carte inf;
	int ocupat;
} elementHash;

typedef struct {
	elementHash* vector;
	int nrElem;
} hashT;

nodLC* inserareLCCrescator(nodLC* prim, Carte car) {
	nodLC* nou = (nodLC*)malloc(sizeof(nodLC));
	nou->inf = (Carte*)malloc(sizeof(Carte));
	nou->inf->anPublicare = car.anPublicare;
	nou->inf->id = car.id;
	nou->inf->autor = (char*)malloc(strlen(car.autor) + 1);
	strcpy(nou->inf->autor, car.autor);
	nou->inf->titlu = (char*)malloc(strlen(car.titlu) + 1);
	strcpy(nou->inf->titlu, car.titlu);

	if (prim == NULL) {
		prim = nou;
		nou->next = prim;
	}
	else {
		nodLC* temp = prim;
		nodLC* prev = NULL;

		do {
			if (temp->inf->id > nou->inf->id) {
				break;
			}
			prev = temp;
			temp = temp->next;
		} while (temp != prim);

		if (prev == NULL) {
			nou->next = prim;
			nodLC* ultim = prim;
			while (temp->next != prim) {
				temp = temp->next;
			}
			temp->next = nou;
			return nou;
		}
		else if (temp == prim) {
			nou->next = prim;
			prev->next = nou;
		}
		else {
			prev->next = nou;
			nou->next = temp;
		}
	}
	return prim;
}

nodLC* actualizareTitlu(nodLC* prim, int id, char* titlulNou) {
	if (prim->inf->id == id) {
		free(prim->inf->titlu);
		prim->inf->titlu = (char*)malloc(strlen(titlulNou) + 1);
		strcpy(prim->inf->titlu, titlulNou);
		return prim;
	}
	nodLC* temp = prim->next;
	while (temp != prim)
	{
		if (temp->inf->id == id) {
			free(temp->inf->titlu);
			temp->inf->titlu = (char*)malloc(strlen(titlulNou) + 1);
			strcpy(temp->inf->titlu, titlulNou);
		}
		temp = temp->next;
	}
	return prim;
}

void afisareLC(nodLC* prim) {
	nodLC* temp = prim;
	if (temp != NULL) {
		printf("ID: %d, An: %d, Autor: %s, Titlu: %s\n",
			temp->inf->id, temp->inf->anPublicare, temp->inf->autor, temp->inf->titlu);
		temp = temp->next;
	}
	while (temp != prim)
	{
		printf("ID: %d, An: %d, Autor: %s, Titlu: %s\n",
			temp->inf->id, temp->inf->anPublicare, temp->inf->autor, temp->inf->titlu);
		temp = temp->next;
	}
}

int counterCartiAn(nodLC* prim, int an) {
	int counter = 0;
	if (prim->inf->anPublicare == an) {
		counter++;
	}
	nodLC* temp = prim->next;
	while (temp != prim) {
		if (temp->inf->anPublicare == an) {
			counter++;
		}
		temp = temp->next;
	}
	return counter;
}

nodLC* stergereID(nodLC* prim, int id) {
	nodLC* temp = prim;
	nodLC* prev = NULL;

	do {
		if (temp->inf->id == id) {
			break;
		}
		prev = temp;
		temp = temp->next;
	} while (temp != prim);

	if (prev == NULL) {
		nodLC* ultim = prim;
		while (ultim->next != prim) {
			ultim = ultim->next;
		}
		prim = prim->next;
		ultim->next = prim;
	} 
	
	else if (temp == prim) {
		prev->next = prim;
	}

	else {
		prev->next = temp->next;
	}

	return prim;
}

int functieHash(hashT tabela, char* autor) {
	return autor[0] % tabela.nrElem;
}

hashT inserareHash(hashT tabela, Carte car) {
	int poz = functieHash(tabela, car.autor);
	int index = 0;

	while (tabela.vector[(poz + index) % tabela.nrElem].ocupat == 1)
	{
		index++;
	}

	int pozFin = (poz + index) % tabela.nrElem;
	tabela.vector[pozFin].inf.anPublicare = car.anPublicare;
	tabela.vector[pozFin].inf.id = car.id;
	tabela.vector[pozFin].inf.autor = (char*)malloc(strlen(car.autor) + 1);
	strcpy(tabela.vector[pozFin].inf.autor, car.autor);
	tabela.vector[pozFin].inf.titlu = (char*)malloc(strlen(car.titlu) + 1);
	strcpy(tabela.vector[pozFin].inf.titlu, car.titlu);
	tabela.vector[pozFin].ocupat = 1;

	return tabela;
}

hashT lista_hash(nodLC* prim) {
	hashT tabela;
	tabela.nrElem = 23;
	tabela.vector = (elementHash*)malloc(tabela.nrElem * sizeof(elementHash));
	for (int i = 0; i < tabela.nrElem; i++) {
		tabela.vector[i].ocupat = 0;
	}

	inserareHash(tabela, *prim->inf);
	nodLC* temp = prim->next;
	while (temp != prim) {
		inserareHash(tabela, *temp->inf);
		temp = temp->next;
	}

	return tabela;
}

void afisareHash(hashT tabela) {
	for (int i = 0; i < tabela.nrElem; i++) {
		if (tabela.vector[i].ocupat == 1) {
			printf("Pozitia %d:\n", i);
			printf("\tID: %d, An: %d, Autor: %s, Titlu: %s\n",
				tabela.vector[i].inf.id, tabela.vector[i].inf.anPublicare, tabela.vector[i].inf.autor,
				tabela.vector[i].inf.titlu);
		}
	}
}

hashT stergereAutor(hashT tabela, char* autor, Carte* out) {
	int poz = functieHash(tabela, autor);
	int index = 0;

	while (tabela.vector[(poz + index) % tabela.nrElem].ocupat == 1 &&
		strcmp(tabela.vector[(poz + index) % tabela.nrElem].inf.autor, autor) != 0) {
		index++;
	}

	int pozFin = (poz + index) % tabela.nrElem;
	(*out).anPublicare = tabela.vector[pozFin].inf.anPublicare;
	(*out).id = tabela.vector[pozFin].inf.id;
	(*out).autor = (char*)malloc(strlen(tabela.vector[pozFin].inf.autor) + 1);
	strcpy((*out).autor, tabela.vector[pozFin].inf.autor);
	(*out).titlu = (char*)malloc(strlen(tabela.vector[pozFin].inf.titlu) + 1);
	strcpy((*out).titlu, tabela.vector[pozFin].inf.titlu);

	free(tabela.vector[pozFin].inf.autor);
	free(tabela.vector[pozFin].inf.titlu);

	tabela.vector[pozFin].ocupat = -1;

	return tabela;
}

hashT modificareAutor(hashT tabela, char* autorActual, char* autorNou) {
	Carte car;
	tabela = stergereAutor(tabela, autorActual, &car);
	free(car.autor);
	car.autor = (char*)malloc(strlen(autorNou) + 1);
	strcpy(car.autor, autorNou);

	tabela = inserareHash(tabela, car);

	return tabela;
}

int main() {
	nodLC* prim = NULL;

	FILE* f = fopen("fisier.txt", "r");
	
	Carte car;
	char buffer[128];

	while (fscanf(f,"%d", &car.id) == 1)
	{
		fscanf(f, " %[^\n]", buffer);
		car.titlu = (char*)malloc(strlen(buffer) + 1);
		strcpy(car.titlu, buffer);

		fscanf(f, " %[^\n]", buffer);
		car.autor = (char*)malloc(strlen(buffer) + 1);
		strcpy(car.autor, buffer);

		fscanf(f, "%d", &car.anPublicare);

		/*printf("ID: %d, An: %d, Autor: %s, Titlu: %s\n",
			car.id, car.anPublicare, car.autor, car.titlu);*/
		prim = inserareLCCrescator(prim, car);

		free(car.autor);
		free(car.titlu);
	}
	fclose(f);

	printf("Lista circulara din fisier:\n");
	afisareLC(prim);

	prim = actualizareTitlu(prim, 764, "Lorem Ipsum");
	prim = actualizareTitlu(prim, 981, "Lorem Ipsum");
	prim = actualizareTitlu(prim, 213, "Lorem Ipsum");
	printf("\nLista circulara dupa actualizare\n");
	afisareLC(prim);

	int counter = counterCartiAn(prim, 2022);
	printf("\nAvem %d carti in anul cerut\n", counter);
	counter = counterCartiAn(prim, 2021);
	printf("\nAvem %d carti in anul cerut\n", counter);

	prim = stergereID(prim, 764);
	//prim = stergereID(prim, 213);
	//prim = stergereID(prim, 981);
	printf("\nLista circulara dupa stergere:\n");
	afisareLC(prim);

	hashT tabela = lista_hash(prim);

	printf("\nTabela de dispersie\n");
	afisareHash(tabela);

	tabela = modificareAutor(tabela, "Antionio Deere", "Razvan Bucur");
	printf("\nTabela dupa modificare:\n");
	afisareHash(tabela);

	return 0;
}