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
bool assertUA(Term a, Term b) {
  if (a == b) {
    return true;
  }
  // check if inferences are not currently null and if so, initialize
  checkAndInitialize(a, b);

  /* Check if the logic makes sense for universal affirmative and if not, exit the program
   * Universal Affirmative:
   * Two items cannot be in a partial or universal negative
   * queryPN searches for both a partial or universal negative
  */
  if (queryPN(a, b)) {
    return false;
  }
  //assert(!queryPN(a, b));

  // set the list to the first item if the list is currently null
  List list = a->inferences->nextPosUniversal;
  if (list == NULL) {
    a->inferences->nextPosUniversal = calloc(1, sizeof(struct node));
    a->inferences->nextPosUniversal->term = b;
    return true;
  } else {
    List temp = list;
    while (temp != NULL) {
      assert(!queryPN(temp->term, b));
      temp = temp->next;
    }
  }

  // loop through linked list containing universal positives and check if the term is already exists
  // if the term is already there, break out. If not, continue
  while (list != NULL) {
    if (list->term == b) {
      return true;
    }
    list = list->next;
  }

  // the term does not already exist in the list, so add it to the list
  List listToAdd = malloc(sizeof(struct node));
  listToAdd->term = b;
  listToAdd->next = a->inferences->nextPosUniversal;
  a->inferences->nextPosUniversal = listToAdd;
  return true;
}

// add a universal negative
bool assertUN(Term a, Term b) {
  if (a == b)  {
    return false;
  }
  // check if inferences are not currently null and if so, initialize
  checkAndInitialize(a, b);
  /* Check if the logic makes sense for universal negative and if not, exit the program
   * Universal Negative:
   * Two items cannot be in a partial or universal affirmative
  */
  if (queryPA(a, b)) {
    return false;
  }
  //assert(!queryPA(a, b));
    // set the list to the first item if the list is currently null
  List list = a->inferences->nextNegUniversal;
  if (list == NULL) {
    a->inferences->nextNegUniversal = calloc(1, sizeof(struct node));
    a->inferences->nextNegUniversal->term = b;
    return true;
  } else {
      List temp = list;
      while (temp != NULL) {
        assert(!queryPA(temp->term, b));
        temp = temp->next;
      }
  }
  // loop through linked list and check if the term is already exists
  // if the term is already there, break out. If not, continue
  while (list != NULL) {
    if (list->term == b) {
      return true;
    }
    list = list->next;
  }

  // the term does not already exist, so add it to the list
  List listToAdd = malloc(sizeof(struct node));
  listToAdd->term = b;
  listToAdd->next = a->inferences->nextNegUniversal;
  a->inferences->nextNegUniversal = listToAdd;
  return true;
}

// add a particular affirmative
bool assertPA(Term a, Term b) {
  // check and initialize inference structs for A and B
  checkAndInitialize(a, b);

  // For a particular affirmative, a universal negative containing those two cannot exist
  if (queryUN(a, b)) {
    return false;
  }
  assert(!queryUN(a, b));

  // set first item if list is empty
  List list = a->inferences->nextPosParticular;
  if (list == NULL) {
    a->inferences->nextPosParticular = calloc(1, sizeof(struct node));
    a->inferences->nextPosParticular->term = b;
    return true;
  }

  // loop through linked list of partial affirmatives and check if the term is already exists
  // if the term is already there, break out
  while (list != NULL) {
    if (list->term == b) {
      return true;
    }
    list = list->next;
  }

  // the term does not already exist, so add it to the list
  List listToAdd = malloc(sizeof(struct node));
  listToAdd->term = b;
  listToAdd->next = a->inferences->nextPosParticular;
  a->inferences->nextPosParticular = listToAdd;
  return true;
}

// add a particular negative
bool assertPN(Term a, Term b) {
  // initialize inference arrays
  checkAndInitialize(a, b);

  // for a particular negative, a universal affirmative containing the same elements cannot exist
  if (queryUA(a, b)) {
    return false;
  }

  // set first item if list is empty
  List list = a->inferences->nextNegParticular;
  if (list == NULL) {
    a->inferences->nextNegParticular = calloc(1, sizeof(struct node));
    a->inferences->nextNegParticular->term = b;
    return true;
  }

    // loop through linked list of partial negatives and check if the term is already exists
    // if the term is already there, break out
  while (list != NULL) {
    if (list->term == b) {
      return true;
    }
    list = list->next;
  }

  // the term does not already exist, so add it to the list
  List listToAdd = malloc(sizeof(struct node));
  listToAdd->term = b;
  listToAdd->next = a->inferences->nextNegParticular;
  a->inferences->nextNegParticular = listToAdd;
  return true;
}

// Querying premises for the system

// query a universal affirmative
bool queryUA(Term a, Term b) {
  if (a == b) {
    return true;
  }
  // loop through all universal positives and checks if the term is present
  List list = a->inferences->nextPosUniversal;
  while (list != NULL) {
    Term term = list->term;
    if (term == b) {
      return true;
    }
    // loops through all the universal positives of each term, returning if the term is found
    if (term != NULL){
      if (queryUA(term, b)) {
        return true;
      }
    }
    list = list->next;
  }
  return false;
}

// query a universal negative
bool queryUN(Term a, Term b) {
  if (a == b) {
    return false;
  }
  // loops through all universal negatives of a
  List aNegative = a->inferences->nextNegUniversal;
  while (aNegative != NULL) {
    Term term = aNegative->term;
    if (term == b) {
      return true;
    }
    // loops through all universal negatives of each term, returning if the term is found
    if (queryUN(term, b)) {
      return true;
    }
    aNegative = aNegative->next;
  }

/*
  // check if a reverse negative exists
  // loops through all universal negatives of b
  List bNegative = b->inferences->nextNegUniversal;
  while (bNegative != NULL) {
    Term term = bNegative->term;
    if (term == a) {
      return true;
    }
    bNegative = bNegative->next;
  }
*/
  // handle case: if ALL A are C and NO B are C, then no A are B
  // loops through the universal positives and checks if any universal negatives of the sort exist
  List pos = a->inferences->nextPosUniversal;
  while (pos != NULL) {
    Term term = pos->term;
    if (queryUN(b, term)) {
      return true;
    }
    pos = pos->next;
  }
  return false;
}

/*
Fix: - currently bugged
A -> C
C -> A
A ~> B
C ~?>B CRASH
*/

// query a particular affirmative
bool queryPA(Term a, Term b) {
  // checks if universal affiramtives already exist
  /*
  if (queryUA(a, b) || queryUA(b, a)) {
    return true;
  } */
  // loop through particular positives, checking if one already exists.
  // don't need to go further as not universal
  List list = a->inferences->nextPosParticular;
  while (list != NULL) {
    Term term = list->term;
    if (term == b) {
      return true;
    }
    list = list->next;
  }

  // loop through universal positives
  List univ = a->inferences->nextPosUniversal;
  while (univ != NULL) {
    // check if either a universal affirmative already exists or if a particular affirmative exists
    if ((queryPA(univ->term, b)) || queryUA(univ->term, b)) {
      return true;
    }
    univ = univ->next;
  }
  return false;
}

// query a particular negative
bool queryPN(Term a, Term b) {
  if (queryUN(a, b)) {
    return true;
  }
  if (queryUN(b, a)) {
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

    List univ = a->inferences->nextNegUniversal;
    while (univ != NULL) {
      if (queryUN(univ->term, b)) {
        return true;
      }
      if (queryPN(univ->term, b) && !queryPN(a, univ->term)) {
        return true;
      }
      univ = univ->next;
    }
  return false;
}
