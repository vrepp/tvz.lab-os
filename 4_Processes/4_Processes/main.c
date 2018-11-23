//
//  main.c
//  4_Processes
//
//  Created by Valentin Rep on 12/11/2018.
//  Copyright © 2018 ViCon. All rights reserved.
//

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

static int var = 0;

int main(int argc, const char * argv[]) {
    
    if ( argc != 2 ) {
        printf("Invalid number of arguments\n");
        printf("Usage: program number_of_child_processes\n");
        exit(EXIT_FAILURE);
    }
    
    int num_proc = atoi(argv[1]);
    pid_t pid;
    int i;
    
    for ( i = 0; i < num_proc; i++ ) {
        switch ( pid = fork() ) {
            case -1:
                // error
                printf("Error while forking at index: %d\n", i+1);
                exit(EXIT_FAILURE);
                break;
                
            case 0:
                // child
                var++;
                exit(EXIT_SUCCESS);
                break;
                
            default:
                // parent
                break;
        }
    }
    
    while ( i-- ) {
        wait(NULL);
    }
    
    printf("Processes - VAR: %d\n", var);
    
    exit(EXIT_SUCCESS);
}
