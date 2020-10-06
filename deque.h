/* * * * * * *
 * Deque module (i.e., double ended queue) for Assignment 1
 *
 * created for COMP20007 Design of Algorithms 2020
 * template by Tobias Edwards <tobias.edwards@unimelb.edu.au>
 * implementation by Ethan Edge
 * Reusing from Assignment 1 for Assignment 2.
 */

// You must not change any of the code already provided in this file, such as
// type definitions, constants or functions.
//
// You may, however, add additional functions and/or types which you may need
// while implementing your algorithms and data structures.

#ifndef DEQUE_H
#define DEQUE_H

// DO NOT CHANGE THESE TYPE DEFINITIONS

// The data type that our Deque will contain
typedef float Prob;

// Our Deque is implemented using a doubly-linked list
typedef struct deque Deque;

// The Nodes which make up the doubly-linked list
typedef struct d_node d_Node;

struct deque {
  d_Node *top;
  d_Node *bottom;
  int size;
};

struct d_node {
  d_Node *next;
  d_Node *prev;
  Prob probability;
  char *word;
};

// Create a new empty Deque and return a pointer to it
Deque *new_deque();

// Free the memory associated with a Deque
void free_deque(Deque *deque);

// Create a new Node with a given piece of data
d_Node *new_node(Prob probability, char *word);

// Free the memory associated with a Node
void free_d_node(d_Node *d_node);

// Add an element to the top of a Deque
void deque_push(Deque *deque, Prob probability, char *word);

// Add an element to the bottom of a Deque
void deque_insert(Deque *deque, Prob probability, char *word);

// Return the number of elements in a Deque
int deque_size(Deque *deque);

void deque_remove(Deque *deque);

#endif
