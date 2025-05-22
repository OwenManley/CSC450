/*CSC 450, Assignment #4, Owen Manley. This program answers question #1 on whether 
the parent and child process can access a file descriptor of a process. And, what happens when
they're being written to at the same time.
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char * argv[]){
    int file_write;
    int fd = open("test.txt", O_RDWR);
    
    if(fd < 0){
       printf("File not found/Could not open.");
        exit(1);
    }

    fork();
    
    printf("fd = %d\n", fd);
    file_write = write(fd, "Hello World\n", strlen("Hello World\n"));
    printf("called write(% d, \"Hello World\\n\", %d)." " It returned %d\n", fd, strlen("Hello World\n"), file_write);


}

/*With this program, I was able to answer Question 1. For the first half, both of
parent and child's file descriptors were the same. For the second half, there were two
"Hello World"s. So, a child can have the same file descriptor and can be written the same 
time as the parent.*/