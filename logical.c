#include <stdio.h>
#include "logical.h"
#include <stdlib.h>
#include <assert.h>

void checkAndInitialize(Term term) {
  if (term->inferences == NULL) {
    term->inferences = calloc(1, sizeof(struct sets));
  }
}

// add a universal affirmative
void assertUA(Term a, Term b) {
  assert(!queryUN(a, b));
  // check if a and b are just titles and if so, initialize universal and particular arrays
  checkAndInitialize(a);
  checkAndInitialize(b);

  // loop through linked list containing universal positives and check if the term is already exists
  // if the term is already there, break out
  //printf("%p %p %p\n", a, a->inferences, a->inferences->nextPosUniversal);
  List list = a->inferences->nextPosUniversal;
  if (list == NULL) {
    a->inferences->nextPosUniversal = calloc(1, sizeof(struct node));
    a->inferences->nextPosUniversal->term = b;
    return;
  }

  while (list != NULL) {
    if (list->term == b) {
      return;
    }
    list = list->next;
  }

  // the term does not already exist, so add it to the list
  List listToAdd = malloc(sizeof(struct node));
  listToAdd->term = b;
  listToAdd->next = a->inferences->nextPosUniversal;
  a->inferences->nextPosUniversal = listToAdd;
}

// add a universal negative
void assertUN(Term a, Term b) {
  assert(!queryUA(a, b));
  // check if a and b are just titles and if so, initialize universal and particular arrays
  checkAndInitialize(a);
  checkAndInitialize(b);

  // loop through linked list containing universal positives and check if the term is already exists
  // if the term is already there, break out
  
  List list = a->inferences->nextNegUniversal;
  if (list == NULL) {
    a->inferences->nextNegUniversal = calloc(1, sizeof(struct node));
    a->inferences->nextNegUniversal->term = b;
    return;
  }

  while (list != NULL) {
    if (list->term == b) {
      return;
    }
    list = list->next;
  }

  // the term does not already exist, so add it to the list
  List listToAdd = malloc(sizeof(struct node));
  listToAdd->term = b;
  listToAdd->next = a->inferences->nextNegUniversal;
  a->inferences->nextNegUniversal = listToAdd;
}
// add a particular affirmative
void assertPA(Term a, Term b) {

}
// add a particular negative
void assertPN(Term a, Term b) {

}

// Querying permises for the system

// query a universal affirmative
bool queryUA(Term a, Term b) {
  List list = a->inferences->nextPosUniversal;
  while (list != NULL) {
    Term term = list->term;
    if (term == b) {
      return true;
    }
    if (queryUA(term, b)) {
      return true;
    }
    list = list->next;
  }
  return false;
}
// query a universal negative
bool queryUN(Term a, Term b) {
  List list = a->inferences->nextNegUniversal;
  while (list != NULL) {
    Term term = list->term;
    if (term == b) {
      return true;
    }
    if (queryUN(term, b)) {
      return true;
    }
    list = list->next;
  }
  return false;
}
// query a particular affirmative
bool queryPA(Term a, Term b) {
  return false;
}
// query a particular negative
bool queryPN(Term a, Term b) {
  return false;
}
