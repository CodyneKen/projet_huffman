#Auteur : Baptise Jeudy, pour le cours d'algorithmes des graphes de L2 Info Jean Monnet St-Etienne
#Modifié par Corentin Bohelay (réutilise souvent car pratique)

# le(s) programme(s) à générer
cible    := huff_v0
# fichiers objets dont le .c n'est pas fourni 
# inc_objf := $(addprefix include/, graphe_TLA.o  liste.o)
# options de compilation
CFLAGS = -g -Wall
# fichiers dans ./include
inc_h    := $(wildcard *.h)
inc_c    := $(wildcard *.c)
# fichiers objets à générer
inc_obj  := $(inc_c:.c=.o)
# fichiers à effacer lors d'un make clean
toclean := $(cible) $(inc_obj)

.PHONY: all
all: $(cible)

# les fichiers objets dépendent de tous les .h
$(inc_obj): $(inc_h)

# la cible dépend de tous les fichiers objets
$(cible): $(inc_obj) $(inc_objf)

.PHONY: clean
clean:
	rm -f $(toclean)

