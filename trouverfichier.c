#include <stdio.h>
#include <jansson.h>
#include <string.h>

// Fonction pour chercher un mot (thème) dans le fichier JSON
void chercherThemeDansJSON(const char *theme)
{
    // Ouvrir le fichier JSON
    FILE *file = fopen("jsons/reponsestheme.json", "r");
    if (file == NULL)
    {
        printf("Erreur lors de l'ouverture du fichier.\n");
        return;
    }

    // Charger le contenu du fichier JSON
    json_t *root;
    json_error_t error;
    root = json_loadf(file, 0, &error);
    fclose(file);

    // Vérifier si le thème est présent dans le fichier JSON
    json_t *themeObj = json_object_get(root, theme);
    if (themeObj != NULL && json_is_object(themeObj))
    {
        // Le thème est présent dans le fichier JSON
        printf("Thème : %s\n", theme);

        // Parcourir les pays associés au thème
        const char *valeurTheme;
        json_t *pointsObj;
        json_object_foreach(themeObj, valeurTheme, pointsObj)
        {
            if (json_is_integer(pointsObj))
            {
                int points = json_integer_value(pointsObj);
                printf("  - Valeur : %s, Points : %d\n", valeurTheme, points);
            }
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

// Fonction pour chercher un mot dans le fichier JSON
void chercherMotDansJSON(const char *theme, const char *mot)
{
    // Ouvrir le fichier JSON
    FILE *file = fopen("jsons/reponsestheme.json", "r");
    if (file == NULL)
    {
        printf("Erreur lors de l'ouverture du fichier.\n");
        return;
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

        // Vérifier si le mot est présent dans le thème
        json_t *motObj = json_object_get(themeObj, mot);
        if (motObj != NULL && json_is_integer(motObj))
        {
            // Le mot est présent dans le thème
            int points = json_integer_value(motObj);
            printf("  - Mot : %s, Points : %d\n", mot, points);
        }
        else
        {
            // Le mot n'est pas présent dans le thème
            printf("Le mot '%s' n'est pas présent dans le thème '%s'.\n", mot, theme);
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

int main()
{
    while (1)
    {
        printf("\nMode recherche manuel (1) ou Mode jeu Test (2) ou Quitter (3) :\n");
        int choix;
        scanf("%d", &choix);
        printf("\nVous avez choisi : %d\n", choix);
        switch (choix)
        {
        case 1:
            printf("Mode recherche manuel\n");
            printf("Entrez un thème (ou 'exit' pour quitter) : ");
            char theme[100];
            scanf("%s", theme);

            if (strcmp(theme, "exit") == 0)
            {
                return 0;
            }

            chercherThemeDansJSON(theme);
            break;

        case 2:
            printf("Mode jeu Test\n");
            printf("Entrez un thème : ");
            char themeJeu[100];
            scanf("%s", themeJeu);

            printf("Entrez un mot : ");
            char motJeu[100];
            scanf("%s", motJeu);
            

            chercherMotDansJSON(themeJeu, motJeu);
            break;

        case 3:
            return 0;

        default:
            printf("Choix invalide.\n");
            break;
        }
    }

    return 0;
}
