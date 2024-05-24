#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <malloc.h>
#include <string.h>

typedef struct {
    int numarVagon;
    char* firma;
    int numarBileteVandute;
    int capacitateVagon;
} Vagon;

typedef struct {
    Vagon inf;
    struct nodLD* next;
    struct nodLD* prev;
} nodLD;

typedef struct {
    Vagon* vector;
    int nrElem;
} heap;

nodLD* inserareLD(nodLD* prim, nodLD** ultim, Vagon vag) {
    nodLD* nou = (nodLD*)malloc(sizeof(nodLD));
    nou->inf.capacitateVagon = vag.capacitateVagon;
    nou->inf.numarBileteVandute = vag.numarBileteVandute;
    nou->inf.numarVagon = vag.numarVagon;
    nou->inf.firma = (char*)malloc(strlen(vag.firma) + 1);
    strcpy(nou->inf.firma, vag.firma);

    nou->next = NULL;
    nou->prev = NULL;

    if (prim == NULL) {
        prim = nou;
        *ultim = nou;
    }
    else {
        (*ultim)->next = nou;
        nou->prev = *ultim;
        *ultim = nou;
    }

    return prim;
}

void traversareCapCoada(nodLD* prim) {
    nodLD* temp = prim;
    while (temp != NULL) {
        printf("Nr. Vagon: %d, Nr. Bilete: %d, Capacitate: %d, Firma: %s\n",
            temp->inf.numarVagon, temp->inf.numarBileteVandute, temp->inf.capacitateVagon, temp->inf.firma);
        temp = temp->next;
    }
}

void traversareCoadaCap(nodLD* ultim) {
    nodLD* temp = ultim;
    while (temp != NULL) {
        printf("Nr. Vagon: %d, Nr. Bilete: %d, Capacitate: %d, Firma: %s\n",
            temp->inf.numarVagon, temp->inf.numarBileteVandute, temp->inf.capacitateVagon, temp->inf.firma);
        temp = temp->prev;
    }
}

int numarMinimDeBileteVandute(nodLD* prim) {
    int min = prim->inf.numarBileteVandute;
    nodLD* temp = prim;
    while (temp != NULL) {
        if (min > temp->inf.numarBileteVandute) {
            min = temp->inf.numarBileteVandute;
        }
        temp = temp->next;
    }
    return min;
}

void stergereVagoaneCuNumarMinimDeBilete(nodLD** prim, nodLD** ultim) {
    int numarMinBileteVandute = numarMinimDeBileteVandute(*prim);
    nodLD* temp = *prim;
    while (temp != NULL) {
        if (temp->inf.numarBileteVandute == numarMinBileteVandute) {
            nodLD* deleteMe = temp;
            if (temp == *prim) {
                (*prim) = (*prim)->next;
                if (*prim != NULL) {
                    (*prim)->prev = NULL;
                }
            }
            else if (temp == *ultim) {
                nodLD* ant = temp->prev;
                if (ant != NULL) {
                    ant->next = NULL;
                }
                *ultim = ant;
            }
            else {
                nodLD* ant = temp->prev;
                nodLD* urm = temp->next;
                if (ant != NULL) {
                    ant->next = urm;
                }
                if (urm != NULL) {
                    urm->prev = ant;
                }
            }
            temp = temp->next;
            free(deleteMe->inf.firma);
            free(deleteMe);
        }
        else {
            temp = temp->next;
        }
    }
}

int getNrVagoane(nodLD* prim) {
    int nr = 0;
    nodLD* temp = prim;
    while (temp != NULL) {
        nr++;
        temp = temp->next;
    }
    return nr;
}

void filtrare(heap h, int index) {
    int indexRad = index;
    int indexS = 2 * index + 1;
    int indexD = 2 * index + 2;

    if (indexS < h.nrElem &&
        (h.vector[indexS].numarBileteVandute / (float)h.vector[indexS].capacitateVagon) <
        (h.vector[indexRad].numarBileteVandute / (float)h.vector[indexRad].capacitateVagon)) {
        indexRad = indexS;
    }

    if (indexD < h.nrElem &&
        (h.vector[indexD].numarBileteVandute / (float)h.vector[indexD].capacitateVagon) <
        (h.vector[indexRad].numarBileteVandute / (float)h.vector[indexRad].capacitateVagon)) {
        indexRad = indexD;
    }

    if (indexRad != index) {
        Vagon aux = h.vector[index];
        h.vector[index] = h.vector[indexRad];
        h.vector[indexRad] = aux;

        filtrare(h, indexRad);
    }
}

void salvareaListaInHeap(nodLD* prim, heap* h) {
    (*h).nrElem = getNrVagoane(prim);
    (*h).vector = (Vagon*)malloc((*h).nrElem * sizeof(Vagon));

    int index = 0;
    nodLD* temp = prim;
    while (temp != NULL) {
        (*h).vector[index].capacitateVagon = temp->inf.capacitateVagon;
        (*h).vector[index].numarBileteVandute = temp->inf.numarBileteVandute;
        (*h).vector[index].numarVagon = temp->inf.numarVagon;
        (*h).vector[index].firma = (char*)malloc(strlen(temp->inf.firma) + 1);
        strcpy((*h).vector[index].firma, temp->inf.firma);

        index++;
        temp = temp->next;
    }

    for (int i = ((*h).nrElem - 1) / 2; i >= 0; i--) {
        filtrare(*h, i);
    }
}

void modificaNumarulDeBilete(heap* h, int idVagon, int noulNrBileteVandute) {
    for (int i = 0; i < (*h).nrElem; i++) {
        if ((*h).vector[i].numarVagon == idVagon) {
            (*h).vector[i].numarBileteVandute = noulNrBileteVandute;
        }
    }

    for (int i = ((*h).nrElem - 1) / 2; i >= 0; i--) {
        filtrare(*h, i);
    }
}

int main() {
    nodLD* prim = NULL;
    nodLD* ultim = NULL;

    FILE* f = fopen("fisier.txt", "r");
    if (f == NULL) {
        printf("Fisierul nu a fost deschis!");
        return -1;
    }

    int nrVagoane;
    fscanf(f, "%d", &nrVagoane);

    Vagon vag;
    char buffer[128];

    for (int i = 0; i < nrVagoane; i++) {
        fscanf(f, "%d", &vag.numarVagon);

        fscanf(f, " %[^\n]", buffer);
        vag.firma = (char*)malloc(strlen(buffer) + 1);
        strcpy(vag.firma, buffer);

        fscanf(f, "%d", &vag.numarBileteVandute);
        fscanf(f, "%d", &vag.capacitateVagon);

        prim = inserareLD(prim, &ultim, vag);

        free(vag.firma);
    }
    fclose(f);

    printf("Lista Cap -> Coada din fisier:\n");
    traversareCapCoada(prim);

    printf("\nLista Coada -> Cap din fisier:\n");
    traversareCoadaCap(ultim);

    stergereVagoaneCuNumarMinimDeBilete(&prim, &ultim);

    printf("\nLista Cap -> Coada dupa stergere:\n");
    traversareCapCoada(prim);

    printf("\nLista Coada -> Cap dupa stergere:\n");
    traversareCoadaCap(ultim);

    printf("\nStructura HEAP:\n");
    heap h;
    salvareaListaInHeap(prim, &h);
    for (int i = 0; i < h.nrElem; i++) {
        printf("Nr. Vagon: %d, Nr.Bilete: %d, Capacitate: %d, Firma: %s, Grad Ocupare: %f\n",
            h.vector[i].numarVagon, h.vector[i].numarBileteVandute, h.vector[i].capacitateVagon, h.vector[i].firma,
            h.vector[i].numarBileteVandute / (float)h.vector[i].capacitateVagon * 100);
    }

    modificaNumarulDeBilete(&h, 10, 10);
    printf("\nStructura HEAP dupa modificare:\n");
    for (int i = 0; i < h.nrElem; i++) {
        printf("Nr. Vagon: %d, Nr.Bilete: %d, Capacitate: %d, Firma: %s, Grad Ocupare: %f\n",
            h.vector[i].numarVagon, h.vector[i].numarBileteVandute, h.vector[i].capacitateVagon, h.vector[i].firma,
            h.vector[i].numarBileteVandute / (float)h.vector[i].capacitateVagon* 100);
    }

    return 0;
}
