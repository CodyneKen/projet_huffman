#include "huff.h"

void occurence(FILE *fic, int tab[N_CHAR]) {
    int c = -1;
    while ((c = fgetc(fic)) != EOF) {
        if (c < N_CHAR) {
            tab[c]++;
        } else
            printf("not a char supported :%c\n", c);
    }
}

noeud *creer_feuille(int *tab, int index) {
    int i;
    noeud noeud1;
    noeud *arbre_huffman = NULL;

    arbre_huffman = malloc(1 * sizeof(noeud));

    if (tab[index] != 0) {
        noeud1.c = index;
        noeud1.occ = tab[index];
        noeud1.nb_bit = -1;
        arbre_huffman[index] = noeud1;
    }
    return arbre_huffman;
}

int main(int argc, char **argv) {
    FILE *f;
    int *occ = calloc(N_CHAR, sizeof(int));
    int i, x = 0;
    noeud noeud1;
    noeud *arbre_huffman[N_CHAR];

    if (argc < 2) {
        printf("Usage %s : <file_name>\nMerci de fournir un nom de fichier\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    /* on essaye d'ouvrir le fichier, erreur sinon :*/
    if ((f = fopen(argv[1], "r"))) {
        occurence(f, occ);
        printf("occurences : \n");
        for (i = 0; i < N_CHAR; i++)
            if (occ[i] != 0)
                printf("%d ", occ[i]);
        printf("\n");
        for (i = 0; i < N_CHAR; i++) {
            if (occ[i] != 0) {
                noeud1.c = i;
                noeud1.occ = occ[i];
                noeud1.nb_bit = -1;
                arbre_huffman[x] = &noeud1;
                printf("occ[%c] = %d\n", arbre_huffman[x]->c, arbre_huffman[x]->occ);
                x++;
            }
        }
    } else fprintf(stderr, "Cannot open this file ! Please check if exists");

}