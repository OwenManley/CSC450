/*Owen Manley, Assignment #5, CSC 450. This is a re-write of wordaverage.c to use strtok more effectively. Doing so allows the program to accurately depict how many words per 
line are in the paragraph.*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Total sentences = 6.
// Word count = 151.
// 1. Copy each sentence.
// 2. Calculate total sentences.
// 3. Split the sentences up.
// 4. Get total Number of words.
// 5. Calculate average of words per sentence. 
// - Freeing memory allocation to get proper average.

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//NumberofSentences is a function that duplicates each sentence. It does so through tokenization after each period.
int NumberofSentences(char * str){
    //total_sentences = sentence count.
    int total_sentences = 0;
    //Sentence is equal to each period, replicating a sentence.
    char *copy = strdup(str);
    char * sentence = strtok(copy, ".");
    //Sentence count goes up when iterating through the string.
    while(sentence != NULL){
        if (strlen(sentence) > 0){
            total_sentences++;
        }//end if
        sentence = strtok(NULL, ".");
    }//end while
    //Free up memory for copied sentences.
    free(copy);
    return total_sentences;
}// end NumberofSentences

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//SplitSentences splits each sentence apart that are copied. It puts each sentence into its own array.
void SplitSentences(char *arr[], char *str){
    char *copy = strdup(str);
    char * split;
    int i = 0;
    split = strtok(copy, ".");
    while(split != NULL){
        if (strlen(split) > 0){
            arr[i++] = strdup(split);
        }//end if
        split = strtok(NULL, ".");
    }//end while
    //Free up memory for SplitSentences array.
    free(copy);
}//end SplitSentences

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//WordCount is a function that duplicates each word. It does so through tokenization after each space.
int WordCount(char * sentence){
    int words = 0;
    char *copy = strdup(sentence);
    char *token = strtok(copy, " ");

    while(token != NULL){
        words++;
        token = strtok(NULL, " \n");
    }//end while
    //Free up memory for duplicated words.
    free(copy);
    return words;
}//end WordCount

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int main(int argc, char* argv[]){
    char str[] = "In essence, ROS is a flexible framework that aims to ease the development of 
                creating robot software. It’s a pseudo operating system consisting of a wide range of 
                software tools and libraries that make life a little easier for robotics developers. One 
                of it’s main strengths is the open-source nature of the framework. This was 
                intentionally designed in order to promote collaboration between groups, capitalise 
                on their strengths and build on each other’s work, with the end result being more 
                robust and effective robot software. As such, individuals or groups can create their 
                own tools and libraries and publish them for use by the wider ROS community. This 
                modularity makes life a little easier for budding roboticists (which you most likely 
                are if you’re reading this) as it allows you to use as much or as little of ROS as you’d 
                like, picking and choosing what suits your particular application best."; 

    //total_sentences is the copied sentences from NumberofSentences function.
    int total_sentences = NumberofSentences(str);
    //Free up memory for the total amount of sentences.
    char **arr = malloc(total_sentences * sizeof(char *));
    //Calls the array from SplitSentences.
    SplitSentences(arr, str);
    //Copy_of_words is the variable for the total words in each sentence.
    int copy_of_words = 0;
    int i = 0;
    //"i" represents each word in each sentence.
    for(i=0; i < total_sentences; i++){
        copy_of_words += WordCount (arr[i]);
        //Free memory for each word.
        free(arr[i]);
    }//end for
    //Free memory for the sentences.
    free(arr);
    //Calculates the avarage by dividing the words of each sentences by the total number of sentences.
    int avg = copy_of_words/total_sentences;
    printf("Average number of words per sentences = %d\n", avg);
    return 0;
}//end main

