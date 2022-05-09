#include "huff.h"

void occurence(FILE *fic, int tab[N_CHAR]){
  int c = 0;
  while ( (c = fgetc(fic)) != EOF ){
    if(c<N_CHAR){
      /* printf("%c", c); */
      /* printf("(%d)", c); */
      /* printf("%c", c); */
      tab[c]++;
    }
    else
      printf("not a char supported :char n°%d -> %c\n", c, c);
  }
  printf("\n");
}

void creer_noeud(noeud *arbre[N_CHAR], int taille){

  noeud *plusPetits = calloc(1, sizeof(noeud));

  int *low1=malloc(sizeof(int)), *low2=malloc(sizeof(int));
  if (!low1 || !low2)
    fprintf(stderr, "pb allocation mem pour low1 et/ou low2 a la ligne %d\n", __LINE__);

  find2Lowest(arbre, taille, low1, low2);
  printf("2 plus petits : %d %d\n", *low1, *low2);
  /* ligne suivante jamais accédée si la taille est bien modifiée à chaque itération de creer_noeud, mais par sécurité*/
  if (*low1 == *low2){
    printf("Plus qu'1 seul noeud : %d avec %d occurrences\n", *low1, arbre[*low1]->occ);
    return;
  }

  plusPetits->occ = arbre[*low1]->occ + arbre[*low2]->occ;
  plusPetits->gauche = arbre[*low1];
  plusPetits->droit = arbre[*low2];
  printf("occurences 2 plus petit : %d\n", plusPetits->occ );
  arbre[*low1]=plusPetits;
  arbre[*low2]=NULL;

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
  for (i=0; i<N_CHAR; i++)
    if (arbre[i] && arbre[i]->occ)
      display_noeud(arbre[i]);

  printf("\n");
}

void display_noeud(noeud* noeud){
      printf("char n°%d :", noeud->c);
      printf("%c ->", noeud->c);
      printf("%d \n", noeud->occ); 
}

void detruire_arbre_huff(noeud *tab[N_CHAR]){
  int i;
  for (i=0; i<N_CHAR; i++)
    if (tab[i] && est_feuille(tab[i]))
      detruire_noeud(tab[i]);
}

void detruire_noeud(noeud* noeud){
  if (noeud->gauche)
    detruire_noeud(noeud->gauche);
  if (noeud->droit)
    detruire_noeud(noeud->droit);
  detruire_feuille(noeud);
}

void detruire_feuille(noeud* noeud){
  free(noeud);
}

int est_feuille(noeud* noeud){
  return !(noeud->gauche || noeud->droit);
}


void find2Lowest(noeud** arbre, int nbElement, int *low1, int *low2){
  /* indices des 2 premiers elmt non vides de tab */
  int i = 0; 
  if (arbre){

    if (nbElement == 0){
      fprintf(stderr, "erreur, tab de taille 0 dans find2Lowest, low1 et low2 non changés \n");
      return;
    }

    if (nbElement == 1){
      /* tant que arbre[i] pas défini (donc pas d'occurence) */
      while (!arbre[i])
        i++;

      fprintf(stderr, "erreur, tab de taille 1 dans find2Lowest, on donne 2 fois le même en return\n");
      *low1=i;
      *low2=i;
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
      if(arbre[i] && arbre[i]->occ<e1){
        e2=e1;
        i2=i1;
        e1=arbre[i]->occ;
        i1=i;
      }
      else if (arbre[i] && arbre[i]->occ<e2){
        e2=arbre[i]->occ;
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
  int taille = 0;

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
      taille++;
      arbre_huffman[i]=creer_feuille(occ, i);
    }
  }

  printf("arbre avant réduction :\n\n");
  display_huffman(arbre_huffman);

  while(taille > 1){
        creer_noeud(arbre_huffman, taille);
        taille--;
  }
  printf("\n");
  printf("arbre après réduction :\n\n");
  display_huffman(arbre_huffman);
  display_noeud(arbre_huffman[115]->gauche);
  display_noeud(arbre_huffman[115]->droit);

  if (arbre_huffman)
    detruire_arbre_huff(arbre_huffman);
  if (occ)
    free(occ);

}






