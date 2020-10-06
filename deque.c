/* * * * * * *
 * Deque module (i.e., double ended queue) for Assignment 1
 *
 * created for COMP20007 Design of Algorithms 2020
 * template by Tobias Edwards <tobias.edwards@unimelb.edu.au>
 * implementation by Ethan Edge.
 * Reusing from Assignment 1 for Assignment 2.
 */

// You must not change any of the code already provided in this file, such as
// type definitions, constants or functions.
//
// You may, however, add additional functions and/or types which you may need
// while implementing your algorithms and data structures.

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "deque.h"

#define INITIAL 5
#define MAX_WORD_LEN  99

// Create a new empty Deque and return a pointer to it
Deque *new_deque() {
	Deque *deque = malloc(sizeof(*deque));
	assert(deque);

	deque->top = NULL;
	deque->bottom = NULL;
	deque->size = 0;

	return deque;
}

// Free the memory associated with a Deque
void free_deque(Deque *deque) {
  // Remove (and thus free) all of the nodes in the Deque.
  while (deque->size > 0) {
    deque_remove(deque);
  }

	// Free the deque struct itself
	free(deque);
}

// Create a new Node with given probability and string
d_Node *new_node(Prob probability, char *word) {
  d_Node *d_node = malloc(sizeof(*d_node));
  assert(d_node);

  d_node->next = NULL;
  d_node->prev = NULL;
  d_node->probability = probability;
  d_node->word = malloc(sizeof(char) * strlen(word));
  strcpy(d_node->word, word);

  return d_node;
}

// Free the memory associated with a Node
void free_d_node(d_Node *d_node) {
  free(d_node->word);
  free(d_node);
}

// Add an element to the top of a Deque
void deque_push(Deque *deque, Prob probability, char *word) {
  d_Node *new = new_node(probability, word);

  if (deque->size > 0) {
    new->next = deque->top;
    deque->top->prev = new;
  } else {
    // If the Deque was initially empty then new is both the top and bottom
    deque->bottom = new;
  }

  deque->top = new;
  deque->size++;
}

// Add an element to the bottom of a Deque
void deque_insert(Deque *deque, Prob probability, char *word) {
  d_Node *new = new_node(probability, word);

  if (deque->size > 0) {
    new->prev = deque->bottom;
    deque->bottom->next = new;
  } else {
    // If the Deque was initially empty then new is both the top and bottom
    deque->top = new;
  }

  deque->bottom = new;
  deque->size++;
}

// Remove bottom element from a Deque
void deque_remove(Deque *deque) {
  if (deque->size == 0) {
    return;
  }

  d_Node *old_bottom = deque->bottom;

  if (deque->size == 1) {
    deque->top = NULL;
    deque->bottom = NULL;
  } else {
    deque->bottom = old_bottom->prev;
    deque->bottom->next = NULL;
  }

  deque->size--;

  free(old_bottom);

}

// Return the number of elements in a Deque
int deque_size(Deque *deque) {
  return deque->size;
}


