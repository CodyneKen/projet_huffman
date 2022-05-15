#ifndef PROJET_HUFFMAN
#define PROJET_HUFFMAN

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <stdint.h>
#include <unistd.h>

/*max number of chars able to be encoded by the algo*/
#define N_CHAR 128
/*max number of bits for encoding*/
#define BIT_ENC 256

#ifdef __unix__
#define IS_POSIX 1
#else
#define IS_POSIX 0
#endif

/* commandes utliles : hexdump -C test */

/* STRUCTURES */

typedef struct noeud {
    int c;
    int occ;
    int enc;
    int nb_bit;
    struct noeud *gauche;
    struct noeud *droit;
} noeud;


/* struct buffer utilisée pour manipuler et écrire les encodages huffman sur le fichier de sortie */
typedef struct buffer {
    /* la taille max d'un encoding de huffman est 16bits, donc en ayant 32 bits, on se donne de la marge de secours */
    /* unsigned int 32bits, car signed peut provoquer erreurs sur bitwise operators */
    uint32_t bits;
    int size;
} buffer;


/* CREATION */
noeud *creer_feuille(int *tab, int index);

void creer_noeud(noeud *arbre[N_CHAR], int taille);

void creer_code(noeud *element);

void creer_code_aux(noeud *element, int code, int profondeur);


/* READ/WRITE */

void write_huffman(FILE *f, noeud **arbre);

void write_noeud(FILE *f, noeud *noeud);


void write_code(FILE *in, FILE *out, noeud **alphabet);

void write_binary(FILE *f, int nbr_bits, int codage);

/* RECUP / BIN_MANIPULATION*/

void init_buffer(buffer *BUFFER);

int get_enc(int c, noeud **alphabet);

int get_nb(int c, noeud **alphabet);

void enq_bit(int n, buffer *b);

void append_bits(char c, buffer *b, noeud **alphabet);

void write_8bits(buffer *BUFFER, FILE *out);


/* AFFICHAGE */
void display_huffman(noeud **arbre);

void display_noeud(noeud *noeud);

void affichage_code(int nbr_bits, int codage);

/* NETTOYAGE */
void detruire_arbre_huff(noeud *tab[N_CHAR]);

void detruire_noeud(noeud *noeud);

void detruire_feuille(noeud *noeud);

/* INFORMATION */
void occurrence(FILE *fic, int tab[N_CHAR]);

int est_feuille(noeud *noeud);

void find2Lowest(noeud **arbre, int nbElement, int *low1, int *low2);

void usage(int argc, char **argv);

void print_man(char **argv);

FILE *launch_comp(FILE *file, char *fname);

FILE *launch_decomp(FILE *, char *directory);

#endif