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

noeud* creer_feuille(int* tab, int index){
  /* tab est le tableau des occurrences, index est l'index du char, donc sa valeur ascii/int*/
  noeud *tmp_noeud = calloc(1, sizeof(noeud));
  if (!tmp_noeud)
    fprintf(stderr, "tmp_noeud erreur a la ligne %d\n", __LINE__);

  tmp_noeud->c = index;
  tmp_noeud->occ = tab[index];
  return tmp_noeud;

}

void display_huffman(noeud** arbre){
  int i;
  for (i=0; i<N_CHAR; i++){
    if (arbre[i] && arbre[i]->occ){
      printf("char n°%d :", i);
      printf("%c ->", arbre[i]->c);
      printf("%d \n", arbre[i]->occ);
    }
  }
  printf("\n");
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
  /* indices des 2 premiers elmt non vides de tab */
  int i = 0; 
  if (tab){

    if (nbElement == 0){
      fprintf(stderr, "erreur, tab de taille 0 dans find2Lowest, low1 et low2 non changés \n");
      return;
    }

    if (nbElement == 1){
      while (!tab[i])
        i++;

      fprintf(stderr, "erreur, tab de taille 1 dans find2Lowest, on donne 2 fois le même en return\n");
      *low1=tab[i];
      *low2=tab[i];
      return;
    }

    /*si taille != 0 ou != 1, le tableau est normal, on le traite :*/

    /* i1, i2, les indices des 2 plus petits elements */
    int i1, i2;
    i1 = i2 = 0;
    /* e1, e2, les 2 plus petits elements */
    int e1, e2;
    e1 = e2 = INT_MAX;

    for (i=0; i<N_CHAR; i++){
      if(tab[i]!=0 && tab[i]<e1){
        e2=e1;
        i2=i1;
        e1=tab[i];
        i1=i;
      }
      else if (tab[i]!=0 && tab[i]<e2){
        e2=tab[i];
        i2=i;
      }
    }
    /* IMPORTANT LA FONCTION RETOURNE LES >>INDICES<< DES 2 PLUS PETITS, PLUS FLEXIBLE */
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
  int nbElement = 0;

  if (argc<2){
    printf("Merci de fournir un nom de fichier\n");
    exit(EXIT_FAILURE);
  }
  /* on essaye d'ouvrir le fichier, erreur sinon :*/
  if ((f = fopen(argv[1], "r"))){
    occurence(f, occ);
  }
  printf("occurences :");
  noeud ** arbre_huffman = NULL;
  arbre_huffman = calloc(N_CHAR, sizeof(noeud*));
  if (!arbre_huffman)
    fprintf(stderr, "arbre_huffman erreur a la ligne %d\n", __LINE__);

  for (i=0; i<N_CHAR; i++){
    if (occ[i]){
      nbElement++;
      arbre_huffman[i]=creer_feuille(occ, i);
    }
  }

  display_huffman(arbre_huffman);

  int *low1=malloc(sizeof(int)), *low2=malloc(sizeof(int));
  if (!low1 || !low2)
    fprintf(stderr, "pb allocation mem pour low1 et/ou low2 a la ligne %d\n", __LINE__);

  find2Lowest(occ, nbElement, low1, low2);
  printf("2 plus petits : %d %d\n", *low1, *low2);

  noeud *plusPetits = calloc(1, sizeof(noeud));
  plusPetits->occ = arbre_huffman[*low1]->occ + arbre_huffman[*low2]->occ;
  printf("occurences 2 plus petit : %d\n", plusPetits->occ );

/*   if (arbre_huffman) */
/*     detruire_arbre_huff(&arbre_huffman); */
/*   if (occ) */
/*     free(occ); */

}






