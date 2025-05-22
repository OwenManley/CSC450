/*Owen Manley, CSC450, Assignment #7. This program creates 5 threads to loop up to 50 times incrementing by
a certain amount. Using semaphores, all threads are able to synchronize to one stage before continuing to increment.*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <limits.h>

//1. Create 5 threads.
//2. Each thread loops 50 times.
//3. Each loop increments a value by 0.9*MAX_INT.
//4. Print the iteration number.
/*5.Implement a two-pase barrier using semaphores so that each thread finishes their
loop and iteration before starting the next one (synchronization).*/

#define THREADS 5
#define ITERATIONS 50
//Defining constants for both the number of threads and iterations since they're fixed.
sem_t s1;
sem_t s2;
int counter = 0;
pthread_mutex_t mutex;

void two_phase_barrier(){
int i;
//PHASE 1 - Waits for threads to all arrive at same instance.
        pthread_mutex_lock(&mutex); //Uses mutex lock to prevent race condition when threads are arriving to same instance.
        counter++;
        if(counter == THREADS){
                printf("All threads have reached the barrier, releasing s1...\n"); //Debugging statements to let me know that each thread has made it to each stage.
                for(i = 0; i < THREADS; i++){
                        sem_post(&s1);
                        printf("S1 released. Phase 2 imminent...\n");
                }//end for
        }//end if

        pthread_mutex_unlock(&mutex); //mutex can now be unlocked now that threads have been released.

        sem_wait(&s1); //wait for all threads

//PHASE 2 - Sends threads forward.

        pthread_mutex_lock(&mutex); //lock mutex again for same reason.
        counter --; //After phase 1, each thread will decrement the counter.
        if(counter == 0){ //once all threads leave the barrier, all threads are released again.
                printf("All threads have passed phase 1. Commencing Phase 2...\n");
                for(i = 0; i < THREADS; i++){
                        sem_post(&s2); //release threads again
                }//end for
        }//end if
        pthread_mutex_unlock(&mutex); //unlocks mutex

        sem_wait(&s2); //waits for all threads again
}//end two_phase_barrier

void* thread_function(void* arg){
        int i;
        int j;
        int tid = *(int*)arg; //Converts thread argument to an ID.
        free(arg);//Cleans up arguments so no warnings appear
        int max = ((int) 0.9 * INT_MAX);
        for(i = 0; i < ITERATIONS; i++){
                volatile int dummy = 0; //ensures the compiler doesn't make any unexpected changes.
                for (j = 0; j < max; j++){
                        dummy += 1;
                }//end inner for

                printf("Thread %1d completed iteration %d\n", tid, i);
                two_phase_barrier();
        }//end outer for

        pthread_exit(NULL); //Terminates threads.

}//end thread_funtion

int main(){
        int j;
        int i;
        pthread_t threads[THREADS]; //Array to hold the threads.

        pthread_mutex_init(&mutex, NULL); //Initializes mutex.
        sem_init(&s1, 0, 0); //initialize s1
        sem_init(&s2, 0, 0); //initialize s2

        for (j = 0; j < THREADS; j++){ //Creates the threads and passes their IDs to thread_function.
                int* tid = malloc(sizeof(int));
                *tid = j;
                pthread_create(&threads[j], NULL, thread_function, (void*)tid);
        }//end for

        for(i = 0; i < THREADS; i++){// Uses pthread_join ti wait for each other thread to complete.
                pthread_join(threads[i], NULL);
        }//end for

        pthread_mutex_destroy(&mutex); //Destroy to clean before exiting the program.
        sem_destroy(&s1);
        sem_destroy(&s2);
        return 0;

}//end main
