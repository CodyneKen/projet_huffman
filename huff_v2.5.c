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

noeud *creer_feuille(int *tab, int index) {
    noeud *arbre_huffman;
    arbre_huffman = (noeud *) calloc(1, sizeof(noeud));

    if (tab[index] != 0) {
        arbre_huffman[index].c = index;
        arbre_huffman[index].occ = tab[index];

        printf("char n°%d : ", index);
        printf("%c ->", arbre_huffman[index].c);
        printf("%d \n", arbre_huffman[index].occ);
    }
    return arbre_huffman;
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
    int min = 1;
    for (i = 0; i < N_CHAR; i++) {
        if (occ[i] != 0 && occ[i] <= min)
            min = occ[i];
    }
    printf("occurences :");
    noeud *arbre_huffman = calloc(N_CHAR, sizeof(noeud));
    /* for (i = 0; i < N_CHAR; i++) {
        printf("char n°%d :", i);
        printf("%c ->", arbre_huffman[i].c);
        printf("%d \n", arbre_huffman[i].occ);
    }
    printf("\n");
    */
    arbre_huffman = creer_feuille(occ, min);
}
