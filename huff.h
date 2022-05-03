#include <stdio.h>
#include <stdlib.h>

/*max number of chars able to be encoded by the algo*/
#define N_CHAR 256
/*max number of bits for encoding*/
#define BIT_ENC 256

typedef struct noeud {
  char c;
  int occ;
  int enc[BIT_ENC];
  int bit_num;
  struct noeud *gauche;
  struct noeud *droite;
}noeud;

void occurence(FILE *fic, int tab[256]);


