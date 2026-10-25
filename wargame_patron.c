//git add .                        # prepare toutes les modifications pour le commit
//git commit -m "Description"      # enregistre une version avec un message
//git push                         # envoie les commits sur GitHub
//gcc -Wall -g -o wargame wargame_patron.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define NB_LIGNES 10
#define NB_COLONNES 10
#define INFINI 10000


int PROF_MAX = 3; //profondeur max de l'algo minimax 
int elagage = 1; //1 pour activer l'élagage alpha-beta, 0 pour le désactiver
long nb_noeuds = 0;

long cumul_noeuds_x = 0;
double cumul_temps_x = 0.0;
int nb_coups_x = 0;

long cumul_noeuds_o = 0;
double cumul_temps_o = 0.0;
int nb_coups_o = 0;


#define MAX(a, b) ((a) > (b) ? (a) : (b)) //car n'existe pas de MAX et MIN
#define MIN(a, b) ((a) < (b) ? (a) : (b))

/*#define DEBUG*/

typedef struct pion_s
{
	int couleur;
	int valeur;
}Pion;

Pion *plateauDeJeu;


void f_affiche_plateau(Pion *plateau);
int f_convert_char2int(char c);
char f_convert_int2char(int i);

int f_min(Pion* plateau, int profondeur, int alpha, int beta);
int f_max(Pion* plateau, int profondeur, int alpha, int beta);
int f_avance(Pion* plateau, int joueur);



int f_convert_char2int(char c)
{
#ifdef DEBUG
	printf("dbg: entering %s %d\n", __FUNCTION__, __LINE__);
#endif
	if(c>='A' && c<='Z')
		return (int)(c-'A');
	if(c>='a' && c<='z')
		return (int)(c-'a');
	return -1;
}

char f_convert_int2char(int i)
{
#ifdef DEBUG
	printf("dbg: entering %s %d\n", __FUNCTION__, __LINE__);
#endif

	return (char)i+'A';
}

Pion *f_init_plateau()
{
	int i, j;
	Pion *plateau=NULL;


#ifdef DEBUG
	printf("dbg: entering %s %d\n", __FUNCTION__, __LINE__);
#endif

	plateau = (Pion *)malloc(NB_LIGNES*NB_COLONNES*sizeof(Pion));
	if(plateau == NULL)
	{
		printf("error: unable to allocate memory\n");
		exit(EXIT_FAILURE);
	}

	for(i=0; i<NB_LIGNES; i++)
	{
		for(j=0; j<NB_COLONNES; j++)
		{
			plateau[i*NB_COLONNES+j].couleur = 0;
			plateau[i*NB_COLONNES+j].valeur = 0;
		}
	}

	plateau[9*NB_COLONNES+5].couleur = 1;
	plateau[9*NB_COLONNES+5].valeur = 1;

	plateau[9*NB_COLONNES+6].couleur = 1;
	plateau[9*NB_COLONNES+6].valeur = 2;

	plateau[9*NB_COLONNES+7].couleur = 1;
	plateau[9*NB_COLONNES+7].valeur = 3;

	plateau[9*NB_COLONNES+8].couleur = 1;
	plateau[9*NB_COLONNES+8].valeur = 2;

	plateau[9*NB_COLONNES+9].couleur = 1;
	plateau[9*NB_COLONNES+9].valeur = 1;

	plateau[8*NB_COLONNES+0].couleur = 1;
	plateau[8*NB_COLONNES+0].valeur = 1;

	plateau[8*NB_COLONNES+1].couleur = 1;
	plateau[8*NB_COLONNES+1].valeur = 3;

	plateau[8*NB_COLONNES+2].couleur = 1;
	plateau[8*NB_COLONNES+2].valeur = 3;

	plateau[8*NB_COLONNES+3].couleur = 1;
	plateau[8*NB_COLONNES+3].valeur = 1;

	plateau[8*NB_COLONNES+6].couleur = 1;
	plateau[8*NB_COLONNES+6].valeur = 1;

	plateau[8*NB_COLONNES+7].couleur = 1;
	plateau[8*NB_COLONNES+7].valeur = 1;

	plateau[8*NB_COLONNES+8].couleur = 1;
	plateau[8*NB_COLONNES+8].valeur = 1;

	plateau[7*NB_COLONNES+1].couleur = 1;
	plateau[7*NB_COLONNES+1].valeur = 1;

	plateau[7*NB_COLONNES+2].couleur = 1;
	plateau[7*NB_COLONNES+2].valeur = 1;

	plateau[2*NB_COLONNES+7].couleur = -1;
	plateau[2*NB_COLONNES+7].valeur = 1;

	plateau[2*NB_COLONNES+8].couleur = -1;
	plateau[2*NB_COLONNES+8].valeur = 1;

	plateau[1*NB_COLONNES+1].couleur = -1;
	plateau[1*NB_COLONNES+1].valeur = 1;

	plateau[1*NB_COLONNES+2].couleur = -1;
	plateau[1*NB_COLONNES+2].valeur = 1;

	plateau[1*NB_COLONNES+3].couleur = -1;
	plateau[1*NB_COLONNES+3].valeur = 1;

	plateau[1*NB_COLONNES+6].couleur = -1;
	plateau[1*NB_COLONNES+6].valeur = 1;

	plateau[1*NB_COLONNES+7].couleur = -1;
	plateau[1*NB_COLONNES+7].valeur = 3;

	plateau[1*NB_COLONNES+8].couleur = -1;
	plateau[1*NB_COLONNES+8].valeur = 3;

	plateau[1*NB_COLONNES+9].couleur = -1;
	plateau[1*NB_COLONNES+9].valeur = 1;

	plateau[0*NB_COLONNES+0].couleur = -1;
	plateau[0*NB_COLONNES+0].valeur = 1;

	plateau[0*NB_COLONNES+1].couleur = -1;
	plateau[0*NB_COLONNES+1].valeur = 2;

	plateau[0*NB_COLONNES+2].couleur = -1;
	plateau[0*NB_COLONNES+2].valeur = 3;

	plateau[0*NB_COLONNES+3].couleur = -1;
	plateau[0*NB_COLONNES+3].valeur = 2;

	plateau[0*NB_COLONNES+4].couleur = -1;
	plateau[0*NB_COLONNES+4].valeur = 1;

#ifdef DEBUG
printf("dbg: exiting %s %d\n", __FUNCTION__, __LINE__);
#endif

return plateau;
}

void f_affiche_plateau(Pion *plateau)
{
	int i,j,k;


#ifdef DEBUG
	printf("dbg: entering %s %d\n", __FUNCTION__, __LINE__);
#endif

	printf("\n    ");
	for(k=0; k<NB_COLONNES; k++)
		printf("%2c ",f_convert_int2char(k));
	printf("\n    ");
	for(k=0; k<NB_COLONNES; k++)
		printf("-- ");
	printf("\n");
	for(i=NB_LIGNES-1; i>=0; i--)
	{
		printf("%2d ", i);
		for(j=0; j<NB_COLONNES; j++)
		{
			printf("|");
			switch(plateau[i*NB_COLONNES+j].couleur)
			{
			case -1:
				printf("%do",plateau[i*NB_COLONNES+j].valeur);
				break;
			case 1:
				printf("%dx",plateau[i*NB_COLONNES+j].valeur);
				break;
			default:
				printf("  ");
			}
		}
		printf("|\n    ");
		for(k=0; k<NB_COLONNES; k++)
			printf("-- ");
		printf("\n");
	}
	printf("    ");

#ifdef DEBUG
	printf("dbg: exiting %s %d\n", __FUNCTION__, __LINE__);
#endif
}


int f_gagnant()
{
	int i, j, somme1=0, somme2=0;


#ifdef DEBUG
	printf("dbg: entering %s %d\n", __FUNCTION__, __LINE__);
#endif

	//Quelqu'un est-il arrive sur la ligne de l'autre
	for(i=0; i<NB_COLONNES; i++)
	{
		if(plateauDeJeu[i].couleur == 1)
			return 1;
		if(plateauDeJeu[(NB_LIGNES-1)*NB_COLONNES+i].couleur == -1)
			return -1;
	}

	//taille des armees
	for(i=0; i<NB_LIGNES; i++)
	{
		for(j=0; j<NB_COLONNES; j++)
		{
			if(plateauDeJeu[i*NB_COLONNES+j].couleur == 1)
				somme1++;
			if(plateauDeJeu[i*NB_COLONNES+j].couleur == -1)
				somme2++;
		}
	}
	if(somme1==0)
		return -1;
	if(somme2==0)
		return 1;

#ifdef DEBUG
	printf("dbg: exiting %s %d\n", __FUNCTION__, __LINE__);
#endif
	return 0;
}


/**
 * Prend comme argument la ligne et la colonne de la case
 * 	pour laquelle la bataille a lieu
 * Renvoie le couleur du gagnant
 * */
int f_bataille(Pion *plateau, int l, int c)
{
    int i, j, mini, maxi, minj, maxj;
    int somme = 0;

#ifdef DEBUG
    printf("dbg: entering %s %d\n", __FUNCTION__, __LINE__);
#endif
    mini = (l - 1 < 0) ? 0 : l - 1;
    maxi = (l + 1 > NB_LIGNES - 1) ? NB_LIGNES - 1 : l + 1;
    minj = (c - 1 < 0) ? 0 : c - 1;
    maxj = (c + 1 > NB_COLONNES - 1) ? NB_COLONNES - 1 : c + 1;

    for (i = mini; i <= maxi; i++)
    {
        for (j = minj; j <= maxj; j++)
        {
            somme += plateau[i * NB_COLONNES + j].couleur * plateau[i * NB_COLONNES + j].valeur;
        }
    }
    somme -= plateau[l * NB_COLONNES + c].couleur * plateau[l * NB_COLONNES + c].valeur;

#ifdef DEBUG
    printf("dbg: exiting %s %d\n", __FUNCTION__, __LINE__);
#endif
    if (somme < 0)
        return -1;
    if (somme > 0)
        return 1;

    return plateau[l * NB_COLONNES + c].couleur;
}

/**
 * Prend la ligne et colonne de la case d'origine
 * 	et la ligne et colonne de la case de destination
 * Renvoie 1 en cas d'erreur
 * Renvoie 0 sinon
 * */
int f_test_mouvement(Pion *plateau, int l1, int c1, int l2, int c2, int couleur)
{
#ifdef DEBUG
	printf("dbg: entering %s %d\n", __FUNCTION__, __LINE__);
	printf("de (%d,%d) vers (%d,%d)\n", l1, c1, l2, c2);
#endif
	/* Erreur, hors du plateau */
	if(l1 < 0 || l1 >= NB_LIGNES || l2 < 0 || l2 >= NB_LIGNES ||
			c1 < 0 || c1 >= NB_COLONNES || c2 < 0 || c2 >= NB_COLONNES)
		return 1;
	/* Erreur, il n'y a pas de pion a deplacer ou le pion n'appartient pas au joueur*/
	if(plateau[l1*NB_COLONNES+c1].valeur == 0 || plateau[l1*NB_COLONNES+c1].couleur != couleur)
		return 1;
	/* Erreur, tentative de tir fratricide */
	if(plateau[l2*NB_COLONNES+c2].couleur == plateau[l1*NB_COLONNES+c1].couleur)
		return 1;

	if(l1-l2 >1 || l2-l1 >1 || c1-c2 >1 || c2-c1 >1 || (l1==l2 && c1==c2))
		return 1;
#ifdef DEBUG
	printf("dbg: exiting %s %d\n", __FUNCTION__, __LINE__);
#endif
	return 0;
}


/**
 * Prend la ligne et colonne de la case d'origine
 * 	et la ligne et colonne de la case de destination
 *  et effectue le trantement de l'operation demandée
 * Renvoie 1 en cas d'erreur
 * Renvoie 0 sinon
 * */
int f_bouge_piece(Pion *plateau, int l1, int c1, int l2, int c2, int couleur)
{
	int gagnant=0;


#ifdef DEBUG
	printf("dbg: entering %s %d\n", __FUNCTION__, __LINE__);
#endif

	if(f_test_mouvement(plateau, l1, c1, l2, c2, couleur) != 0)
		return 1;


	/* Cas ou il n'y a personne a l'arrivee */
	if(plateau[l2*NB_COLONNES+c2].valeur == 0)
	{
		plateau[l2*NB_COLONNES+c2].couleur = plateau[l1*NB_COLONNES+c1].couleur;
		plateau[l2*NB_COLONNES+c2].valeur = plateau[l1*NB_COLONNES+c1].valeur;
		plateau[l1*NB_COLONNES+c1].couleur = 0;
		plateau[l1*NB_COLONNES+c1].valeur = 0;
	}
	else
	{
		gagnant=f_bataille(plateau, l2, c2);
		/* victoire */
		if(gagnant == couleur)
		{
			plateau[l2*NB_COLONNES+c2].couleur = plateau[l1*NB_COLONNES+c1].couleur;
			plateau[l2*NB_COLONNES+c2].valeur = plateau[l1*NB_COLONNES+c1].valeur;
			plateau[l1*NB_COLONNES+c1].couleur = 0;
			plateau[l1*NB_COLONNES+c1].valeur = 0;
		}
		/* defaite */
		else if(gagnant != 0)
		{
			plateau[l1*NB_COLONNES+c1].couleur = 0;
			plateau[l1*NB_COLONNES+c1].valeur = 0;
		}
	}

#ifdef DEBUG
	printf("dbg: exiting %s %d\n", __FUNCTION__, __LINE__);
#endif
	return 0;
}

//Calcul du nombre de pions sur le plateau du joueur
int f_nbPions(Pion* jeu, int joueur)
{
	int nbPion=0;
	int i, j;
	for (i = 0; i < NB_COLONNES; ++i)
	{
		for (j = 0; j < NB_LIGNES; ++j)
		{
			if (jeu[i * NB_COLONNES + j].couleur == joueur)
			{
				++nbPion;
			}
		}
	}
	return nbPion;
}

//Calcul de la valeur de tous les pions du joueur
int f_valeur(Pion* jeu, int joueur)
{
	int i, j;
	int valeur=0;
	for (i = 0; i < NB_COLONNES; ++i)
	{
		for (j = 0; j < NB_LIGNES; ++j)
		{
			if (jeu[i * NB_COLONNES + j].couleur == joueur)
			{
				valeur += jeu[i * NB_COLONNES + j].valeur;
			}
		}
	}
	return valeur;
}

/* un pion près des colonnes centrales (E, F) est plus polyvalent
 * qu'un pion sur le bord. Bonus de 0 (bord) à 8 (centre) par pion.
 * Sert surtout à départager des coups qui seraient sinon a egalite */
int f_centre(Pion *plateau, int joueur)
{
	int total = 0;
	for(int i = 0; i < NB_LIGNES; i++)
		for(int j = 0; j < NB_COLONNES; j++)
			if(plateau[i*NB_COLONNES+j].couleur == joueur)
			{
				int dist = abs(2*j - (NB_COLONNES-1));  
				total += (NB_COLONNES - dist);
			}
	return total;
}

/* un pion isolé n'a aucun allié parmi ses 8 voisins,
 * comme f_bataille() compare les sommes des voisins, un pion seul n'est
 * soutenu par personne et perd facilement son combat. */
int f_isoles(Pion *plateau, int joueur)
{
	int n = 0;
	for(int i = 0; i < NB_LIGNES; i++)
		for(int j = 0; j < NB_COLONNES; j++)
		{
			if(plateau[i*NB_COLONNES+j].couleur != joueur)
				continue;

			int allies = 0;
			for(int a = MAX(0, i-1); a <= MIN(NB_LIGNES-1, i+1); a++)
				for(int b = MAX(0, j-1); b <= MIN(NB_COLONNES-1, j+1); b++)
					if((a != i || b != j) && plateau[a*NB_COLONNES+b].couleur == joueur)
						allies++;

			if(allies == 0){n++;}
		}
	return n;
}

/**
 * Prend l'etat du jeu et le numéro du joueur
 * 	évalue la position (victoire, nombre de pions, avancement et matériel)
 * Renvoie un score positif si favorable, négatif si défavorable, ou +/- INFINI en cas de fin de partie
 */
int f_eval(Pion* jeu,int joueur)
{
	for(int k = 0; k < NB_COLONNES; k++)
	{
		if(jeu[k].couleur == 1)
			return joueur * INFINI;       
		if(jeu[(NB_LIGNES-1)*NB_COLONNES + k].couleur == -1)
			return -joueur * INFINI;      
	}


	int nbPions = f_nbPions(jeu, joueur) - f_nbPions(jeu, -joueur);
	int avance = f_avance(jeu, joueur) - f_avance(jeu, -joueur);
	int materiel = f_valeur(jeu, joueur) - f_valeur(jeu, -joueur);
	int centre = f_centre(jeu, joueur) - f_centre(jeu, -joueur);
	int isoles = f_isoles(jeu, joueur) - f_isoles(jeu, -joueur);

	return 10 * nbPions + 5 * avance + 2 * materiel + centre - 5 * isoles;
}

//copie du plateau
void f_copie_plateau(Pion* source, Pion* destination)
{
	int i, j;
	for (i = 0; i < NB_LIGNES; i++)
	{
		for (j = 0; j < NB_COLONNES; j++)
		{
			destination[i * NB_COLONNES + j].couleur = source[i * NB_COLONNES + j].couleur;
			destination[i * NB_COLONNES + j].valeur = source[i * NB_COLONNES + j].valeur;
		}
	}
}

//mise a zero du plateau
Pion* f_raz_plateau()
{
	Pion* jeu = NULL;
	int i, j;
	jeu = (Pion *) malloc(NB_LIGNES * NB_COLONNES * sizeof (Pion));
	for (i = 0; i < NB_LIGNES; i++)
	{
		for (j = 0; j < NB_COLONNES; j++)
		{
			jeu[i * NB_COLONNES + j].couleur = 0;
			jeu[i * NB_COLONNES + j].valeur = 0;
		}
	}
	return jeu;
}


/**
 * Fonction f_min : Minimise le score (tour de l'adversaire : -1 / o).
 * Explore les mouvements possibles pour le joueur -1, simule chaque coup 
 * sur une copie du plateau, appelle f_max et applique l'élagage Alpha-Bêta 
 * (arrête l'exploration si beta <= alpha).
 * Renvoie le pire score (minimum) pour le joueur 1.
 */

int f_min(Pion* plateau, int profondeur, int alpha, int beta)
{
	nb_noeuds++;

	if(profondeur == 0)
		return f_eval(plateau, 1);

	int pire_score = INFINI;

	for(int i1 = 0; i1 < NB_LIGNES; i1++)
	for(int j1 = 0; j1 < NB_COLONNES; j1++)
	{
		if(plateau[i1*NB_COLONNES+j1].couleur != -1)
			continue;

		for(int i2 = MAX(0, i1-1); i2 <= MIN(NB_LIGNES-1, i1+1); i2++)
		for(int j2 = MAX(0, j1-1); j2 <= MIN(NB_COLONNES-1, j1+1); j2++)
		{
			if(f_test_mouvement(plateau, i1, j1, i2, j2, -1) != 0)
				continue;

			Pion copie[NB_LIGNES*NB_COLONNES];   /* copie sur la pile, pas de malloc */
			f_copie_plateau(plateau, copie);
			f_bouge_piece(copie, i1, j1, i2, j2, -1);

			int score = f_max(copie, profondeur - 1, alpha, beta);
			pire_score = MIN(pire_score, score);
			beta = MIN(beta, pire_score);

			if(elagage && beta <= alpha)
				return pire_score;
		}
	}
	return pire_score;
}

/**
 * Fonction f_max : Maximise le score du joueur 1 (1 / x).
 * Explore les mouvements possibles pour le joueur 1, simule chaque coup 
 * sur une copie du plateau, appelle f_min et applique l'élagage Alpha-Bêta 
 * (arrête l'exploration si beta <= alpha).
 * Renvoie le meilleur score (maximum) qu'il est possible d'obtenir.
 */

int f_max(Pion* plateau, int profondeur, int alpha, int beta)
{
	nb_noeuds++;

	if(profondeur == 0)
		return f_eval(plateau, 1);

	int meilleur_score = -INFINI;

	for(int i1 = 0; i1 < NB_LIGNES; i1++)
	for(int j1 = 0; j1 < NB_COLONNES; j1++)
	{
		if(plateau[i1*NB_COLONNES+j1].couleur != 1)
			continue;

		for(int i2 = MAX(0, i1-1); i2 <= MIN(NB_LIGNES-1, i1+1); i2++)
		for(int j2 = MAX(0, j1-1); j2 <= MIN(NB_COLONNES-1, j1+1); j2++)
		{
			if(f_test_mouvement(plateau, i1, j1, i2, j2, 1) != 0)
				continue;

			Pion copie[NB_LIGNES*NB_COLONNES];
			f_copie_plateau(plateau, copie);
			f_bouge_piece(copie, i1, j1, i2, j2, 1);

			int score = f_min(copie, profondeur - 1, alpha, beta);
			meilleur_score = MAX(meilleur_score, score);
			alpha = MAX(alpha, meilleur_score);

			if(elagage && beta <= alpha)
				return meilleur_score;
		}
	}
	return meilleur_score;
}

/* Somme de l'avancement des pions du joueur,
 * un pion proche de la ligne adverse rapporte plus. 
 */

int f_avance(Pion *plateau, int joueur) 
{
    int total = 0;
    for (int i = 0; i < NB_LIGNES; i++) 
	{
        for (int j = 0; j < NB_COLONNES; j++) 
		{
            if (plateau[i * NB_COLONNES + j].couleur == joueur) 
			{
                if (joueur == 1) 
				{
                    total += NB_LIGNES - 1 - i;
                } else { total += i; }
            }
        }
    }  
    return total;
}

/**
 * Calcule et joue le meilleur cout
 * */
void f_IA(int joueur)
{

	int alpha = -INFINI, beta = INFINI;
	int meilleur = (joueur == 1) ? -INFINI - 1 : INFINI + 1;
	int bl1 = -1, bc1 = -1, bl2 = -1, bc2 = -1;

	nb_noeuds = 0;
	clock_t debut = clock();

#ifdef DEBUG
	printf("dbg: entering %s %d\n", __FUNCTION__, __LINE__);
#endif

	for(int i1 = 0; i1 < NB_LIGNES; i1++)
	{
		for(int j1 = 0; j1 < NB_COLONNES; j1++)
		{
			if(plateauDeJeu[i1*NB_COLONNES+j1].couleur != joueur)
				continue;

			for(int i2 = MAX(0, i1-1); i2 <= MIN(NB_LIGNES-1, i1+1); i2++)
			{
				for(int j2 = MAX(0, j1-1); j2 <= MIN(NB_COLONNES-1, j1+1); j2++)
				{
					if(f_test_mouvement(plateauDeJeu, i1, j1, i2, j2, joueur) != 0)
						continue;

					Pion *copie = f_raz_plateau();
					f_copie_plateau(plateauDeJeu, copie);
					f_bouge_piece(copie, i1, j1, i2, j2, joueur);

					int score;
					if (joueur == 1) {
						score = f_min(copie, PROF_MAX - 1, alpha, beta);
					} else {
						score = f_max(copie, PROF_MAX - 1, alpha, beta);
					}
					free(copie);

					if(joueur == 1 && score > meilleur)
					{
						meilleur = score; 
						if (elagage) {
							alpha = MAX(alpha, score);
						}
						bl1 = i1; bc1 = j1; 
						bl2 = i2; bc2 = j2;
					}
					else if(joueur == -1 && score < meilleur)
					{
						meilleur = score; 
						if (elagage) {
							beta = MIN(beta, score);
						}
						bl1 = i1; bc1 = j1; 
						bl2 = i2; bc2 = j2;
					}
				}
			}
		}
	}

	double ms = 1000.0 * (clock() - debut) / CLOCKS_PER_SEC;
	printf("elagage=%d, noeuds=%ld temps=%.1f ms\n", elagage, nb_noeuds, ms);

	if (joueur == 1) {
        cumul_noeuds_x += nb_noeuds;
        cumul_temps_x += ms;
        nb_coups_x++;
    } else {
        cumul_noeuds_o += nb_noeuds;
        cumul_temps_o += ms;
        nb_coups_o++;
    }

	if(bl1 != -1)
	{
		printf("IA %c joue : %c%d -> %c%d (score %d)\n", joueur == 1 ? 'x' : 'o',
		       f_convert_int2char(bc1), bl1, f_convert_int2char(bc2), bl2, meilleur);
		f_bouge_piece(plateauDeJeu, bl1, bc1, bl2, bc2, joueur);
	}

#ifdef DEBUG
	printf("dbg: exiting %s %d\n", __FUNCTION__, __LINE__);
#endif
}

/**
 * Demande le choix du joueur humain et calcule le coup demande
 * */
void f_humain(int joueur)
{
	char c1, c2;
	char buffer[32];
	int l1, l2;


#ifdef DEBUG
	printf("dbg: entering %s %d\n", __FUNCTION__, __LINE__);
#endif

	printf("joueur ");
	switch(joueur)
	{
	case -1:
		printf("o ");
		break;
	case 1:
		printf("x ");
		break;
	default:
		printf("inconnu ");
	}
	printf("joue:\n");
	while(1)
	{
		fgets(buffer, 32, stdin);
		if(sscanf(buffer, "%c%i%c%i\n", &c1, &l1, &c2, &l2) == 4)
		{
			if(f_bouge_piece(plateauDeJeu, l1, f_convert_char2int(c1), l2, f_convert_char2int(c2), joueur) == 0)
				break;
		}
		fflush(stdin);
		printf("mauvais choix\n");
	}

#ifdef DEBUG
	printf("dbg: exiting %s %d\n", __FUNCTION__, __LINE__);
#endif
}

void f_afficher_stats(int mode)
{
    printf("\n==================================================\n");
    printf("              STATISTIQUES DE LA PARTIE           \n");
    printf("==================================================\n");

    if (mode == 1) // Humain vs IA (IA = o)
    {
        if (nb_coups_o > 0) {
            printf("IA (o) :\n");
            printf("  - Nombre de coups joues : %d\n", nb_coups_o);
            printf("  - Moeuds visites en moyenne : %ld\n", cumul_noeuds_o / nb_coups_o);
            printf("  - Temps d'execution moyen   : %.2f ms\n", cumul_temps_o / nb_coups_o);
        }
    }
    else if (mode == 3) // IA vs IA
    {
        printf("IA 1 (x) :\n");
        if (nb_coups_x > 0) {
            printf("  - Nombre de coups joues : %d\n", nb_coups_x);
            printf("  - Noeuds visites en moyenne : %ld\n", cumul_noeuds_x / nb_coups_x);
            printf("  - Temps d'execution moyen   : %.2f ms\n", cumul_temps_x / nb_coups_x);
        }

        printf("\nIA 2 (o) :\n");
        if (nb_coups_o > 0) {
            printf("  - Nombre de coups joues : %d\n", nb_coups_o);
            printf("  - Noeuds visites en moyenne : %ld\n", cumul_noeuds_o / nb_coups_o);
            printf("  - Temps d'execution moyen   : %.2f ms\n", cumul_temps_o / nb_coups_o);
        }
    }
    printf("==================================================\n\n");
}

int main(int argv, char *argc[])
{
    int fin = 0, mode = 0, ret, joueur = 1;
    printf("1 humain vs IA\n2 humain vs humain\n3 IA vs IA\n");
    scanf("%d", &mode);
    getchar();

    plateauDeJeu = f_init_plateau();

    while (!fin)
    {
        f_affiche_plateau(plateauDeJeu);
        if(mode == 1)
        {
            if(joueur > 0)
                f_humain(joueur);
            else
                f_IA(joueur);
        }
        else if(mode == 2)
        {
            f_humain(joueur);
        }
        else
        {
            f_IA(joueur);
        }

        if ((ret = f_gagnant()) != 0)
        {
            switch (ret)
            {
            case 1:
                f_affiche_plateau(plateauDeJeu);
                printf("\nJoueur x gagne!\n");
                f_afficher_stats(mode);
                fin = 1;
                break;
            case -1:
                f_affiche_plateau(plateauDeJeu);
                printf("\nJoueur o gagne!\n");
                f_afficher_stats(mode);
                fin = 1;
                break;
            }
        }
        joueur = -joueur;
    }

#ifdef DEBUG
    printf("dbg: exiting %s %d\n", __FUNCTION__, __LINE__);
#endif

    return 0;
}
