#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <malloc.h>
#include <string.h>

typedef struct {
	int id;
	char* titlu;
	char* categorie;
	int nrPagini;
	int nrAutori;
} Articol;

typedef struct {
	Articol inf;
	struct nodLS* next;
} nodLS;

typedef struct {
	nodLS** vector;
	int nrElem;
} hashT;

int functieHash(hashT tabela, char* categorie) {
	return categorie[0] % tabela.nrElem;
}

void inserareLS(nodLS** prim, Articol art) {
	nodLS* nou = (nodLS*)malloc(sizeof(nodLS));
	nou->inf.id = art.id;
	nou->inf.nrAutori = art.nrAutori;
	nou->inf.nrPagini = art.nrPagini;
	nou->inf.categorie = (char*)malloc(strlen(art.categorie) + 1);
	strcpy(nou->inf.categorie, art.categorie);
	nou->inf.titlu = (char*)malloc(strlen(art.titlu) + 1);
	strcpy(nou->inf.titlu, art.titlu);
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

void inserareHash(hashT tabela, Articol art) {
	int poz = functieHash(tabela, art.categorie);
	inserareLS(&tabela.vector[poz], art);
}

void afisareLS(nodLS* prim) {
	nodLS* temp = prim;
	while (temp != NULL) {
		printf("\tID: %d, Nr. Pagini: %d, Nr. Autori: %d, Categorie: %s, Titlu: %s\n",
			temp->inf.id, temp->inf.nrPagini, temp->inf.nrAutori, temp->inf.categorie, temp->inf.titlu);
		temp = temp->next;
	}
}

void afisareHash(hashT tabela) {
	for (int i = 0; i < tabela.nrElem; i++) {
		if (tabela.vector[i] != NULL) {
			printf("Poztie %d:\n", i);
			afisareLS(tabela.vector[i]);
		}
	}
}

int counterAutori(hashT tabela) {
	int counter = 0;
	for (int i = 0; i < tabela.nrElem; i++) {
		if (tabela.vector[i] != NULL) {
			nodLS* temp = tabela.vector[i];
			while (temp != NULL) {
				counter += temp->inf.nrAutori;
				temp = temp->next;
			}
		}
	}
	return counter;
}

int counterArticoleCategorie(hashT tabela, char* categorie) {
	int counter = 0;
	int poz = functieHash(tabela, categorie);

	if (tabela.vector[poz] != NULL)
	{
		nodLS* temp = tabela.vector[poz];
		while (temp != NULL)
		{
			counter++;
			temp = temp->next;
		}
	}

	return counter;
}

Articol* articoleDinCategorie(hashT tabela, int* nrElem, char* categorie) {
	*nrElem = counterArticoleCategorie(tabela, categorie);

	if (*nrElem > 0) {
		Articol* vector = (Articol*)malloc(*nrElem * sizeof(Articol));
		int poz = functieHash(tabela, categorie);
		int index = 0;
		
		nodLS* temp = tabela.vector[poz];
		while (temp != NULL)
		{
			vector[index] = temp->inf;
			index++;
			temp = temp->next;
		}

		return vector;
	}

	return NULL;
}

void stergereArticol(hashT tabela, char* categorie, int id) {
	int poz = functieHash(tabela, categorie);

	if (tabela.vector[poz] != NULL) {
		nodLS* temp = tabela.vector[poz];
		nodLS* prev = NULL;
		while (temp != NULL)
		{
			if (temp->inf.id == id) {
				nodLS* deleteMe = temp;
				if (prev == NULL) {
					tabela.vector[poz] = temp->next;
				}
				else {
					prev->next = temp->next;
				}
				temp = temp->next;
				free(deleteMe->inf.categorie);
				free(deleteMe->inf.titlu);
			}
			else {
				prev = temp;
				temp = temp->next;
			}
		}
	}
}

void incrementareNrPagini(hashT tabela) {
	for (int i = 0; i < tabela.nrElem; i++) {
		if (tabela.vector[i] != NULL) {
			nodLS* temp = tabela.vector[i];
			while (temp != NULL)
			{
				temp->inf.nrPagini += 1;
				temp = temp->next;
			}
		}
	}
}

int counterCategorii(hashT tabela) {
	int counter = 0;
	for (int i = 0; i < tabela.nrElem; i++) {
		if (tabela.vector[i] != NULL) {
			counter++;
		}
	}
	return counter;
}

int* vectorNrPagini(hashT tabela, int* nrElem) {
	*nrElem = counterCategorii(tabela);

	if (*nrElem > 0) {
		int* vector = (int*)malloc(*nrElem * sizeof(int));
		int index = 0;

		for (int i = 0; i < tabela.nrElem; i++) {
			if (tabela.vector[i] != NULL) {
				nodLS* temp = tabela.vector[i];
				int nrPaginiCat = 0;
				while (temp != NULL)
				{
					nrPaginiCat += temp->inf.nrPagini;
					temp = temp->next;
				}
				vector[index] = nrPaginiCat;
				index++;
			}
		}
		return vector;
	}

	return NULL;
}

int main() {
	hashT tabela;
	tabela.nrElem = 23;
	tabela.vector = (nodLS**)malloc(tabela.nrElem * sizeof(nodLS*));
	for (int i = 0; i < tabela.nrElem; i++) {
		tabela.vector[i] = NULL;
	}

	FILE* f = fopen("fisier.txt", "r");

	Articol art;
	char buffer[128];

	while (fscanf(f, "%d", &art.id) == 1)
	{
		fscanf(f, " %[^\n]", buffer);
		art.titlu = (char*)malloc(strlen(buffer) + 1);
		strcpy(art.titlu, buffer);

		fscanf(f, "%s", buffer);
		art.categorie = (char*)malloc(strlen(buffer) + 1);
		strcpy(art.categorie, buffer);

		fscanf(f, "%d", &art.nrPagini);
		fscanf(f, "%d", &art.nrAutori);

		/*printf("ID: %d, Nr. Pagini: %d, Nr. Autori: %d, Categorie: %s, Titlu: %s\n",
			art.id, art.nrPagini, art.nrAutori, art.categorie, art.titlu);*/

		inserareHash(tabela, art);

		free(art.categorie);
		free(art.titlu);
	}
	fclose(f);

	printf("Tabela de dispersie din fisier:\n");
	afisareHash(tabela);

	int nrAutoriHash = counterAutori(tabela);
	printf("\nTabela are %d autori!\n", nrAutoriHash);

	/*int counter = counterArticoleCategorie(tabela, "Web");
	printf("\n%d\n", counter);*/

	int nrArticole;
	Articol* vectorArticoleCat = articoleDinCategorie(tabela, &nrArticole, "Web");
	printf("\nVectorul 1:\n");
	for (int i = 0; i < nrArticole; i++) {
		printf("ID: %d, Nr. Pagini: %d, Nr. Autori: %d, Categorie: %s, Titlu: %s\n",
			vectorArticoleCat[i].id, vectorArticoleCat[i].nrPagini, vectorArticoleCat[i].nrAutori,
			vectorArticoleCat[i].categorie, vectorArticoleCat[i].titlu);
	}

	int nrArticole2;
	Articol* vectorArticoleCat2 = articoleDinCategorie(tabela, &nrArticole2, "Cloud");
	printf("\nVectoru 2:\n");
	for (int i = 0; i < nrArticole2; i++) {
		printf("ID: %d, Nr. Pagini: %d, Nr. Autori: %d, Categorie: %s, Titlu: %s\n",
			vectorArticoleCat2[i].id, vectorArticoleCat2[i].nrPagini, vectorArticoleCat2[i].nrAutori,
			vectorArticoleCat2[i].categorie, vectorArticoleCat2[i].titlu);
	}

	int nrArticole3;
	Articol* vectorArticoleCat3 = articoleDinCategorie(tabela, &nrArticole3, "Security");
	printf("\nVectoru 3:\n");
	for (int i = 0; i < nrArticole3; i++) {
		printf("ID: %d, Nr. Pagini: %d, Nr. Autori: %d, Categorie: %s, Titlu: %s\n",
			vectorArticoleCat3[i].id, vectorArticoleCat3[i].nrPagini, vectorArticoleCat3[i].nrAutori,
			vectorArticoleCat3[i].categorie, vectorArticoleCat3[i].titlu);
	}

	int nrArticole4;
	Articol* vectorArticoleCat4 = articoleDinCategorie(tabela, &nrArticole4, "FullStack");
	printf("\nVectoru 4:\n");
	for (int i = 0; i < nrArticole4; i++) {
		printf("ID: %d, Nr. Pagini: %d, Nr. Autori: %d, Categorie: %s, Titlu: %s\n",
			vectorArticoleCat4[i].id, vectorArticoleCat4[i].nrPagini, vectorArticoleCat4[i].nrAutori,
			vectorArticoleCat4[i].categorie, vectorArticoleCat4[i].titlu);
	}

	stergereArticol(tabela, "Web", 9);
	stergereArticol(tabela, "AI", 98);
	stergereArticol(tabela, "Mobile", 55);
	printf("Tabela dupa stergere:\n");
	afisareHash(tabela);

	printf("\nTabela dupa incrementare:\n");
	incrementareNrPagini(tabela);
	afisareHash(tabela);

	int nrElem;
	int* vectorPaginiCategorii = vectorNrPagini(tabela, &nrElem);
	printf("\nVectorul cu numarul de pagini per categorie: ");
	for (int i = 0; i < nrElem; i++) {
		printf("%d ", vectorPaginiCategorii[i]);
	}

	return 0;
}