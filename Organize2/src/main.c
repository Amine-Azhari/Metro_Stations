#include "common.h"
#include "hash_table.h"
#include "graph.h"
#include "sorting.h"

int main(int argc, char** argv)

{
    int choice;
    char station_input[100], start_input[100], end_input[100];
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


    FILE *fptr;

    fptr = fopen(argv[1], "r");

    if (!fptr) {
        perror("Erreur ouverture fichier");
        return 1;
    }

    char myString[100];

    while (fgets(myString, sizeof(myString), fptr)) {
        if (strncmp(myString, "STATION;", 8) == 0) {

            char *type = strtok(myString, ";"); 
            char *id   = strtok(NULL, ";");
            char *name = strtok(NULL, ";");

            if (id && name) {
                int station_id = atoi(id);

                name[strcspn(name,"\n")]='\0';

                char *station_name = malloc(strlen(name) + 1);
                strcpy(station_name, name);
                stations[station_id] = station_name;
                hash_table_insert(name,station_id);
            }
        }

        else if (strncmp(myString, "EDGE;", 5) == 0)
        {
            char *type = strtok(myString, ";"); // "EDGE"
            char *start_str   = strtok(NULL, ";");
            char *end_str = strtok(NULL, ";");
            char *time_str = strtok(NULL, ";");

            if (start_str && end_str && time_str)
            {
                int start_station = atoi(start_str);
                int end_station = atoi(end_str);
                int time =atoi(time_str);
                
                if (edge_count[start_station] >= 10) {
                    
                    adjacency_list[start_station] = realloc(
                        adjacency_list[start_station], 
                        (edge_count[start_station] + 5) * sizeof(Edge)
                    );
                }
                
                adjacency_list[start_station][edge_count[start_station]].dest = end_station;
                adjacency_list[start_station][edge_count[start_station]].time = time;
                edge_count[start_station]++;
            }
        }
    }
            
        
    

    // Close the file
    fclose(fptr);
    do {
        printf("===== MENU RESEAU DE TRANSPORT =====\n");
        printf("1- Afficher les informations d'une station\n");
        printf("2- Lister les voisins d'une station\n");
        printf("3- Calculer un chemin minimal\n");
        printf("4- Afficher les stations triées par degré\n");
        printf("0- Quitter\n");
        printf("Votre choix :");

        
        scanf(" %d", &choice);
        getchar();

        switch(choice)
        {
            case (1): {
                
                printf("Nom ou ID de la station : ");
                fgets(station_input, sizeof(station_input), stdin);
                display_station_info(station_input);
                break;
            }

            case (2): {
               
                printf("Nom ou ID de la station : ");
                fgets(station_input, sizeof(station_input), stdin);
                list_station_neighbors(station_input);
                break;
            }

            case 3: {
                    
                    printf("Station de départ : ");
                    fgets(start_input, sizeof(start_input), stdin);
                    printf("Station d'arrivée : ");
                    fgets(end_input, sizeof(end_input), stdin);
                    find_shortest_path(start_input, end_input);  
                    break;
                }
            
            case (4): {
                int choix_tri;
                printf("\n===== TRI DES STATIONS PAR DEGRÉ =====\n");
                printf("Choisissez l'algorithme de tri :\n");
                printf("1 - Tri par sélection\n");
                printf("2 - Tri par insertion\n");
                printf("3 - Tri rapide (Quicksort)\n");
                printf("Votre choix : ");
                scanf("%d", &choix_tri);
                getchar(); 
                
                
                if (choix_tri >= 1 && choix_tri <= 3) {
                    afficher_stations_triees(choix_tri);
                } else {
                    printf("Choix invalide ! Veuillez choisir 1, 2 ou 3.\n");
                }
                break;

            }

            case 0:
                printf("Au revoir !\n");
                break;
            default:
                printf("Choix invalide. Essayez encore.\n");
        }
    } while (choice !=0);


    free_hash_table();
    free_graph_data();
        
    return 0;

}