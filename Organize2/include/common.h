#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include <string.h>
#include <time.h>

#define NUM_STATIONS 125
#define HASH_SIZE 127

// Structures
typedef struct {
    int dest;
    int time; 
} Edge;

typedef struct {
    int id_station;  
    int degre;       
} StationDegre;

// Variables globales
extern Edge** adjacency_list;
extern char** stations;
extern int* edge_count;

#endif