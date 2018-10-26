//
//  main.c
//  2_Processes
//
//  Created by Valentin Rep on 20/10/2018.
//  Copyright © 2018 ViCon. All rights reserved.
//

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, const char * argv[]) {
    
    setbuf(stdout, NULL);
    //int num_processes[5] = {10, 100, 1000, 10000, 100000};
    
    if ( argc != 2 ) {
        printf("Invalid number of arguments\n");
        printf("Usage: program number_of_child_processes\n");
        return 1;
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
                printf("D%d\n", i+1);
                exit(EXIT_SUCCESS);
                break;
                
            default:
                // parent
                printf("R%d\n", i+1);
                break;
        }
    }
    
    while ( i-- ) {
        wait(NULL);
    }
    
    return 0;
}
