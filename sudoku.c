#include "sudoku.h"
#include <time.h>

#ifdef _WIN32
#include <windows.h>
#endif

/* --------- COULEURS + NETTOYAGE ECRAN --------- */

void setColor(int texte, int fond) {
#ifdef _WIN32
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, texte + 16 * fond);
#else
    (void)texte;
    (void)fond;
#endif
}

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    printf("\033[2J\033[H");
#endif
}

/* --------- FONCTIONS UTILITAIRES --------- */

int lireEntier(const char* message) {
    char ligne[100];
    int valeur;

    printf("%s", message);
    fgets(ligne, sizeof(ligne), stdin);
    sscanf(ligne, "%d", &valeur);

    return valeur;
}

void lireChaine(char* buffer, int taille, const char* message) {
    printf("%s", message);
    fgets(buffer, taille, stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
}

void attendreEntree() {
    printf("Appuyez sur ENTREE pour continuer...");
    getchar();
}

/* --------- GRILLE + REGLES --------- */

void chargerGrilleDepuisFichier(int grille[9][9], int grilleInitiale[9][9], const char* nomFichier) {
    FILE* f = fopen(nomFichier, "r");
    int i, j;

    if (f == NULL) {
        printf("Erreur : impossible d'ouvrir le fichier %s\n", nomFichier);
        printf("Utilisation de la grille interne par defaut.\n");
        attendreEntree();
        return;
    }

    for (i = 0; i < 9; i++) {
        for (j = 0; j < 9; j++) {
            if (fscanf(f, "%d", &grille[i][j]) != 1) {
                printf("Erreur de lecture dans le fichier %s\n", nomFichier);
                printf("Utilisation de la grille interne par defaut.\n");
                fclose(f);
                attendreEntree();
                return;
            }
            grilleInitiale[i][j] = grille[i][j];
        }
    }

    fclose(f);
}

void chargerSolutionDepuisFichier(int solution[9][9], const char* nomFichier) {
    FILE* f = fopen(nomFichier, "r");
    int i, j;

    if (f == NULL) {
        printf("Erreur : impossible d'ouvrir le fichier solution %s\n", nomFichier);
        attendreEntree();
        return;
    }

    for (i = 0; i < 9; i++) {
        for (j = 0; j < 9; j++) {
            if (fscanf(f, "%d", &solution[i][j]) != 1) {
                printf("Erreur de lecture dans le fichier solution %s\n", nomFichier);
                fclose(f);
                attendreEntree();
                return;
            }
        }
    }

    fclose(f);
}

void sauvegarderPartie(int grille[9][9], int grilleInitiale[9][9], int niveau) {
    FILE* f = fopen("sauvegarde.txt", "w");
    int i, j;

    if (f == NULL) {
        printf("Erreur : impossible de creer le fichier de sauvegarde.\n");
        attendreEntree();
        return;
    }

    fprintf(f, "%d\n", niveau);

    for (i = 0; i < 9; i++) {
        for (j = 0; j < 9; j++) {
            fprintf(f, "%d ", grille[i][j]);
        }
        fprintf(f, "\n");
    }

    for (i = 0; i < 9; i++) {
        for (j = 0; j < 9; j++) {
            fprintf(f, "%d ", grilleInitiale[i][j]);
        }
        fprintf(f, "\n");
    }

    fclose(f);
    printf("Partie sauvegardee dans sauvegarde.txt\n");
    attendreEntree();
}

int chargerSauvegarde(int grille[9][9], int grilleInitiale[9][9], int* niveau) {
    FILE* f = fopen("sauvegarde.txt", "r");
    int i, j;

    if (f == NULL) {
        printf("Aucune sauvegarde trouvee.\n");
        attendreEntree();
        return 0;
    }

    if (fscanf(f, "%d", niveau) != 1) {
        printf("Erreur de lecture du niveau dans sauvegarde.txt\n");
        fclose(f);
        attendreEntree();
        return 0;
    }

    for (i = 0; i < 9; i++) {
        for (j = 0; j < 9; j++) {
            if (fscanf(f, "%d", &grille[i][j]) != 1) {
                printf("Erreur de lecture de la grille dans sauvegarde.txt\n");
                fclose(f);
                attendreEntree();
                return 0;
            }
        }
    }

    for (i = 0; i < 9; i++) {
        for (j = 0; j < 9; j++) {
            if (fscanf(f, "%d", &grilleInitiale[i][j]) != 1) {
                printf("Erreur de lecture de la grille initiale dans sauvegarde.txt\n");
                fclose(f);
                attendreEntree();
                return 0;
            }
        }
    }

    fclose(f);
    printf("Sauvegarde chargee depuis sauvegarde.txt\n");
    attendreEntree();
    return 1;
}

/* --------- AFFICHAGE GRILLE --------- */

void afficherGrille(int grille[9][9], int grilleInitiale[9][9]) {
    int i, j;
    printf("\nGrille de Sudoku :\n\n");
    for (i = 0; i < 9; i++) {
        if (i % 3 == 0 && i != 0) {
            setColor(7, 0);
            printf("------+-------+------\n");
        }
        for (j = 0; j < 9; j++) {
            if (j % 3 == 0 && j != 0) {
                setColor(7, 0);
                printf("| ");
            }
            if (grille[i][j] == 0) {
                setColor(8, 0);
                printf(". ");
            } else {
                if (grilleInitiale[i][j] != 0) {
                    setColor(11, 0); /* fixe */
                } else {
                    setColor(10, 0); /* joueur */
                }
                printf("%d ", grille[i][j]);
            }
        }
        setColor(7, 0);
        printf("\n");
    }
    setColor(7, 0);
    printf("\n");
}

/* --------- REGLES SUDOKU --------- */

int valeur_autorisee(int grille[9][9], int ligne, int colonne, int valeur) {
    int i, j;
    int debut_ligne, debut_colonne;

    for (i = 0; i < 9; i++) {
        if (grille[ligne][i] == valeur) {
            return 0;
        }
    }

    for (i = 0; i < 9; i++) {
        if (grille[i][colonne] == valeur) {
            return 0;
        }
    }

    debut_ligne   = (ligne / 3) * 3;
    debut_colonne = (colonne / 3) * 3;

    for (i = debut_ligne; i < debut_ligne + 3; i++) {
        for (j = debut_colonne; j < debut_colonne + 3; j++) {
            if (grille[i][j] == valeur) {
                return 0;
            }
        }
    }

    return 1;
}

/* --------- TITRE + MENUS --------- */

void afficherTitre() {
    setColor(14, 0);
    printf("==========================================\n");
    printf("            JEU SUDOKU                    \n");
    printf("      Projet de Developpement            \n");
    printf("==========================================\n");
    setColor(7, 0);
}

void afficherMenu() {
    clearScreen();
    afficherTitre();
    printf("\n");
    printf("  Selectionnez une option :\n");
    printf("  --------------------------\n\n");
    printf("  1. Nouvelle partie\n");
    printf("  2. Charger une partie\n");
    printf("  3. Regles du jeu\n");
    printf("  4. Quitter\n");
    printf("  5. Aide (conseils)\n");
    printf("\n");
}

void afficherRegles() {
    clearScreen();
    afficherTitre();
    printf("\n          REGLES DU SUDOKU               \n\n");

    printf("Le Sudoku se joue sur une grille de 9x9 cases.\n");
    printf("Regles :\n");
    printf("1. Chaque ligne doit contenir les chiffres de 1 a 9.\n");
    printf("2. Chaque colonne doit contenir les chiffres de 1 a 9.\n");
    printf("3. Chaque region 3x3 doit contenir les chiffres de 1 a 9.\n");
    printf("4. Aucun chiffre ne peut se repeter sur une ligne,\n");
    printf("   une colonne ou une region 3x3.\n\n");

    attendreEntree();
}

void afficherAideGenerale() {
    clearScreen();
    afficherTitre();
    printf("\n              AIDE - CONSEILS            \n\n");
    printf("Quelques strategies simples pour vous aider :\n\n");
    printf("- Cherchez les lignes, colonnes ou blocs 3x3\n");
    printf("  ou il ne manque qu'un seul chiffre.\n");
    printf("- Utilisez la regle : un chiffre de 1 a 9 n'apparait\n");
    printf("  qu'une seule fois dans chaque ligne, colonne et bloc 3x3.\n");
    printf("- Quand vous etes bloque, utilisez l'option d'aide pendant\n");
    printf("  la partie pour remplir une case correcte automatiquement.\n\n");

    attendreEntree();
}

/* --------- DIFFICULTE --------- */

int choisirDifficulte() {
    int niveau;

    clearScreen();
    afficherTitre();
    printf("\n          NIVEAU DE DIFFICULTE           \n\n");

    printf("1. Facile  (peu de cases vides)\n");
    printf("2. Moyen   (plus de cases vides)\n");
    printf("3. Difficile (encore plus)\n");

    /* Validation de la saisie : on redemande tant que ce n'est pas 1, 2 ou 3 */
    do {
        niveau = lireEntier("Votre choix (1-3) : ");
        if (niveau < 1 || niveau > 3) {
            printf("Choix invalide, veuillez entrer un nombre entre 1 et 3.\n");
        }
    } while (niveau < 1 || niveau > 3);

    printf("\nNiveau de difficulte enregistre ! (choix = %d)\n", niveau);
    attendreEntree();

    return niveau;
}

/* --------- LOGIQUE DE PARTIE --------- */

int grilleTerminee(int grille[9][9]) {
    int i, j;
    for (i = 0; i < 9; i++) {
        for (j = 0; j < 9; j++) {
            if (grille[i][j] == 0) {
                return 0;
            }
        }
    }
    return 1;
}

void donnerAide(int grille[9][9], int grilleInitiale[9][9], int solution[9][9], int* nbAides) {
    int i, j;

    for (i = 0; i < 9; i++) {
        for (j = 0; j < 9; j++) {
            if (grille[i][j] == 0 && grilleInitiale[i][j] == 0) {
                grille[i][j] = solution[i][j];
                printf("Aide : la case (%d, %d) doit contenir le chiffre %d.\n",
                       i + 1, j + 1, solution[i][j]);
                (*nbAides)++;
                attendreEntree();
                return;
            }
        }
    }

    printf("Aucune aide disponible : soit la grille est remplie,\n");
    printf("soit il ne reste que des cases initiales.\n");
    attendreEntree();
}

void nouvellePartie(int grille[9][9], int grilleInitiale[9][9], int solution[9][9]) {
    int ligne, colonne, valeur;
    char tampon[100];
    int niveau;
    time_t debut, fin;
    int nbAides = 0;

    niveau = choisirDifficulte();

    switch (niveau) {
    case 1:
        chargerGrilleDepuisFichier(grille, grilleInitiale, "grille_facile.txt");
        break;
    case 2:
        chargerGrilleDepuisFichier(grille, grilleInitiale, "grille_moyen.txt");
        break;
    case 3:
        chargerGrilleDepuisFichier(grille, grilleInitiale, "grille_difficile.txt");
        break;
    default:
        chargerGrilleDepuisFichier(grille, grilleInitiale, "grille_facile.txt");
        break;
    }

    chargerSolutionDepuisFichier(solution, "solution.txt");

    debut = time(NULL);

    while (1) {
        int choixLocal;

        clearScreen();
        afficherTitre();
        afficherGrille(grille, grilleInitiale);

        printf("1. Jouer un coup\n");
        printf("2. Aide (remplir une case correcte)\n");
        printf("0. Retour au menu principal\n");
        choixLocal = lireEntier("Votre choix : ");

        if (choixLocal == 0) {
            int choixSave;
            long temps;

            printf("Vous quittez la partie.\n");
            printf("Voulez-vous sauvegarder la partie en cours ?\n");
            printf("1. Oui\n");
            printf("2. Non\n");
            choixSave = lireEntier("Votre choix : ");

            if (choixSave == 1) {
                sauvegarderPartie(grille, grilleInitiale, niveau);
            }

            printf("Retour au menu principal.\n");
            fin = time(NULL);
            temps = (long)(fin - debut);
            printf("Temps de jeu : %ld secondes.\n", temps);
            printf("Nombre d'aides utilisees : %d\n", nbAides);
            attendreEntree();
            break;
        } else if (choixLocal == 2) {
            donnerAide(grille, grilleInitiale, solution, &nbAides);
            if (grilleTerminee(grille)) {
                int base, score;
                long temps;

                fin = time(NULL);
                temps = (long)(fin - debut);

                if (niveau == 1)      base = 100;
                else if (niveau == 2) base = 200;
                else                  base = 300;

                score = base - (int)(temps / 5) - 10 * nbAides;
                if (score < 0) score = 0;

                clearScreen();
                afficherTitre();
                afficherGrille(grille, grilleInitiale);
                printf("Bravo, vous avez completement rempli la grille de Sudoku !\n");
                printf("Temps de jeu : %ld secondes.\n", temps);
                printf("Nombre d'aides utilisees : %d\n", nbAides);
                printf("Votre score est : %d points.\n", score);
                printf("Merci d'avoir joue, tres beau travail.\n");
                attendreEntree();
                break;
            }
            continue;
        } else if (choixLocal == 1) {
            printf("Entrez un coup (ligne 1-9).\n");

            printf("Ligne : ");
            fgets(tampon, sizeof(tampon), stdin);
            sscanf(tampon, "%d", &ligne);

            printf("Colonne (1-9) : ");
            fgets(tampon, sizeof(tampon), stdin);
            sscanf(tampon, "%d", &colonne);

            printf("Valeur (1-9) : ");
            fgets(tampon, sizeof(tampon), stdin);
            sscanf(tampon, "%d", &valeur);

            ligne--;
            colonne--;

            if (ligne < 0 || ligne >= 9 ||
                colonne < 0 || colonne >= 9 ||
                valeur < 1 || valeur > 9) {

                printf("Coordonnees ou valeur invalides.\n");
                attendreEntree();
                continue;
            }

            if (grilleInitiale[ligne][colonne] != 0) {
                printf("Cette case est fixe, vous ne pouvez pas la modifier.\n");
                attendreEntree();
                continue;
            }

            if (!valeur_autorisee(grille, ligne, colonne, valeur)) {
                printf("Coup invalide : la valeur ne respecte pas les regles du Sudoku.\n");
                attendreEntree();
                continue;
            }

            grille[ligne][colonne] = valeur;

            if (grilleTerminee(grille)) {
                int base, score;
                long temps;

                fin = time(NULL);
                temps = (long)(fin - debut);

                if (niveau == 1)      base = 100;
                else if (niveau == 2) base = 200;
                else                  base = 300;

                score = base - (int)(temps / 5) - 10 * nbAides;
                if (score < 0) score = 0;

                clearScreen();
                afficherTitre();
                afficherGrille(grille, grilleInitiale);
                printf("Bravo, vous avez completement rempli la grille de Sudoku !\n");
                printf("Temps de jeu : %ld secondes.\n", temps);
                printf("Nombre d'aides utilisees : %d\n", nbAides);
                printf("Votre score est : %d points.\n", score);
                printf("Merci d'avoir joue, tres beau travail.\n");
                attendreEntree();
                break;
            }
        } else {
            printf("Choix invalide.\n");
            attendreEntree();
        }
    }
}

void chargerPartie() {
    int niveau;
    int grille[9][9];
    int grilleInitiale[9][9];
    int solution[9][9];

    clearScreen();
    afficherTitre();
    printf("\n          CHARGER UNE PARTIE             \n\n");

    if (chargerSauvegarde(grille, grilleInitiale, &niveau)) {
        chargerSolutionDepuisFichier(solution, "solution.txt");
        continuerPartie(grille, grilleInitiale, solution, niveau);
    } else {
        printf("Impossible de charger une partie.\n");
        attendreEntree();
    }
}

void continuerPartie(int grille[9][9], int grilleInitiale[9][9], int solution[9][9], int niveau) {
    int ligne, colonne, valeur;
    char tampon[100];
    time_t debut, fin;
    int nbAides = 0;

    debut = time(NULL);

    while (1) {
        int choixLocal;

        clearScreen();
        afficherTitre();
        afficherGrille(grille, grilleInitiale);

        printf("1. Jouer un coup\n");
        printf("2. Aide (remplir une case correcte)\n");
        printf("0. Retour au menu principal\n");
        choixLocal = lireEntier("Votre choix : ");

        if (choixLocal == 0) {
            int choixSave;
            long temps;

            printf("Vous quittez la partie.\n");
            printf("Voulez-vous sauvegarder la partie en cours ?\n");
            printf("1. Oui\n");
            printf("2. Non\n");
            choixSave = lireEntier("Votre choix : ");

            if (choixSave == 1) {
                sauvegarderPartie(grille, grilleInitiale, niveau);
            }

            printf("Retour au menu principal.\n");
            fin = time(NULL);
            temps = (long)(fin - debut);
            printf("Temps de jeu : %ld secondes.\n", temps);
            printf("Nombre d'aides utilisees : %d\n", nbAides);
            attendreEntree();
            break;
        } else if (choixLocal == 2) {
            donnerAide(grille, grilleInitiale, solution, &nbAides);
            if (grilleTerminee(grille)) {
                int base, score;
                long temps;

                fin = time(NULL);
                temps = (long)(fin - debut);

                if (niveau == 1)      base = 100;
                else if (niveau == 2) base = 200;
                else                  base = 300;

                score = base - (int)(temps / 5) - 10 * nbAides;
                if (score < 0) score = 0;

                clearScreen();
                afficherTitre();
                afficherGrille(grille, grilleInitiale);
                printf("Bravo, vous avez completement rempli la grille de Sudoku !\n");
                printf("Temps de jeu : %ld secondes.\n", temps);
                printf("Nombre d'aides utilisees : %d\n", nbAides);
                printf("Votre score est : %d points.\n", score);
                printf("Merci d'avoir joue, tres beau travail.\n");
                attendreEntree();
                break;
            }
            continue;
        } else if (choixLocal == 1) {
            printf("Entrez un coup (ligne 1-9).\n");

            printf("Ligne : ");
            fgets(tampon, sizeof(tampon), stdin);
            sscanf(tampon, "%d", &ligne);

            printf("Colonne (1-9) : ");
            fgets(tampon, sizeof(tampon), stdin);
            sscanf(tampon, "%d", &colonne);

            printf("Valeur (1-9) : ");
            fgets(tampon, sizeof(tampon), stdin);
            sscanf(tampon, "%d", &valeur);

            ligne--;
            colonne--;

            if (ligne < 0 || ligne >= 9 ||
                colonne < 0 || colonne >= 9 ||
                valeur < 1 || valeur > 9) {

                printf("Coordonnees ou valeur invalides.\n");
                attendreEntree();
                continue;
            }

            if (grilleInitiale[ligne][colonne] != 0) {
                printf("Cette case est fixe, vous ne pouvez pas la modifier.\n");
                attendreEntree();
                continue;
            }

            if (!valeur_autorisee(grille, ligne, colonne, valeur)) {
                printf("Coup invalide : la valeur ne respecte pas les regles du Sudoku.\n");
                attendreEntree();
                continue;
            }

            grille[ligne][colonne] = valeur;

            if (grilleTerminee(grille)) {
                int base, score;
                long temps;

                fin = time(NULL);
                temps = (long)(fin - debut);

                if (niveau == 1)      base = 100;
                else if (niveau == 2) base = 200;
                else                  base = 300;

                score = base - (int)(temps / 5) - 10 * nbAides;
                if (score < 0) score = 0;

                clearScreen();
                afficherTitre();
                afficherGrille(grille, grilleInitiale);
                printf("Bravo, vous avez completement rempli la grille de Sudoku !\n");
                printf("Temps de jeu : %ld secondes.\n", temps);
                printf("Nombre d'aides utilisees : %d\n", nbAides);
                printf("Votre score est : %d points.\n", score);
                printf("Merci d'avoir joue, tres beau travail.\n");
                attendreEntree();
                break;
            }
        } else {
            printf("Choix invalide.\n");
            attendreEntree();
        }
    }
}


