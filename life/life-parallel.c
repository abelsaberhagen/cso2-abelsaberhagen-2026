#include "life.h"
#include <pthread.h>

pthread_barrier_t barrier;

LifeBoard* create_board() {
    int new_width = (state->width)/threads;
    int new_height = (state->height)/threads;

    LifeBoard *next_state = LB_new(new_width, new_height);    
}

void simulate_life_parallel(int threads, LifeBoard *state, int steps) {

    pthread_t lb_thread;

    for (int t = 0; t < threads; t++) {

        int new_width = state->width/threads;
        int new_height = state->height/threads;

        pthread_create(create_board())

    }
    LifeBoard *next_state = LB_new(state->width, state->height);    
    
    for (int step = 0; step < steps; step += 1) {

        /* We use the range [1, width - 1) here instead of
         * [0, width) because we fix the edges to be all 0s.
         */
        for (int y = 1; y < state->height - 1; y += 1) {
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
        LB_swap(state, next_state);
    }
    LB_del(next_state);
}
