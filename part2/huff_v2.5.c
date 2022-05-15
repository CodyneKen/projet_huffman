#include "huff.h"

void occurence(FILE *fic, int tab[N_CHAR]) {
    char c = '\0';
    while ((c = fgetc(fic)) != EOF) {
        if (c < N_CHAR) {
            /* printf("%c", c); */
            tab[(int) (c)]++;
        } else
            printf("not a char supported :%c", c);
    }
    printf("\n");
}

/*
   noeud* creer_feuille(int numchar, int occchar){
   noeud tmp_noeud = calloc(1, sizeof(noeud));
   tmp_noeud.c = numchar;
   tmp_noeud.occ = occchar;
   return tmp_noeud;
   }
   */

noeud *creer_feuille(int *tab, int index) {
    /* tab est le tableau des occurrences, index est l'index du char, donc sa valeur ascii/int*/
    noeud *tmp_noeud = calloc(1, sizeof(noeud));
    tmp_noeud->c = index;
    tmp_noeud->occ = tab[index];
    return tmp_noeud;

}


int main(int argc, char **argv) {

    FILE *f;
    int *occ = calloc(N_CHAR, sizeof(int));
    int i;

    if (argc < 2) {
        printf("Merci de fournir un nom de fichier\n");
        exit(EXIT_FAILURE);
    }
    /* on essaye d'ouvrir le fichier, erreur sinon :*/
    if ((f = fopen(argv[1], "r"))) {
        occurence(f, occ);
    }
    printf("occurences :");
    noeud *arbre_huffman = calloc(N_CHAR, sizeof(noeud *));
    for (i = 0; i < N_CHAR; i++) {
        if (occ[i]) {
            arbre_huffman[i] = *creer_feuille(occ, i);
            printf("char n°%d :", i);
            printf("%c ->", arbre_huffman[i].c);
            printf("%d \n", arbre_huffman[i].occ);
        }
    }
    printf("\n");

}
