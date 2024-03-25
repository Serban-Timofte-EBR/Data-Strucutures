#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct ContBancar {
    char iban[25];
    char moneda[4];
    char *titular;
    float sold;
};

struct Nod {
    struct ContBancar cb;
    struct Nod *next;
};

typedef struct ContBancar ContBancar;
typedef struct Nod Nod;

Nod *creazaNod(ContBancar cont) {
    Nod *nou = (Nod *)malloc(sizeof(Nod));
    if (!nou) {
        printf("Eroare la alocarea memoriei pentru un nou nod!\n");
        return NULL;
    }
    nou->cb = cont; 
    nou->next = NULL;
    return nou;
}

Nod *inserare_sfarsit(Nod *cap, ContBancar cont) {
    Nod *nou = creazaNod(cont);
    if (!nou) return cap;

    if (!cap) return nou;

    Nod *t = cap;
    while (t->next != NULL) t = t->next;
    t->next = nou; 
    return cap;
}

Nod *interschimbNoduri(Nod *cap) {
    if (!cap || !cap->next) return cap; 

    Nod *temp = cap->next;
    cap->next = temp->next;
    temp->next = cap;
    cap = temp;

    Nod *current = cap->next->next; 
    Nod *previous = cap->next; 

    while (current && current->next) {
        previous->next = current->next;
        previous = current;
        Nod *next = current->next->next;
        current->next->next = current;
        current->next = next;
        current = next;
    }

    return cap;
}

int main() {
    FILE *f = fopen("Conturi.txt", "r");
    if (!f) {
        printf("Nu se poate deschide fisierul.\n");
        return -1;
    }

    Nod *cap = NULL;
    ContBancar contB;
    char nume_buff[128];

    while (fscanf(f, "%s %s %f %s\n", contB.iban, contB.moneda, &contB.sold, nume_buff) == 4) {
        contB.titular = (char *)malloc(strlen(nume_buff) + 1);
        strcpy(contB.titular, nume_buff);

        cap = inserare_sfarsit(cap, contB);

    }

    Nod *t = cap;
    while (t) {
        printf("IBAN: %s, Titular: %s, Moneda: %s, Sold: %.2f\n", t->cb.iban, t->cb.titular, t->cb.moneda, t->cb.sold);
        t = t->next;
    }
    printf("\n");

    cap = interschimbNoduri(cap);
    t = cap;
    while (t) {
        printf("IBAN: %s, Titular: %s, Moneda: %s, Sold: %.2f\n", t->cb.iban, t->cb.titular, t->cb.moneda,
