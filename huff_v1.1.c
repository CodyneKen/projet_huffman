#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {

    FILE *f;
    char str[15 * 2 + 1];
    if (argc < 2) {
        printf("Merci de fournir un nom de fichier\n");
        exit(EXIT_FAILURE);
    }
/* on essaye d'ouvrir le fichier, erreur sinon :*/
    if ((f = fopen(argv[1], "r"))) {
        while ((c = fgetc(f)) != EOF)
            printf("%c", c);
    }
}
