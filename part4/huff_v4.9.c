#include "huff.h"

void occurence(FILE *fic, int tab[N_CHAR]) {
    int c = 0;
    while ((c = fgetc(fic)) != EOF) {
        if (c < N_CHAR) {
            /* printf("%c", c); */
            /* printf("(%d)", c); */
            /* printf("%c", c); */
            tab[c]++;
        }
        /* else */
        /* printf("not a char supported :char n°%d -> %c\n", c, c); */
    }
    else
      fprintf(stderr, "not a char supported :char n°%d -> %c\n", c, c);
  }
}

void creer_noeud(noeud *arbre[N_CHAR], int taille) {

    noeud *plusPetits = calloc(1, sizeof(noeud));

    int *low1 = malloc(sizeof(int)), *low2 = malloc(sizeof(int));
    if (!low1 || !low2)
        fprintf(stderr, "pb allocation mem pour low1 et/ou low2 a la ligne %d\n", __LINE__);

    find2Lowest(arbre, taille, low1, low2);
    /* ligne suivante jamais accédée si la taille est bien modifiée à chaque itération de creer_noeud, mais par sécurité*/
    if (*low1 == *low2) {
        return;
    }

    plusPetits->occ = arbre[*low1]->occ + arbre[*low2]->occ;
    plusPetits->gauche = arbre[*low1];
    plusPetits->droit = arbre[*low2];
    arbre[*low1] = plusPetits;
    arbre[*low2] = NULL;

}


noeud *creer_feuille(int *tab, int index) {
    /* tab est le tableau des occurrences, index est l'index du char, donc sa valeur ascii/int*/
    noeud *tmp_noeud = calloc(1, sizeof(noeud));
    if (!tmp_noeud)
        fprintf(stderr, "tmp_noeud erreur a la ligne %d\n", __LINE__);

    tmp_noeud->c = index;
    tmp_noeud->occ = tab[index];
    return tmp_noeud;

}

void creer_code(noeud *element) {
    creer_code_aux(element, 0, 0);
}

void creer_code_aux(noeud *element, int code, int profondeur){
  if (est_feuille(element)){
    element->enc = code;
    element->nb_bit = profondeur;
  }
  else{
    creer_code_aux(element->gauche, code << 1, profondeur+1);
    creer_code_aux(element->droit, (code << 1)+1, profondeur+1);
  }
}

void affichage_code(int nbr_bits, int codage){
  int i = nbr_bits;
  int n = codage;
  while(i){
    printf("%d", n&1 );
    n = n >> 1;
    i--;
  }
}

int compterNoeud(noeud **arbre){
  int i=0;
  int compt=0;
  for (i=0; i<N_CHAR; i++)
    if (arbre[i])
      compt++;
  return compt;
}
/* READ */
/* renvoie le noeud suivant du file */
/* lit le fichier et renvoie le tableau d'huffman associé (alphabet)*/

void read_header(FILE* f, char* originalName, noeud** arbre){
  int nb_noeud;
  fscanf(f, "FILE:<%s>\n", originalName);
  fscanf(f, "%d\n", &nb_noeud);
  read_huffman(f, arbre, nb_noeud);
}

void read_huffman(FILE* f, noeud** alphabet, int nb_noeud){
  int i;
  noeud* tmp;
    for (i=0; i<nb_noeud; i++){
        tmp = malloc(sizeof(noeud));
        read_noeud(f, tmp);
        alphabet[tmp->c]= tmp;
    }
} 

void read_noeud(FILE* f, noeud* noeud){
        fread(&noeud, sizeof(noeud), 1, f);
}

void read_code(FILE* in, FILE* out, noeud** alphabet){
  /* on créé l'arbre a partir de l'alphabet */
  int i, j;
  int taille = 0;
  /* prev_pos utilisé avec ftell pour enregistrer la position dans le fichier */
  /* utile pour voir si on est arrivé a un tag FILE:"nom_fichier" */
  char c;
  int bit;
  noeud ** arbre_huffman = NULL;
  arbre_huffman = calloc(N_CHAR, sizeof(noeud*));
  if (!arbre_huffman)
    fprintf(stderr, "arbre_huffman erreur a la ligne %d\n", __LINE__);
  noeud* ptr_noeud = malloc(sizeof(noeud*));


  /* création d'une copie de alphabet, pour le transformer en arbre d'huffman*/
  for (i=0; i<N_CHAR; i++){
    if (alphabet[i]){
      taille++;

      arbre_huffman[i]=calloc(1, sizeof(noeud));
      if (!arbre_huffman[i])
        fprintf(stderr, "erreur alloc arbre_huffman noeud a la ligne %d\n", __LINE__);

      arbre_huffman[i]->c = alphabet[i]->c;
      arbre_huffman[i]->occ = alphabet[i]->occ;
 
    }
  }
 /* création de l'arbre */ 
  while(taille > 1){
    creer_noeud(arbre_huffman, taille);
    taille--;
  }
/* trouver la racine de l'arbre */
  while(!arbre_huffman[i])
    i++;

  ptr_noeud = arbre_huffman[i];
  while ((c = fgetc(in)) != EOF ){
    if (c == 'F'){
      ungetc(c, in);
      if (test_FILEtag(in))
        /* arrivé a un tag FILE, donc début du fichier suivant */
        break;
      c = fgetc(in);
    }

    for (j=7; j>=0; j--){
      bit = c | (1<<j);
      if (parcours_arbre(bit, ptr_noeud)){
        fputc(ptr_noeud->c, out);
        ptr_noeud = arbre_huffman[i];
      }
    } 
  }
}

int test_FILEtag(FILE* f){
  /* ftell donne la position actuelle dans le fichier, après le check, réussite ou pas, on reviens la ou on etais, juste avant le 'F' */
  long int prev_pos = ftell(f);
  if (fgetc(f) == 'F'
      && fgetc(f) == 'I'
      && fgetc(f) == 'L'
      && fgetc(f) == 'E'
      && fgetc(f) == ':'
      && fgetc(f) == '<'){
    /* TRES FORTEMENT probable que nous somme au début d'un fichier, le suivant */
    fseek(f, prev_pos, SEEK_CUR);
    return 1;
  }
  /* nous ne sommes pas au début d'un fichier */
  fseek(f, prev_pos, SEEK_CUR);
  return 0;

}

int parcours_arbre(int bit, noeud* ptr_noeud){
  /* avance d'un noeud dans l'arbre selon le bit, renvoie 1 si on est a la fin de l'arbre, dc le noeud actuel est la valeur de l'encodage */
  if (!ptr_noeud)
    fprintf(stderr, "erreur parcours_arbre pas censé etre ici\n");

  if (!bit){
    if (ptr_noeud->gauche)
      ptr_noeud=ptr_noeud->gauche;
    else
      fprintf(stderr, "erreur parcours_arbre pas censé etre ici\n");
  }

  if (bit){
    if (ptr_noeud->droit)
      ptr_noeud=ptr_noeud->droit;
    else
      fprintf(stderr, "erreur parcours_arbre pas censé etre ici\n");
  }
  
  if(est_feuille(ptr_noeud))
    return 1;
  return 0;
}

/* BIT READ */

/* WRITE */
void write_header(FILE* f, char* originalName, noeud** arbre){
  fprintf(f, "FILE:<%s>\n", originalName);
  fprintf(f, "%d\n", compterNoeud(arbre) );
  write_huffman(f, arbre);
}

/*ecrit le TABLEAU dans le fichier, ne marche pas avec des arbres( pas d'appel récursifs)*/
void write_huffman(FILE*  f, noeud** arbre){
  int i;
  for (i=0; i<N_CHAR; i++)
    if (arbre[i] && arbre[i]->occ)
      write_noeud(f, arbre[i]);
}

void write_noeud(FILE*  f, noeud* noeud){
  /* printf("char n°%d :", noeud->c); */
  fwrite(&noeud, sizeof(struct noeud), 1, f);
}

void write_code(FILE*  in, FILE* out, noeud** alphabet){
  int c = 0;
  /* ici on ecrits par paquets de 8bits (1o =1char, minimum writeable), on met donc l'encodeage d'un char dans le BUFFER, puis s'il depasse 8bits, on ecrits 8bits, les enleve du buffer, etc... */
  buffer* BUFFER = malloc(sizeof(buffer)); 
  init_buffer(BUFFER);

  /* remet la tete de lecture du fichier à compresser au début du fichier */
  rewind(in);
  /* normalement out est positionné juste après l'écriture du header, idéal */
  while ( (c = fgetc(in)) != EOF ){
    /* on ecrit l'encodage du char c (si supporté) dans le buffer (a droite) */
    if (c<0 || c>127)
      fprintf(stderr, "CHAR non supp\n");
    else
      append_encbits(c, BUFFER, alphabet);

    /* si buffer a plus de 8 bits, on les écrits, jusqua buffsize<8 (supprime les 8bits a gauche du buffer) */
    while(BUFFER->size >= 8)
      write_8bits(BUFFER, out);
  printf("BREAK1\n");
  }
  printf("BREAK1\n");
  write_leftover(BUFFER, out);
  printf("BREAK2\n");
}

void init_buffer(buffer* BUFFER){
  BUFFER->bits = 0;
  BUFFER->size = 0;
}

int get_enc(int c, noeud** alphabet){
  return alphabet[c]->enc;
}

int get_nb(int c, noeud** alphabet){
  return alphabet[c]->nb_bit;
}
/* BIT WRITE */
/*enqueue_bit : rajoute 1 bit, "à droite" du buffer si n == 0, 0, sinon 1*/
void enq_bit(int n, buffer* b){
  /* on créé 1 espace a droite du buffer */
  b->bits = b->bits << 1;
  b->size++;
/*si n != 0, on OR le buffer avec la valeur 1, donc change 1er bit, le <<0 shift le 1, et permet de manipuler des bits individuellement sans avoir a écrire |= 2, |=4, ou toutes les puissances de 2 pour manipuler les bits individuellements*/
 if (n)
   /* si le bit a ajouter est non-null */
   b->bits |= (1 << 0);
}

void append_encbits(char c,buffer* b, noeud** alphabet){
  int i;
  /* bit sera soit 0 soit 1*/
  int bit;
  int length = get_nb(c, alphabet);
  int enc = get_enc(c, alphabet);
  for (i=0; i<length; i++){
    /* recup le ieme bit de enc */
    bit = enc & (1 << i) ;
    /* l'ajoute au buffer */
    enq_bit(bit, b);
  }
}

/* enleve 8 bits a gauche du buffer (les plus anciens)*/
void write_8bits(buffer* b, FILE* out){
  int i = 0;
  int buffsize = b->size;
  char write_buff = 0;
  /* prendre le buffsize-ieme char, mettre dans write_buff, repeter jusqu'a size-7 ieme */
  /* si on inverse 7 et 0 dans l'entete 'for' ici, cela inverse l'ordre d'ecriture des bits au fichier */
  for (i=0; i<=7; i-- ){
        if (b->bits & (1<<(buffsize-i)))
          write_buff |= 1 << (7-i);
  }
  /* on ecrit 1 byte, 1 fois dans out */
  fwrite(&write_buff, 1, 1, out);
  /* une fois que l'on as recup les 8bits, on reduit la taille, pas besoin de remettre les bits a 0, marche comme une pile */
  b->size -= 8;
}

void write_leftover(buffer* b, FILE* out){
  /* on sait qu'il reste entre 1 et 7 bits, buffer->size<8 */
    /* le dernier paquet de l'encodage est le nombre de bits utiles (un char 0<n<8) */
  int i = 0;
  int buffsize = b->size;
  char write_buff = 0;
  for (i=0; i<=7; i-- ){
    /* si on est a cours de bits à ecrire dans le buffer, on écrit des 0 */
        if (buffsize-i<0)
          write_buff = write_buff << 1;
        else if (b->bits & (1<<(buffsize-i)))
          write_buff |= 1 << (7-i);
  }
  fwrite(&write_buff, 1, 1, out);
  /* a la fin de l'encodage, on écrit la taille utile du dernier octet écrit */
  fprintf(out, "%c", b->size); 
  b->size = 0;
}


/* DESTRUCTION*/

void detruire_arbre_huff(noeud *tab[N_CHAR]){
  int i;
  for (i=0; i<N_CHAR; i++)
    if (tab[i] && est_feuille(tab[i]))
      detruire_noeud(tab[i]);
}

/*enqueue_bit : rajoute 1 bit, "à droite" du buffer si n == 0, 0, sinon 1*/
void enq_bit(int n, buffer *b) {
    /* on créé 1 espace a droite du buffer */
    b->bits = b->bits << 1;
    b->size++;
/*operateur ternaire <=> si n != 0, on OR le buffer avec la valeur 1, donc change 1er bit, le <<0 shift le 1, et permet de manipuler des bits individuellement sans avoir a écrire |= 2, |=4, ou toutes les puissances de 2 pour manipuler les bits individuellements*/
    if (n)
        /* si le bit a ajouté est non-null */
        b->bits |= (1 << 0);
}

void append_bits(char c, buffer *b, noeud **alphabet) {
    int i;
    /* bit sera soit 0 soit 1*/
    int bit;
    int length = get_nb(c, alphabet);
    int enc = get_enc(c, alphabet);
    for (i = 0; i < length; i++) {
        /* recup le ieme bit de enc */
        bit = enc & (1 << i);
        /* l'ajoute au buffer */
        enq_bit(bit, b);
    }
}

/* enleve 8 bits a gauche du buffer (les plus anciens)*/
void write_8bits(buffer *b, FILE *out) {
    int i = 0;
    int buffsize = b->size;
    char write_buff = 0;
    /* prendre le buffsize-ieme char, mettre dans write_buff, repeter jusqu'a size-7 ieme */
    /* si on inverse 7 et 0 dans l'entete 'for' ici, cela inverse l'ordre d'ecriture des bits au fichier */
    for (i = 7; i >= 0; i--) {
        if (b->bits & (1 << (buffsize - i)))
            write_buff |= 1 << (7 - i);
    }
    /* on ecrit 1 byte, 1 fois dans out */
    fwrite(&write_buff, 1, 1, out);
    /* une fois que l'on as recup les 8bits, on reduit la taille, pas besoin de remettre les bits a 0, marche comme une pile */
    b->size -= 8;
}


/* DESTRUCTION*/

void detruire_arbre_huff(noeud *tab[N_CHAR]) {
    int i;
    for (i = 0; i < N_CHAR; i++)
        if (tab[i] && est_feuille(tab[i]))
            detruire_noeud(tab[i]);
}

void detruire_noeud(noeud *noeud) {
    if (noeud->gauche)
        detruire_noeud(noeud->gauche);
    if (noeud->droit)
        detruire_noeud(noeud->droit);
    detruire_feuille(noeud);
}

void detruire_feuille(noeud *noeud) {
    free(noeud);
}

int est_feuille(noeud *noeud) {
    return !(noeud->gauche || noeud->droit);
}


void find2Lowest(noeud **arbre, int nbElement, int *low1, int *low2) {
    /* indices des 2 premiers elmt non vides de tab */
    int i = 0;
    if (arbre) {

        if (nbElement == 0) {
            fprintf(stderr, "erreur, tab de taille 0 dans find2Lowest, low1 et low2 non changés \n");
            return;
        }

        if (nbElement == 1) {
            /* tant que arbre[i] pas défini (donc pas d'occurence) */
            while (!arbre[i])
                i++;

            fprintf(stderr, "erreur, tab de taille 1 dans find2Lowest, on donne 2 fois le même en return\n");
            *low1 = i;
            *low2 = i;
            return;
        }

        /*si taille != 0 ou != 1, le tableau est normal, on le traite :*/

        /* i1, i2, les indices des 2 plus petits elements */
        int i1, i2;
        i1 = i2 = 0;
        /* e1, e2, les 2 plus petits elements */
        int e1, e2;
        e1 = e2 = INT_MAX;

        for (i = 0; i < N_CHAR; i++) {
            if (arbre[i] && arbre[i]->occ < e1) {
                e2 = e1;
                i2 = i1;
                e1 = arbre[i]->occ;
                i1 = i;
            } else if (arbre[i] && arbre[i]->occ < e2) {
                e2 = arbre[i]->occ;
                i2 = i;
            }
        }
        /* IMPORTANT LA FONCTION RETOURNE LES >>INDICES<< DES 2 PLUS PETITS, PLUS FLEXIBLE */
        *low1 = i1;
        *low2 = i2;
        return;
    } else {
        fprintf(stderr, "erreur, tab vide donné a find2Lowest\n");
        return;
    }
}

int main(int argc, char **argv) {
    FILE *fin;
    int *occ = calloc(N_CHAR, sizeof(int));
    if (!occ)
        fprintf(stderr, "occ erreur a la ligne %d\n", __LINE__);
    int i;
    int taille = 0;

  FILE* fin;
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
  fin = fopen(argv[1], "r");
  /* f = fopen(argv[1], "rb"); */
  if (!fin){
    fprintf(stderr,"fichier impossible à ouvrir\n");
    exit(EXIT_FAILURE);
  }
  occurence(fin, occ);
  noeud ** arbre_huffman = NULL;
  arbre_huffman = calloc(N_CHAR, sizeof(noeud*));
  noeud ** alphabet = NULL;
  alphabet = calloc(N_CHAR, sizeof(noeud*));

    for (i = 0; i < N_CHAR; i++) {
        if (occ[i]) {
            taille++;
            arbre_huffman[i] = creer_feuille(occ, i);
            alphabet[i] = arbre_huffman[i];
        }
    }

  while(taille > 1){
    creer_noeud(arbre_huffman, taille);
    taille--;
  }

  i=0;
  while(!arbre_huffman[i])
    i++;

  /* on sait que i est non-vide, pour tester: */
  char *compname = (char *) malloc(strlen(argv[1])+5 );
  if (!compname){
    fprintf(stderr, "compname erreur a la ligne %d\n", __LINE__);
    exit(EXIT_FAILURE);
  }

  strcpy(compname, argv[1]);
  compname = strcat(compname, "comp");
  FILE *fcomp = fopen(compname, "wb");
  if (!fcomp){
    fprintf(stderr,"fichier impossible à ouvrir\n");
    exit(EXIT_FAILURE);
  }

  creer_code(arbre_huffman[i]);
  write_header(fcomp, argv[1], alphabet);
  write_code(fin, fcomp, alphabet);

    /*     if (arbre_huffman) */
    /*       detruire_arbre_huff(arbre_huffman); */
    /*     if (occ) */
    /*       free(occ); */

}






