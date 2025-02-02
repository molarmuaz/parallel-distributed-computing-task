//Muhammad Muaz Ahmed i221125 Section D PDC Assignment 1 | Question 1

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define RESULT 3072441 
#define THREADS 1  // Tested with 1, 2, 4, 8, 16
#define EXP_NODES 3072441
#define EXP_EDGES 117185083

// Struct to divide the file into threads
struct ThreadSegment {
    char* f_name;
    int* n_deg;
    int edges;
    long start;
    long end;
};

//mutex lock
pthread_mutex_t lock;

// It is less complicated if there are global variables rather than shared variables that can cause a miscount
int total_n_deg[RESULT + 1] = {0};  
int total_edges = 0;                    


void* thread_proc(void* arg) { // Calculates the node degrees and edges of each chunk/thread
    struct ThreadSegment* data = (struct ThreadSegment*)arg;
    FILE* f = fopen(data->f_name, "r");
    if (!f) {
        fprintf(stderr, "ERROR %s\n", data->f_name);
        return NULL;
    }

    
    fseek(f, data->start, SEEK_SET); // finds the start point
    char line[128];
    int from, to;

    while (ftell(f) < data->end && fgets(line, sizeof(line), f) != NULL) {

        if (line[0] == '#') continue; //ignores the initial lines that do not represent the graph

        if (sscanf(line, "%d %d", &from, &to) == 2) {
            data->n_deg[from]++;
            data->n_deg[to]++;
            data->edges++;
        }
    }

    fclose(f);
    return NULL;
}


int main() {

    clock_t start_time = clock(); //Timer Starts

    pthread_mutex_init(&lock, NULL); //Lock initialized

    char* f_name = "com-orkut.ungraph.txt";
    FILE* file = fopen(f_name, "r");
    if (!file) {
        fprintf(stderr, "Error opening file\n");
        return 1;
    }

    fseek(file, 0L, SEEK_END);
    long f_size = ftell(file);
    fclose(file);

    pthread_t threads[THREADS]; //Array of threads in the amount we chose
    struct ThreadSegment thread_data[THREADS]; //Array of segments
    long chunk_size = f_size / THREADS; //Dividing the file into the segments

    for (int i = 0; i < THREADS; i++) {
        thread_data[i].edges = 0;
        thread_data[i].f_name = f_name;
        thread_data[i].start = i * chunk_size;
        thread_data[i].n_deg = calloc(RESULT + 1, sizeof(int));
        

        if (i == THREADS-1) {
            thread_data[i].end = f_size;
        } 
        else {
            thread_data[i].end = (i + 1) * chunk_size;
            
            FILE* f = fopen(f_name, "r");
            fseek(f, thread_data[i].end, SEEK_SET);
            while (fgetc(f) != '\n' && ftell(f) < f_size) {
                thread_data[i].end++;
            }
            fclose(f);
        }

        pthread_create(&threads[i], NULL, thread_proc, &thread_data[i]);
    }

    //After calculations are finished we rejoin the threads
    for (int i = 0; i < THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    // Sum Calculations 

    for (int i = 0; i < THREADS; i++) {
        pthread_mutex_lock(&lock);
        for (int j = 1; j <= RESULT; j++) {
            total_n_deg[j] += thread_data[i].n_deg[j];
        }
        total_edges += thread_data[i].edges;
        pthread_mutex_unlock(&lock);

        free(thread_data[i].n_deg);
    }

    int total_nodes = 0;
    for (int i = 1; i <= RESULT; i++) {
        if (total_n_deg[i] > 0) {
            total_nodes++;
        }
    }

    clock_t end_time = clock();
    double time_taken = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
    printf("Time taken: %.2f seconds\n", time_taken);
    printf("Threads: %d\n", THREADS);
    printf("Total nodes: %d\n", total_nodes);
    printf("Total edges: %d\n", total_edges);
    int node_diff = EXP_NODES - total_nodes;
    int edge_diff = EXP_EDGES - total_edges;
    printf("Difference in nodes: %d (actual: %d, calculated: %d)\n", node_diff, EXP_NODES, total_nodes);
    printf("Difference in edges: %d (actual: %d, calculated: %d)\n", edge_diff, EXP_EDGES, total_edges);

    pthread_mutex_destroy(&lock); //Lock destroyed

    return 0;
}
