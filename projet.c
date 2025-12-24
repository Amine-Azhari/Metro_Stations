#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include <string.h>
#include <time.h> //hadi bash ndiru clock

#define NUM_STATIONS 125
#define HASH_SIZE 127

typedef struct {
    int dest;
    int time; //wieght
}Edge;

//hena dert structure dyal hash tables coz they asked forit to look for stations
typedef struct HashEntry {
    char *name;
    int id;
    struct HashEntry *next;
} HashEntry;

typedef struct {
    int id_station;  
    int degre;       
} StationDegre;

Edge** adjacency_list;
char** stations;
int* edge_count;
HashEntry** hash_table;

// ghadi ykhessuna desfonctions utilitaires pour les tables de hashage dakshi 3lash zedthum hena
unsigned int hash(const char *name) {
    unsigned int hash_value = 0;
    while (*name) {
        hash_value = (hash_value * 31) + *name;
        name++;
    }
    return hash_value % HASH_SIZE;
}

// hena l'initialisation 
void init_hash_table() {
    hash_table = malloc(HASH_SIZE * sizeof(HashEntry*));
    for (int i = 0; i < HASH_SIZE; i++) {
        hash_table[i] = NULL;
    }
}

// hena kandiru l'ajout dyal une station f table de hachage
void hash_table_insert(char *name, int id) {
    unsigned int index = hash(name);
    
    // kandiru une nouvelle entrée
    HashEntry *new_entry = malloc(sizeof(HashEntry));
    new_entry->name = malloc(strlen(name) + 1);
    strcpy(new_entry->name, name);
    new_entry->id = id;
    new_entry->next = NULL;
    
    // u hena  kanziduha debut de la list
    if (hash_table[index] == NULL) {
        hash_table[index] = new_entry;
    } else {
        new_entry->next = hash_table[index];
        hash_table[index] = new_entry;
    }
}

//hadi hiya la fonction li telbuha menaa for the search processs
int hash_table_search(const char *name) {
    unsigned int index = hash(name);
    HashEntry *current = hash_table[index];
    
    while (current != NULL) {
        if (strcmp(current->name, name) == 0) {
            return current->id;  
        }
        current = current->next;
    }
    return -1;  
}

//hena dert had la fonction bash tseheel 3Liya research f la commande 1 et 2

int find_station(char *input) {
    input[strcspn(input, "\n")] = '\0';
    
    char *endptr;
    long id = strtol(input, &endptr, 10);
    
    if (*endptr == '\0') {
        int station_id = (int)id;
        if (station_id >= 0 && station_id < NUM_STATIONS && stations[station_id] != NULL) {
            return station_id;
        } else {
            printf("ID invalide: %d\n", station_id);
            return -1;
        }
    } else {
        // ila kan name kanidru table dyal hashge
        int station_id = hash_table_search(input);
        if (station_id == -1) {
            printf("Station '%s' non trouvée\n", input);
        }
        return station_id;
    }
}

// hena kanverifier wash ID valide wella la
bool is_valid_id(int id) {
    return (id >= 0 && id < NUM_STATIONS && stations[id] != NULL);
}

//hadi hiya la fonction lewela dyal affichafe dyal les infos pour une station
void display_station_info(char *input) {
    int station_id = find_station(input);
    if (station_id == -1) return;
    
    printf("\n=== INFORMATIONS STATION ===\n");
    printf("ID      : %d\n", station_id);
    printf("Nom     : %s\n", stations[station_id]);
    printf("Degré   : %d voisin(s) direct(s)\n", edge_count[station_id]);
    printf("=============================\n\n");
}

//hadi la deuxieme fonction pour lister les voisins d'une station
void list_station_neighbors(char *input) {
    int station_id = find_station(input);
    if (station_id == -1) return;
    
    printf("\n=== VOISINS DE '%s' (ID: %d) ===\n", stations[station_id], station_id);
    
    if (edge_count[station_id] == 0) {
        printf("Aucun voisin direct.\n");
    } else {
        for (int i = 0; i < edge_count[station_id]; i++) {
            int dest = adjacency_list[station_id][i].dest;
            int time = adjacency_list[station_id][i].time;
            printf("→ %d : %s (temps: %d min)\n", dest, stations[dest], time);
        }
    }
    printf("===============================\n\n");
}


//to find the station who has the minimal distance between the unvisited stations
//very useful for djikstra alogrithm
int min_distance(int distances[], bool visited[], int size)
{
    int min = INT_MAX, min_index = -1;
    for (int v = 0; v<size; v++)
    {
        if ( !visited[v] && distances[v] <= min )
        {
            min = distances[v];
            min_index = v;
        }
    }
    return min_index;
}

//Djikstra Hohohooo
//hena dert wa7ed 3variables pour stocker les calculs dyal dij
int **dijkstra_distances = NULL;    // distances mn source distance
int **dijkstra_predecessors = NULL; // 
bool *dijkstra_computed = NULL; // hada kay3ti true ila caculé

//dert initialisation dijikstra
void init_dijkstra_storage() {
    dijkstra_distances = malloc(NUM_STATIONS * sizeof(int*));
    dijkstra_predecessors = malloc(NUM_STATIONS * sizeof(int*));
    dijkstra_computed = malloc(NUM_STATIONS * sizeof(bool));
    
    for (int i = 0; i < NUM_STATIONS; i++) {
        dijkstra_distances[i] = NULL;
        dijkstra_predecessors[i] = NULL;
        dijkstra_computed[i] = false;
    }
}

void compute_dijkstra_from_source(int source) {
    // hena zedt hadi bash opyimiser shwiya complexité de telle façon si c'est deja calculé maghadish n3awedu calcul
    if (dijkstra_computed[source]) {
        return;
    }

    dijkstra_distances[source] = malloc(NUM_STATIONS * sizeof(int));
    dijkstra_predecessors[source] = malloc(NUM_STATIONS * sizeof(int));
    
    bool visited[NUM_STATIONS];

    for (int i = 0; i < NUM_STATIONS; i++) {
        dijkstra_distances[source][i] = INT_MAX;
        dijkstra_predecessors[source][i] = -1;
        visited[i] = false;
    }
    dijkstra_distances[source][source] = 0; //hadi khelass distance mn source l source raha nulle

    for (int count = 0; count < NUM_STATIONS; count++) {
        // hena khdemt b la fonction dyalk mindstance
        int u = min_distance(dijkstra_distances[source], visited, NUM_STATIONS);
        
        if (u == -1) break; 
        
        visited[u] = true;
        
        // hena kandir mise à jour dyal distance dyal neighbours
        for (int i = 0; i < edge_count[u]; i++) {
            int neighbor = adjacency_list[u][i].dest;
            int time = adjacency_list[u][i].time;
            
            if (!visited[neighbor]) {
                int new_dist = dijkstra_distances[source][u] + time;
                
                if (new_dist < dijkstra_distances[source][neighbor]) {
                    dijkstra_distances[source][neighbor] = new_dist;
                    dijkstra_predecessors[source][neighbor] = u;
                }
            }
        }
    }
    
    dijkstra_computed[source] = true;
}

void find_shortest_path(char *start_input, char *end_input) {
    
    int start = find_station(start_input);
    int end = find_station(end_input);
    
    if (start == -1 || end == -1) {
        printf("Erreur : Station invalide !\n");
        return;
    }
    
    
    compute_dijkstra_from_source(start);

    // hena kan dir verification wash chemin kayn wella la
    if (dijkstra_distances[start][end] == INT_MAX) {
        printf("\n  Pas de chemin entre %s et %s\n", stations[start], stations[end]);
        return;
    }
    
    // hena kandir l'affichage 
    printf("\n═══════════════════════════════════════\n");
    printf("CHEMIN LE PLUS COURT\n");
    printf("═══════════════════════════════════════\n");
    printf("De     : %s (ID: %d)\n", stations[start], start);
    printf("À      : %s (ID: %d)\n", stations[end], end);
    printf("Temps  : %d minutes\n", dijkstra_distances[start][end]);
    printf("═══════════════════════════════════════\n");
    
    // hena zedt hadi bash ndiru reconstruction dyal chemin (7it galluha lina f sujet)
    printf("Chemin : ");
    
    // kanstocker chemin f wa7eed tableau
    int path[NUM_STATIONS];
    int path_length = 0;
    int current = end;
    
    // kan9eleb 3la les predecesseurs
    while (current != -1) {
        path[path_length++] = current;
        current = dijkstra_predecessors[start][current];
    }
    
    // hena l'affichage 
    for (int i = path_length - 1; i >= 0; i--) {
        printf("%s", stations[path[i]]);
        if (i > 0) printf(" → ");
    }
    
    printf("\n═══════════════════════════════════════\n\n");
}

//hadi function dyal tri pra selectoin
void tri_selection(StationDegre arr[], int n, int* comparisons, int* swaps) {
    *comparisons = 0;  
    *swaps = 0;
    for (int i = 0; i < n - 1; i++) {
        int min_idx = i;

        for (int j = i + 1; j < n; j++) {
            (*comparisons)++;
            if (arr[j].degre < arr[min_idx].degre) {
                min_idx = j;  
            }
        }

        if (min_idx != i) {
            StationDegre temp = arr[i]; 
            arr[i] = arr[min_idx];
            arr[min_idx] = temp; 
            (*swaps)++;
            
        }
    }        

}

//function dyal tri par insertion 
void tri_insertion(StationDegre arr[], int n, int* comparisons, int* swaps) {
    *comparisons = 0;  
    *swaps = 0;
    for (int i = 1; i < n; i++) {
        StationDegre key = arr[i]; 
        int j = i - 1;
        (*comparisons)++;

        while (j >= 0 && arr[j].degre > key.degre) {
            if (j != i - 1) (*comparisons)++;
            arr[j + 1] = arr[j]; 
            (*swaps)++;
            j--; 

        }

        if (j + 1 != i) { 
            arr[j + 1] = key;
            (*swaps)++;

        }

    }
}

//function dyal quicksort
int partition(StationDegre arr[], int low, int high, int* comparisons, int* swaps) {
    int pivot = arr[high].degre;  
    int i = low - 1;   

    for (int j = low; j <= high - 1; j++) {
        (*comparisons)++;

        if (arr[j].degre <= pivot) {
            i++;
            if (i != j) {
                StationDegre temp = arr[i];
                arr[i] = arr[j];
                arr[j] = temp;
                (*swaps)++;  
            }

        }


    }

    if (i + 1 != high) {
        StationDegre temp = arr[i + 1];
        arr[i + 1] = arr[high];
        arr[high] = temp;
        (*swaps)++;
    }
    return i+1;

}

//function récurssivve dyal tri quicksort
void tri_rapide_recursif(StationDegre arr[], int low, int high, int* comparisons, int* swaps) {
    if (low < high) {
        int pi = partition(arr, low, high, comparisons, swaps);
        tri_rapide_recursif(arr, low, pi - 1, comparisons, swaps);  
        tri_rapide_recursif(arr, pi + 1, high, comparisons, swaps);
    }
}

void tri_rapide(StationDegre arr[], int n, int* comparisons, int* swaps) {
    *comparisons = 0;  
    *swaps = 0;        
    
    
    tri_rapide_recursif(arr, 0, n - 1, comparisons, swaps);
}

void afficher_stations_triees(int choix_tri) {
    int stations_valides = 0;
    for (int i = 0; i < NUM_STATIONS; i++) {
        if (stations[i] != NULL) {
            stations_valides++;
        }
    }

    StationDegre *tab_stations = malloc(stations_valides * sizeof(StationDegre));
    if (tab_stations == NULL) {
        printf("Erreur d'allocation mémoire\n");
        return;  
    }
    int index = 0;
    for (int i = 0; i < NUM_STATIONS; i++) {
        if (stations[i] != NULL) {
            tab_stations[index].id_station = i;
            tab_stations[index].degre = edge_count[i];
            index++;
        }

    }

    int comparisons = 0, swaps = 0;  
    char* nom_tri = "";             
    clock_t start, end;              
    double cpu_time_used;   

    start = clock();
    switch(choix_tri) {
        case 1:
            nom_tri = "Tri par sélection";
            tri_selection(tab_stations, stations_valides, &comparisons, &swaps);
            break;
        case 2:
            nom_tri = "Tri par insertion";
            tri_insertion(tab_stations, stations_valides, &comparisons, &swaps);
            break;
        case 3:
            nom_tri = "Tri rapide (Quicksort)";
            tri_rapide(tab_stations, stations_valides, &comparisons, &swaps);
            break;
        default:
            
            printf("Choix de tri invalide\n");
            free(tab_stations);  
            return;
    }
    
    end = clock();  
    
    // hena kandir calcul dyal clock
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    
    
    printf("\n══════════════════════════════════════════════════════════\n");
    printf("STATIONS TRIÉES PAR DEGRÉ CROISSANT (%s)\n", nom_tri);
    printf("══════════════════════════════════════════════════════════\n");
    
    printf("%-4s | %-40s | %-6s\n", "ID", "NOM", "DEGRÉ");
    printf("------|------------------------------------------|--------\n");
    
    for (int i = 0; i < stations_valides; i++) {
        printf("%-4d | %-40s | %-6d\n", 
               tab_stations[i].id_station,                         
               stations[tab_stations[i].id_station],               
               tab_stations[i].degre);                             
    }
    
    printf("\n══════════════════════════════════════════════════════════\n");
    printf("STATISTIQUES DU TRI :\n");
    printf("• Algorithme utilisé   : %s\n", nom_tri);
    printf("• Nombre de stations   : %d\n", stations_valides);
    printf("• Nombre de comparaisons : %d\n", comparisons);
    printf("• Nombre d'échanges      : %d\n", swaps);
    printf("• Temps d'exécution      : %.6f secondes\n", cpu_time_used);
    printf("══════════════════════════════════════════════════════════\n\n");
    
    free(tab_stations);




}



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

    if (dijkstra_distances != NULL) {
        for (int i = 0; i < NUM_STATIONS; i++) {
            if (dijkstra_distances[i] != NULL) {
                free(dijkstra_distances[i]);
            }
            if (dijkstra_predecessors[i] != NULL) {
                free(dijkstra_predecessors[i]);
            }
        }
        free(dijkstra_distances);
        free(dijkstra_predecessors);
    }
    if (dijkstra_computed != NULL) {
        free(dijkstra_computed);
    }
    
    
    if (hash_table != NULL) {
        for (int i = 0; i < HASH_SIZE; i++) {
            HashEntry *entry = hash_table[i];
            while (entry != NULL) {
                HashEntry *next = entry->next;
                free(entry->name);
                free(entry);
                entry = next;
            }
        }
        free(hash_table);
    }
    
    
    for (int i = 0; i < NUM_STATIONS; i++) {
        if (stations[i] != NULL) {
            free(stations[i]);
        }
        if (adjacency_list[i] != NULL) {
            free(adjacency_list[i]);
        }
    }
    
    free(adjacency_list);
    free(stations);
    free(edge_count);
    
    return 0;

}
