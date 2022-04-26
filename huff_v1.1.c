#include "huff.h"

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
  for (i=0; i<N_CHAR; i++)
    printf("%d",occ[i]);
  printf("\n");
  
}
