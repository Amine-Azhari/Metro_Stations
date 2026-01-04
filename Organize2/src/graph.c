#include "common.h"
#include "hash_table.h"
#include "graph.h"

Edge** adjacency_list = NULL;
char** stations = NULL;
int* edge_count = NULL;

int **dijkstra_distances = NULL;
int **dijkstra_predecessors = NULL;
bool *dijkstra_computed = NULL;

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

int find_station(char *input) {
    input[strcspn(input, "\r\n")] = '\0';
    
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
        int station_id = hash_table_search(input);
        if (station_id == -1) {
            printf("Station '%s' non trouvée\n", input);
        }
        return station_id;
    }
}

void display_station_info(char *input) {
    int station_id = find_station(input);
    if (station_id == -1) return;
    
    printf("\n=== INFORMATIONS STATION ===\n");
    printf("ID      : %d\n", station_id);
    printf("Nom     : %s\n", stations[station_id]);
    printf("Degré   : %d voisin(s) direct(s)\n", edge_count[station_id]);
    printf("=============================\n\n");
}

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
    dijkstra_distances[source][source] = 0;

    for (int count = 0; count < NUM_STATIONS; count++) {
        int u = min_distance(dijkstra_distances[source], visited, NUM_STATIONS);
        
        if (u == -1) break; 
        
        visited[u] = true;
        
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

void free_graph_data() {
    for (int i = 0; i < NUM_STATIONS; i++) {
        if (stations[i]) free(stations[i]);
        if (adjacency_list[i]) free(adjacency_list[i]);
        if (dijkstra_distances[i]) free(dijkstra_distances[i]);
        if (dijkstra_predecessors[i]) free(dijkstra_predecessors[i]);
    }
    free(stations); free(adjacency_list); free(edge_count);
    free(dijkstra_distances); free(dijkstra_predecessors); free(dijkstra_computed);
}
