/* CSC 450, Assignment #4, Owen Manley. This program answers Question #2 on assignment #4 which uses signals to make it so pressing Ctrl+C
after a period of time will print "Hello" for everytime press.
*/
#include <stdio.h>
#include <signal.h>
#include <unistd.h>

int ctrl_c_count = 0;
int time_elapsed = 0;

void sigint_handler(int signo) {
    ctrl_c_count++;
}

void sigalrm_handler(int signo) {
    time_elapsed = 1;
}

int main() {
    struct sigaction sa_int, sa_alrm;
    sigset_t mask, old_mask;

    // Set up SIGINT handler
    sa_int.sa_handler = sigint_handler;
    sa_int.sa_flags = SA_RESTART;
    sigemptyset(&sa_int.sa_mask);
    sigaction(SIGINT, &sa_int, NULL);
    
    // Set up SIGALRM handler
    sa_alrm.sa_handler = sigalrm_handler;
    sa_alrm.sa_flags = SA_RESTART;
    sigemptyset(&sa_alrm.sa_mask);
    sigaction(SIGALRM, &sa_alrm, NULL);
    
    // Set an alarm for 10 seconds (100 was too long for testing)
    alarm(100);
    
    // Wait for SIGALRM
    while (!time_elapsed) {
        pause();  // Keep pausing until SIGALRM is received
    }

    // Ignore SIGINT after 100 seconds
    signal(SIGINT, SIG_IGN);
    
    // Print stored Hellos
    printf("\n");
    int i;
    int count; 
    for (i = 0; i < ctrl_c_count; i++) {
        printf("Hello\n");
        count++;
        fflush(stdout);  // I had to include this or the program would stop me from pressing Ctrl+C at around 50 presses.
    }
    printf("\n");
    printf("Number of Hello's: %d\n", count);
    
    // Wait another 100 seconds before termination
    sleep(100);
    
    return 0;
}