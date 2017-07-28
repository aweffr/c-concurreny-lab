#include <common.h>

// This file is to test makefile and learn how to build simple project.

int main(int argc, char * argv[]) {
    
    printf("Hello Zoule!\n");
    
    int fin = -1;

    for(int i = 0; i < 10; i++) {
        fin = max(i, fin);
    }

    printf("fin = %d\n", fin);

    fflush(stdout);

    return 0;
}