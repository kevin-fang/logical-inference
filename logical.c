#include <stdio.h>
#include "logical.h"
#include <stdlib.h>
#include <assert.h>

// makeTerm creates a new term with a given title, callocing the correct amount of space needed
Term makeTerm(String title) {
  Term newTerm = calloc(1, sizeof(struct term));
  newTerm->title = title;
  return newTerm;
}

// checkAndInitialize checks if the inferences struct of two terms are null, and if so calloc space for it
void checkAndInitialize(Term first, Term second) {
  if (first->inferences == NULL) {
    first->inferences = calloc(1, sizeof(struct sets));
  }
  if (second->inferences == NULL) {
    second->inferences = calloc(1, sizeof(struct sets));
  }
}

// add a universal affirmative
void assertUA(Term a, Term b) {
  // check if inferences are not currently null and if so, initialize
  checkAndInitialize(a, b);

  /* Check if the logic makes sense for universal affirmative and if not, exit the program
   * Universal Affirmative:
   * Two items cannot be in a partial or universal negative
   * queryPN searches for both a partial or universal negative
  */
  assert(!queryPN(a, b));

  // set the list to the first item if the list is currently null
  List list = a->inferences->nextPosUniversal;
  if (list == NULL) {
    a->inferences->nextPosUniversal = calloc(1, sizeof(struct node));
    a->inferences->nextPosUniversal->term = b;
    return;
  }

  // loop through linked list containing universal positives and check if the term is already exists
  // if the term is already there, break out. If not, continue
  while (list != NULL) {
    if (list->term == b) {
      return;
    }
    list = list->next;
  }

  // the term does not already exist in the list, so add it to the list
  List listToAdd = malloc(sizeof(struct node));
  listToAdd->term = b;
  listToAdd->next = a->inferences->nextPosUniversal;
  a->inferences->nextPosUniversal = listToAdd;
}

// add a universal negative
void assertUN(Term a, Term b) {
  // check if inferences are not currently null and if so, initialize
  checkAndInitialize(a, b);
  /* Check if the logic makes sense for universal negative and if not, exit the program
   * Universal Negative:
   * Two items cannot be in a partial or universal affirmative
  */
  assert(!queryPA(a, b));
    // set the list to the first item if the list is currently null
  List list = a->inferences->nextNegUniversal;
  if (list == NULL) {
    a->inferences->nextNegUniversal = calloc(1, sizeof(struct node));
    a->inferences->nextNegUniversal->term = b;
    return;
  }
  // loop through linked list and check if the term is already exists
  // if the term is already there, break out. If not, continue
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
  // check and initialize inference structs for A and B
  checkAndInitialize(a, b);

  // For a particular affirmative, a universal negative containing those two cannot exist
  assert(!queryUN(a, b));

  // set first item if list is empty
  List list = a->inferences->nextPosParticular;
  if (list == NULL) {
    a->inferences->nextPosParticular = calloc(1, sizeof(struct node));
    a->inferences->nextPosParticular->term = b;
    return;
  }

  // loop through linked list of partial affirmatives and check if the term is already exists
  // if the term is already there, break out
  while (list != NULL) {
    if (list->term == b) {
      return;
    }
    list = list->next;
  }

  // the term does not already exist, so add it to the list
  List listToAdd = malloc(sizeof(struct node));
  listToAdd->term = b;
  listToAdd->next = a->inferences->nextPosParticular;
  a->inferences->nextPosParticular = listToAdd;
}

// add a particular negative
void assertPN(Term a, Term b) {
  // initialize inference arrays
  checkAndInitialize(a, b);

  // for a particular negative, a universal affirmative containing the same elements cannot exist
  assert(!queryUA(a, b));

  // set first item if list is empty
  List list = a->inferences->nextNegParticular;
  if (list == NULL) {
    a->inferences->nextNegParticular = calloc(1, sizeof(struct node));
    a->inferences->nextNegParticular->term = b;
    return;
  }

    // loop through linked list of partial negatives and check if the term is already exists
    // if the term is already there, break out
  while (list != NULL) {
    if (list->term == b) {
      return;
    }
    list = list->next;
  }

  // the term does not already exist, so add it to the list
  List listToAdd = malloc(sizeof(struct node));
  listToAdd->term = b;
  listToAdd->next = a->inferences->nextNegParticular;
  a->inferences->nextNegParticular = listToAdd;
}

// Querying permises for the system

// query a universal affirmative
// todo: implement particulars
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
// todo: implement particulars
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
  if (queryUA(a, b)) {
    return true;
  }
  List list = a->inferences->nextPosParticular;
  while (list != NULL) {
    Term term = list->term;
    if (term == b) {
      return true;
    }
    list = list->next;
  }
  return false;
}
// query a particular negative
bool queryPN(Term a, Term b) {
  if (queryUN(a, b)) {
    return true;
  }
  List list = a->inferences->nextNegParticular;
  while (list != NULL) {
    Term term = list->term;
    if (term == b) {
      return true;
    }
    list = list->next;
  }
  return false;
}
