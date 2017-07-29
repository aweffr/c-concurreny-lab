#include "common.h"
#define MAX_BUF 100
#define MAX_T 4

pthread_cond_t empty, full;
pthread_mutex_t mutex;

typedef struct __arg_list {
    int tid;
    int loops;
} arg_list;

int buffer[MAX_BUF];
int full_ptr = 0; // next slot to fill
int use_ptr  = 0; // next slot to fetch
int count    = 0; // valid slot in buffer

void put(int value) {
    buffer[full_ptr] = value;
    full_ptr = (full_ptr + 1) % MAX_BUF;
    count++;
}

int get() {
    int tmp = buffer[use_ptr];
    use_ptr = (use_ptr + 1) % MAX_BUF;
    count--;
    return tmp;
}

void *producer(void *args) {
    arg_list* arg = (arg_list*) args;
    for(int i = 0; i < arg->loops; i++) {
        Pthread_mutex_lock(&mutex);
        while(count == MAX_BUF)
            Pthread_cond_wait(&empty, &mutex);
        put(i);
        // printf("tid:%d put(%d)\n", arg->tid, i);
        Pthread_cond_signal(&full);
        Pthread_mutex_unlock(&mutex);
    }

    return NULL;
}

void *consumer(void *args) {
    arg_list* arg = (arg_list*) args;
    for(int i = 0; i < arg->loops; i++) {
        Pthread_mutex_lock(&mutex);
        while (count == 0)
            Pthread_cond_wait(&full, &mutex);
        int tmp = get();
        // printf("tid:%d get()=%d\n", arg->tid, tmp);
        Pthread_cond_signal(&empty);
        Pthread_mutex_unlock(&mutex);
    }

    return NULL;
}

int main(int argc, char * argv[]) {
    Pthread_mutex_init(&mutex);
    pthread_cond_init(&empty, NULL);
    pthread_cond_init(&full, NULL);    
    // empty = PTHREAD_COND_INITIALIZER;
    // full = PTHREAD_COND_INITIALIZER;
    // mutex = PTHREAD_MUTEX_INITIALIZER;

    pthread_t t[MAX_T];
    arg_list args[MAX_T];

    for(int i = 0; i < MAX_T; i++){
        args[i].tid = i;
        args[i].loops = 100000000;
    }

    for(int i = 0; i < MAX_T; i++) {
        if (i < (MAX_T / 2))
            Pthread_create(&t[i], NULL, producer, &args[i]);
        else
            Pthread_create(&t[i], NULL, consumer, &args[i]);
    }

    for(int i = 0; i < MAX_T; i++) {
        Pthread_join(t[i], NULL);
    }

    return 0;
}