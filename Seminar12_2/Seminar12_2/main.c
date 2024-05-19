#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>

#define HTABLE_SIZE 200

struct ContBancar
{
	char iban[25]; // 24 bytes pt iban + 1 byte terminator string
	char moneda[4]; // 3 bytes pt cod moneda + 1 byte terminator string
	char* titular;
	float sold;
};

struct NodABC
{
	unsigned short int cheie;
	struct ContBancar data;
	struct NodABC* st, * dr;
};


typedef struct ContBancar ContBancar;
typedef struct NodABC NodABC;

NodABC* inserare_nod_arbore(NodABC* r, ContBancar contB,
	unsigned short int key, unsigned char* flag)
{
	if (r != NULL)
	{
		if (key < r->cheie)
		{
			r->st = inserare_nod_arbore(r->st, contB, key, flag);
		}
		else
		{
			if (key > r->cheie)
			{
				r->dr = inserare_nod_arbore(r->dr, contB, key, flag);
			}
			else
			{
				*flag = 0; // flag setat pe ESEC!
			}
		}
	}
	else
	{
		// locul unde nodul nou se insereaza
		NodABC* nou = (NodABC*)malloc(sizeof(NodABC));
		nou->cheie = key;
		nou->data = contB;
		nou->st = NULL;
		nou->dr = NULL;

		*flag = 1; // flag setat pe SUCCES!
		r = nou;
	}

	return r;
}

void inordine(NodABC* r)
{
	if (r)
	{
		inordine(r->st);
		printf("%d ", r->cheie);
		inordine(r->dr);
	}
}

ContBancar cautareContdupaCheie(NodABC* rad, unsigned short int cheieCautata) {
	if (rad == NULL) {
		ContBancar contBancarGol = { "", "", NULL, -1.0 };
		return contBancarGol;
	}

	if (cheieCautata == rad->cheie) {
		return rad->data;
	}

	if (cheieCautata < rad->cheie) {
		return cautareContdupaCheie(rad->st, cheieCautata);
	}

	if (cheieCautata > rad->cheie) {
		return cautareContdupaCheie(rad->dr, cheieCautata);
	}
}

ContBancar cautareContdupaIban(NodABC* rad, char* IBAN) {
	if (rad == NULL) {
		ContBancar contBancarGol = { "", "", NULL, -1.0 };
		return contBancarGol;
	}

	if (strcmp(rad->data.iban, IBAN) == 0) {
		return rad->data;
	}

	ContBancar contST = cautareContdupaIban(rad->st, IBAN);
	if (contST.sold != -1.0) {
		return contST;
	}

	return cautareContdupaIban(rad->dr, IBAN);

}

void dezalocareArbore(NodABC* rad) {
	if (rad != NULL) {
		dezalocareArbore(rad->st);
		dezalocareArbore(rad->dr);
		free(rad->data.titular);
		free(rad);
	}
}

int main()
{
	FILE* f = fopen("ConturiABC.txt", "r");
	NodABC* root = NULL; // arbore binar de cautare empty
	unsigned short int key;
	char nume_buff[128];

	struct ContBancar contB;
	fscanf(f, "%hu", &key);
	while (key != 0)
	{
		fscanf(f, "%s\n", contB.iban);

		fscanf(f, " %[^\n]", nume_buff);
		contB.titular = (char*)malloc(strlen(nume_buff) + 1);
		strcpy(contB.titular, nume_buff);

		fscanf(f, "%s", contB.moneda);
		fscanf(f, "%f", &contB.sold);

		// inserare cont bancar in arbore binar de cautare
		unsigned char inserat = 0;
		root = inserare_nod_arbore(root, contB, key, &inserat);
		if (inserat != 0)
		{
			contB.titular = NULL; // pentru a elimina partajarea dintre ultimul nod inserat si contB
			contB.iban[0] = 0;
		}
		else
		{
			// contul bancar nu a fost inserat in ABC
			free(contB.titular); // dezalocare mem heap pt titular
		}
		key = 0; // reinitializare cheie pentru a verifica existenta urmatorului cont bancar in fisier

		fscanf(f, "%hu", &key);
	}

	printf("Arbore dupa creare: ");
	inordine(root);
	printf("\n\n");

	// TODO: cautare cont bancar dupa cheie

	unsigned short int cheieDeCautare = 61;
	ContBancar contCautat = cautareContdupaCheie(root, cheieDeCautare);
	printf("\nContul cu cheie de cautare %d este: \n", cheieDeCautare);
	printf("IBAN: %s \t Moneda: %s \t Sold = %f \t Titular = %s\n",
		contCautat.iban, contCautat.moneda, contCautat.sold, contCautat.titular);

	// TODO: cautare cont bancar dupa iban

	ContBancar contCautat2 = cautareContdupaIban(root, "RO04RNCB1234666690556222");
	printf("\nContul:\n");
	printf("IBAN: %s \t Moneda: %s \t Sold = %f \t Titular = %s\n",
		contCautat2.iban, contCautat2.moneda, contCautat2.sold, contCautat2.titular);

	// TODO: dezalocare ABC

	dezalocareArbore(root);

	fclose(f);
	return 0;
}