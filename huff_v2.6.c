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

/*
   noeud* creer_feuille(int numchar, int occchar){
   noeud tmp_noeud = calloc(1, sizeof(noeud));
   tmp_noeud.c = numchar;
   tmp_noeud.occ = occchar;
   return tmp_noeud;
   }
   */

noeud* creer_feuille(int* tab, int index){
  /* tab est le tableau des occurrences, index est l'index du char, donc sa valeur ascii/int*/
  noeud *tmp_noeud = calloc(1, sizeof(noeud));
  if (!tmp_noeud)
    fprintf(stderr, "tmp_noeud erreur a la ligne %d\n", __LINE__);

  tmp_noeud->c = index;
  tmp_noeud->occ = tab[index];
  return tmp_noeud;

}
/* pas fonctionnel pour l'instant : */
void detruire_arbre_huff(noeud *tab[N_CHAR]){
  int i;
  for (i=0; i<N_CHAR; i++){
    if (tab[i])
      free(tab[i]);
  }
  return;
}

void find2Lowest(int *tab, int nbElement, int *low1, int *low2){
  if (tab){
    if (nbElement == 1){
      /*ne marche pas car passer un tableau a une fonction le transforme en pointeur simple, l'information est perdue:*/
      /* size_t nbElement = sizeof(occ)/sizeof(occ[0]); */
      fprintf(stderr, "erreur, tab de taille 1 dans find2Lowest, on donne 2 fois le même en return\n");
      *low1=tab[0];
      *low2=tab[0];
      return;
    }
    /* met le plus petit elmt des 2 premiers dans i1, et 2e plus petit dans i2 */
    int i1 = (tab[0]<tab[1]?tab[0]:tab[1]);
    int i2 = (tab[0]<tab[1]?tab[1]:tab[0]);
    int i;
    for (i=2; i<nbElement; i++){
      if(tab[i]<i1 && tab[i]<i2){
        i2=i1;
        i1=tab[i];
      }
      else if (tab[i]<i2){
        i2=tab[i];
      }
    }
    *low1 = i1;
    *low2 = i2;
    return;
  }
  else{
    fprintf(stderr, "erreur, tab vide donné a find2Lowest\n");
    return;
  }
}


int main (int argc, char** argv){

  FILE* f;
  int *occ = calloc(N_CHAR, sizeof(int));
  if (!occ)
    fprintf(stderr, "occ erreur a la ligne %d\n", __LINE__);
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
  noeud *arbre_huffman = calloc(N_CHAR, sizeof(noeud*));
  if (!arbre_huffman)
    fprintf(stderr, "arbre_huffman erreur a la ligne %d\n", __LINE__);

  for (i=0; i<N_CHAR; i++){
    /* ne choisir que les char avec occ[i]>0 créé un segfault avec le char 123 "{" on ne sait pas pourquoi pour le moment */
    if (occ[i]>0 && i!=123){
      arbre_huffman[i]=*creer_feuille(occ, i);
      printf("char n°%d :", i);
      printf("%c ->", arbre_huffman[i].c);
      printf("%d \n", arbre_huffman[i].occ);
    }
  }
  printf("\n");

  int *low1=malloc(sizeof(int)), *low2=malloc(sizeof(int));
  if (!low1 || !low2)
    fprintf(stderr, "pb allocation mem pour low1 et/ou low2 a la ligne %d\n", __LINE__);
  /* size_t arrsize = sizeof(occ)/sizeof(occ[0]); */
  find2Lowest(occ, N_CHAR, low1, low2);
  printf("2 plus petits : %d %d\n", *low1, *low2);

/*   if (arbre_huffman) */
/*     detruire_arbre_huff(&arbre_huffman); */
/*   if (occ) */
/*     free(occ); */

}






