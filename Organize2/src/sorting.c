#include "common.h"
#include "sorting.h"

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