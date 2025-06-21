/*
The purpose of this programming project is to explore process synchronization. 
This will be accomplished by writing a simulation to the Producer / Consumer problem described below. 
Your simulation will be implemented using Pthreads.  Tutorials on the Pthread functions and their usage can be found in our text, in our notes, or online.  
This simulation is a modification to the programming project found at the end of Chapter 6 and most of the text of this project specification is directly 
from that project description.
*/

//********************************************************************
//
// Joseph W. Mace
// Operating Systems
// Programming Project #3: Process Synchronization Using Pthreads - Main
// March 14, 2025
//
//********************************************************************
#include <iostream>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <semaphore.h> 
#include "buffer.h"

using namespace std;

//max number of iterations 
int max_iterations = 5;
sem_t emp, full;

//********************************************************************
//
// Producer Thread
//
// This thread uses the insert_item function to insert 
// a randomly generated number from 0-100 into the buffer
// The logic is minimal here, as most of it takes place in 
// the insert_item function.
// I use semephores for variables emp(empty) and full to make sure that
// the threads do not try to produce when the buffer is full and consume
// when the buffer is empty.
//
// Return Value
// ------------
// void                  
//
// Value Parameters
// ----------------
//
// Reference Parameters
// --------------------
// iterations     int             Number of iterations that will take place
// sleep_time     int             Time that the thread will sleep
// emp            sem_t           Tracks empty slots in buffer
// full           sem_t           Tracks occupied spots in buffer
//
//*******************************************************************
void *producer(void *param)
{
    buffer_item rand_item;
    int iterations = 0;

    while (iterations < max_iterations) {
        printf("Producer thread producing...\n");  // Add debug print here

        // Sleep for a random period of time (1-3 seconds)
        int sleep_time = rand() % 3;
        sleep(sleep_time);

        rand_item = rand() % 100;  // Generate a random number (item to insert)

        // Wait if the buffer is full
        sem_wait(&emp);  // Block if the buffer is full

        // Insert item into the buffer
        if (insert_item(rand_item)) {
            printf("Producer failed to insert item: %d\n", rand_item);
        } else {
            printf("Producer produced item: %d\n", rand_item);
        }

        display_buffer_status();

        // Signal that there’s an item in the buffer
        sem_post(&full);

        iterations++;
    }

    return NULL;
}

//********************************************************************
//
// Producer Thread
//
// This thread uses the remove_item to consume an integer from the buffer
// It is very similar to the producer thread in its logic and they way it used the remove_item function
//
// Return Value
// ------------
// void                  
//
// Value Parameters
// ----------------
//
// Reference Parameters
// --------------------
// iterations     int             Number of iterations that will take place
// sleep_time     int             Time that the thread will sleep
// emp            sem_t           Tracks empty slots in buffer
// full           sem_t           Tracks occupied spots in buffer
//
//*******************************************************************
void *consumer(void *param)
{
    buffer_item rand_item;
    int iterations = 0;

    while (iterations < max_iterations) {
        printf("Consumer thread consuming...\n");  // Add debug print here

        // Sleep for a random period of time (1-3 seconds)
        int sleep_time = 5;
        sleep(sleep_time);

        // Wait if the buffer is empty
        sem_wait(&full);  // Block if the buffer is empty

        // Remove item from the buffer
        if (remove_item(&rand_item)) {
            printf("Consumer failed to remove item.\n");
        } else {
            printf("Consumer consumed item: %d\n", rand_item);
        }

        display_buffer_status();

        // Signal that there is space in the buffer
        sem_post(&emp);

        iterations++;
    }

    return NULL;
}


int main(int argc, char *argv[])
{
    // Seed the random number generator
    srand(time(NULL));

    // Initialize mutex and semaphores
    pthread_mutex_init(&buffer_mutex, NULL);
    sem_init(&emp, 0, BUFFER_SIZE);  // Initialize empty semaphore with BUFFER_SIZE
    sem_init(&full, 0, 0);  // Initialize full semaphore with 0 (no items initially)

    // Initialize the buffer
    initialize_buffer();

    // Create producer and consumer threads
    cout << "Creating threads..." << endl;

    pthread_t producer_thread, consumer_thread;
    if (pthread_create(&producer_thread, NULL, producer, NULL) != 0)
    {
        perror("Failed to create producer thread");
        exit(1);
    }
    if (pthread_create(&consumer_thread, NULL, consumer, NULL) != 0)
    {
        perror("Failed to create consumer thread");
        exit(1);
    }

    // Sleep for a while to simulate the operation
    sleep(10); // Adjust this duration based on your testing needs

    // Join threads
    pthread_join(producer_thread, NULL);
    pthread_join(consumer_thread, NULL);

    // Destroy mutex and semaphores
    pthread_mutex_destroy(&buffer_mutex);
    sem_destroy(&emp);
    sem_destroy(&full);

    // Exit the program
    return 0;
}
