//
//  main.c
//  5_CheckIn
//
//  Created by Valentin Rep on 24/11/2018.
//  Copyright © 2018 ViCon. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>

#define NUM_PASSENGERS 100
#define MAX_LOGIN_TIME 5
#define MAX_FETCH_TIME 3
#define MAX_RESERVE_TIME 2
#define CONFIRMATION_ODD 4
#define FIELD_SIZE 10

typedef struct {
    int data[FIELD_SIZE][FIELD_SIZE];
} Spots;

static Spots arrSpots = { .data = {0} };

pthread_mutex_t lock;

void printSpots(Spots spots) {
    printf("\n");
    for ( int i = 0; i < FIELD_SIZE; i++ ) {
        printf("\n");
        for ( int j = 0; j < FIELD_SIZE; j++ ) {
            printf(" %d ", spots.data[i][j]);
        }
    }
    printf("\n\n");
}

void loginToCloud() {
    int loginTime = 1 + (rand() % MAX_LOGIN_TIME);
    //printf("Connecting to Cloud (%d s)...\n", loginTime);
    sleep(loginTime);
}

int selectSpot(Spots spots) {
    int spot = (rand() % NUM_PASSENGERS);
    
    return spot;
}

Spots getSpots() {
    Spots spots;
    
    spots = arrSpots;
    
    int fetchTime = 1 + (rand() % MAX_FETCH_TIME);
    //printf("Fetching the list of spots (%d s)...\n", fetchTime);
    sleep(fetchTime);
    
    return spots;
}

void reserveSpot(int spot) {
    int reserveTime = 1 + (rand() % MAX_RESERVE_TIME);
    //printf("Reserving the spot (%d s)...\n", reserveTime);
    sleep(reserveTime);
    
    int x = spot / FIELD_SIZE;
    int y = spot % FIELD_SIZE;
    
    arrSpots.data[x][y] = 1;
}

void *checkInProcess(void *p) {
    srand((unsigned) time(NULL));
    
    // 1. login to cloud
    loginToCloud();
    
    // 2. get seats
    Spots spots;
    pthread_mutex_lock(&lock);
    spots = getSpots();
    pthread_mutex_unlock(&lock);
    
    // 3. select a spot
    int spot = selectSpot(spots);
    
    // 4. confirm selection
    // if confirmation = 0 => customer declined (in 25% of cases), repeat selection
    int confirmation = 0;
    while ( confirmation == 0 ) {
        spot = selectSpot(spots);
        confirmation = (rand() % CONFIRMATION_ODD);
    }
    
    // 5. set spot to 'reserved'
    //pthread_mutex_lock(&lock);
    reserveSpot(spot);
    //pthread_mutex_unlock(&lock);
    
    pthread_exit(NULL);
}

void outputFinal() {
    int spotsAvailable = 0;
    int spotsReserved = 0;
    
    for ( int i = 0; i < FIELD_SIZE; i++ ) {
        for ( int j = 0; j < FIELD_SIZE; j++ ) {
            spotsReserved += arrSpots.data[i][j];
        }
    }
    
    spotsAvailable = NUM_PASSENGERS - spotsReserved;
    
    printf("Total spots: %d\n", NUM_PASSENGERS);
    printf("Available spots: %d\n", spotsAvailable);
    printf("Reserved spots: %d\n", spotsReserved);
    
    printSpots(arrSpots);
}


int main(int argc, const char * argv[]) {
    srand((unsigned) time(NULL));
    
    pthread_t threads[NUM_PASSENGERS];
    int i, status;
    
    if ( pthread_mutex_init(&lock, NULL) != 0 ) {
        printf("Error on mutex init\n");
        exit(EXIT_FAILURE);
    }
    
    // get number of first N passengers; others will have to wait 1s to connect to WiFi
    int N = (rand() % NUM_PASSENGERS);
    
    printf("First %d passengers\n", N);
    for ( i = 0; i < N; i++ ) {
        status = pthread_create(&threads[i], NULL, checkInProcess, NULL);
        if ( status ) {
            printf("Error while creating thread: %d (err: %d)\n", i, status);
            exit(EXIT_FAILURE);
        }
    }
    
    // wait 1s for WiFi
    sleep(1);
    
    printf("Other %d passengers\n", NUM_PASSENGERS-N);
    for ( i = N; i < NUM_PASSENGERS; i++ ) {
        status = pthread_create(&threads[i], NULL, checkInProcess, NULL);
        if ( status ) {
            printf("Error while creating thread: %d (err: %d)\n", i, status);
            exit(EXIT_FAILURE);
        }
    }
    
    for ( i = 0; i < NUM_PASSENGERS; i++ ) {
        pthread_join(threads[i], NULL);
    }
    
    if ( pthread_mutex_destroy(&lock) != 0 ) {
        printf("Error on mutex destroy\n");
        exit(EXIT_FAILURE);
    }
    
    outputFinal();
    
    return EXIT_SUCCESS;
}
