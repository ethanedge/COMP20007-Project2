/* * * * * * *
 * Text Analysis module for Assignment 2.
 *
 * created for COMP20007 Design of Algorithms 2020
 * template by Tobias Edwards <tobias.edwards@unimelb.edu.au>
 * implementation by Ethan Edge
 */

#ifndef TEXT_ANALYSIS_H
#define TEXT_ANALYSIS_H

#include "deque.h"

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
void problem_2_a();

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
void problem_2_b();

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
void problem_2_c();

typedef struct node node_t;

struct node {
  int     frequency;
  char    letter;
  node_t  **next;
};

typedef struct {
  node_t *root;
} trie_t;

void free_trie(trie_t *trie);
void free_node(node_t *node);

// Question 1
trie_t *create_trie(void);
node_t *create_node(char letter);
node_t *allocate_child_pointers(node_t *node);
trie_t *word_into_trie(trie_t *trie, char *word);
int char_pointer_position(char letter);
void print_trie_preorder(trie_t *trie, node_t *node);

// Question 2
void print_prefixes(trie_t *trie, node_t *node, char *old_prefix, int K,
                    int count);

// Question 3
node_t *find_stub_frequency(trie_t *trie, int *stub_frequency, char *stub,
                            int stub_len);
void find_stubbed_words(trie_t *trie, Deque *high_prob_words,
                        int stub_frequency, char *old_str, node_t *traverse,
                        _Bool start);

void print_stubbed_words(Deque *high_prob_words);
#endif
