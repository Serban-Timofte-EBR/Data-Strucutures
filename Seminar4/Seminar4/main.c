#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>

struct ContBancar
{
    char iban[25];  // 24 bytes pt iban + 1 byte terminator string
    char moneda[4]; // 3 bytes pt cod moneda + 1 byte terminator string
    char* titular;
    float sold;
};

struct Nod
{
    struct ContBancar cb;
    struct Nod* next;
};

typedef struct ContBancar ContBancar;
typedef struct Nod Nod;

Nod* inserare_sfarsit(Nod* p, ContBancar cont)
{
    Nod* nou = (Nod*)malloc(sizeof(Nod));
    nou->cb = cont;
    nou->next = NULL;

    if (p == NULL)
        return nou;

    Nod* t = p;
    while (t->next != NULL)
        t = t->next;
    // t este ultimul nod din lista simpla p
    t->next = nou;

    return p;
}
Nod* interschimbNoduri(Nod* p)
{
    if (p == NULL || p->next == NULL)
        return p;

    Nod* cap = p;
    Nod* prev = NULL;
    Nod* curr = p;
    Nod* next = p->next;

    while (curr != NULL && next != NULL)
    {
        curr->next = next->next;
        next->next = curr;

        if (prev != NULL)
            prev->next = next;

        if (cap == p)
            cap = next;

        prev = curr;
        curr = curr->next;

        if (curr != NULL)
            next = curr->next;
    }

    return cap;
}

Nod* interschimb_iban_adiacente(Nod* cap, char* cIBAN) {
    if (cap == NULL || cap->next == NULL) {
        return cap;
    }
    if (strcmp(cap->cb.iban, cIBAN) == 0) {
        Nod* p = cap;
        Nod* q = cap->next;
        Nod* s = q->next;

        p->next = s;
        q->next = p;
        cap = q;
    }
    else {
        Nod* p = cap->next;
        Nod* r = cap;
        //pentru a ma opri pe penultimul nod ( ca sa am ce sa mai interschimb )
        while (p->next != NULL) {
            if (strcmp(p->cb.iban, cIBAN) == 0) {
                Nod* q = p->next;
                Nod* s = q->next;
                p->next = s;
                q->next = p;
                r->next = q;

                return cap;
            }
            else {
                r = r->next;
                p = p->next;
            }
        }
    }

    return cap;
}

Nod* interschimb_iban_oarecare(Nod* prim, char* cIBAN1, char* cIBAN2) {
    if (prim == NULL || prim->next == NULL) {
        return prim;
    }
    if (strcmp(prim->cb.iban, cIBAN1) == 0) {
        Nod* p = prim;
        Nod* q, *t;
        
        if (strcmp(prim->next->cb.iban, cIBAN2) == 0) {
            //insterschimb 1 cu 2
            return interschimb_iban_adiacente(prim, cIBAN1);
        }
        else {
            //interschimb 1 cu i
            q = prim->next->next;
            t = prim->next;
            while (q) {
                if (strcmp(q->cb.iban, cIBAN2) == 0) {
                    Nod* s = p->next, *w = q->next;
                    p->next = w;
                    q->next = s;
                    t->next = p;
                    prim = q;
                    return q;
                }
                else {
                    q = q->next;
                    t = t->next;
                }
            }
        }
    }

    else {
        if (strcmp(prim->cb.iban, cIBAN2) == 0) {
            Nod* p = prim;
            if (strcmp(prim->next->cb.iban, cIBAN1) == 0) {
                return interschimb_iban_adiacente(prim, cIBAN2);
            }
            else {
                Nod* q = prim->next->next;
                Nod* t = prim->next;
                while (q) {
                    if (strcmp(q->cb.iban, cIBAN1) == 0) {
                        Nod* s = p->next, * w = q->next;
                        p->next = w;
                        q->next = s;
                        t->next = p;
                        prim = q;
                        return q;
                    }
                    else {
                        q = q->next;
                        t = t->next;
                    }
                }
            }
        }
        else
        {
            //cazul p si q
            // 1. q == p->next 
            // 2. cIBAN1 cu cIBAN2 || cIBAN1 cu cIBAN2
        }
    }
}

int main()
{
    FILE* f = fopen("Conturi.txt", "r");
    Nod* cap = NULL;

    char nume_buff[128];
    struct ContBancar contB;
    fscanf(f, "%s\n", contB.iban);
    while (strlen(contB.iban))
    {
        fscanf(f, "%[^\n]", nume_buff);
        contB.titular = (char*)malloc(strlen(nume_buff) + 1);
        strcpy(contB.titular, nume_buff);

        fscanf(f, "%s", contB.moneda);
        fscanf(f, "%f", &contB.sold);

        // inserare nod in lista simpla
        cap = inserare_sfarsit(cap, contB);

        contB.titular = NULL; // pentru a elimina partajarea dintre ultimul nod inserat si contB
        contB.iban[0] = 0;
        fscanf(f, "%s\n", contB.iban);
    }

    Nod* t = cap;
    while (t != NULL)
    {
        printf("%s %s\n", t->cb.iban, t->cb.titular);
        t = t->next;
    }
    printf("\n");

    // interschimb noduri lista simpla cu modificare adrese de legatura
    //cap = interschimbNoduri(cap);
    cap = interschimb_iban_adiacente(cap, "RO04BRDE1234567890111222");
    printf("Lista simpla dupa interschimb!\n");
    t = cap;
    while (t != NULL)
    {
        printf("%s %s\n", t->cb.iban, t->cb.titular);
        t = t->next;
    }

    //EXTENSIE PENTRU CAMP CARE POATE AVEA VALOAREA DUBLICATA IN LISTA SIMPLA (titular, sold, moneda)

    // dezalocare lista simpla
    while (cap) {
        t = cap;
        cap = cap->next;
        free(t->cb.titular);
        free(t);
    }

    fclose(f);
    return 0;
}