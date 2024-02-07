#include <stdio.h>
#include <stdlib.h>
#include <jansson.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#include "coreApp.h"

#define MAX_PSEUDO_LENGTH 50

// Fonction pour supprimer les accents et convertir en minuscules
void removeAccentsAndUppercase(char *str)
{
    int i;

    // Convertir en minuscules
    for (i = 0; str[i] != '\0'; i++)
    {
        str[i] = tolower((unsigned char)str[i]);
    }
}

// Fonction pour rechercher un pseudo dans la liste
int recherchePseudo(const char *pseudo)
{

    // Ouvrir le fichier JSON
    FILE *file = fopen("jsons/pseudos.json", "r");
    if (file == NULL)
    {
        printf("Erreur lors de l'ouverture du fichier.\n");
        return 0;
    }

    // Charger le contenu du fichier JSON
    json_t *root;
    json_error_t error;
    root = json_loadf(file, 0, &error);
    fclose(file);

    // Vérifier si le pseudo est valide
    if (strlen(pseudo) < 3)
    {
        printf("Le pseudo doit avoir au moins 3 lettres.\n");
        return 0;
    }

    // Vérifier si le pseudo existe déjà
    json_t *pseudosArray = json_object_get(root, "pseudos");

    size_t index;
    json_t *value;

    // Parcourir le tableau des pseudos
    json_array_foreach(pseudosArray, index, value)
    {
        const char *existingPseudo = json_string_value(value);
        if (strcmp(existingPseudo, pseudo) == 0)
        {
            // Le pseudo existe déjà
            return 1;
        }
    }

    // Le pseudo n'existe pas
    return 0;
}

// Fonction pour ajouter un pseudo à la liste
void ajouterPseudo(const char *pseudo)
{
    // Ouvrir le fichier JSON en mode lecture/écriture
    FILE *file = fopen("jsons/pseudos.json", "r+");
    if (file == NULL)
    {
        printf("Erreur lors de l'ouverture du fichier.\n");
        return;
    }

    // Charger le contenu du fichier JSON
    json_t *root = json_loadf(file, 0, NULL);
    if (root == NULL)
    {
        printf("Erreur lors du chargement du JSON.\n");
        fclose(file);
        return;
    }

    // Vérifier si le pseudo est valide
    if (strlen(pseudo) < 3)
    {
        printf("Le pseudo doit avoir au moins 3 lettres.\n");
        fclose(file);
        json_decref(root); // Libérer la mémoire utilisée par le JSON
        return;
    }

    // Vérifier si le champ 'pseudos' existe
    json_t *pseudosArray = json_object_get(root, "pseudos");
    if (!json_is_array(pseudosArray))
    {
        printf("Le champ 'pseudos' n'est pas un tableau.\n");
        fclose(file);
        json_decref(root); // Libérer la mémoire utilisée par le JSON
        return;
    }

    // Ajouter le nouveau pseudo à la liste
    json_array_append_new(pseudosArray, json_string(pseudo));

    // Écrire les modifications dans le fichier JSON
    fseek(file, 0, SEEK_SET);               // Se positionner au début du fichier
    json_dumpf(root, file, JSON_INDENT(4)); // Écrire le JSON dans le fichier avec indentation
    fclose(file);

    json_decref(root); // Libérer la mémoire utilisée par le JSON
}

// Fonction pour trouver un thème aléatoire dans le fichier JSON
char *findRandomTheme()
{
    // Ouvrir le fichier JSON
    FILE *file = fopen("jsons/reponsestheme.json", "r");
    if (file == NULL)
    {
        fprintf(stderr, "Erreur lors de l'ouverture du fichier.\n");
        return NULL;
    }

    // Charger le contenu du fichier JSON
    json_t *root;
    json_error_t error;
    root = json_loadf(file, 0, &error);
    fclose(file);

    // Vérifier si le fichier JSON est correctement chargé
    if (!root)
    {
        fprintf(stderr, "Erreur lors de la lecture du fichier JSON : %s\n", error.text);
        return NULL;
    }

    // Extraire les thèmes du fichier JSON
    json_t *themesObj = root;
    if (!themesObj || !json_is_object(themesObj))
    {
        fprintf(stderr, "Le fichier JSON ne contient pas d'objet.\n");
        json_decref(root);
        return NULL;
    }

    // Extraire les clés (thèmes) du fichier JSON
    size_t themesCount = json_object_size(themesObj);
    const char **themes = (const char **)malloc(themesCount * sizeof(const char *));
    size_t index = 0;
    const char *theme;
    json_t *themeValue;
    json_object_foreach(themesObj, theme, themeValue)
    {
        themes[index++] = theme;
    }

    // Fermer le fichier JSON
    json_decref(root);

    // Choisir un thème aléatoire
    srand(time(NULL));
    size_t randomIndex = rand() % themesCount;
    char *randomTheme = strdup(themes[randomIndex]);

    // Libérer la mémoire allouée
    free(themes);

    return randomTheme;
}

// Fonction pour trouver 5 thèmes aléatoires différents dans le fichier JSON
char **findRandomThemes()
{
    // Ouvrir le fichier JSON contenant les thèmes
    FILE *file = fopen("jsons/reponsestheme.json", "r");
    if (!file)
    {
        fprintf(stderr, "Erreur lors de l'ouverture du fichier de thèmes.\n");
        return NULL;
    }

    // Charger le fichier JSON
    json_t *root;
    json_error_t error;
    root = json_loadf(file, 0, &error);
    fclose(file);

    if (!root)
    {
        fprintf(stderr, "Erreur lors du chargement du fichier JSON : %s\n", error.text);
        return NULL;
    }

    // Allouer de la mémoire pour stocker les thèmes
    char **themes = malloc(json_object_size(root) * sizeof(char *));
    if (!themes)
    {
        fprintf(stderr, "Erreur d'allocation de mémoire pour les thèmes.\n");
        json_decref(root);
        return NULL;
    }

    // Récupérer les clés (thèmes) du fichier JSON
    size_t index = 0;
    const char *theme;
    json_t *value;
    json_object_foreach(root, theme, value)
    {
        themes[index++] = strdup(theme);
    }

    // Mélanger les thèmes
    srand(time(NULL)); // Initialiser la seed pour rand()
    for (size_t i = json_object_size(root) - 1; i > 0; i--)
    {
        size_t j = rand() % (i + 1);
        char *temp = themes[i];
        themes[i] = themes[j];
        themes[j] = temp;
    }

    // Libérer la mémoire utilisée par le JSON
    json_decref(root);

    return themes;
}

// Fonction pour obtenir un pseudo aléatoire pour un joueur connecté
char *get_random_pseudo(const char *connected_pseudo)
{
    // Charge la liste de pseudos depuis le fichier JSON
    json_t *root;
    json_error_t error;

    root = json_load_file("jsons/pseudos.json", 0, &error);
    if (!root)
    {
        fprintf(stderr, "Erreur lors de la lecture du fichier JSON : %s\n", error.text);
        return NULL;
    }

    // Récupère la liste de pseudos
    json_t *pseudos_array = json_object_get(root, "pseudos");
    if (!json_is_array(pseudos_array))
    {
        fprintf(stderr, "La propriété 'pseudos' n'est pas un tableau JSON.\n");
        json_decref(root);
        return NULL;
    }

    size_t array_size = json_array_size(pseudos_array);
    if (array_size == 0)
    {
        fprintf(stderr, "La liste des pseudos est vide.\n");
        json_decref(root);
        return NULL;
    }

    // Convertit la liste de pseudos en tableau de chaînes C
    const char **pseudos_list = (const char **)malloc(array_size * sizeof(char *));
    for (size_t i = 0; i < array_size; i++)
    {
        pseudos_list[i] = json_string_value(json_array_get(pseudos_array, i));
    }

    // Assurez-vous que le pseudo connecté est dans la liste
    int connected_index = -1;
    for (size_t i = 0; i < array_size; i++)
    {
        if (strcmp(pseudos_list[i], connected_pseudo) == 0)
        {
            connected_index = i;
            break;
        }
    }

    if (connected_index == -1)
    {
        fprintf(stderr, "Le pseudo connecté n'est pas dans la liste.\n");
        free(pseudos_list);
        json_decref(root);
        return NULL;
    }

    // Supprimez le pseudo du joueur connecté de la liste
    for (size_t i = connected_index; i < array_size - 1; i++)
    {
        pseudos_list[i] = pseudos_list[i + 1];
    }

    // Choisissez un pseudo au hasard parmi les pseudos restants
    srand(time(NULL));
    int random_index = rand() % (array_size - 1);
    char *random_pseudo = strdup(pseudos_list[random_index]);

    // Libère la mémoire allouée
    free(pseudos_list);
    json_decref(root);

    return random_pseudo;
}

/////////////////////////////////////////////////////////////////////////////// FONCTIONS ///////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////// FONCTIONS (POUR ADELE) ///////////////////////////////////////////////////////////////////////////////

// Fonction pour se connecter avec le nom du pseudo
int connexion(const char *pseudo)
{
    // Ouvrir le fichier JSON
    FILE *file = fopen("jsons/pseudos.json", "r");
    if (file == NULL)
    {
        printf("Erreur lors de l'ouverture du fichier.\n");
        return 0;
    }

    // Charger le contenu du fichier JSON
    json_t *root;
    json_error_t error;
    root = json_loadf(file, 0, &error);
    fclose(file);

    // Vérifier si le pseudo est valide
    if (strlen(pseudo) < 3)
    {
        printf("Le pseudo doit avoir au moins 3 lettres.\n");
        return 0;
    }

    // Vérifier si le pseudo existe déjà
    json_t *pseudosArray = json_object_get(root, "pseudos");
    if (recherchePseudo(pseudo))
    {
        printf("Le pseudo '%s' existe déjà.\n", pseudo);
        return 1;
    }

    // Ajouter le nouveau pseudo
    ajouterPseudo(pseudo);

    // // Enregistrer les modifications dans le fichier JSON
    // FILE *writeFile = fopen("jsons/pseudos.json", "w");
    // if (writeFile == NULL)
    // {
    //     printf("Erreur lors de l'ouverture du fichier en écriture.\n");
    //     return 0;
    // }

    // json_dumpf(root, writeFile, JSON_INDENT(2));
    // fclose(writeFile);

    // // Libérer la mémoire
    // json_decref(root);

    printf("Le pseudo '%s' a été ajouté avec succès.\n", pseudo);
    return 1;
}

// Fonction pour afficher tous les pseudos
char **fetchAllPlayers(const char *pseudoconnecte)
{
    // Charger le fichier JSON
    FILE *file = fopen("jsons/pseudos.json", "r");
    if (!file)
    {
        fprintf(stderr, "Erreur lors de l'ouverture du fichier.\n");
        return NULL;
    }

    // Obtenir la taille du fichier
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    rewind(file);

    // Allouer de la mémoire pour stocker le contenu du fichier
    char *buffer = (char *)malloc(size + 1);
    if (!buffer)
    {
        fprintf(stderr, "Erreur d'allocation de mémoire.\n");
        fclose(file);
        return NULL;
    }

    // Lire le contenu du fichier dans le buffer
    fread(buffer, 1, size, file);
    buffer[size] = '\0'; // Ajouter la terminaison de la chaîne

    // Fermer le fichier
    fclose(file);

    // Charger le JSON à partir du buffer
    json_t *root;
    json_error_t error;
    root = json_loads(buffer, 0, &error);

    // Libérer la mémoire utilisée par le buffer
    free(buffer);

    // Vérifier les erreurs lors du chargement du JSON
    if (!root)
    {
        fprintf(stderr, "Erreur lors du chargement du JSON : %s\n", error.text);
        return NULL;
    }

    // Accéder au tableau de pseudos
    json_t *pseudos = json_object_get(root, "pseudos");
    if (!json_is_array(pseudos))
    {
        fprintf(stderr, "Le champ 'pseudos' n'est pas un tableau.\n");
        json_decref(root);
        return NULL;
    }

    // Compter le nombre de pseudos
    size_t numPlayers = json_array_size(pseudos);

    // Allouer de l'espace pour le tableau de pointeurs de chaînes de caractères
    char **pseudoArray = (char **)malloc((numPlayers + 1) * sizeof(char *));
    if (!pseudoArray)
    {
        fprintf(stderr, "Erreur d'allocation de mémoire pour le tableau de pseudos.\n");
        // Libérer la mémoire utilisée par le JSON
        json_decref(root);
        return NULL;
    }

    // Parcourir le tableau de pseudos et ajouter chaque pseudo au tableau
    size_t index;
    json_t *value;
    for (index = 0; index < numPlayers; index++)
    {
        value = json_array_get(pseudos, index);
        if (json_is_string(value))
        {
            const char *pseudo = json_string_value(value);

            if (pseudoconnecte != NULL && strcmp(pseudo, pseudoconnecte) == 0)
            {
                continue;
            }

            // Allouer de l'espace pour le pseudo actuel
            pseudoArray[index] = strdup(pseudo);
            if (!pseudoArray[index])
            {
                fprintf(stderr, "Erreur d'allocation de mémoire pour un pseudo.\n");
                // Libérer la mémoire utilisée par les pseudos déjà ajoutés
                for (size_t i = 0; i < index; i++)
                {
                    free(pseudoArray[i]);
                }
                // Libérer la mémoire utilisée par le tableau de pseudos
                free(pseudoArray);
                // Libérer la mémoire utilisée par le JSON
                json_decref(root);
                return NULL;
            }
        }
    }

    // Marquer la fin du tableau avec NULL
    pseudoArray[numPlayers] = NULL;

    // Libérer la mémoire utilisée par le JSON
    json_decref(root);

    return pseudoArray;
}

// Fonction pour créer une nouvelle partie et enregistrer les modifications dans le fichier JSON
int creategame(const char *connected_pseudo, const char *recherchePseudo)
{
    // Charge le JSON depuis le fichier
    json_t *root;
    json_error_t error;

    root = json_load_file("jsons/matches.json", 0, &error);
    if (!root)
    {
        fprintf(stderr, "Erreur lors de la lecture du fichier JSON : %s\n", error.text);
        return 0;
    }

    // Récupère la liste de matchs
    json_t *matches_array = json_object_get(root, "matches");
    if (!json_is_array(matches_array))
    {
        fprintf(stderr, "La propriété 'matches' n'est pas un tableau JSON.\n");
        json_decref(root);
        return 0;
    }

    // Création des 5 manches
    json_t *parties_array = json_array();
    char **themes = findRandomThemes(); // Obtenir 5 thèmes aléatoires différents

    for (int i = 0; i < 5; ++i)
    {
        // Création de la nouvelle partie
        json_t *new_partie = json_object();
        json_object_set_new(new_partie, "id_match", json_integer(json_array_size(matches_array) + 1));
        json_object_set_new(new_partie, "id_manche", json_integer(i + 1));
        json_object_set_new(new_partie, "theme", json_string(themes[i])); // Utilisation du thème
        json_object_set_new(new_partie, "score1", json_integer(0));
        json_object_set_new(new_partie, "score2", json_integer(0));

        // Ajout de la nouvelle partie au tableau des parties
        json_array_append_new(parties_array, new_partie);
    }

    // Création du nouveau match
    json_t *new_match = json_object();
    json_object_set_new(new_match, "joueur1", json_string(connected_pseudo));
    json_object_set_new(new_match, "joueur2", json_string(recherchePseudo));
    json_object_set_new(new_match, "score1", json_integer(-1));
    json_object_set_new(new_match, "score2", json_integer(-1));
    json_object_set_new(new_match, "id_match", json_integer(json_array_size(matches_array) + 1));
    json_object_set_new(new_match, "tour", json_string(connected_pseudo));
    json_object_set_new(new_match, "parties", parties_array);

    // Ajout du nouveau match à la liste des matchs
    json_array_append_new(matches_array, new_match);

    // Enregistrement des modifications dans le fichier
    json_dump_file(root, "jsons/matches.json", JSON_INDENT(4));

    // Récupération de l'ID du match créé
    int id_match = json_array_size(matches_array);

    // Libération de la mémoire allouée
    json_decref(root);

    // Libérer la mémoire allouée pour les thèmes
    for (int i = 0; i < 5; ++i)
    {
        free(themes[i]);
    }
    free(themes);

    return id_match; // Retourne l'ID du match créé
}

// Fonction pour vérifier les réponses des utilisateurs renvoie le nombre de points gagné (0,1,2,3) récupérer dans le fichier JSON
int chercherMotDansJSON(const char *theme, const char *mot)
{
    // Ouvrir le fichier JSON
    FILE *file = fopen("jsons/reponsestheme.json", "r");
    if (file == NULL)
    {
        printf("Erreur lors de l'ouverture du fichier.\n");
        return 0;
    }

    // Charger le contenu du fichier JSON
    json_t *root;
    json_error_t error;
    root = json_loadf(file, 0, &error);
    fclose(file);

    // Vérifier si le mot est présent dans le fichier JSON
    json_t *themeObj = json_object_get(root, theme);
    if (themeObj != NULL && json_is_object(themeObj))
    {
        // Le thème est présent dans le fichier JSON
        printf("\nThème : %s\n", theme);

        char motSansAccents[256]; // Ajustez la taille selon vos besoins

        // Copier la chaîne dans motSansAccents avant de la modifier
        strncpy(motSansAccents, mot, sizeof(motSansAccents));
        motSansAccents[sizeof(motSansAccents) - 1] = '\0'; // Assurer la null-termination

        // Appeler la fonction
        removeAccentsAndUppercase(motSansAccents);

        // Vérifier si le mot est présent dans le thème
        json_t *motObj = json_object_get(themeObj, motSansAccents);
        if (motObj != NULL && json_is_integer(motObj))
        {
            // Le mot est présent dans le thème
            int points = json_integer_value(motObj);
            printf("  - Mot : %s, Points : %d\n", motSansAccents, points);
            return points;
        }
        else
        {
            // Le mot n'est pas présent dans le thème
            printf("Le mot '%s' n'est pas présent dans le thème '%s'.\n", motSansAccents, theme);
            return 0;
        }
    }
    else
    {
        // Le thème n'est pas présent dans le fichier JSON
        printf("Le thème '%s' n'est pas présent dans le fichier JSON.\n", theme);
    }

    // Libérer la mémoire
    json_decref(root);
}

// Fonction pour récupérer le thème correspondant à l'identifiant du match et de la manche
char *RecupTheme(int id_match, int id_manche)
{
    // Charger le JSON depuis le fichier
    json_t *root;
    json_error_t error;
    root = json_load_file("jsons/matches.json", 0, &error);
    if (!root)
    {
        fprintf(stderr, "Erreur lors de la lecture du fichier JSON : %s\n", error.text);
        return NULL;
    }

    // Récupérer la liste de matchs
    json_t *matches_array = json_object_get(root, "matches");
    if (!json_is_array(matches_array))
    {
        fprintf(stderr, "La propriété 'matches' n'est pas un tableau JSON.\n");
        json_decref(root);
        return NULL;
    }

    // Parcourir les matchs
    size_t index;
    json_t *match;
    json_array_foreach(matches_array, index, match)
    {
        // Récupérer les parties du match
        json_t *parties_array = json_object_get(match, "parties");
        if (!json_is_array(parties_array))
        {
            fprintf(stderr, "La propriété 'parties' n'est pas un tableau JSON.\n");
            continue;
        }

        // Parcourir les parties du match
        size_t i;
        json_t *partie;
        json_array_foreach(parties_array, i, partie)
        {
            // Vérifier l'identifiant du match et de la manche
            json_t *id_match_json = json_object_get(partie, "id_match");
            json_t *id_manche_json = json_object_get(partie, "id_manche");
            if (json_integer_value(id_match_json) == id_match && json_integer_value(id_manche_json) == id_manche)
            {
                // Récupérer le thème de la manche
                json_t *theme_json = json_object_get(partie, "theme");
                // Vérifier si le thème existe et est une chaîne de caractères
                if (theme_json != NULL && json_is_string(theme_json))
                {
                    const char *theme = json_string_value(theme_json);
                    // printf("Thème présent dans la fonction recupTheme : %s\n", theme);
                    //  Retourner le thème
                    return strdup(theme);
                }
                else
                {
                    // Gérer le cas où le thème est absent ou n'est pas une chaîne de caractères
                    fprintf(stderr, "Erreur : Thème manquant ou invalide.\n");
                    return NULL;
                }
            }
        }
    }

    // Libérer la mémoire utilisée
    json_decref(root);

    fprintf(stderr, "Aucun thème trouvé pour l'identifiant de match %d et l'identifiant de manche %d.\n", id_match, id_manche);
    return NULL;
}

// Fonction pour mettre à jour le score
char *mettreAJourScore(int id_match, int id_manche, const char *pseudo, const char *mot) {
    // Récupérer le thème
    const char *theme = RecupTheme(id_match, id_manche);
    if (theme == NULL) {
        fprintf(stderr, "Erreur : Impossible de récupérer le thème.\n");
        return "-1";
    }

    // Récupérer le score pour l'instant
    int score = chercherMotDansJSON(theme, mot);
    if (score == -1) {
        fprintf(stderr, "Erreur : Impossible de récupérer le score.\n");
        return "-1";
    }

    // Ouvrir et lire le fichier JSON
    FILE *fichier = fopen("jsons/matches.json", "r+");
    if (fichier == NULL) {
        fprintf(stderr, "Erreur : Impossible d'ouvrir le fichier JSON.\n");
        return "-1";
    }

    json_error_t error;
    json_t *root = json_loadf(fichier, 0, &error);
    if (!root) {
        fprintf(stderr, "Erreur : Impossible de charger le fichier JSON (%s)\n", error.text);
        fclose(fichier);
        return "-1";
    }

    // Recherche du match correspondant
    json_t *matches = json_object_get(root, "matches");
    if (!json_is_array(matches)) {
        fprintf(stderr, "Erreur : \"matches\" n'est pas un tableau JSON.\n");
        fclose(fichier);
        json_decref(root);
        return "-1";
    }

    int i;
    for (i = 0; i < json_array_size(matches); i++) {
        json_t *match = json_array_get(matches, i);
        // Recherche de la manche correspondante
        json_t *parties = json_object_get(match, "parties");
        if (!json_is_array(parties)) {
            fprintf(stderr, "Erreur : \"parties\" n'est pas un tableau JSON.\n");
            fclose(fichier);
            json_decref(root);
            return "-1";
        }

        int j;
        for (j = 0; j < json_array_size(parties); j++) {
            json_t *partie = json_array_get(parties, j);
            json_t *id_match_json = json_object_get(partie, "id_match");
            json_t *id_manche_json = json_object_get(partie, "id_manche");
            if (json_integer_value(id_manche_json) == id_manche && json_integer_value(id_match_json) == id_match) {
                // Mettre à jour le score pour le pseudo donné
                json_t *score1 = json_object_get(partie, "score1");
                json_t *score2 = json_object_get(partie, "score2");

                // Mettre à jour le score approprié selon le pseudo
                if (strcmp(pseudo, json_string_value(json_object_get(match, "joueur1"))) == 0) {
                    if(json_integer_value(score1) == -1){
                        json_integer_set(score1, 0);
                    }
                    json_integer_set(score1, json_integer_value(score1) + score); // Ajout du score
                } else if (strcmp(pseudo, json_string_value(json_object_get(match, "joueur2"))) == 0) {
                    if(json_integer_value(score2) == -1){
                        json_integer_set(score2, 0);
                    }
                    json_integer_set(score2, json_integer_value(score2) + score); // Ajout du score
                } else {
                    fprintf(stderr, "Erreur : Pseudo non trouvé dans le match.\n");
                    fclose(fichier);
                    json_decref(root);
                    return "-1";
                }

                // Enregistrement des modifications dans le fichier JSON
                fseek(fichier, 0, SEEK_SET);
                json_dumpf(root, fichier, JSON_INDENT(4));
                fclose(fichier);
                json_decref(root);

                // Convertir le score mis à jour en une chaîne de caractères
                char *score_string = malloc(sizeof(char) * 10); // Assez grand pour contenir des nombres jusqu'à 9 chiffres
                if (score_string == NULL) {
                    fprintf(stderr, "Erreur : Impossible d'allouer de la mémoire.\n");
                    return "-1";
                }
                sprintf(score_string, "%d", score);
                return score_string;
            }
        }
    }

    fprintf(stderr, "Erreur : ID de match non trouvé dans le fichier JSON.\n");
    fclose(fichier);
    json_decref(root);
    return "-1";
}

// Fonction pour récupérer l'adversaire du joueur pour une partie donnée
char *getAdversaire(const char *connected_pseudo, int id_match) {
    // Ouvrir et lire le fichier JSON
    FILE *fichier = fopen("jsons/matches.json", "r");
    if (fichier == NULL) {
        fprintf(stderr, "Erreur : Impossible d'ouvrir le fichier JSON.\n");
        return NULL;
    }

    json_error_t error;
    json_t *root = json_loadf(fichier, 0, &error);
    fclose(fichier);

    if (!root) {
        fprintf(stderr, "Erreur : Impossible de charger le fichier JSON (%s)\n", error.text);
        return NULL;
    }

    // Recherche du match correspondant
    json_t *matches = json_object_get(root, "matches");
    if (!json_is_array(matches)) {
        fprintf(stderr, "Erreur : \"matches\" n'est pas un tableau JSON.\n");
        json_decref(root);
        return NULL;
    }

    int i;
    for (i = 0; i < json_array_size(matches); i++) {
        json_t *match = json_array_get(matches, i);
        // Recherche de la partie correspondante
        json_t *parties = json_object_get(match, "parties");
        if (!json_is_array(parties)) {
            fprintf(stderr, "Erreur : \"parties\" n'est pas un tableau JSON.\n");
            json_decref(root);
            return NULL;
        }

        int j;
        for (j = 0; j < json_array_size(parties); j++) {
            json_t *partie = json_array_get(parties, j);
            json_t *idPartie_json = json_object_get(partie, "id_manche");
            if (!json_is_integer(idPartie_json)) {
                fprintf(stderr, "Erreur : \"id_manche\" n'est pas un entier JSON.\n");
                continue;
            }

            if (json_integer_value(idPartie_json) == id_match) {
                const char *joueur1 = json_string_value(json_object_get(match, "joueur1"));
                const char *joueur2 = json_string_value(json_object_get(match, "joueur2"));

                if (strcmp(connected_pseudo, joueur1) == 0) {
                    char *adversaire = strdup(joueur2);
                    json_decref(root);
                    return adversaire;
                } else if (strcmp(connected_pseudo, joueur2) == 0) {
                    char *adversaire = strdup(joueur1);
                    json_decref(root);
                    return adversaire;
                } else {
                    fprintf(stderr, "Erreur : Pseudo non trouvé dans le match.\n");
                    json_decref(root);
                    return NULL;
                }
            }
        }
    }
    fprintf(stderr, "Erreur : ID de partie non trouvé dans le fichier JSON.\n");
    json_decref(root);
    return NULL;
}

// Fonction pour changer le tour de la partie et vérifier si la partie est terminée
int changementTour(int id_match, int id_manche) {
    // Ouvrir et lire le fichier JSON
    FILE *fichier = fopen("jsons/matches.json", "r+");
    if (fichier == NULL) {
        fprintf(stderr, "Erreur : Impossible d'ouvrir le fichier JSON.\n");
        return -1;
    }

    // Charger le contenu JSON en mémoire
    json_error_t error;
    json_t *root = json_loadf(fichier, 0, &error);
    if (!root) {
        fprintf(stderr, "Erreur : Impossible de charger le fichier JSON (%s)\n", error.text);
        fclose(fichier);
        return -1;
    }

    // Fermer le fichier après avoir chargé le JSON en mémoire
    fclose(fichier);

    // Recherche du match correspondant
    json_t *matches = json_object_get(root, "matches");
    if (!json_is_array(matches)) {
        fprintf(stderr, "Erreur : \"matches\" n'est pas un tableau JSON.\n");
        json_decref(root);
        return -1;
    }

    // Parcourir les matches
    int i;
    for (i = 0; i < json_array_size(matches); i++) {
        json_t *match = json_array_get(matches, i);
        json_t *id_match_json = json_object_get(match, "id_match");
        if (!json_is_integer(id_match_json)) {
            fprintf(stderr, "Erreur : \"id_match\" n'est pas un entier JSON.\n");
            continue;
        }

        if (json_integer_value(id_match_json) == id_match) {
            // Recherche de la partie correspondante
            json_t *parties = json_object_get(match, "parties");
            if (!json_is_array(parties)) {
                fprintf(stderr, "Erreur : \"parties\" n'est pas un tableau JSON.\n");
                json_decref(root);
                return -1;
            }

            // Parcourir les parties
            int j;
            for (j = 0; j < json_array_size(parties); j++) {
                json_t *partie = json_array_get(parties, j);
                json_t *id_manche_json = json_object_get(partie, "id_manche");
                if (!json_is_integer(id_manche_json)) {
                    fprintf(stderr, "Erreur : \"id_manche\" n'est pas un entier JSON.\n");
                    continue;
                }

                if (json_integer_value(id_manche_json) == id_manche) {
                    // Vérifier si c'est la dernière manche
                    if (j == json_array_size(parties) - 1) {
                        // Si c'est la dernière manche, mettre "FINI" comme tour
                        json_t *tour = json_object_get(match, "tour");
                        json_string_set(tour, "FINI");
                    } else {
                        // Sinon, mettre le nom de l'adversaire comme tour
                        const char *joueur1 = json_string_value(json_object_get(match, "joueur1"));
                        const char *joueur2 = json_string_value(json_object_get(match, "joueur2"));
                        const char *connected_pseudo = json_string_value(json_object_get(match, "tour"));

                        if (strcmp(connected_pseudo, joueur1) == 0) {
                            json_t *tour = json_object_get(match, "tour");
                            json_string_set(tour, joueur2);
                        } else if (strcmp(connected_pseudo, joueur2) == 0) {
                            json_t *tour = json_object_get(match, "tour");
                            json_string_set(tour, joueur1);
                        } else {
                            fprintf(stderr, "Erreur : Pseudo non trouvé dans le match.\n");
                            json_decref(root);
                            return -1;
                        }
                    }

                    // Enregistrement des modifications dans le fichier JSON
                    FILE *fichier = fopen("jsons/matches.json", "w");
                    if (fichier == NULL) {
                        fprintf(stderr, "Erreur : Impossible d'ouvrir le fichier JSON pour écriture.\n");
                        json_decref(root);
                        return -1;
                    }

                    // Réécrire le fichier JSON uniquement avec les parties mises à jour
                    json_dumpf(root, fichier, JSON_INDENT(4));
                    fclose(fichier);
                    json_decref(root);
                    return 1;
                }
            }
            fprintf(stderr, "Erreur : ID de manche non trouvé dans le match.\n");
            json_decref(root);
            return -1;
        }
    }
    fprintf(stderr, "Erreur : ID de match non trouvé dans le fichier JSON.\n");
    json_decref(root);
    return -1;
}

// Fonction pour vérifier que l'identifiant de la partie existe déjà et que c'est bien le tour du joueur
int verifGame(const char* connected_pseudo, int id_match) {
    // Ouvrir et lire le fichier JSON
    FILE *fichier = fopen("jsons/matches.json", "r");
    if (fichier == NULL) {
        fprintf(stderr, "Erreur : Impossible d'ouvrir le fichier JSON.\n");
        return -1;
    }

    // Charger le contenu JSON en mémoire
    json_error_t error;
    json_t *root = json_loadf(fichier, 0, &error);
    if (!root) {
        fprintf(stderr, "Erreur : Impossible de charger le fichier JSON (%s)\n", error.text);
        fclose(fichier);
        return -1;
    }

    // Fermer le fichier après avoir chargé le JSON en mémoire
    fclose(fichier);

    // Recherche du match correspondant
    json_t *matches = json_object_get(root, "matches");
    if (!json_is_array(matches)) {
        fprintf(stderr, "Erreur : \"matches\" n'est pas un tableau JSON.\n");
        json_decref(root);
        return -1;
    }

    // Parcourir les matches
    int i;
    for (i = 0; i < json_array_size(matches); i++) {
        json_t *match = json_array_get(matches, i);
        json_t *id_match_json = json_object_get(match, "id_match");
        if (!json_is_integer(id_match_json)) {
            fprintf(stderr, "Erreur : \"id_match\" n'est pas un entier JSON.\n");
            continue;
        }

        if (json_integer_value(id_match_json) == id_match) {
            // Vérifier si c'est bien le tour du joueur
            const char *tour = json_string_value(json_object_get(match, "tour"));
            if (strcmp(tour, connected_pseudo) == 0) {
                json_decref(root);
                return 1; // C'est bien le tour du joueur
            } else {
                json_decref(root);
                return -1; // Ce n'est pas le tour du joueur
            }
        }
    }

    fprintf(stderr, "Erreur : ID de match non trouvé dans le fichier JSON.\n");
    json_decref(root);
    return -1;
}

// Fonction pour obtenir la prochaine manche à jouer
int nextManche(int id_match) {
    // Ouvrir et lire le fichier JSON
    FILE *fichier = fopen("jsons/matches.json", "r");
    if (fichier == NULL) {
        fprintf(stderr, "Erreur : Impossible d'ouvrir le fichier JSON.\n");
        return -1;
    }

    // Charger le contenu JSON en mémoire
    json_error_t error;
    json_t *root = json_loadf(fichier, 0, &error);
    if (!root) {
        fprintf(stderr, "Erreur : Impossible de charger le fichier JSON (%s)\n", error.text);
        fclose(fichier);
        return -1;
    }

    // Fermer le fichier après avoir chargé le JSON en mémoire
    fclose(fichier);

    // Recherche du match correspondant
    json_t *matches = json_object_get(root, "matches");
    if (!json_is_array(matches)) {
        fprintf(stderr, "Erreur : \"matches\" n'est pas un tableau JSON.\n");
        json_decref(root);
        return -1;
    }

    // Parcourir les matches
    int i;
    for (i = 0; i < json_array_size(matches); i++) {
        json_t *match = json_array_get(matches, i);
        json_t *id_match_json = json_object_get(match, "id_match");
        if (!json_is_integer(id_match_json)) {
            fprintf(stderr, "Erreur : \"id_match\" n'est pas un entier JSON.\n");
            continue;
        }

        if (json_integer_value(id_match_json) == id_match) {
            // Recherche de la prochaine manche non jouée
            json_t *parties = json_object_get(match, "parties");
            if (!json_is_array(parties)) {
                fprintf(stderr, "Erreur : \"parties\" n'est pas un tableau JSON.\n");
                json_decref(root);
                return -1;
            }

            // Parcourir les parties pour trouver la prochaine manche non jouée
            int j;
            for (j = 0; j < json_array_size(parties); j++) {
                json_t *partie = json_array_get(parties, j);
                json_t *score1 = json_object_get(partie, "score1");
                json_t *score2 = json_object_get(partie, "score2");

                // Si le score est -1, alors la manche n'a pas encore été jouée
                if (json_integer_value(score1) == -1 || json_integer_value(score2) == -1) {
                    json_decref(root);
                    return j + 1; // La prochaine manche à jouer est la manche suivante non jouée
                }
            }

            // Si toutes les manches ont été jouées, retourner 0
            json_decref(root);
            return 0;
        }
    }

    fprintf(stderr, "Erreur : ID de match non trouvé dans le fichier JSON.\n");
    json_decref(root);
    return -1;
}

/////////////////////////////////////////////////////////////////////////////// FONCTIONS (POUR ADELE) ///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////// TESTS /////////////////////////////////////////////////////////////////////////////////

// int main()
// {
//     // // Exemple d'utilisation
//     // connexion("lucie");  // Pseudo existant
//     // connexion("Jonathan");   // Pseudo ajouté
//     // connexion("ab");     // Pseudo trop court

//     // char *randomTheme = findRandomTheme();
//     // if (randomTheme) {
//     //     printf("Thème aléatoire : %s\n", randomTheme);
//     //     free(randomTheme);
//     // } else {
//     //     printf("Erreur lors de la récupération du thème aléatoire.\n");
//     // }

//     // const char* connected_player = "lucie";

//     // char* random_pseudo = get_random_pseudo(connected_player);
//     // if (random_pseudo) {
//     //     printf("Pseudo aléatoire pour le joueur connecté : %s\n", random_pseudo);
//     //     free(random_pseudo);
//     // }

//     // // Exemple d'utilisation de la fonction creategame
//     // creategame("lucie","antoine");

//     // int points = chercherMotDansJSON("PaysCommencantparP", "PEROU");
//     // printf("Points : %d\n", points);

//     fetchAllPlayers();

//     return 0;
// }

// int main()
// {

//     // connexion("lucie");

//     // char **pseudos = fetchAllPlayers("Achrafe\n");
//     // if (pseudos)
//     // {
//     //     printf("Liste des pseudos :\n");

//     //     // Afficher les pseudos jusqu'à ce que NULL soit rencontré
//     //     for (size_t i = 0; pseudos[i] != NULL; i++)
//     //     {
//     //         printf("%s", pseudos[i]);
//     //     }

//     //     // Libérer la mémoire allouée pour les pseudos
//     //     for (size_t i = 0; pseudos[i] != NULL; i++)
//     //     {
//     //         free(pseudos[i]);
//     //     }
//     //     free(pseudos); // Libérer la mémoire du tableau principal
//     // }
//     // else
//     // {
//     //     printf("Erreur lors de la récupération des pseudos.\n");
//     // }

//     // int id_match = 1;
//     // int id_manche = 1;

//     // char *theme = RecupTheme(id_match, id_manche);
//     // printf("%s\n", RecupTheme(id_match, id_manche));
//     // if (theme)
//     // {
//     //     printf("Thème pour le match %d et la manche %d : %s\n", id_match, id_manche, theme);
//     //     free(theme);
//     // }
//     // else
//     // {
//     //     printf("Aucun thème trouvé pour le match %d et la manche %d.\n", id_match, id_manche);
//     // }

//     // return 0;

//     // printf("%d",creategame("Agathe\n",
//     //            "Achrafe\n"));

//     // return 0;

//     // Appel de la fonction RecupTheme avec des valeurs d'exemple pour id_match et id_manche
//     int id_match = 1;
//     int id_manche = 2;
//     char *theme = RecupTheme(id_match, id_manche);

//     // Vérification si la récupération du thème a réussi
//     if (theme != NULL)
//     {
//         printf("Thème pour le match %d et la manche %d : %s\n", id_match, id_manche, theme);
//         free(theme);
//     }
//     else
//     {
//         printf("Erreur lors de la récupération du thème.\n");
//     }

//     return 0;
// }

// int main()
// {
//     creategame("Agathe\n", "Achrafe\n");
//     //printf("%s",mettreAJourScore(1, 5, "Agathe\n", "français"));
//     return 0;
// }

int main() {
    // Pseudo du joueur connecté et ID de la partie
    // const char *connected_pseudo = "Agathe\n"; // Mettez ici le pseudo du joueur connecté
    // int idPartie = 1; // Mettez ici l'ID de la partie

    // // Récupération du score du match pour le joueur connecté
    // // char *score = scoreGame(connected_pseudo, idPartie);
    // // if (score == NULL) {
    // //     fprintf(stderr, "Erreur : Impossible de récupérer le score du match.\n");
    // //     return EXIT_FAILURE;
    // // }
    // // printf("Score du match pour %s : %s\n", connected_pseudo, score);
    // // free(score);

    // // Récupération de l'adversaire du joueur pour la partie donnée
    // char *adversaire = getAdversaire(connected_pseudo, idPartie);
    // if (adversaire == NULL) {
    //     fprintf(stderr, "Erreur : Impossible de récupérer l'adversaire.\n");
    //     return EXIT_FAILURE;
    // }
    // printf("Adversaire de %s : %s\n", connected_pseudo, adversaire);
    // free(adversaire);


    //     int id_match = 1; // ID du match
    // int id_manche = 3; // ID de la manche

    // int partie_terminee = changementTour(id_match, id_manche);
    // if (partie_terminee == -1) {
    //     fprintf(stderr, "Erreur lors du changement de tour.\n");
    //     return 1;
    // }

    // if (partie_terminee) {
    //     printf("La partie n'est pas terminée. Le tour a été changé.\n");
    // } else {
    //     printf("La partie est terminée.\n");
    // }

    //     // Exemple d'utilisation de la fonction verifGame
    // const char* joueur_connecte = "Agathe\n";

    // int resultat = verifGame(joueur_connecte, id_match);
    // if (resultat == 1) {
    //     printf("C'est bien le tour du joueur %s dans la partie %d.\n", joueur_connecte, id_match);
    // } else if (resultat == -1) {
    //     printf("Ce n'est pas le tour du joueur %s dans la partie %d.\n", joueur_connecte, id_match);
    // } else {
    //     printf("Erreur lors de la vérification de la partie.\n");
    // }


    // Exemple d'utilisation de la fonction nextManche
    int id_match = 1;

    int prochaine_manche = nextManche(id_match);
    if (prochaine_manche == -1) {
        printf("Erreur lors de la recherche de la prochaine manche.\n");
    } else if (prochaine_manche == 0) {
        printf("Toutes les manches ont été jouées pour le match %d.\n", id_match);
    } else {
        printf("La prochaine manche à jouer pour le match %d est la manche %d.\n", id_match, prochaine_manche);
    }

    return 0;


    return 0;

    return EXIT_SUCCESS;
}
///////////////////////////////////////////////////////////////////////////////// TESTS /////////////////////////////////////////////////////////////////////////////////
