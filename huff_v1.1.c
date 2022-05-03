#include <stdio.h>
#include <stdlib.h>

<<<<<<< HEAD
int main (int argc, char** argv){
        
  FILE* f;
  char c = '\0';
=======
int main(int argc, char **argv) {
>>>>>>> origin/mathis

    FILE *f;
    char str[15 * 2 + 1];
    if (argc < 2) {
        printf("Merci de fournir un nom de fichier\n");
        exit(EXIT_FAILURE);
    }
/* on essaye d'ouvrir le fichier, erreur sinon :*/
<<<<<<< HEAD
  if ((f = fopen(argv[1], "r"))){
      while ( (c = fgetc(f)) != EOF )
        printf("%c", c);
  }
=======
    if ((f = fopen(argv[1], "r"))) {
        fread(str, 15, 2, f);
        str[15 * 2] = '\0';
        printf(str);
        printf("\n");
    }
>>>>>>> origin/mathis
}
