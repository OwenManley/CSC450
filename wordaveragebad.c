#include <string.h>
#include <stdio.h>
#define LINE_DELIMITERS "\n"
#define WORD_DELIMITERS " " 

static int wordcount(char *s) {
   int count = 1;

   if (strtok(s, WORD_DELIMITERS) == NULL)
      return 0;
   while (strtok(NULL, WORD_DELIMITERS) != NULL)
      count++;
   return count;
}

double wordaverage(char *s) {      /* return average size of words in s */
   int linecount = 1;
   char *nextline;
   int words;

   nextline = strtok(s, LINE_DELIMITERS);
   if (nextline == NULL)
      return 0.0;
   words = wordcount(nextline);
   while ((nextline = strtok(NULL, LINE_DELIMITERS)) != NULL) {
      words += wordcount(nextline);
      linecount++;
   }
   return (double)words/linecount;
}

int main ()
{
char str[] = "Neuroscience has been exploring the wilderness of the brain for well over a century.\
 With A Thousand Brains, at last we have a map. Jeff Hawkins takes on questions most neuroscientists\
 don’t even dare ask,and finds answers in a new theory that explains now only how we make sense of the\
 world, but how we are deceived. In a world threatened by the disintegration of truth into conspiracy and\
 delusion, everyone should read this remarkable book."; 
double avg = wordaverage(str);
printf("Avg =%i", avg);
return 0;
}
