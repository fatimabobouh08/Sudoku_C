#ifndef SUDOKU_H_INCLUDED
#define SUDOKU_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Fonctions utilitaires */
int  lireEntier(const char* message);
void lireChaine(char* buffer, int taille, const char* message);
void attendreEntree();
void chargerGrilleDepuisFichier(int grille[9][9],
                                int grilleInitiale[9][9],
                                const char* nomFichier);

/* Charger la solution complète d'une grille */
void chargerSolutionDepuisFichier(int solution[9][9],
                                  const char* nomFichier);

/* Grille + regles du Sudoku */
void afficherGrille(int grille[9][9],
                    int grilleInitiale[9][9]);
int  valeur_autorisee(int grille[9][9],
                      int ligne,
                      int colonne,
                      int valeur);

/* Affichage du titre et du menu */
void afficherTitre();
void afficherMenu();

/* Regles et aide generale */
void afficherRegles();
void afficherAideGenerale();

int  choisirDifficulte();

int  grilleTerminee(int grille[9][9]);

/* Nouvelle partie / chargement / sauvegarde */
void nouvellePartie(int grille[9][9],
                    int grilleInitiale[9][9],
                    int solution[9][9]);
void chargerPartie();
void sauvegarderPartie(int grille[9][9],
                       int grilleInitiale[9][9],
                       int niveau);
int  chargerSauvegarde(int grille[9][9],
                       int grilleInitiale[9][9],
                       int* niveau);

/* Continuer une partie déjà commencée */
void continuerPartie(int grille[9][9],
                     int grilleInitiale[9][9],
                     int solution[9][9],
                     int niveau);

/* Aide pendant la partie (remplit une case correcte) */
void donnerAide(int grille[9][9],
                int grilleInitiale[9][9],
                int solution[9][9],
                int* nbAides);

/* Couleurs / nettoyage écran */
void setColor(int texte, int fond);
void clearScreen();

#endif // SUDOKU_H_INCLUDED


