//
//  main.c
//  4_Threads
//
//  Created by Valentin Rep on 12/11/2018.
//  Copyright © 2018 ViCon. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

static int var = 0;

void *initThread(void *p) {
    var++;
    
    pthread_exit(NULL);
}

int main(int argc, const char * argv[]) {
    
    if ( argc != 2 ) {
        printf("Invalid number of arguments\n");
        printf("Usage: program number_of_threads\n");
        exit(EXIT_FAILURE);
    }
    
    int num_thread = atoi(argv[1]);
    pthread_t threads[num_thread];
    int i, status;
    
    for ( i = 0; i < num_thread; i++ ) {
        status = pthread_create(&threads[i], NULL, initThread, NULL);
        if ( status ) {
            printf("Error while creating thread: %d (err: %d)\n", i, status);
            exit(EXIT_FAILURE);
        }
    }
    
    for ( i = 0; i < num_thread; i++ ) {
        pthread_join(threads[i], NULL);
    }
    
    printf("Threads - VAR: %d\n", var);
    
    exit(EXIT_SUCCESS);
}
