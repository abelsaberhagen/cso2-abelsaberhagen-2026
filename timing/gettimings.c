#define _XOPEN_SOURCE 700
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>



__attribute__((noinline)) void empty() {
    __asm__("");
    return;
}

__attribute__((noinline)) void random_number() {
    __asm__("");
    double rand_double = drand48();
    printf("%f\n", rand_double);
    return;
}

__attribute__((noinline)) void parent_id() {
    __asm__("");
    pid_t ppid = getppid();
    printf("%d\n", ppid);
    return;
}

__attribute__((noinline)) void fork_call() {
    __asm__("");
    pid_t pid = fork();
    if (pid == 0) {
        printf("This is child\n");
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
        const struct timespec *duration = 500;
        const struct timespec *unslept;

        nanosleep(duration, unslept);
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
    short unsigned int seed_num[6] = {155, 0, 155, 47, 201, 39};
    seed48(seed_num);

    switch (argv[1][0]){
        case '1':
            printf("case 1\n");
            empty();
            break;
        case '2':
            printf("case 2\n");
            random_number();
            break;
        case '3':
            printf("case 3\n");
            parent_id();
            break;
        case '4':
            printf("case 4\n");
            fork_call();
            break;
        case '5':
            printf("case 5\n");
            process_terminated();
            break;
        case '6':
            printf("case 6\n");
            process_exit();
            break;
        case '7':
            printf("case 7\n");
            bin_true();
            break;
        case '8':
            printf("case 8\n");
            mk_rm_dir();
            break;
    }

}


