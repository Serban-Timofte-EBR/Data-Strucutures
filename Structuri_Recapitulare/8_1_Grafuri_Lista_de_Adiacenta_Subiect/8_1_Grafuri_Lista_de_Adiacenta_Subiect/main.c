#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <malloc.h>
#include <string.h>

typedef struct {
	int id;
	char* denumire;
	char* localitate;
	float suprafata;
	int numar_angajati;
} Magazin;

typedef struct {
	int id;
	float distanta;
	struct nodADJ* next;
} nodAdj;

typedef struct {
	Magazin info;
	nodAdj* head;
} NodLista;

typedef struct {
	NodLista* noduri;
	int nrNoduri;
} Graf;

void adaugaMuchie(Graf* graf, int idStart, int idEnd, float distanta) {
    nodAdj* nou = (nodAdj*)malloc(sizeof(nodAdj));
    nou->id = idEnd;
    nou->distanta = distanta;
    nou->next = graf->noduri[idStart].head;
    graf->noduri[idStart].head = nou;
}

Graf citesteGraf(const char* numeFisier) {
    Graf graf;
    graf.nrNoduri = -1;
    FILE* f = fopen(numeFisier, "r");
    if (!f) {
        return graf;
    }

    fscanf(f, "%d", &graf.nrNoduri);

    graf.noduri = (NodLista*)malloc(graf.nrNoduri * sizeof(NodLista));
    for (int i = 0; i < graf.nrNoduri; i++) {
        graf.noduri[i].head = NULL;
        graf.noduri[i].info.denumire = (char*)malloc(50 * sizeof(char));
        graf.noduri[i].info.localitate = (char*)malloc(50 * sizeof(char));
        fscanf(f, "%d \"%[^\"]\" \"%[^\"]\" %f %d", &graf.noduri[i].info.id, graf.noduri[i].info.denumire, graf.noduri[i].info.localitate, &graf.noduri[i].info.suprafata, &graf.noduri[i].info.numar_angajati);
    }

    int nrArce;
    fscanf(f, "%d", &nrArce);
    for (int i = 0; i < nrArce; i++) {
        int idStart, idEnd;
        float distanta;
        fscanf(f, "%d %d %f", &idStart, &idEnd, &distanta);
        adaugaMuchie(&graf, idStart - 1, idEnd - 1, distanta);
        adaugaMuchie(&graf, idEnd - 1, idStart - 1, distanta); 
    }

    fclose(f);
    return graf;
}

void afiseazaGraf(Graf graf) {
    for (int i = 0; i < graf.nrNoduri; i++) {
        printf("Magazin %d: %s (%s), Suprafata: %.2f, Angajati: %d\n",
            graf.noduri[i].info.id,
            graf.noduri[i].info.denumire,
            graf.noduri[i].info.localitate,
            graf.noduri[i].info.suprafata,
            graf.noduri[i].info.numar_angajati);

        nodAdj* adj = graf.noduri[i].head;
        while (adj) {
            printf("  -> Magazin %d, Distanta: %.2f\n", adj->id + 1, adj->distanta);
            adj = adj->next;
        }
    }
}

int main() {
    Graf graf = citesteGraf("fisier.txt");
    if (graf.nrNoduri == -1) {
        printf("Fisierul nu a fost deschis\n");
        return -1;
    }

    printf("Graf initial:\n");
    afiseazaGraf(graf);

	return 0;
}