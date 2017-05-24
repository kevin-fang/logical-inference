#include <stdio.h>
#include "logical.h"
#include <stdlib.h>

void checkAndInitialize(Term term) {
  if (term->univ == NULL) {
    term->univ = malloc(sizeof(struct universals));
  }
  if (term->part == NULL) {
    term->part = malloc(sizeof(struct particulars));
  }
}

// add a universal affirmative
void assertUA(Term a, Term b) {
  // check if a and b are just titles and if so , initialize universal and particular arrays
  checkAndInitialize(a);
  checkAndInitialize(b);

  // loop through linked list containing universal positives and check if the term is already exists
  // if the term is already there, break out
  List list = a->univ->nextPos;
  while (list != NULL) {
    if (list->term == b) {
      return;
    }
    list = list->next;
  }

  // the term does not already exist, so add it to the list
  List listToAdd = malloc(sizeof(struct node));
  listToAdd->term = b;
  a->univ->nextPos->next = a->univ->nextPos;
  a->univ->nextPos = listToAdd;
}

// add a universal negative
void assertUN(Term a, Term b) {

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
  List list = a->univ->nextPos;
  while (list != NULL) {
    if (list->term == b) {
      return true;
    }
    list = list->next;
  }
  return false;
}
// query a universal negative
bool queryUN(Term a, Term b) {
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
