/*Owen Manley, CSC450, Assignment #6. This assignment creates two threads with different functions that have the scope of PROCESS.
The purpose of the assignment is to compare the results of the program when it has different scopes.*/


//OBSERVATIONS//
/*I ran the program with the contention scope set to "PROCESS" for 10 seconds. I ran it for the same time when setting the contention 
scope to "SYSTEM" as well. When thread B was in its scope of "PROCESS" x got incremented to 329882056.355889. When its scope was set to
"SYSTEM" x got to 341329649.285208. The reason the values are different are because the scope is what determines what the threads 
compete with in terms of resources. When set in the "PROCESS" scope, it was a smaller value because Thread B only competes with other
threads in the program. When set to "SYSTEM" x is incremented higher because it competes with system resources and operates at the kernel level.
Running on the system level better utilizes the resources of the system and scheduling is handled by the scheduler directly. */
//OBSERVATIONS//

#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <signal.h>
#include <unistd.h>


//1.Recieve a SIGINT
//2.Create two threads on signal.
//3.Set the scope to PROCESS.
//4.Thread B executes a for loop that increments by a small value.
//5.Thread A waits for B to terminate, then prints the result of B.
//6.Repeat the process again but with scope as SYSTEM.
//7.Compare results.

volatile sig_atomic_t start_threads = 0; //Var that handles the first SIGINT.
volatile sig_atomic_t stop = 0; //Var that handles the second SIGINT.

void handle_signal(int sig){ // Handles the SIGINT presses. 
        if (!start_threads){
                start_threads = 1;
        }//end if
        else {
                stop = 1;
        }//end else
}//end handle_signal

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void* threadB_function(void* args){ //Incrementing function for thread B.
        time_t start = time(NULL); //For getting the lifetime of thread B...
        time_t most_recent_time = start; //Makes it so the time is incremented correctly instead of printing "10 seconds" over and over again.
        double* result = malloc(sizeof(double)); //To prevent segmentation fault.
        double x = 0.0; // Value to increment.

        while(!stop){
                x += 000.1; //Increment by .1.
                if (time(NULL) - most_recent_time >= 10){ //Print lifetime of thread B after every 10 seconds that go by.
                        printf("Thread B lifetime: %ld seconds\n", time(NULL) - start);
                        most_recent_time = time(NULL); //Resets the timer.
                        printf("Thread B working...\n");

                }//outer if

        }//end while

        *result = x;
        pthread_exit(result); //Exit Thread and save results so thread A can read the results of B.

}//end threadB_function

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void* threadA_function(void* args){ //Function for thread A.
        pthread_t* threadB_ID = (pthread_t*) args; //To read the ID of B.
        void* threadB_result;

        pthread_join(*threadB_ID, &threadB_result); //Results of B are now in A.
        double* x = (double*) threadB_result;
        printf("\nSignal recieved. Threads have ceased. Thread B's x = %f\n", *x); //Print this from A. Now that SIGINT has been pressed, A can stop waiting and will print results of B.

        pthread_exit(NULL); //Exit thread.
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int main(){

        pthread_t p1, p2; //Declare thread vars
        pthread_attr_t attr; //declare scope var

        pthread_attr_init(&attr); //initialize scope var

        pthread_attr_setscope(&attr, PTHREAD_SCOPE_PROCESS); //where the scope is being set. can switch between process and system.

        signal(SIGINT, handle_signal); //For the SIGINT.

        printf("Program is running. Waiting for signal...\n"); //To let the user know the program is running without sending SIGINT.

        while(!start_threads){ //While the SIGINT isn't sent the process sleeps.
                sleep(1);
        }//end while

        printf("\nSignal Received! Threads have started... (Send a signal to stop threads!)\n"); //To let the user know that the signal has been recieved.

        pthread_create(&p2, &attr, threadB_function, NULL); // Creates thread B.
        pthread_create(&p1, &attr, threadA_function, &p2); // Creates thread A that has the pointer of B so the results of B can be recieved.
        
        pthread_join(p1, NULL); // Makes it so thread A waits for thread B to finish.
        printf("Exiting threads..."); // To let the user know that the threads are being exited.
        return 0;
}//end main

