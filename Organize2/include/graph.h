#ifndef GRAPH_H
#define GRAPH_H

void init_dijkstra_storage();
int find_station(char *input);
void display_station_info(char *input);
void list_station_neighbors(char *input);
void find_shortest_path(char *start_input, char *end_input);
void free_graph_data();

#endif