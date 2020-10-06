/* * * * * * *
 * Hashing module for Assignment 2.
 *
 * created for COMP20007 Design of Algorithms 2020
 * template by Tobias Edwards <tobias.edwards@unimelb.edu.au>
 * implementation by Ethan Edge
 */

#include "hash.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>

#define MAX_WORD_LEN        256
#define BINARY_LENGTH       6

#define LC_ASCII_DIFF       97
#define UP_ASCII_DIFF       39
#define D_ASCII_DIFF        -4

#define LOWERCASE_LB        97
#define LOWERCASE_UP        122

#define UPPERCASE_LB        65
#define UPPERCASE_UP        90

#define DIGIT_LB            48
#define DIGIT_UP            57

#define BINARY_64           64
#define DOUBLE              2



// Implements a solution to Problem 1 (a), which reads in from stdin:
//   N M
//   str_1
//   str_2
//   ...
//   str_N
// And outputs (to stdout) the hash values of the N strings 1 per line.
void problem_1_a() {

  // Takes in N words and outputs their hash values based on the
  // specifications in the calculate_hash() and character_mapping_int()
  // functions, which use M as the mod number, eg. 5 % M.


  int N, M;                        // Number of strings and mod number
  char word[MAX_WORD_LEN + 1];     // Temporary variable to hold each word
  int hash_value = 0;              // Each word's hash value

  int i;

  scanf("%d %d", &N, &M);

  // Calculates and prints the hash for each word
  for(i = 0; i < N; i++)
  {
    scanf("%s", word);
    hash_value = calculate_hash(word, M);
    printf("%d\n", hash_value);
  }
}

int calculate_hash(char* word, int M)
{
  // Takes in string and outputs its hash based on mapping each character
  // to a specific integer (specified in character_mapping_int()), converting
  // that integer to a 6 digit binary string, concatenating all the binary
  // strings and calculating that value in decimal mod M.


  int w_len = strlen(word);  // Length of word

  int previous = 0;          // Holds the total hash before current addition
  int current = 0;           // Holds current hash between two additions

  int i, j;

  int d;                     // Our character's mapped value
  int exponent;              // 64's exponent

  // Traverses the word to calculate the hash
  for(i = 0; i < w_len; i++)
  {
    d = character_mapping_int(word[i]);
    exponent = w_len - i - 1;

    // Sets up our current variable
    if (exponent != 0)
    {
      current = (((d % M) * (BINARY_64 % M)) % M);

    }
    else
    {
      current = d;
    }

    // Uses Horner's rule to simplify all of our 64 factors
    // (- 1) because we already dealt with one 64 above
    for(j = 0; j < exponent - 1; j++)
    {
      current = (((current % M) * (BINARY_64 % M)) % M);
    }

    // Uses Horner's rule to simplify all of components between the summations
    previous = (((previous % M) + (current % M)) % M);

  }

  return previous;
}

int character_mapping_int(char ch)
{
  // Takes in a character and maps it to its relative integer value
  // 'a' = 0 .... 'z' = 25, 'A' = 26 .... 'Z' = 51, '0' = 52 .... '9' = 61


  int c = ch;

  // Utilises ASCII values and their difference to our desired values to
  // to calculate our character mappings
  if(c >= LOWERCASE_LB && c <= LOWERCASE_UP) // Lowercase
  {
    return (c - LC_ASCII_DIFF);
  }
  else if(c >= UPPERCASE_LB && c <= UPPERCASE_UP)   // Uppercase
  {
    return (c - UP_ASCII_DIFF);
  }
  else if(c >= DIGIT_LB  && c <= DIGIT_UP)   // Digit
  {
    return (c - D_ASCII_DIFF);
  }

  // Another symbol was entered, not in specification
  return EXIT_FAILURE;
}

// Implements a solution to Problem 1 (b), which reads in from stdin:
//   N M K
//   str_1
//   str_2
//   ...
//   str_N
// Each string is inputed (in the given order) into a hash table with size
// M. The collision resolution strategy must be linear probing with step
// size K. If an element cannot be inserted then the table size should be
// doubled and all elements should be re-hashed (in index order) before
// the element is re-inserted.
//
// This function must output the state of the hash table after all insertions
// are performed, in the following format
//   0: str_k
//   1:
//   2: str_l
//   3: str_p
//   4:
//   ...
//   (M-2): str_q
//   (M-1):
void problem_1_b() {

  int N, M, K;                    // Num strings, mod number, step size
  char word[MAX_WORD_LEN + 1];    // Temporarily holds our words
  int i;

  scanf("%d %d %d", &N, &M, &K);

  // Create the hash table
  char** hash_table;
  hash_table = create_hash_table(M);

  // Processes all the words
  for(i = 0; i < N; i++)
  {
    scanf("%s", word);

    hash_table = process_word(hash_table, word, &M, K);
  }

  print_hash_table(hash_table, M);
}

char** process_word(char** hash_table, char* word, int *M, int K)
{
  // Each word inputted has it's hash value calculated, it is then put in that
  // space of the hash table if it's free. Otherwise it keeps jumping by K
  // (including wrapping around from the end of the hash table to the start)
  // until it finds a free spot.

  // If it reaches its initial spot. The hash table is doubled and everything
  // is rehashed.

  int hash_value;                 // A word's hash value
  int stepper;                    // hash_value + n*K, n is an integer

  hash_value = calculate_hash(word, *M);
  stepper = hash_value;

  // Puts the word into its position
  if(hash_table[hash_value][0] == '\0')
  {
    insert_hash_table(hash_table, word, hash_value);
    //print_hash_table(hash_table, M);  //DEBUG
  }

  // There's a collision
  else
  {
    while(hash_table[stepper][0] != '\0')
    {
      // We move by the step value K
      stepper += K;

      // We've gone off the edge, wrap around
      if(stepper >= *M)
      {
        stepper = stepper % *M;
      }

      // We've gotten back to the original position, double the hash table
      if(stepper == hash_value)
      {
        hash_table = double_hash_table(hash_table, M, K);

        // Recalculate the hash for the word we couldn't insert
        hash_value = calculate_hash(word, *M);
        stepper = hash_value;
      }
    }

    // We've found a valid spot for the word, insert
    insert_hash_table(hash_table, word, stepper);
  }

  return hash_table;
}

char** double_hash_table(char** old_hash_table, int *M, int K)
{
  // Doubles the size of M and creates a new hash table with this new size.
  // Rehashes all the old values into this new hash table as well as the last
  // word which couldn't be placed.
  // Frees all the memory relating to the previous hash_table.

  char word[MAX_WORD_LEN + 1];    // Temporarily holds our words
  int old_size = *M;               // Size of previous hash table
  int i;

  // Double the size/mod number
  (*M) = (*M) * DOUBLE;

  // Creating a new hash table that is doubled
  char** new_hash_table;
  new_hash_table = create_hash_table(*M);

  // Rehashes and inserts all the previous strings into the new hash table
  for(i = 0; i < old_size; i++)
  {
    if(old_hash_table[i][0] != '\0')
    {
      strcpy(word, old_hash_table[i]);

      free(old_hash_table[i]);
      old_hash_table[i] = NULL;

      process_word(new_hash_table, word, M, K);
    }
  }

  free(old_hash_table);
  old_hash_table = NULL;

  return new_hash_table;
}

void insert_hash_table(char **hash_table, char *word, int hash_value)
{
  // Inserts string 'word' into hash_table at position hash_value.

  free(hash_table[hash_value]);
  hash_table[hash_value] = malloc((strlen(word) + 1) * sizeof(char));
  strcpy(hash_table[hash_value], word);
}

char **create_hash_table(int size)
{
  // Creates a hash table of size 'size', sets all strings to the empty string

  char** hash_table;
  hash_table = malloc(size * sizeof(char*));
  assert(hash_table);

  int i;
  for(i = 0; i < size; i++)
  {
    hash_table[i] = malloc(sizeof(char));
    assert(hash_table[i]);
    hash_table[i][0] = '\0';
  }

  return hash_table;
}

void print_hash_table(char** hash_table, int size)
{
  // Goes through the hash table and prints all the indexes along with their
  // respective word if it exists.

  // Also frees all the memory related to the hash table.

  int i;
  for(i = 0; i < size; i++)
  {
    printf("%d: ", i);
    printf("%s", hash_table[i]);
    printf("\n");

    free(hash_table[i]);
    hash_table[i] = NULL;
  }

  free(hash_table);
  hash_table = NULL;
}










