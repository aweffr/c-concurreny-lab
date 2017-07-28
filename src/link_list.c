#include <common.h>

// basic node structure
typedef struct __node_t {
    int key, value;
    struct __node_t * next;
} node_t;

// basic list structure (one used per list)
typedef struct __list_t {
    node_t * head;
    pthread_mutex_t lock;
    pthread_mutexattr_t __lock_attr;
} list_t;

void list_init(list_t *lst) {
    lst->head = NULL;
    Pthread_reentrant_mutex_init(&lst->lock, &lst->__lock_attr);
}

int list_insert(list_t *lst, int key, int value) {
    Pthread_mutex_lock(&lst->lock);
    node_t *new = malloc(sizeof(node_t));
    if(new == NULL) {
        fprintf(stderr, "malloc fail\n");
        Pthread_mutex_unlock(&lst->lock);
        return -1;
    }
    new->key = key;
    new->value = value;
    new->next = lst->head;
    lst->head = new;
    Pthread_mutex_unlock(&lst->lock);
    return 0;
}

int list_lookup(list_t *lst, int key, int *out_ptr) {
    Pthread_mutex_lock(&lst->lock);
    node_t *curr = lst->head;
    while (curr != NULL) {
        if(curr->key == key) {
            if (out_ptr != NULL) {
                *out_ptr = curr->value;
            }
            pthread_mutex_unlock(&lst->lock);
            return 0;
        }
        curr = curr->next;
    }
    Pthread_mutex_unlock(&lst->lock);
    return -1;
}

void list_iter (list_t *lst) {
    Pthread_mutex_lock(&lst->lock);
    node_t * curr = lst->head;
    while(curr != NULL) {
        printf("key:%d value:%d\n", curr->key, curr->value);
        curr = curr->next;
    }
}

int main(int argc, char * argv[]) {
    list_t lst;
    list_init(&lst);
    
    for(int i = 0; i < 10000; i++) {
        int key = i, value = 3 * i + 1;
        list_insert(&lst, key, value);
    }
    
    system("PAUSE");
    list_iter(&lst);
    
    int key = 25;
    if (list_lookup(&lst, key, NULL) == 0) {
        fprintf(stdout, "%d is in the list!\n", key);
    }
    else {
        fprintf(stdout, "%d is NOT in the list!\n", key);
    }

    return 0;
}