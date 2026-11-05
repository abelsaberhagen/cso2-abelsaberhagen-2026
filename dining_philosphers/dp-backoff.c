#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>
#include <unistd.h>
#include <time.h>   

pthread_barrier_t barrier; // optional: to hopefully make deadlock more consistent

pthread_t philosopher[5];
pthread_mutex_t chopstick[5];

void *pickUpAndEat(void *arg) {
    int n = (int) (long)arg;
    int attempts = 0;
    // optional: sync up threads to make deadlock hopefully happen more consistently
    pthread_barrier_wait(&barrier);

    // take two chopsticks
    pthread_mutex_lock(&chopstick[n]);
    printf("Philosopher %d got chopstick %d\n", n, n);
    int lock_check = pthread_mutex_trylock(&chopstick[(n+1)%5]);
    while (lock_check > 0) {
        srand(time(NULL));
        printf("Philosopher %d set down chopstick %d\n", n, n);
        pthread_mutex_unlock(&chopstick[n]);
        pthread_mutex_lock(&chopstick[n]);
        printf("Philosopher %d got chopstick %d\n", n, n);
        lock_check = pthread_mutex_trylock(&chopstick[(n+1)%5]);

        attempts++;
        int min = (50 * attempts);
        int max = (100 * attempts);
        int wait_time = rand() % ((max - min) + 1) + 50;
        usleep(wait_time);
    }
   
    printf ("Philosopher %d is eating\n",n);
    sleep(1);

    printf("Philosopher %d set down chopstick %d\n", n, (n+1)%5);
    pthread_mutex_unlock(&chopstick[(n+1)%5]);
    printf("Philosopher %d set down chopstick %d\n", n, n);
    pthread_mutex_unlock(&chopstick[n]);
    return NULL;
}

int main(int argc, const char *argv[]) {
    pthread_barrier_init(&barrier, NULL, 5);

    for(int i = 0; i < 5; i += 1)
        pthread_mutex_init(&chopstick[i], NULL);

    for(int i =0; i < 5; i += 1)
        pthread_create(&philosopher[i], NULL, pickUpAndEat, (void *)(size_t)i);

    for(int i=0; i < 5; i += 1)
        pthread_join(philosopher[i], NULL);

    for(int i=0; i < 5; i += 1)
        pthread_mutex_destroy(&chopstick[i]);

    pthread_barrier_destroy(&barrier);

    return 0;
}