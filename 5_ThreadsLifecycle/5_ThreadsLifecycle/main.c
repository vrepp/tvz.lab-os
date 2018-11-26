//
//  main.c
//  5_ThreadsLifecycle
//
//  Created by Valentin Rep on 24/11/2018.
//  Copyright © 2018 ViCon. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>

#define NUM_THREADS 5
#define MAX_PROGRAMMING_N 10
#define MAX_SLEEPING_N 6
#define MAX_OTHER_N 3
#define MAX_CAR_SERVICE_N 100
#define MAX_COOK_N 2


pthread_mutex_t mutex;
static int num_food;
static int table = 0;

enum food {
    cokolada    = 1 << 0,
    cevapi      = 1 << 1,
    janjetina   = 1 << 2,
    strukle     = 1 << 3,
    grah        = 1 << 4,
    keksi       = 1 << 5,
    kelj        = 1 << 6,
    jabuka      = 1 << 7
};

typedef struct {
    int num_table_empty;
    int num_ate;
    int num_decline;
} food_data_t;

typedef struct {
    int id;
    const char *name;
    enum food consumes;
    food_data_t food_data;
} thread_data_t;

unsigned long int computeFactorial(int n) {
    if (n >= 1) {
        return n * computeFactorial(n-1);
    }
    else {
        return 1;
    }
}


void programira(thread_data_t *data) {
    printf("%s[%d] sada programira\n", data->name, data->id);
    
    unsigned long int factorial;
    
    int n = 1 + (rand() % MAX_PROGRAMMING_N);
    factorial = computeFactorial(n);
}

void spava(thread_data_t *data) {
    printf("%s[%d] sada spava\n", data->name, data->id);
    
    int time = 5 + (rand() % MAX_SLEEPING_N);
    if ( data->id == 0 ) {
        time = 10;
    }
    
    sleep(time);
}

void vozi_auto_na_servis(thread_data_t *data) {
    printf("%s[%d] sada vozi auto na servis\n", data->name, data->id);
    
    int time = 0;
    
    int type = 1 + (rand() % MAX_CAR_SERVICE_N);
    if ( type <= 25 ) {
        time = 4;
    }
    else if ( type <= 45 ) {
        time = 2;
    }
    else {
        time = 2 + (rand() % MAX_OTHER_N);
    }
    
    sleep(time);
}

void gleda_tv(thread_data_t *data) {
    printf("%s[%d] sada gleda tv\n", data->name, data->id);
    
    int time = 2 + (rand() % MAX_OTHER_N);
    sleep(time);
}

void igra_tenis(thread_data_t *data) {
    printf("%s[%d] sada igra tenis\n", data->name, data->id);
    
    int time = 2 + (rand() % MAX_OTHER_N);
    sleep(time);
}

void slusa_sviranje_klavira(thread_data_t *data) {
    printf("%s[%d] sada slusa sviranje klavira\n", data->name, data->id);
    
    int time = 2 + (rand() % MAX_OTHER_N);
    sleep(time);
}

void jede(thread_data_t *data) {
    printf("%s[%d] sada jede\n", data->name, data->id);
    
    pthread_mutex_lock(&mutex);
    
    if ( table == 0 ) {
        // empty table
        data->food_data.num_table_empty++;
    }
    else if ( data->consumes & table ) {
        // eat food
        data->food_data.num_ate++;
        table = 0;
    }
    else {
        // leave food
        data->food_data.num_decline++;
    }
    
    pthread_mutex_unlock(&mutex);
}

enum food kuha(thread_data_t *data) {
    printf("%s[%d] sada kuha\n", data->name, data->id);
    
    int time = 1 + (rand() % MAX_COOK_N);
    sleep(time);
    
    int r = (rand() % 8);
    return 1 << r;
}

void odmara(thread_data_t *data) {
    printf("%s[%d] sada odmara\n", data->name, data->id);
    
    int time = 1 + (rand() % MAX_COOK_N);
    sleep(time);
}


// Tin
// spava->programira->jede->vozi_auto_na_servis
void *threadTin(void *arg) {
    thread_data_t *data = (thread_data_t *)arg;
    printf("Starting thread: %d %s \n", data->id, data->name);
    
    // define food
    data->consumes = cokolada | strukle | grah | keksi | kelj | jabuka;
    
    // activities
    while ( num_food ) {
        programira(data);
        jede(data);
        vozi_auto_na_servis(data);
    }
    
    pthread_exit(data);
}

// Davor
// spava->programira->jede->gleda_tv
void *threadDavor(void *arg) {
    thread_data_t *data = (thread_data_t *)arg;
    printf("Starting thread: %d %s \n", data->id, data->name);
    
    // define food
    data->consumes = cevapi | janjetina | strukle | grah | kelj | jabuka;
    
    // activities
    while ( num_food ) {
        spava(data);
        programira(data);
        jede(data);
        gleda_tv(data);
    }
    
    pthread_exit(data);
}

// Ivica
// spava->igra_tenis->jede->programira
void *threadIvica(void *arg) {
    thread_data_t *data = (thread_data_t *)arg;
    printf("Starting thread: %d %s \n", data->id, data->name);
    
    // define food
    data->consumes = cokolada | cevapi | janjetina | strukle | keksi | jabuka;
    
    // activities
    while ( num_food ) {
        spava(data);
        igra_tenis(data);
        jede(data);
        programira(data);
    }
    
    pthread_exit(data);
}

// Ivan
// spava->sluša_sviranje_klavira->jede->programira
void *threadIvan(void *arg) {
    thread_data_t *data = (thread_data_t *)arg;
    printf("Starting thread: %d %s \n", data->id, data->name);
    
    // define food
    data->consumes = cokolada | cevapi | janjetina | grah | keksi | kelj | jabuka;
    
    // activities
    while ( num_food ) {
        spava(data);
        slusa_sviranje_klavira(data);
        jede(data);
        programira(data);
    }
    
    pthread_exit(data);
}

// Kuharica
// kuha_jelo->stavlja_jelo_na_stol->odmara_se
void *threadKuharica(void *arg) {
    thread_data_t *data = (thread_data_t *)arg;
    printf("Starting thread: %d %s \n", data->id, data->name);
    
    // activities
    while ( num_food ) {
        int cooked_food = kuha(data);
    
        while ( table != 0 ) {
            sleep(1);
        }
        
        // stavlja hranu na stol
        table = cooked_food;
        num_food--;
    
        odmara(data);
    }
    
    pthread_exit(data);
}


void check_status(thread_data_t *data, int status) {
    if ( status ) {
        printf("Error while creating thread %d [%s], (err: %d)\n", data->id, data->name, status);
        exit(EXIT_FAILURE);
    }
}


int main(int argc, const char * argv[]) {
    srand((unsigned) time(NULL));
    
    if ( argc != 2 ) {
        printf("Invalid number of arguments\n");
        printf("Usage: program number_of_threads\n");
        exit(EXIT_FAILURE);
    }
    
    num_food = atoi(argv[1]);
    
    void *threadsVoids[NUM_THREADS] = { threadTin, threadDavor, threadIvica, threadIvan, threadKuharica };
    const char *name[NUM_THREADS] = {"Tin", "Davor", "Ivica", "Ivan", "Kuharica"};
    pthread_t threads[NUM_THREADS];
    thread_data_t thread_data[NUM_THREADS];
    int i, status;
    
    // init mutex
    if ( pthread_mutex_init(&mutex, NULL) != 0 ) {
        printf("Error on mutex init\n");
        exit(EXIT_FAILURE);
    }
    
    // create threads
    for ( i = 0; i < NUM_THREADS; i++ ) {
        thread_data[i].id = i;
        thread_data[i].name = name[i];
        thread_data[i].food_data.num_table_empty = 0;
        thread_data[i].food_data.num_ate = 0;
        thread_data[i].food_data.num_decline = 0;
        status = pthread_create(&threads[i], NULL, threadsVoids[i], &thread_data[i]);
        check_status(&thread_data[i], status);
    }
    
    // join threads
    for ( i = 0; i < NUM_THREADS; i++ ) {
        void *thread_result;
        pthread_join(threads[i], &thread_result);
        thread_data_t *data = thread_result;
        
        printf("\n%s[%d]: jede: %d; odbio: %d; stol prazan: %d\n\n",
               data->name,
               data->id,
               data->food_data.num_ate,
               data->food_data.num_decline,
               data->food_data.num_table_empty
        );
    }
    
    // destroy mutex
    if ( pthread_mutex_destroy(&mutex) != 0 ) {
        printf("Error on mutex destroy\n");
        exit(EXIT_FAILURE);
    }
    
    return EXIT_SUCCESS;
}
