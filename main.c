#include "sudoku.h"

int main() {
    int continuer = 1;
    int choix;
    int grille[9][9];
    int grilleInitiale[9][9];
    int solution[9][9];

    while (continuer) {
        afficherMenu();

        /* Validation de la saisie du menu principal (1 à 5) */
        do {
            choix = lireEntier("Votre choix (1-5) : ");
            if (choix < 1 || choix > 5) {
                printf("Choix invalide, veuillez entrer un nombre entre 1 et 5.\n");
            }
        } while (choix < 1 || choix > 5);

        switch (choix) {
            case 1:
                nouvellePartie(grille, grilleInitiale, solution);
                break;

            case 2:
                chargerPartie();
                break;

            case 3:
                afficherRegles();
                break;

            case 4:
                continuer = 0;
                printf("\n==========================================\n");
                printf("   Merci d'avoir joue au Sudoku !        \n");
                printf("==========================================\n\n");
                printf("A bientot !\n");
                attendreEntree();
                break;

            case 5:
                afficherAideGenerale();
                break;
        }
    }

    return 0;
}
