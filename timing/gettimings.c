#define _XOPEN_SOURCE 700
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <math.h>



long long nsecs() {
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);
    clock_gettime(CLOCK_MONOTONIC, &end);
    long long start_time = start.tv_sec*1000000000 + start.tv_nsec;
    long long end_time = end.tv_sec*1000000000 + end.tv_nsec;
    long long nothing_time_elapsed = end_time - start_time;
    return nothing_time_elapsed;
}

__attribute__((noinline)) void empty() {
    __asm__("");
    return;
}

__attribute__((noinline)) void random_number() {
    __asm__("");
    drand48();
    //printf("%f\n", rand_double);
    return;
}

__attribute__((noinline)) void parent_id() {
    __asm__("");
    getppid();
    //printf("%d\n", ppid);
    return;
}

__attribute__((noinline)) void fork_call() {
    __asm__("");
    pid_t pid = fork();
    if (pid == 0) {
        printf("This is child\n");
        exit(0);
    } else {
        printf("This is parent\n");
        int status;
        waitpid(pid, &status, 0);
        printf("Child exited with status code: %d\n", WEXITSTATUS(status));
    }
    return;
}

__attribute__((noinline)) void process_terminated() {
    __asm__("");
    pid_t pid = fork();
    if (pid == 0) {
        printf("This is child\n");
        exit(0);
    } else {
        printf("This is parent\n");

        const struct timespec duration = {.tv_sec = 0, .tv_nsec = 500};
        const struct timespec *duration_ptr = &duration;

        nanosleep(duration_ptr, NULL);
        int status;
        waitpid(pid, &status, 0);
        printf("Child exited with status code: %d\n", WEXITSTATUS(status));
    }
    return;
}

__attribute__((noinline)) void process_exit() {
    __asm__("");
    pid_t pid = fork();
    if (pid == 0) {
        exit(0);
    } else {
        int status;
        waitpid(pid, &status, 0);
        printf("Child exited with status code: %d\n", WEXITSTATUS(status));
    }
    return;
}

__attribute__((noinline)) void bin_true() {
    __asm__("");
    if(system("/bin/true")){};
    return;
}

__attribute__((noinline)) void mk_rm_dir() {
    __asm__("");
    if(system("mkdir /tmp/testdir")){};
    if(system("rm -r /tmp/testdir")){};
    return;
}




int main(int argc, char *argv[]) {
    char chosen_func = argv[1][0];

    // Timing nothing
    long nothing_time = nsecs(); 
    printf("Nothing: %ld\n", nothing_time);

    long double time = 0.0;

    short unsigned int seed_num[6] = {155, 0, 155, 47, 201, 39};
    seed48(seed_num);
    struct timespec start, end;
    //struct timespec t;
    int trials = 100;
    long double avg_time = 0.0;
    long long end_time = 0;
    long long start_time = 0;
    switch (chosen_func){
        case '1':

            printf("case 1\n");
    

            time = 0.0;
            for (int i = 0; i < trials; i++) {
                clock_gettime(CLOCK_MONOTONIC, &start);
                empty();
                clock_gettime(CLOCK_MONOTONIC, &end);
                end_time = (end.tv_sec*1000000000) + end.tv_nsec;
                start_time = (start.tv_sec*1000000000) + start.tv_nsec;
                long double elapsed = (end_time - start_time) - nothing_time;
                time += elapsed;
            }
            printf("Timing: %Lf\n", time);
            avg_time = time / trials;
        
            break;
        case '2':
            printf("case 2\n");
            // clock_gettime(CLOCK_MONOTONIC, &t);
            // random_number();
            // time = (t.tv_sec*1000000000 + t.tv_nsec) - nothing_time;
            // break;

            time = 0.0;
            for (int i = 0; i < trials; i++) {
                clock_gettime(CLOCK_MONOTONIC, &start);
                random_number();
                clock_gettime(CLOCK_MONOTONIC, &end);
                end_time = (end.tv_sec*1000000000) + end.tv_nsec;
                start_time = (start.tv_sec*1000000000) + start.tv_nsec;
                long double elapsed = (end_time - start_time) - nothing_time;
                time += elapsed;
            }
            printf("Timing: %Lf\n", time);
            avg_time = time / trials;
        
            break;
        case '3':
            printf("case 3\n");
            // clock_gettime(CLOCK_MONOTONIC, &t);
            // parent_id();
            // time = (t.tv_sec*1000000000 + t.tv_nsec) - nothing_time;

            time = 0.0;
            for (int i = 0; i < trials; i++) {
                clock_gettime(CLOCK_MONOTONIC, &start);
                parent_id();
                clock_gettime(CLOCK_MONOTONIC, &end);
                end_time = (end.tv_sec*1000000000) + end.tv_nsec;
                start_time = (start.tv_sec*1000000000) + start.tv_nsec;
                long double elapsed = (end_time - start_time) - nothing_time;
                time += elapsed;
            }
            printf("Timing: %Lf\n", time);
            avg_time = time / trials;
            break;
        case '4':
            printf("case 4\n");
            // clock_gettime(CLOCK_MONOTONIC, &t);
            // fork_call();
            // time = (t.tv_sec*1000000000 + t.tv_nsec) - nothing_time;

            time = 0.0;
            for (int i = 0; i < trials; i++) {
                clock_gettime(CLOCK_MONOTONIC, &start);
                fork_call();
                clock_gettime(CLOCK_MONOTONIC, &end);
                end_time = (end.tv_sec*1000000000) + end.tv_nsec;
                start_time = (start.tv_sec*1000000000) + start.tv_nsec;
                long double elapsed = (end_time - start_time) - nothing_time;
                time += elapsed;
            }
            printf("Timing: %Lf\n", time);
            avg_time = time / trials;
            break;
        case '5':
            printf("case 5\n");
            // clock_gettime(CLOCK_MONOTONIC, &t);
            // process_terminated();
            // time = (t.tv_sec*1000000000 + t.tv_nsec) - nothing_time;

            time = 0.0;
            for (int i = 0; i < trials; i++) {
                clock_gettime(CLOCK_MONOTONIC, &start);
                process_terminated();
                clock_gettime(CLOCK_MONOTONIC, &end);
                end_time = (end.tv_sec*1000000000) + end.tv_nsec;
                start_time = (start.tv_sec*1000000000) + start.tv_nsec;
                long double elapsed = (end_time - start_time) - nothing_time;
                time += elapsed;
            }
            printf("Timing: %Lf\n", time);
            avg_time = time / trials;
            break;
        case '6':
            printf("case 6\n");
            // clock_gettime(CLOCK_MONOTONIC, &t);
            // process_exit();
            // time = (t.tv_sec*1000000000 + t.tv_nsec) - nothing_time;

            time = 0.0;
            for (int i = 0; i < trials; i++) {
                clock_gettime(CLOCK_MONOTONIC, &start);
                process_exit();
                clock_gettime(CLOCK_MONOTONIC, &end);
                end_time = (end.tv_sec*1000000000) + end.tv_nsec;
                start_time = (start.tv_sec*1000000000) + start.tv_nsec;
                long double elapsed = (end_time - start_time) - nothing_time;
                time += elapsed;
            }
            printf("Timing: %Lf\n", time);
            avg_time = time / trials;
            break;
        case '7':
            printf("case 7\n");
            // clock_gettime(CLOCK_MONOTONIC, &t);
            // bin_true();
            // time = (t.tv_sec*1000000000 + t.tv_nsec) - nothing_time;

            time = 0.0;
            for (int i = 0; i < trials; i++) {
                clock_gettime(CLOCK_MONOTONIC, &start);
                bin_true();
                clock_gettime(CLOCK_MONOTONIC, &end);
                end_time = (end.tv_sec*1000000000) + end.tv_nsec;
                start_time = (start.tv_sec*1000000000) + start.tv_nsec;
                long double elapsed = (end_time - start_time) - nothing_time;
                time += elapsed;
            }
            printf("Timing: %Lf\n", time);
            avg_time = time / trials;
            break;
        case '8':
            printf("case 8\n");
            // clock_gettime(CLOCK_MONOTONIC, &t);
            // mk_rm_dir();
            // time = (t.tv_sec*1000000000 + t.tv_nsec) - nothing_time;

            time = 0.0;
            for (int i = 0; i < trials; i++) {
                clock_gettime(CLOCK_MONOTONIC, &start);
                mk_rm_dir();
                clock_gettime(CLOCK_MONOTONIC, &end);
                end_time = (end.tv_sec*1000000000) + end.tv_nsec;
                start_time = (start.tv_sec*1000000000) + start.tv_nsec;
                long double elapsed = (end_time - start_time) - nothing_time;
                time += elapsed;
            }
            printf("Timing: %Lf\n", time);
            avg_time = time / trials;
            break;
    }


    printf("Timing %c: %Lf, after %d trials\n", chosen_func, avg_time, trials);


}


