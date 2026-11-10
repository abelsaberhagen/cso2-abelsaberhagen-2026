#include "life.h"
#include <pthread.h>
#include <stdlib.h>

pthread_barrier_t barrier;

typedef struct {
    int thread_id;
    int start_row;
    int end_row;
    int height;
    LifeBoard *state;
    int steps;
} ThreadArgs;

void* thread_simulate(void* args) {

    ThreadArgs *thread_properties = (ThreadArgs *) args;

    int start_row = thread_properties->start_row;
    int end_row = thread_properties->end_row;
    LifeBoard *state = thread_properties->state;
    int steps = thread_properties->steps;
    LifeBoard *next_state = LB_new(state->width, state->height);     
    
    for (int step = 0; step < steps; step += 1) {

        /* We use the range [1, width - 1) here instead of
         * [0, width) because we fix the edges to be all 0s.
         */
        for (int y = start_row; y < end_row - 1; y += 1) {
            for (int x = 1; x < state->width - 1; x += 1) {
                
                /* For each cell, examine a 3x3 "window" of cells around it,
                 * and count the number of live (true) cells in the window. */
                int live_in_window = 0;
                for (int y_offset = -1; y_offset <= 1; y_offset += 1)
                    for (int x_offset = -1; x_offset <= 1; x_offset += 1)
                        if (LB_get(state, x + x_offset, y + y_offset))
                            live_in_window += 1;
                
                /* Cells with 3 live neighbors remain or become live.
                   Live cells with 2 live neighbors remain live. */
                LB_set(next_state, x, y,
                    live_in_window == 3 /* dead cell with 3 neighbors or live cell with 2 */ ||
                    (live_in_window == 4 && LB_get(state, x, y)) /* live cell with 3 neighbors */
                );
            }
        }
        
        /* now that we computed next_state, make it the current state */
        pthread_barrier_wait(&barrier);
        LB_swap(state, next_state);
        pthread_barrier_wait(&barrier);
    }
    LB_del(next_state);
    return NULL;
    



}

void simulate_life_parallel(int threads, LifeBoard *state, int steps) {
    
    pthread_t *thread_ids = malloc(threads * sizeof(pthread_t));
    ThreadArgs *args = malloc(threads * sizeof(ThreadArgs));

    pthread_barrier_init(&barrier, NULL, threads);


    int rows_per_thread = state->height / threads;

    for (int t = 0; t < threads; t++) {

        args[t].thread_id = t;
        args[t].start_row = rows_per_thread * t;
        args[t].end_row = args[t].start_row + rows_per_thread;
        args[t].height = rows_per_thread;
        args[t].state = state;
        args[t].steps = steps;

        pthread_create(&thread_ids[t], NULL, thread_simulate, &args[t]);

    }

    for (int t = 0; t < threads; t++) {
        pthread_join(thread_ids[t], NULL);
    }

    pthread_barrier_destroy(&barrier);

    free(thread_ids);
    free(args);
}

