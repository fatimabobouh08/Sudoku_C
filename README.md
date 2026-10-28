# 🎮 Jeu Sudoku en C

Ce projet est une application complète de jeu Sudoku développée en langage C. Il a été réalisé avec l'IDE **Code::Blocks** et se joue entièrement dans le terminal (interface console).

## ✨ Fonctionnalités

- **Interface colorée** : Les chiffres de la grille initiale apparaissent en bleu clair, et ceux que vous ajoutez apparaissent en vert.
- **3 niveaux de difficulté** : Facile, Moyen et Difficile (chargés depuis des fichiers textes externes).
- **Système d'aide** : Si vous êtes bloqué, une option permet de remplir automatiquement une case avec la bonne valeur.
- **Sauvegarde et chargement** : Possibilité de quitter une partie en cours de route, de la sauvegarder (`sauvegarde.txt`), et de la reprendre plus tard depuis le menu principal.
- **Chronomètre et Score** : Le jeu calcule votre temps de résolution. Le score final dépend du niveau de difficulté choisi, du temps écoulé, et du nombre d'aides utilisées.
- **Règles et conseils** : Un menu dédié permet de relire les règles du Sudoku et d'obtenir des astuces de jeu.

## 🚀 Comment lancer le projet

### Option 1 : Avec Code::Blocks (Recommandé)
1. Téléchargez ou clonez ce dépôt sur votre ordinateur.
2. Ouvrez le fichier `sudoku.cbp` avec **Code::Blocks**.
3. Appuyez sur **F9** (ou cliquez sur le bouton "Build and Run").
*Note : Assurez-vous que les fichiers `.txt` (grilles et solutions) sont bien dans le même dossier que l'exécutable.*

### Option 2 : En ligne de commande (GCC)
Si vous avez un compilateur C (comme GCC) installé, ouvrez un terminal dans le dossier du projet et tapez :
```bash
gcc -o sudoku main.c sudoku.c
./sudoku
