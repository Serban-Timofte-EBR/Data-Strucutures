#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <malloc.h>
#include <string.h>

typedef struct {
    int id;
    char* nume;
    char* adresa;
    int nrAngajati;
    float venit;
} Host;

typedef struct {
    Host* inf;
    float cost;
    struct nodLS* next;
} nodLS;

typedef struct {
    Host inf;
    nodLS* capAdicaenta;
    struct nodLL* next;
} nodLL;

typedef struct {
    nodLL* capLL;
    int nrElem;
} graf;

typedef struct {
    int varf_src;
    int varf_desc;
} elemVect;

void inserareLL(nodLL** prim, Host inf) {
    nodLL* nou = (nodLL*)malloc(sizeof(nodLL));
    nou->inf.id = inf.id;
    nou->inf.nrAngajati = inf.nrAngajati;
    nou->inf.venit = inf.venit;
    nou->inf.nume = (char*)malloc(strlen(inf.nume) + 1);
    strcpy(nou->inf.nume, inf.nume);
    nou->inf.adresa = (char*)malloc(strlen(inf.adresa) + 1);
    strcpy(nou->inf.adresa, inf.adresa);
    nou->capAdicaenta = NULL;
    nou->next = NULL;

    if (*prim == NULL) {
        *prim = nou;
    }
    else {
        nodLL* temp = *prim;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = nou;
    }
}

nodLL* nodDupaID(nodLL* prim, int id) {
    nodLL* temp = prim;
    while (temp != NULL) {
        if (temp->inf.id == id) {
            return temp;
        }
        temp = temp->next;
    }
    return NULL;
}

void inserareLS(nodLS** prim, Host* inf, float cost) {
    nodLS* nou = (nodLS*)malloc(sizeof(nodLS));
    nou->inf = inf;
    nou->cost = cost;
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

void afisareLS(nodLS* prim) {
    nodLS* temp = prim;
    while (temp != NULL) {
        printf("\tID: %d, Nr. angajati: %d, Venit: %5.2f, Nume: %s, Adresa: %s, Cost: %5.2f\n",
            temp->inf->id, temp->inf->nrAngajati, temp->inf->venit, temp->inf->nume, temp->inf->adresa, temp->cost);
        temp = temp->next;
    }
}

void afisareGraf(graf graf) {
    nodLL* temp = graf.capLL;
    while (temp != NULL) {
        printf("ID: %d, Nr. angajati: %d, Venit: %5.2f, Nume: %s, Adresa: %s\n",
            temp->inf.id, temp->inf.nrAngajati, temp->inf.venit, temp->inf.nume, temp->inf.adresa);
        printf("Vecini:\n");
        afisareLS(temp->capAdicaenta);
        temp = temp->next;
    }
}

int counterArcePondere(graf graf, float prag) {
    int counter = 0;

    nodLL* temp = graf.capLL;
    while (temp != NULL) {
        nodLS* temp2 = temp->capAdicaenta;
        while (temp2 != NULL) {
            if (temp2->cost > prag) {
                counter++;
            }
            temp2 = temp2->next;
        }
        temp = temp->next;
    }

    return counter;
}

elemVect* vectorMuchiiPrag(graf graf, float prag, int* nrElem) {
    *nrElem = counterArcePondere(graf, prag);
    elemVect* vector = (elemVect*)malloc(*nrElem * sizeof(elemVect));
    int index = 0;

    nodLL* temp = graf.capLL;
    while (temp != NULL) {
        nodLS* temp2 = temp->capAdicaenta;
        while (temp2 != NULL) {
            if (temp2->cost > prag) {
                vector[index].varf_src = temp->inf.id;
                vector[index].varf_desc = temp2->inf->id;
                index++;
            }
            temp2 = temp2->next;
        }
        temp = temp->next;
    }

    return vector;
}

int counterHostsPrag(graf graf, float prag) {
    int counter = 0;
    nodLL* temp = graf.capLL;
    while (temp != NULL) {
        if (temp->inf.venit > prag) {
            counter++;
        }
        temp = temp->next;
    }
    return counter;
}

Host* hosturiPestePrag(graf graf, int* nrElem, float prag) {
    *nrElem = counterHostsPrag(graf, prag);
    Host* vector = (Host*)malloc(*nrElem * sizeof(Host));
    int index = 0;

    nodLL* temp = graf.capLL;
    while (temp != NULL) {
        if (temp->inf.venit > prag) {
            vector[index].id = temp->inf.id;
            vector[index].nrAngajati = temp->inf.nrAngajati;
            vector[index].venit = temp->inf.venit;
            vector[index].nume = (char*)malloc(strlen(temp->inf.nume) + 1);
            strcpy(vector[index].nume, temp->inf.nume);
            vector[index].adresa = (char*)malloc(strlen(temp->inf.adresa) + 1);
            strcpy(vector[index].adresa, temp->inf.adresa);
            index++;
        }
        temp = temp->next;
    }

    return vector;
}

float getPondereMinima(graf graf) {
    float min = graf.capLL->capAdicaenta->cost;
    nodLL* temp = graf.capLL;
    while (temp != NULL) {
        nodLS* temp2 = temp->capAdicaenta;
        while (temp2 != NULL) {
            if (temp2->cost < min) {
                min = temp2->cost;
            }
            temp2 = temp2->next;
        }
        temp = temp->next;
    }
    return min;
}

int areHostulPondereMin(int id, float pondereMin, graf graf) {
    int flag = 0;
    nodLL* temp = graf.capLL;
    while (temp != NULL) {
        if (temp->inf.id == id) {
            nodLS* temp2 = temp->capAdicaenta;
            while (temp2 != NULL) {
                if (temp2->cost == pondereMin) {
                    flag = 1;
                }
                temp2 = temp2->next;
            }
        }
        temp = temp->next;
    }
    return flag;
}

Host* vectorHostsFaraPondereMin(Host* vectorI, int nrElemI, int* nrElemF, graf graf) {
    float pondereMin = getPondereMinima(graf);;
    *nrElemF = 0;
    for (int i = 0; i < nrElemI; i++) {
        if (areHostulPondereMin(vectorI[i].id, pondereMin, graf) == 0) {
            (*nrElemF)++;
        }
    }

    Host* vectorNou = (Host*)malloc(*nrElemF * sizeof(Host));
    int index = 0;
    for (int i = 0; i < nrElemI; i++) {
        if (areHostulPondereMin(vectorI[i].id, pondereMin, graf) == 0) {
            vectorNou[index] = vectorI[i];
            index++;
        }
        else {
            free(vectorI[i].adresa);
            free(vectorI[i].nume);
        }
    }

    return vectorNou;
}

void dezalocareLS(nodLS* prim) {
    nodLS* temp = prim;
    while (temp != prim) {
        nodLS* aux = temp->next;
        free(temp);
        temp = aux;
    }
}

void dezalocareLL(nodLL* prim) {
    nodLL* temp = prim;
    while (temp != NULL) {
        nodLL* aux = temp->next;
        dezalocareLS(temp->capAdicaenta);
        free(temp->inf.adresa);
        free(temp->inf.nume);
        free(temp);
        temp = aux;
    }
}

void dezalocareVector(Host* vector, int nrElem) {
    for (int i = 0; i < nrElem; i++) {
        free(vector[i].adresa);
        free(vector[i].nume);
    }
    free(vector);
}

int main() {
    FILE* f = fopen("fisier.txt", "r");
    if (f == NULL) {
        return -1;
    }

    int nrElem = 0;
    fscanf(f, "%d", &nrElem);

    graf graf;
    graf.capLL = NULL;
    graf.nrElem = nrElem;

    Host h;
    char buffer[128];

    for (int i = 0; i < graf.nrElem; i++) {
        fscanf(f, "%d", &h.id);

        fscanf(f, " %[^\n]", buffer);
        h.nume = (char*)malloc(strlen(buffer) + 1);
        strcpy(h.nume, buffer);

        fscanf(f, " %[^\n]", buffer);
        h.adresa = (char*)malloc(strlen(buffer) + 1);
        strcpy(h.adresa, buffer);

        fscanf(f, "%d", &h.nrAngajati);
        fscanf(f, "%f", &h.venit);

        /*printf("ID: %d, Nr. angajati: %d, Venit: %5.2f, Nume: %s, Adresa: %s\n",
            h.id, h.nrAngajati, h.venit, h.nume, h.adresa);*/
        inserareLL(&graf.capLL, h);

        free(h.nume);
        free(h.adresa);
    }

    int nrArce;
    fscanf(f, "%d", &nrArce);
    int start;
    int ultim;
    float cost;

    for (int i = 0; i < nrArce; i++) {
        fscanf(f, "%d", &start);
        fscanf(f, "%d", &ultim);
        fscanf(f, "%f", &cost);
        //printf("\n%d %d, %5.2f\n", start, ultim, cost);
        nodLL* nodGraf = nodDupaID(graf.capLL, start);
        nodLL* nodGrafAdiacent = nodDupaID(graf.capLL, ultim);
        inserareLS(&nodGraf->capAdicaenta, &nodGrafAdiacent->inf, cost);
    }

    fclose(f);

    printf("Graful citit din fisier:\n");
    afisareGraf(graf);

    int nrElemVec = counterArcePondere(graf, 10.0);
    printf("\n%d\n", nrElemVec);

    elemVect* vector = vectorMuchiiPrag(graf, 10.0, &nrElemVec);
    printf("\nVectorul:\n");
    for (int i = 0; i < nrElemVec; i++) {
        printf("%d - %d\n", vector[i].varf_src, vector[i].varf_desc);
    }
    free(vector);

    // prag  = 8000;
    //int nrElemVec2 = counterHostsPrag(graf, 8000);
    //printf("\n%d\n", nrElemVec2);
    int nrElemVec2 = 0;
    Host* hostsPeste8000 = hosturiPestePrag(graf, &nrElemVec2, 8000);
    printf("\nVectorul cu hosturi cu venit peste 8000:\n");
    for (int i = 0; i < nrElemVec2; i++) {
        printf("ID: %d, Nr. angajati: %d, Venit: %5.2f, Nume: %s, Adresa: %s\n",
            hostsPeste8000[i].id, hostsPeste8000[i].nrAngajati, hostsPeste8000[i].venit,
            hostsPeste8000[i].nume, hostsPeste8000[i].adresa);
    }

    /*float pondereMin = getPondereMinima(graf);
    printf("\n%5.2f\n", pondereMin);*/

    int nrElemVec3 = 0;
    Host* vectornou = vectorHostsFaraPondereMin(hostsPeste8000, nrElemVec2, &nrElemVec3, graf);
    printf("\nVectorul dupa stergerea hostului cu pondere minima:\n");
    for (int i = 0; i < nrElemVec3; i++) {
        printf("ID: %d, Nr. angajati: %d, Venit: %5.2f, Nume: %s, Adresa: %s\n",
            vectornou[i].id, vectornou[i].nrAngajati, vectornou[i].venit, vectornou[i].nume, vectornou[i].adresa);
    }

    dezalocareLL(graf.capLL);
    free(hostsPeste8000);
    dezalocareVector(vectornou, nrElemVec3);

	return 0;
}