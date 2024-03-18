#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>

struct ContBancar {
    char iban[25]; // 24 bytes pentru IBAN + 1 byte terminator de șir
    char moneda[4]; // 3 bytes pentru codul monedei + 1 byte terminator de șir
    char* titular;
    float sold;
};

struct  Nod
{
    struct ContBancar cB;
    struct Nod* next;
};

typedef struct ContBancar ContBancar;
typedef struct Nod Nod;

Nod* inserare_sfarsit(Nod* p, ContBancar cont) {
    Nod* nou = (Nod*)malloc(sizeof(Nod));
    nou->cB = cont;
    nou->next = NULL;

    if (p == NULL) {
        return nou;
    }
    
    Nod* temp = p;
    while (temp->next != NULL) {
        temp = temp->next;
    }
    temp->next = nou;
    return p;
}

int main() {
    FILE* f = fopen("Conturi.txt", "r");
    if (f == NULL) {
        printf("Fisierul nu poate fi deschis.\n");
        return -1;
    }

    Nod* prim = NULL;

    struct ContBancar contB;
    char buffer[256];
    while (!feof(f)) {
        // Citirea IBAN
        if (fgets(buffer, sizeof(buffer), f) == NULL) break;
        buffer[strlen(buffer) - 1] = '\0'; // Înlăturăm newline-ul
        strcpy(contB.iban, buffer);

        // Citirea titularului
        if (fgets(buffer, sizeof(buffer), f) == NULL) break;
        buffer[strlen(buffer) - 1] = '\0'; // Înlăturăm newline-ul
        contB.titular = (char*)malloc(strlen(buffer) + 1);
        strcpy(contB.titular, buffer);

        // Citirea monedei
        if (fgets(buffer, sizeof(buffer), f) == NULL) break;
        buffer[strlen(buffer) - 1] = '\0'; // Înlăturăm newline-ul
        strcpy(contB.moneda, buffer);

        // Citirea soldului
        if (fgets(buffer, sizeof(buffer), f) == NULL) break;
        contB.sold = (float)atof(buffer);

        // Afisarea datelor curente
        printf("%s %s %s %.2f\n", contB.iban, contB.titular, contB.moneda, contB.sold);

        prim = inserare_sfarsit(prim, contB);

        // Eliberarea memoriei alocate pentru titular înainte de a suprascrie datele la următoarea iterare
        free(contB.titular);
    }

    Nod* t = prim;
    while (t != NULL) {
        printf("%s %s\n", t->cB.iban, t->cB.titular);
        t = t->next;
    }

    fclose(f);
    return 0;
}
