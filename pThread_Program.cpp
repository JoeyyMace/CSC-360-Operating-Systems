/*
The purpose of this assignment is to become familiar with the pThreads API and write simple
programs using pThreads. The first program is already done for you. You will have to modify it to
obtain timing results and explore the use of data parallelism techniques for computing PI.
*/

#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

#define MAX_THREADS 512

void *compute_pi(void *);

long sample_points;
long total_hits;
long total_misses;
long hits[MAX_THREADS];
long sample_points_per_thread;
int num_threads;
clock_t start_time, end_time;  // For timing the execution
double thread_time;
FILE *output_file;

int main(int argc, char *argv[])
{
  /* local variables */
  int ii;
  int retval;
  pthread_t p_threads[MAX_THREADS];
  pthread_attr_t attr;
  double computed_pi;

  // Open CSV file to write results
    output_file = fopen("execution_times_static_thread_count.csv", "a");
    if (output_file == NULL) {
        printf("Error opening file for writing!\n");
        return -1;
    }

  /* initialize local variables */
  retval = 0;

  pthread_attr_init(&attr);
  pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);

  /* parse command line arguments into sample points and number of threads */
  /* there is no error checking here!!!!! */
  sample_points = atol(argv[1]);
  num_threads = atoi(argv[2]);
  printf( "Enter number of sample points: " );
  scanf( "%d", &sample_points );
  /*printf( "Enter number of threads: " );
  scanf( "%d", &num_threads );*/
  
  

  total_hits = 0;
  sample_points_per_thread = sample_points / num_threads;

  //Record Start time
  start_time = clock();

  //Creating Threads
  for (ii = 0; ii < num_threads; ii++)
  {
    hits[ii] = ii;
    pthread_create(&p_threads[ii], &attr, compute_pi, (void *)&hits[ii]);
  }

  //Joining Threads
  for (ii = 0; ii < num_threads; ii++)
  {
    pthread_join(p_threads[ii], NULL);
    total_hits += hits[ii];
  }

  // Calculate Pi
  computed_pi = 4.0 * (double)total_hits / ((double)(sample_points));

  //Record ending time
  end_time = clock();

  thread_time = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;

  printf("Computed PI = %lf\n", computed_pi);
  printf("Execution Time: %lf seconds\n", thread_time);

  // Log the number of threads and execution time to the CSV file
    fprintf(output_file, "%d, %lf\n", sample_points, thread_time);

  // Close the file
  fclose(output_file);

  /* return to calling environment */
  return (retval);
}

void *compute_pi(void *s)
{
  int seed;
  long ii;
  long *hit_pointer;
  long local_hits;
  double rand_no_x;
  double rand_no_y;

  hit_pointer = (long *)s;
  seed = *hit_pointer;
  local_hits = 0;

  for (ii = 0; ii < sample_points_per_thread; ii++)
  {
    rand_no_x = (double)(rand_r(&seed)) / (double)RAND_MAX;
    rand_no_y = (double)(rand_r(&seed)) / (double)RAND_MAX;
    if (((rand_no_x - 0.5) * (rand_no_x - 0.5) +
         (rand_no_y - 0.5) * (rand_no_y - 0.5)) <= 0.25)
      local_hits++;
    seed *= ii;
  }

  *hit_pointer = local_hits;
  pthread_exit(0);
}
