//
//  main.c
//  2_Numbers
//
//  Created by Valentin Rep on 20/10/2018.
//  Copyright © 2018 ViCon. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define SIZE 100000
#define FIRST_LETTER 'A'
#define NUM_LETTERS 26

char numbers[SIZE + 1];

int main(int argc, const char * argv[]) {
    
    srand((unsigned) time(NULL));
    for ( int i = 0; i < SIZE; i++ ) {
        numbers[i] = FIRST_LETTER + (rand() % NUM_LETTERS);
    }
    
    printf("%s\n", numbers);
    sleep(10);
    
    return 0;
}
