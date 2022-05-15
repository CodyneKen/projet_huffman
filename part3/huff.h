#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <locale.h>

/*max number of chars able to be encoded by the algo*/
#define N_CHAR 128
/*max number of bits for encoding*/
#define BIT_ENC 256

typedef struct noeud {
    int c;
    int occ;
    int enc;
    int nb_bit;
    struct noeud *gauche;
    struct noeud *droit;
} noeud;


/* CREATION */
noeud *creer_feuille(int *tab, int index);

void creer_noeud(noeud *arbre[N_CHAR], int taille);

void creer_code(noeud *element);

void creer_code_aux(noeud *element, int code, int profondeur);

/* AFFICHAGE */
void display_huffman(noeud **arbre);

void display_noeud(noeud *noeud);

void affichage_code(int nbr_bits, int codage);

/* NETTOYAGE */
void detruire_arbre_huff(noeud *tab[N_CHAR]);

void detruire_noeud(noeud *noeud);

void detruire_feuille(noeud *noeud);

/* INFORMATION */
void occurence(FILE *fic, int tab[256]);

int est_feuille(noeud *noeud);

void find2Lowest(noeud **arbre, int nbElement, int *low1, int *low2);

