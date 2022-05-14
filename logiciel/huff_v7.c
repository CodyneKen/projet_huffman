#include "huff.h"

void occurrence(FILE *fic, int tab[N_CHAR]) {
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

void creer_code_aux(noeud *element, int code, int profondeur) {
    if (est_feuille(element)) {
        element->enc = code;
        element->nb_bit = profondeur;
    } else {
        creer_code_aux(element->gauche, code << 1, profondeur + 1);
        creer_code_aux(element->droit, (code << 1) + 1, profondeur + 1);
    }
}

void affichage_code(int nbr_bits, int codage) {
    int i = nbr_bits;
    int n = codage;
    while (i) {
        printf("%d", n & 1);
        n = n >> 1;
        i--;
    }
}

int compterNoeud(noeud **arbre) {
    int i = 0;
    int compt = 0;
    for (i = 0; i < N_CHAR; i++)
        if (arbre[i])
            compt++;
    return compt;
}

/* READ/WRITE */
void write_header(char *originalName, FILE *f, noeud **arbre) {
    fprintf(f, "FILE:%s\n", originalName);
    fprintf(f, "%d\n", compterNoeud(arbre));
    write_huffman(f, arbre);
}

/*ecrit le TABLEAU dans le fichier, ne marche pas avec des arbres( pas d'appel récursifs)*/
void write_huffman(FILE *f, noeud **arbre) {
    int i;
    for (i = 0; i < N_CHAR; i++)
        if (arbre[i] && arbre[i]->occ)
            write_noeud(f, arbre[i]);
}

void write_noeud(FILE *f, noeud *noeud) {
    /* printf("char n°%d :", noeud->c); */
    fwrite(&noeud, sizeof(struct noeud), 1, f);
}

void write_code(FILE *in, FILE *out, noeud **alphabet) {
    int c = 0;
    /* la strategie ici est d'ecrire par paquets de 8bits (1o =1char, minimum writeable), on met donc l'encodeage d'un char dans le BUFFER, puis s'il depasse 8bits, on ecrits 8bits, les enleve du buffer, etc... */
    buffer *BUFFER = malloc(sizeof(buffer));
    init_buffer(BUFFER);

    /* remet la tete de lecture du fichier à compresser au début du fichier */
    rewind(in);
    /* normalement out est positionné juste après l'écriture du header, idéal */
    while ((c = fgetc(in)) != EOF) {
        /* si buffer a plus de 8 bits, on les écrit, jusqu'a buffsize<8 (supprime les 8bits à gauche du buffer) */
        while (BUFFER->size > 8)
            write_8bits(BUFFER, out);
        /* on écrit l'encodage du char c dans le buffer (a droite) */
        append_bits(c, BUFFER, alphabet);
    }
}

void init_buffer(buffer *BUFFER) {
    BUFFER->bits = 0;
    BUFFER->size = 0;
}

int get_enc(int c, noeud **alphabet) {
    return alphabet[c]->enc;
}

int get_nb(int c, noeud **alphabet) {
    return alphabet[c]->nb_bit;
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
            /* tant qu'arbre[i] pas défini (donc pas d'occurence) */
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

char *get_extension(char *name) {
    char *point = strrchr(name, '.'); /* Permet de trouver le dernier '.' de name */
    if (!point) {
        fprintf(stderr, "Le fichier n'a pas d'extension\n");
        exit(EXIT_FAILURE);
    }
    return point + 1;
}

void usage(int argc, char **argv) {
    fprintf(stderr, "Usage %s [Options] <file_name> [<directory>]\n", argv[0]);
    fprintf(stderr, "Options :\n");
    fprintf(stderr, "\t-c\t: fait une compression de <file_name>;\n");
    fprintf(stderr,
            "\t-d\t: fait une decompression de <file_name> si <directory> est renseigne, decompresse dans ce repertoire;\n");
    fprintf(stderr, "\t-h\t: affiche le man;\n");
    exit(EXIT_FAILURE);
}


void print_man(char **argv) {
    int i;
    fprintf(stderr, "%s(3)\tPersonnal C Functions\t%s(3)\n", argv[0], argv[0]);
    fprintf(stderr, "NAME\n\tnoeud **compression(),\n");
    fprintf(stderr, "\nSYNOPSYS\n\t#include \"huff.h\"\n");
    fprintf(stderr, "compression(FILE *f) va lancer une compression du fichier par l'arbre de Huffman\n");
    fprintf(stderr,
            "decompression(FILE *f) va lancer une decompression du fichier en recreant l'arbre de Huffman\n");
    fprintf(stderr, "\n");
    exit(EXIT_FAILURE);
}

FILE *launch_comp(FILE *file, char *fname) {
    /* Dans cette fonction a partir d'un fichier on va lancer sa compression.*/
    int i, taille = 0;
    int *occ = NULL;
    noeud **arbre_huffman = NULL, **alphabet = NULL;
    occ = calloc(N_CHAR, sizeof(int));
    if (!occ) {
        fprintf(stderr, "Erreur lors de l'allocation memoire de occ\n");
        exit(EXIT_FAILURE);
    }
    arbre_huffman = calloc(N_CHAR, sizeof(noeud *));
    if (!arbre_huffman) {
        fprintf(stderr, "Erreur lors de l'allocation memoire de arbre_huffman\n");
        exit(EXIT_FAILURE);
    }
    alphabet = calloc(N_CHAR, sizeof(noeud *));
    if (!alphabet) {
        fprintf(stderr, "Erreur lors de l'allocation memoire de alphabet\n");
        exit(EXIT_FAILURE);
    }
    occurrence(file, occ);
    if (!arbre_huffman)
        fprintf(stderr, "arbre_huffman erreur a la ligne %d\n", __LINE__);
    if (!alphabet)
        fprintf(stderr, "alphabet erreur a la ligne %d\n", __LINE__);

    for (i = 0; i < N_CHAR; i++) {
        if (occ[i]) {
            taille++;
            arbre_huffman[i] = creer_feuille(occ, i);
            alphabet[i] = arbre_huffman[i];
        }
    }

    while (taille > 1) {
        creer_noeud(arbre_huffman, taille);
        taille--;
    }

    i = 0;
    while (!arbre_huffman[i])
        i++;

    /* on sait que i est non-vide, pour tester: */
    char *compname = (char *) malloc((strlen(fname) + 4) * sizeof(char));
    if (!compname) {
        fprintf(stderr, "Erreur de l'attribution memoire de compname\n");
        exit(EXIT_FAILURE);
    }
    strcpy(compname, fname);
    compname = strcat(compname, "comp");
    FILE *fcomp = fopen(compname, "wb");
    if (!fcomp) {
        fprintf(stderr, "fichier impossible à ouvrir. Veuillez verifier son existence\n");
        exit(EXIT_FAILURE);
    }

    creer_code(arbre_huffman[i]);
    write_header(fname, fcomp, alphabet);
    write_code(file, fcomp, alphabet);
    return fcomp;
}

FILE *launch_decomp(FILE *, char *directory) {
    FILE *fout = NULL;
    fprintf(stdout, "En construction\n");
    return fout;
}

int main(int argc, char **argv) {

    FILE *fin, *fout;
    int *occ;
    occ = calloc(N_CHAR, sizeof(int));
    if (!occ)
        fprintf(stderr, "occ erreur a la ligne %d\n", __LINE__);
    int i, c, taille = 0;
    int compression, decompression, multi_files;

    char *ext;

    compression = decompression = multi_files = 0;


    /* Traitement des arguments de la ligne de commande a l'aide de la fonction getopt */
    while ((c = getopt(argc, argv, "cd:h")) != -1) {
        switch (c) {
            case 'c':
                compression = 1;
                break;
            case 'd':
                decompression = 1;
                break;
            case 'h':
                print_man(argv);
            default:
                usage(argc, argv);
        }
    }

    /* On teste si on a suffisamment d'arguments ou si on veut une compression et une decompression en meme temps*/
    if (argc < 3 || (compression == 1 && decompression == 1)) usage(argc, argv);
    /* on essaye d'ouvrir le fichier, erreur sinon :*/
    fin = fopen(argv[2], "r");
    /* f = fopen(argv[1], "rb"); */
    if (!fin) {
        fprintf(stderr, "fichier impossible à ouvrir\n");
        exit(EXIT_FAILURE);
    }
    ext = get_extension(argv[2]);
    if (compression && strcmp(ext, "txt"))
        fout = launch_comp(fin, argv[2]);
    else if (compression && !strcmp(ext, "txt")) {
        fprintf(stderr, "Ce type de fichier n'est pas pris en charge\n");
        exit(EXIT_FAILURE);
    }

    if (decompression && argv[3])
        fout = launch_decomp(fin, argv[3]);
    else if (decompression && IS_POSIX == 1)
        /* getenv permet d'accéder aux variables d'environnement */
        fout = launch_decomp(fin, getenv("PWD"));

    /*     if (arbre_huffman) */
    /*       detruire_arbre_huff(arbre_huffman); */
    /*     if (occ) */
    /*       free(occ); */

}






