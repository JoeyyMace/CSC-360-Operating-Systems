/*
This is the buffer file for the pThread synchronization file
*/

//********************************************************************
//
// Joseph W. Mace
// Operating Systems
// Programming Project #3: Process Synchronization Using Pthreads - Buffer File
// March 14, 2025
//
//********************************************************************

#include "buffer.h"
#include <iostream>
#include <pthread.h> 

using namespace std;

#define BUFFER_SIZE 5

// Define the global buffer and synchronization variables
buffer_item buffer[BUFFER_SIZE];
int in = 0;
int out = 0;
bool is_prime_mode = false;  // This flag indicates whether to check for prime numbers

pthread_mutex_t buffer_mutex = PTHREAD_MUTEX_INITIALIZER; // Mutex initialization

//********************************************************************
//
// Is Prime Fuction
//
// Checks if int is prime
//
// Return Value
// ------------
// bool                         True/False if Key is found
//
// Parameters
// ------------
// int num                      number you want to check if prime
//
//*******************************************************************
bool is_prime(int num) {
    if (num <= 1) return false;
    for (int i = 2; i * i <= num; i++) {
        if (num % i == 0) return false;
    }
    return true;
}

//********************************************************************
//
// Insert_item function (Producer)
//
// This function is for the producer thread 
// to insert integers into the buffer.
// Uses "in" as an index. Works in a circular behavior
// meaning that it wraps to index 0 after reaching end
//
// Return Value
// ------------
// int                         Return 0 if successful
//
// Value Parameters
// ----------------
// buffer_item  item    to be instered into buffer
// 
//
// Reference Parameters
// --------------------
// sem_t    emp     semephore to track number of empty slots in buffer
// sem_t    full    semephore to track number of full slots in buffer
// pthread_mutex_t  buffer_mutex    mutex in order to apply mutual exclusion between threads
// int      in      keeps track of item to insert. After an item is inserted, the index is updated
//
//*******************************************************************
int insert_item(buffer_item item)
{
    sem_wait(&emp);  // Wait for an empty slot (decrement empty semaphore)
    pthread_mutex_lock(&buffer_mutex);  // Lock buffer for thread-safe access

    buffer[in] = item;  // Insert item at 'in' index
    in = (in + 1) % BUFFER_SIZE;  // Update 'in' index (circular buffer)

    pthread_mutex_unlock(&buffer_mutex);  // Unlock buffer
    sem_post(&full);  // Signal that there's a full slot (increment full semaphore)

    return 0;  // Success
}

//********************************************************************
//
// remove_item function (Producer)
//
// This function is for the consumer thread 
// to remove integers into the buffer
// similar to the insert_item function
//
//
// Return Value
// ------------
// int                         Return 0 if successful
//
// Value Parameters
// ----------------
// buffer_item  item    to be instered into buffer
// 
//
// Reference Parameters
// --------------------
// sem_t    emp     semephore to track number of empty slots in buffer
// sem_t    full    semephore to track number of full slots in buffer
// pthread_mutex_t  buffer_mutex    mutex in order to apply mutual exclusion between threads
// int      out     keeps track of items to be removed. After removal, out is updated
//                  Upon exceding the buffer_size, out wraps to the first index using % BUFFER_SIZE
//
//*******************************************************************
int remove_item(buffer_item *item)
{
    sem_wait(&full);  // Wait for a full slot (decrement full semaphore)
    pthread_mutex_lock(&buffer_mutex);  // Lock buffer for thread-safe access

    *item = buffer[out];  // Remove item from the 'out' index
    out = (out + 1) % BUFFER_SIZE;  // Update 'out' index (circular buffer)

    pthread_mutex_unlock(&buffer_mutex);  // Unlock buffer
    sem_post(&emp);  // Signal that there's an empty slot (increment empty semaphore)

    return 0;  // Success
}

//********************************************************************
//
// is_buffer_full function
//
// This function uses (in + 1) & BUFFER_SIZE == out
// to determine if the buffer is full or not.
// If the buffer is full, is_full is set to 1 if true and 0 if false
//
// Return Value
// ------------
// int                         1/0 if buffer is full or not
//
// Value Parameters
// ----------------
//
// Reference Parameters
// --------------------
// is_full     int             Calculates if buffer is full
//
//*******************************************************************
int is_buffer_full()
{
    pthread_mutex_lock(&buffer_mutex);  // Lock the mutex before checking buffer state

    // Check if the buffer is full (next position after 'in' is 'out')
    int is_full = (in + 1) % BUFFER_SIZE == out;

    pthread_mutex_unlock(&buffer_mutex);  // Unlock after checking buffer state
    return is_full;
}

//********************************************************************
//
// is_buffer_empty
//
// Simply checks position of index variables
// If in has not moved past out, there are no items in the buffer
//
// Return Value
// ------------
// int                 1/0 if buffer is empty
//
// Value Parameters
// ----------------
//
// Reference Parameters
// --------------------
// is_empty     int             Calculation of empty or non empty buffer
//
//*******************************************************************
int is_buffer_empty()
{
    pthread_mutex_lock(&buffer_mutex);  // Lock the mutex before checking buffer state

    int is_empty = in == out;

    pthread_mutex_unlock(&buffer_mutex);  // Unlock after checking buffer state
    return is_empty;
}

//********************************************************************
//
// Linear Search Function
//
// Initializes buffer to have -1 in all positons to represent empty buffer
//
//*******************************************************************
void initialize_buffer()
{
    in = 0;
    out = 0;
    for (int i = 0; i < BUFFER_SIZE; i++) {
        buffer[i] = -1; // Initialize buffer with some invalid value
    }
}

//********************************************************************
//
// Linear Search Function
//
// Formats the output to display the status of the buffer and the positions of the Read and Write positions
// Uses alot of lines and empty spaces to neatly format the status of the buffer
//
// Return Value
// ------------
// Void
//
//
// Reference Parameters
// --------------------
// in     int             The location of the producer
// out    int             The location of the consumer
//
//*******************************************************************
void display_buffer_status() {
    // Calculate number of occupied slots
    int occupied = (in - out + BUFFER_SIZE) % BUFFER_SIZE;

    // Display buffer occupancy count
    cout << "(buffers occupied: " << occupied << ")\n";

    // Display the buffer values
    cout << "buffers: ";
    for (int i = 0; i < BUFFER_SIZE; i++) {
        cout << (buffer[i] == -1 ? "  -1" : " " + to_string(buffer[i]));
    }
    cout << "\n         ";

    // Display markers for write (W) and read (R) positions
    for (int i = 0; i < BUFFER_SIZE; i++) {
        if (i == in) {
            cout << "  W ";  // Mark where the producer is writing
        } else {
            cout << "    ";  // Empty space for other slots
        }
    }
    cout << "\n         ";

    for (int i = 0; i < BUFFER_SIZE; i++) {
        if (i == out) {
            cout << "  R ";  // Mark where the consumer is reading
        } else {
            cout << "    ";  // Empty space for other slots
        }
    }

    cout << "\n         ---- ---- ---- ---- ----\n";
}
