#include "huff.h"

<<<<<<< HEAD
void occurence(FILE *fic, int tab[N_CHAR]){
  char c = '\0';
  while ( (c = fgetc(fic)) != EOF ){
    if(c<N_CHAR){
      /* printf("%c", c); */
      tab[(int)(c)]++;
    }
    else
      printf("not a char supported :%c", c);
  }
  printf("\n");
}



int main (int argc, char** argv){
        
  FILE* f;
  int *occ = calloc(N_CHAR, sizeof(int));
  int i;

  if (argc<2){
    printf("Merci de fournir un nom de fichier\n");
    exit(EXIT_FAILURE);
  }
/* on essaye d'ouvrir le fichier, erreur sinon :*/

  if ((f = fopen(argv[1], "r"))){
    occurence(f, occ);
  }
  printf("occurences :");
  noeud *arbre_huffman = calloc(N_CHAR, sizeof(noeud));
  for (i=0; i<N_CHAR; i++){
        arbre_huffman[i].c = i;
        arbre_huffman[i].occ = occ[i];
        printf("char n°%d :", i);
        printf("%c ->", arbre_huffman[i].c);
        printf("%d \n", arbre_huffman[i].occ);
  }
  printf("\n");
  
=======
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

int main(int argc, char **argv) {

    FILE *f;
    int *occ = calloc(N_CHAR, sizeof(int));
    if (!occ) {
        printf("Not enough memory, failed to calloc\n");
        exit(EXIT_FAILURE);
    }
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
    for (i = 0; i < N_CHAR; i++)
        printf("%d", occ[i]);
    printf("\n");

    for (i = 0; i < N_CHAR. i++)
    {

    }
>>>>>>> origin/mathis
}
