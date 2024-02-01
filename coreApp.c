#include <stdio.h>
#include <jansson.h>
#include <string.h>

#define MAX_PSEUDO_LENGTH 50

int recherchePseudo(json_t *pseudosArray, const char *pseudo) {
    size_t index;
    json_t *value;

    // Parcourir le tableau des pseudos
    json_array_foreach(pseudosArray, index, value) {
        const char *existingPseudo = json_string_value(value);
        if (strcmp(existingPseudo, pseudo) == 0) {
            // Le pseudo existe déjà
            return 1;
        }
    }

    // Le pseudo n'existe pas
    return 0;
}

void ajouterPseudo(json_t *pseudosArray, const char *pseudo) {
    // Ajouter le nouveau pseudo à la liste
    json_array_append_new(pseudosArray, json_string(pseudo));
}

int connexion(const char *pseudo) {
    // Ouvrir le fichier JSON
    FILE *file = fopen("jsons/pseudos.json", "r");
    if (file == NULL) {
        printf("Erreur lors de l'ouverture du fichier.\n");
        return 0;
    }

    // Charger le contenu du fichier JSON
    json_t *root;
    json_error_t error;
    root = json_loadf(file, 0, &error);
    fclose(file);

    // Vérifier si le pseudo est valide
    if (strlen(pseudo) < 3) {
        printf("Le pseudo doit avoir au moins 3 lettres.\n");
        return 0;
    }

    // Vérifier si le pseudo existe déjà
    json_t *pseudosArray = json_object_get(root, "pseudos");
    if (recherchePseudo(pseudosArray, pseudo)) {
        printf("Le pseudo '%s' existe déjà.\n", pseudo);
        return 1;
    }

    // Ajouter le nouveau pseudo
    ajouterPseudo(pseudosArray, pseudo);

    // Enregistrer les modifications dans le fichier JSON
    FILE *writeFile = fopen("jsons/pseudos.json", "w");
    if (writeFile == NULL) {
        printf("Erreur lors de l'ouverture du fichier en écriture.\n");
        return 0;
    }

    json_dumpf(root, writeFile, JSON_INDENT(2));
    fclose(writeFile);

    // Libérer la mémoire
    json_decref(root);

    printf("Le pseudo '%s' a été ajouté avec succès.\n", pseudo);
    return 1;
}

int main() {
    // Exemple d'utilisation
    connexion("lucie");  // Pseudo existant
    connexion("Jonathan");   // Pseudo ajouté
    connexion("ab");     // Pseudo trop court

    return 0;
}
