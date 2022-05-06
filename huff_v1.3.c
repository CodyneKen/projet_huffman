#include "huff.h"

#define N_CHAR 256

void occurence(FILE *fic, int tab[N_CHAR]) {
    int c = -1;

    while ((c = fgetc(fic)) != EOF) {
        if (c < N_CHAR) {
            tab[c]++;
        } else fprintf(stderr, "not a char supported :%c\n", c);
    }
}

int main(int argc, char **argv) {

    FILE *f;
    int *occ = calloc(N_CHAR, sizeof(int));
    if (!occ) {
        fprintf(stderr, "Not enough memory, failed to malloc\n");
        exit(EXIT_FAILURE);
    }
    int i;

    if (argc < 2) {
        fprintf(stderr, "Usage %s : <file_name>\nMerci de fournir un nom de fichier\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    /* on essaye d'ouvrir le fichier, erreur sinon :*/
    if ((f = fopen(argv[1], "r"))) {
        occurence(f, occ);
        printf("occurences : \n");
        for (i = 0; i < N_CHAR; i++)
            if (occ[i] != 0)
                printf("%d ", occ[i]);
    } else fprintf(stderr, "Cannot open this file ! Please check if exists");

    printf("\n");
}
