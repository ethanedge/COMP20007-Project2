/* * * * * * *
 * Text Analysis module for Assignment 2.
 *
 * created for COMP20007 Design of Algorithms 2020
 * template by Tobias Edwards <tobias.edwards@unimelb.edu.au>
 * implementation by Ethan Edge
 */

#include "text_analysis.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>
#include "deque.h"

#define START             '^'
#define END               '$'

#define MAX_CHILDREN      27
#define MAX_WORD_LEN      99
#define MAX_STUB_WORDS    5

#define CHAR_SHIFT        96
#define END_CHAR_INDEX    0

#define INITIAL_COUNT     -1

trie_t *create_trie(void)
{
  // Creates a new trie. Includes the root node with 0 frequency and the START
  // character.

  // Creates the trie
  trie_t *trie;
  trie = malloc(sizeof(*trie));
  assert(trie);

  // Creates the root node
  node_t *new;
  new = malloc(sizeof(*new));
  assert(new);

  trie->root = new;

  new->frequency = 0;
  new->letter = START;
  new = allocate_child_pointers(new);

  return trie;
}

node_t *allocate_child_pointers(node_t *node)
{
  // Allocates memory for all the potential child pointers of a node
  //recheck this

  // Allocate the memory
  node->next = malloc(sizeof(node_t*) * MAX_CHILDREN);
  assert(node->next);

  int i;
  for (i = 0; i < MAX_CHILDREN; i++)
  {
    (node->next)[i] = malloc(sizeof(node_t));
    assert((node->next)[i]);
    (node->next)[i] = NULL;
  }

  return node;
}

node_t *create_node(char letter)
{
  // Takes a letter and creates a new node with that letter and frequency 1

  node_t *new;
  new = malloc(sizeof(*new));
  assert(new);

  new->frequency = 1;
  new->letter = letter;

  new = allocate_child_pointers(new);

  return new;
}

void free_trie(trie_t *trie)
{
  // Frees all the memory associated with a trie

  node_t *traverse;
  traverse = trie->root;

  free_node(traverse);

  free(trie);
  trie = NULL;
}

void free_node(node_t *node)
{
  // Frees the node and recursively calls this function for all the node's
  // children

  int i;

  // Go through and call the function for all the node's children
  for (i = 0; i < MAX_CHILDREN; i++)
  {
    if (node->next[i])
    {
      free_node(node->next[i]);
    }
  }

  free(node);
  node = NULL;
}

// Build a character level trie for a given set of words.
//
// The input to your program is an integer N followed by N lines containing
// words of length < 100 characters, containing only lowercase letters.
//
// Your program should built a character level trie where each node indicates
// a single character. Branches should be ordered in alphabetic order.
//
// Your program must output the post-order traversal of the characters in
// the trie, on a single line.
void problem_2_a() {

  int N;          // Number of strings
  int i;

  scanf("%d", &N);

  // Create trie
  trie_t *trie;
  trie = create_trie();

  char word[MAX_WORD_LEN + 1];              // Temporary variable to hold word

  // Process all the words
  for (i = 0; i < N; i++)
  {
    scanf("%s", word);

    trie = word_into_trie(trie, word);
  }

  // Print the trie and then free it
  print_trie_preorder(trie, trie->root);
  free_trie(trie);
}

trie_t *word_into_trie(trie_t *trie, char *word)
{
  // Takes an existing trie and a word to insert. The word is broken up into
  // its characters and they are inserted one by one and if the character
  // already exists in the position, the frequency is updated.

  int i;

  int curr_letter;            // Our current letter
  int letter_index;           // The letter's index

  node_t *current;            // Puts to our current node
  current = trie->root;

  int word_len;               // Length of the word
  word_len = strlen(word);

  // We have another word
  trie->root->frequency = trie->root->frequency + 1;

  // Obtain each letter, character by character from the word and process it
  for (i = 0; i <= word_len; i++)
  {
    // Selects next letter in the word or the END character at the end
    if (i == word_len)
    {
      curr_letter = END;
    }
    else
    {
      curr_letter = word[i];
    }

    letter_index = char_pointer_position(curr_letter);

    if ((current->next)[letter_index] == NULL)
    {
      (current->next)[letter_index] = create_node(curr_letter);


      current = (current->next)[letter_index];
    }
    else  // The letter already exists there
    {
      current = (current->next)[letter_index];
      current->frequency = current->frequency + 1;
    }
  }

  return trie;
}

int char_pointer_position(char letter)
{
  // Takes a character and maps it to an integer which will its index in a
  // node's child pointer array.

  // The end character goes to the last position
  if (letter == END)
  {
    return END_CHAR_INDEX;
  }

  // Uses ASCII values to map 'a' to 0... 'z' to 25
  int d;
  d = letter - CHAR_SHIFT;
  return d;

}

void print_trie_preorder(trie_t *trie, node_t *node)
{
  // Traverses the trie using preorder traversal and prints each character
  // on its own line.

  int i;

  printf("%c\n", node->letter);

  // Ensures we don't go investigating a leaf node's children
  if (node->letter == END)
  {
    return;
  }

  // Explore all the child nodes from left to right (preorder) and prints them
  for (i = 0; i < MAX_CHILDREN; i++)
  {
    if (node->next[i] != NULL)
    {
      print_trie_preorder(trie, node->next[i]);
    }
  }
}

// Using the trie constructed in Part (a) this program should output all
// prefixes of length K, in alphabetic order along with their frequencies
// with their frequencies. The input will be:
//   n k
//   str_0
//   ...
//   str_(n-1)
// The output format should be as follows:
//   an 3
//   az 1
//   ba 12
//   ...
//   ye 1
void problem_2_b() {

  int N;              // Number of strings
  int K;              // Length of prefix we want
  int i;

  scanf("%d %d", &N, &K);

  // Create trie
  trie_t *trie;
  trie = create_trie();

  char word[MAX_WORD_LEN + 1];              // Temporary variable to hold word

  // Process all the words
  for (i = 0; i < N; i++)
  {
    scanf("%s", word);

    trie = word_into_trie(trie, word);
  }

  // Find and print our prefixes of length K
  char new_prefix[K];
  print_prefixes(trie, trie->root, new_prefix, K, INITIAL_COUNT);

  free_trie(trie);
}

void print_prefixes(trie_t *trie, node_t *node, char *old_prefix, int K,
                    int count)
{
  // Traverses the trie and prints all the prefixes of length K

  int i;
  char new_prefix[K];
  char letter;
  letter = node->letter;

  count += 1;

  // We don't want to do anything with the start character
  if (letter == START)
  {
  }
  // There's no prefix here
  else if (letter == END)
  {
    return;
  }
  // Found another character, create a new prefix string from the old and append
  // our new character
  else
  {
    strcpy(new_prefix, old_prefix);
    new_prefix[count - 1] = letter;
  }

  // Prefix length requirement is reached, so print
  if (count == K)
  {
    for (i = 0; i < K; i++)
    {
      printf("%c", new_prefix[i]);
    }
    printf(" %d\n", node->frequency);
  }

  // Explore all the child nodes from left to right (preorder) and builds up
  // our prefix string until the length is reached
  for (i = 0; i < MAX_CHILDREN; i++)
  {
    if (node->next[i] != NULL)
    {
      print_prefixes(trie, node->next[i], new_prefix, K, count);
    }
  }
}

// Again using the trie data structure you implemented for Part (a) you will
// provide a list (up to 5) of the most probable word completions for a given
// word stub.
//
// For example if the word stub is "al" your program may output:
//   0.50 algorithm
//   0.25 algebra
//   0.13 alright
//   0.06 albert
//   0.03 albania
//
// The probabilities should be formatted to exactly 2 decimal places and
// should be computed according to the following formula, for a word W with the
// prefix S:
//   Pr(word = W | stub = S) = Freq(word = W) / Freq(stub = S)
//
// The input to your program will be the following:
//   n
//   stub
//   str_0
//   ...
//   str_(n-1)
// That is, there are n + 1 strings in total, with the first being the word
// stub.
//
// If there are two strings with the same probability ties should be broken
// alphabetically (with "a" coming before "aa").
void problem_2_c() {

  int N;                              // Number of strings
  int stub_frequency;                 // Frequency of our stub
  char stub[MAX_WORD_LEN + 1];        // Will hold our stub string
  char word[MAX_WORD_LEN + 1];        // Temporary variable to hold word
  node_t *traverse;                   // Node we use to traverse the trie
  int i;

  scanf("%d", &N);
  scanf("%s", stub);

  int stub_len = strlen(stub);        // The length of our stub

  // Create trie
  trie_t *trie;
  trie = create_trie();

  // Process all the words into the trie
  for (i = 0; i < N; i++)
  {
    scanf("%s", word);

    trie = word_into_trie(trie, word);
  }

  // Get our stub frequency and the node pointer for the end of the stub
  // in the trie
  traverse = find_stub_frequency(trie, &stub_frequency, stub, stub_len);

  // A deque for all our words and their probabilities
  Deque *high_prob_words;
  high_prob_words = new_deque();

  // Finds all the words with the stub and their relative probabilities
  find_stubbed_words(trie, high_prob_words, stub_frequency, stub, traverse,
                     true);

  // Print our 5 (maybe less) highest probability words
  print_stubbed_words(high_prob_words);

  free_trie(trie);
}

node_t *find_stub_frequency(trie_t *trie, int *stub_frequency, char *stub,
                            int stub_len)
{
  // Takes a sting 'stub' and counts its frequency in the trie.
  // Returns the last character in the trie's pointer.

  node_t *traverse;                   // Node we use to traverse the trie
  traverse = trie->root;

  int i;
  char stub_char;                     // The stub's current character
  int char_pos;                       // That character's index
  int count = 0;                      // How much of the stub we have found


  for (i = 0; i < stub_len; i++)
  {
    // Get each character and their relative character index
    stub_char = stub[i];
    char_pos = char_pointer_position(stub_char);

    // Find the character in the trie at our current level's children
    if ( ((traverse->next)[char_pos]) && ((traverse->next)[char_pos])->letter == stub_char )
    {
      count += 1;
      traverse = (traverse->next)[char_pos];

      // We have reached the end of the stub, get frequency
      if (count == stub_len)
      {
        *stub_frequency = traverse->frequency;
      }
    }
  }

  return traverse;

}

void find_stubbed_words(trie_t *trie, Deque *high_prob_words,
                         int stub_frequency, char *old_str,
                         node_t *traverse, _Bool start)
{
  // Finds all the words with the stub prefix. Also finds their relative
  // probabilities. Inserts these two values into a deque.

  char new_str[MAX_WORD_LEN + 1];
  strcpy(new_str, old_str);

  int length;
  length = strlen(new_str);

  int i;

  // Runs this on any recursive calls only
  if (!start)
  {
    // We have reached the end of a word, print information
    if (traverse->letter == END)
    {
      int word_frequency;
      float probability;

      word_frequency = traverse->frequency;

      probability = ((float)word_frequency / stub_frequency);

      // Keeps the word in a deque with its frequency
      deque_push(high_prob_words, probability, old_str);
    }
    // Another character, append to current string
    else
    {
      new_str[length] = traverse->letter;
      new_str[length + 1] = '\0';
    }
  }

  // Explore all the child nodes from left to right (preorder) and builds up
  // our prefix string until the length is reached
  for (i = 0; i < MAX_CHILDREN; i++)
  {
    if (traverse->next[i] != NULL)
    {
      find_stubbed_words(trie, high_prob_words, stub_frequency, new_str,
                         traverse->next[i], false);
    }
  }
}

void print_stubbed_words(Deque *high_prob_words)
{
  // Goes through our high_prob_words deque and prints out the top 5 words
  // and their relative probabilities (based on their probability values).
  // Will print out less than 5 if there are less than 5 words.

  float max_prob;       // Our current maximum probability
  int count = 0;        // How many words we've printed
  int i;

  // Node pointers so we can traverse the deque and keep track of our max
  d_Node *traverse;
  d_Node *curr_max;

  int deque_size;
  deque_size = high_prob_words->size;

  // Print out a max of 5 stub words and their relative probabilities
  while (count < MAX_STUB_WORDS)
  {
    traverse = high_prob_words->top;
    max_prob = traverse->probability;
    curr_max = traverse;

    // Traverse the deque to find it's maximum probability word
    for (i = 1; i < deque_size; i++)
    {
      traverse = traverse->next;

      // New maximum probability found
      if (traverse->probability > max_prob)
      {
        max_prob = traverse->probability;
        curr_max = traverse;
      }
      // If probabilities are the same, base on alphabetical ordering of words
      else if (traverse->probability == max_prob)
      {
        if (strcmp(curr_max->word, traverse->word) > 0)
        {
          max_prob = traverse->probability;
          curr_max = traverse;
        }
      }
    }

    // There are less than 5 words
    if (count == deque_size)
    {
      return;
    }

    count += 1;
    printf("%.2f %s\n", curr_max->probability, curr_max->word);
    curr_max->probability = 0;
  }

  free_deque(high_prob_words);
}








