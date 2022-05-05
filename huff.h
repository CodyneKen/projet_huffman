#include <stdio.h>
#include <stdlib.h>

/*max number of chars able to be encoded by the algo*/
#define N_CHAR 256
/*max number of bits for encoding*/
#define BIT_ENC 256

typedef struct noeud {
    char c; /* caractère initial */
    int occ; /* nbre d'occurrence du caractère */
    int enc[BIT_ENC]; /* codage dans l'arbre */
    int nb_bit; /* nbre bit sur lequel est codé le caractère */
    struct noeud *gauche; /* fils gauche */
    struct noeud *droite; /* fils droit */
} noeud;

void occurence(FILE *fic, int tab[N_CHAR]);


