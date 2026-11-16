#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include "pool.h"

task_fn *tasks;
void **task_args;
void **return_vals;
pthread_t *thread_ids;
int *tasks_completed;

int next_task = 0;
int num_threads;
int num_tasks_completed = 0;
int num_tasks_submitted = 0;
int all_tasks_completed = 0;


pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
pthread_cond_t completed_cond = PTHREAD_COND_INITIALIZER;


void *worker_thread(void *arg) {
    while(1) {
        pthread_mutex_lock(&lock);
        while((next_task == num_tasks_submitted) && !all_tasks_completed) {
            pthread_cond_wait(&cond, &lock);
        }
        if (all_tasks_completed) {
            pthread_mutex_unlock(&lock);
            break;
        }
        int task_id = next_task++;
        task_fn func = tasks[task_id];
        void *func_arg = task_args[task_id];
        pthread_mutex_unlock(&lock);
        void *result = func(func_arg);
        pthread_mutex_lock(&lock);
        return_vals[task_id] = result;
        printf("Thread completed\n");
        num_tasks_completed++;
        tasks_completed[task_id] = 1;
        pthread_cond_broadcast(&completed_cond);
        pthread_mutex_unlock(&lock);
    }
    return NULL;
}


void pool_setup(int threads) {
    num_threads = threads;
    tasks = calloc(1000, sizeof(task_fn)); 
    task_args = calloc(1000, sizeof(void*));
    return_vals = calloc(1000, sizeof(void*));
    thread_ids = calloc(1000, sizeof(pthread_t));

    tasks_completed = calloc(1000, sizeof(int));
    memset(tasks_completed, 0, 1000);


    for (int thread_id = 0; thread_id < threads; thread_id++) {
        pthread_create(&thread_ids[thread_id], NULL, worker_thread, NULL);
    }
}

int pool_submit_task(task_fn task, void *argument) {
    
    pthread_mutex_lock(&lock);
    int task_id = num_tasks_submitted++;
    tasks[task_id] = task;
    task_args[task_id] = argument;
    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&lock);
    printf("Task was submitted successfully.");
    return task_id;
}

void *pool_get_task_result(int task_id) {
    pthread_mutex_lock(&lock);
    while (!tasks_completed[task_id]) {
        pthread_cond_wait(&completed_cond, &lock);
    }
    void *result = return_vals[task_id];
    pthread_mutex_unlock(&lock);
    return result;
}

void pool_wait(void) {
    pthread_mutex_lock(&lock);
    while (num_tasks_completed < num_tasks_submitted) {
        pthread_cond_wait(&completed_cond, &lock);
    }
    pthread_mutex_unlock(&lock);
}

void pool_stop(void) {
    pthread_mutex_lock(&lock);
    all_tasks_completed = 1;
    pthread_cond_broadcast(&cond);
    pthread_mutex_unlock(&lock);
    for(int id = 0; id < num_threads; id++) {
        pthread_join(thread_ids[id], NULL);
    }
    free(tasks);
    free(task_args);
    free(thread_ids);
}


