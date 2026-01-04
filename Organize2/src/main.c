#include "common.h"
#include "hash_table.h"
#include "graph.h"
#include "sorting.h"
#include "loadis.h"


int main(int argc, char** argv)
{
    if (argc < 2) {
        printf("Erreur : Veuillez préciser le fichier.\n");
        return 1;
    }
    int choice;
    adjacency_list = malloc(NUM_STATIONS * sizeof(Edge*));
    stations = malloc(NUM_STATIONS * sizeof(char*));
    edge_count = malloc(NUM_STATIONS * sizeof(int));
    init_hash_table();
    init_dijkstra_storage();

    for (int i = 0; i < NUM_STATIONS; i++) {
        adjacency_list[i] = malloc(10 * sizeof(Edge));
        stations[i] = NULL;
        edge_count[i] = 0;
    }

    load_graph(argv[1]);

    do {
        printf("===== MENU RESEAU DE TRANSPORT =====\n");
        printf("1- Afficher les informations d'une station\n");
        printf("2- Lister les voisins d'une station\n");
        printf("3- Calculer un chemin minimal\n");
        printf("4- Afficher les stations triées par degré\n");
        printf("0- Quitter\n");
        printf("Votre choix : ");

        
        scanf(" %d", &choice);
        getchar();

        display(choice);
        
    } while (choice !=0);


    free_hash_table();
    free_graph_data();
        
    return 0;

}
