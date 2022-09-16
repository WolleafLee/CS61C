/*
 * Include the provided hash table library.
 */
#include "hashtable.h"

/*
 * Include the header file.
 */
#include "philspel.h"

/*
 * Standard IO and file routines.
 */
#include <stdio.h>

/*
 * General utility routines (including malloc()).
 */
#include <stdlib.h>

/*
 * Character utility routines.
 */
#include <ctype.h>

/*
 * String utility routines.
 */
#include <string.h>

/*
 * This hash table stores the dictionary.
 */
HashTable *dictionary;

/*
 * The MAIN routine.  You can safely print debugging information
 * to standard error (stderr) as shown and it will be ignored in 
 * the grading process.
 */
int main(int argc, char **argv) {
  if (argc != 2) {
    fprintf(stderr, "Specify a dictionary\n");
    return 0;
  }
  /*
   * Allocate a hash table to store the dictionary.
   */
  fprintf(stderr, "Creating hashtable\n");
  dictionary = createHashTable(2255, &stringHash, &stringEquals);

  fprintf(stderr, "Loading dictionary %s\n", argv[1]);
  readDictionary(argv[1]);
  fprintf(stderr, "Dictionary loaded\n");

  fprintf(stderr, "Processing stdin\n");
  processInput();

  /*
   * The MAIN function in C should always return 0 as a way of telling
   * whatever program invoked this that everything went OK.
   */
  return 0;
}

/*
 * This should hash a string to a bucket index.  Void *s can be safely cast
 * to a char * (null terminated string) and is already done for you here 
 * for convenience.
 */
unsigned int stringHash(void *s) {
  char *string = (char *)s;
  int length = strlen(string);
  int hashCode = 0;
  for (int i = 0; i < length; i++) {
    hashCode = string[i] - 'a' + hashCode * 52;
  }
  return hashCode;
}

/*
 * This should return a nonzero value if the two strings are identical 
 * (case sensitive comparison) and 0 otherwise.
 */
int stringEquals(void *s1, void *s2) {
  char *string1 = (char *)s1;
  char *string2 = (char *)s2;
  return strcmp(string1, string2) == 0;
}

/*
 * This function should read in every word from the dictionary and
 * store it in the hash table.  You should first open the file specified,
 * then read the words one at a time and insert them into the dictionary.
 * Once the file is read in completely, return.  You will need to allocate
 * (using malloc()) space for each word.  As described in the spec, you
 * can initially assume that no word is longer than 60 characters.  However,
 * for the final 20% of your grade, you cannot assumed that words have a bounded
 * length.  You CANNOT assume that the specified file exists.  If the file does
 * NOT exist, you should print some message to standard error and call exit(1)
 * to cleanly exit the program.
 *
 * Since the format is one word at a time, with new lines in between,
 * you can safely use fscanf() to read in the strings until you want to handle
 * arbitrarily long dictionary chacaters.
 */
void readDictionary(char *dictName) {
  FILE* file = fopen(dictName, "r");
  if (!file) {
    fprintf(stderr, "CAN NOT OPEN THIS FILE!");
    exit(1);
  }
  char* string = malloc(sizeof(char) * 60);
  char temp;
  int i = 0;
  while ((temp = fgetc(file)) != EOF) {
    if (isalpha(temp)) {
      string[i++] = temp;
      if (i + 1 == sizeof(string)) {
        string = realloc(string, sizeof(string) * 2);
      }
    } else {
      string[i] = '\0';
      insertData(dictionary, (void *) string, (void *) string);
      free(string);
      string = malloc(sizeof(char) * 60);
      i = 0;
    }
  }
  fclose(file);
}

/*
 * This should process standard input (stdin) and copy it to standard
 * output (stdout) as specified in the spec (e.g., if a standard 
 * dictionary was used and the string "this is a taest of  this-proGram" 
 * was given to stdin, the output to stdout should be 
 * "this is a teast [sic] of  this-proGram").  All words should be checked
 * against the dictionary as they are input, then with all but the first
 * letter converted to lowercase, and finally with all letters converted
 * to lowercase.  Only if all 3 cases are not in the dictionary should it
 * be reported as not found by appending " [sic]" after the error.
 *
 * Since we care about preserving whitespace and pass through all non alphabet
 * characters untouched, scanf() is probably insufficent (since it only considers
 * whitespace as breaking strings), meaning you will probably have
 * to get characters from stdin one at a time.
 *
 * Do note that even under the initial assumption that no word is longer than 60
 * characters, you may still encounter strings of non-alphabetic characters (e.g.,
 * numbers and punctuation) which are longer than 60 characters. Again, for the 
 * final 20% of your grade, you cannot assume words have a bounded length.
 */
void processInput() {
  char* string1 = malloc(sizeof(char) * 60);
  char* string2 = malloc(sizeof(char) * 60);
  char* string3 = malloc(sizeof(char) * 60);
  char temp;
  int i = 0;
  int size = 60;
  do { 
    temp = fgetc(stdin);
    if (isalpha(temp)) {
      string1[i] = temp;
      string2[i] = (i == 0) ? temp : tolower(temp);
      string3[i] = tolower(temp);
      i++;
      if (i >= size) {
        string1 = realloc(string1, sizeof(char) * size * 2);
        string2 = realloc(string2, sizeof(char) * size * 2);
        string3 = realloc(string3, sizeof(char) * size * 2);
        size *= 2;
      }
    } else {
      if (isalpha(string1[0])) {
        string1[i] = '\0';
        string2[i] = '\0';
        string3[i] = '\0';
        if (findData(dictionary, string1) == NULL &&
            findData(dictionary, string2) == NULL &&
            findData(dictionary, string3) == NULL) {
          fprintf(stdout, "%s [sic]", string1);
        } else {
          fprintf(stdout, "%s", string1);
        } 
      }
      if (temp != EOF) {
        fprintf(stdout, "%c", temp);
      }
      memset(string1, 0, size);
      memset(string2, 0, size);
      memset(string3, 0, size);
      string1 = realloc(string1, sizeof(char) * 60);
      string2 = realloc(string2, sizeof(char) * 60);
      string3 = realloc(string3, sizeof(char) * 60);
      size = 60;
      i = 0;
    }
  } while (temp != EOF);
  free(string1);
  free(string2);
  free(string3);
}
