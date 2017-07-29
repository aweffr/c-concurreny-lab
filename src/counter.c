#include "common.h"

/*
 * TODO: counter里分很多个子counter
 */

typedef struct __counter_t {
    int             value;
    pthread_mutex_t lock;
} counter_t;

void init(counter_t *c) {
    c->value = 0;
    Pthread_mutex_init(&c->lock);
}

void increment(counter_t *c) {
    Pthread_mutex_lock(&c->lock);
    c->value++;
    Pthread_mutex_unlock(&c->lock);
}

void decrement(counter_t *c) {
    Pthread_mutex_lock(&c->lock);
    c->value--;
    Pthread_mutex_unlock(&c->lock);
}

int get(counter_t *c) {
    Pthread_mutex_lock(&c->lock);
    int rc = c->value;
    Pthread_mutex_unlock(&c->lock);
    return rc;
}

#define T_NUM 4

typedef struct __my_args {
    counter_t * cnt;
    int loops;
} my_args;

void* incr(void * arg) {
    my_args* args = (my_args*) arg; 
    for(int i = 0; i < args->loops; i++) {
        increment(args->cnt);
    }
    return NULL;
}

int main(int argc, char * argv[]) {

    pthread_t t[T_NUM];
    my_args args[T_NUM];

    counter_t cnt;
    init(&cnt);

    for(int i = 0; i < T_NUM; i++) {
        args[i].cnt = &cnt;
        args[i].loops = 500000;
        Pthread_create(&t[i], NULL, incr, &args[i]);
    }

    for(int i = 0; i < T_NUM; i++) {
        Pthread_join(t[i], NULL);
    }

    printf("cnt->value = %d\n", cnt.value);

    return 0;
}