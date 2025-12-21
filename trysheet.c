#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include <string.h>

#define NUM_STATIONS 125

typedef struct {
    int dest;
    int time; //wieght
}Edge;

Edge** adjacency_list;
char** stations;
int* edge_count;


void info_about(char* station)
{
    station[strcspn(station, "\n")] = '\0';

    int i = 0;
    while (i < NUM_STATIONS && stations[i] != NULL && strcmp(stations[i], station) != 0)
        i++;

    if (i == NUM_STATIONS || stations[i] == NULL) {
        printf("didn't found!\n");
        return;
    }

    for (int j = 0; j < edge_count[i]; j++) {
        int dest = adjacency_list[i][j].dest;
        int time = adjacency_list[i][j].time;
        printf("-> %s (time: %d)\n", stations[dest], time);
    }
}



int main(int argc, char** argv)
{
    adjacency_list = malloc(NUM_STATIONS * sizeof(Edge*));
    stations = malloc(NUM_STATIONS * sizeof(char*));
    edge_count = malloc(NUM_STATIONS * sizeof(int));

    for (int i = 0; i < NUM_STATIONS; i++) {
        adjacency_list[i] = malloc(10 * sizeof(Edge));
        stations[i] = NULL;
        edge_count[i] = 0;
    }


    FILE *fptr;

    fptr = fopen(argv[1], "r");

    if (!fptr) {
        perror("fopen");
        return 1;
    }

    char myString[100];

    while (fgets(myString, sizeof(myString), fptr)) {
        if (strncmp(myString, "STATION;", 8) == 0) {

            char *type = strtok(myString, ";"); // "STATION"
            char *id   = strtok(NULL, ";");
            char *name = strtok(NULL, ";");

            if (id && name) {
                int station_id = atoi(id);

                char *station_name = malloc(strlen(name) + 1);
                strcpy(station_name, name);
                station_name[strcspn(station_name, "\n")] = '\0';

                stations[station_id] = station_name;
            }
        }

        else if (strncmp(myString, "EDGE;", 5) == 0)
        {
            char *type = strtok(myString, ";"); // "EDGE"
            char *start   = strtok(NULL, ";");
            char *end = strtok(NULL, ";");
            char *time = strtok(NULL, ";");

            if (start && end && time)
            {
                int start_station = atoi(start);
                int end_station = atoi(end);
                adjacency_list[start_station][edge_count[start_station]].dest = end_station;
                adjacency_list[start_station][edge_count[start_station]].time = atoi(time);
                edge_count[start_station]++;
            }
        }
    }

    // Close the file
    fclose(fptr);

    char station[100];
    printf("info about station : ");
    fgets(station, 100, stdin);
    info_about(station);
    
    
}