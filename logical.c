#include <stdio.h>
#include "logical.h"
#include <stdlib.h>
#include <assert.h>

// makeTerm creates a new term with a given title, callocing the correct amount of space needed.
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
  // loop through list.
  List list = a->inferences->nextPosUniversal;
  if (list == NULL) {
    a->inferences->nextPosUniversal = calloc(1, sizeof(struct node));
    a->inferences->nextPosUniversal->term = b;
    return true;
  } else {
    List temp = list;
    while (temp != NULL) {
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
   * queryPA checks for partial and universal affirmative
   */
  if (queryPA(a, b)) {
    return false;
  }

  // set the list to the first item if the list is currently null
  // Loop through the negative list looking.
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
  assertUN(b, a);
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

  assertPA(b, a);
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

bool inList(List list, Term term) {
  while (list != NULL) {
    if (list->term == term) {
      return true;
    }
    list = list->next;
  }
  return false;
}

// Querying premises for the system

// returns if there is a positive loop in the system
bool inPosUnivLoop(Term a, Term b) {
  List posA = a->inferences->nextPosUniversal;
  List posB = b->inferences->nextPosUniversal;
  return inList(posA, b) && inList(posB, a);
}

bool inPosPartLoop(Term a, Term b) {
  List posA = a->inferences->nextPosParticular;
  List posB = b->inferences->nextPosParticular;
  return inList(posA, b) && inList(posB, a);
}

bool inNegUnivLoop(Term a, Term b) {
  List negA = a->inferences->nextNegUniversal;
  List negB = b->inferences->nextNegUniversal;
  return inList(negA, b) && inList(negB, a);
}

// query a universal affirmative
bool queryUA(Term a, Term b) {
  if (a == b) {
    return true;
  }
  // loop through all universal positives and checks if the term is present.
  List list = a->inferences->nextPosUniversal;
  while (list != NULL) {
    Term term = list->term;
    if (term == b) {
      return true;
    }
    // loops through all the universal positives of each term, returning if the term is found to be a universal afirmative
    if (term != NULL) {
      // if there is currently a recurring loop, skip the current term.
      if (inPosUnivLoop(term, b)) {
        list = list->next;
        continue;
      }
      // recursive call to check for universal affirmatives
      if (queryUA(term, b)) {
        return true;
      }
    }
    list = list->next;
  }
  return false;
}

bool queryUNHelper(Term a, Term b, bool firstTime) {
  if (a == b) return false;

  if (firstTime && queryUNHelper(b, a, false)) return true;
  // loops through all universal negatives of term a
  List aNegative = a->inferences->nextNegUniversal;
  while (aNegative != NULL) {
    Term term = aNegative->term;
    if (term == b) return true; // exact match

    // loops through all universal negatives of each term, returning if the term is a universal negative
    // if it's in an infinite loop, skip the term.
    if (inNegUnivLoop(term, b)) {
      aNegative = aNegative->next;
      continue;
    }
    if (queryUNHelper(b, term, false)) return true;
    aNegative = aNegative->next;
  }

  // handle case: if ALL A are C and NO B are C, then no A are B
  // loops through the universal positives and checks if any universal negatives of the sort exist
  List pos = a->inferences->nextPosUniversal;
  while (pos != NULL) {
    Term term = pos->term;
    if (!firstTime && queryUNHelper(b, term, false)) {
      return true;
    }
    pos = pos->next;
  }
  return false;
}

// query a universal negative
bool queryUN(Term a, Term b) {
  return queryUNHelper(a, b, true);
}

bool queryPAHelper(Term a, Term b, bool firstTime) {
  if (a == b) return true; // if they're equal, then of course it's true

  if (firstTime && queryPAHelper(b, a, false)) return true;
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
  while (univ != NULL && firstTime) {
    // if there is a positive loop, skip the current term. Still need to check though
    if (inPosUnivLoop(univ->term, b)) {
      univ = univ->next;
      continue;
    }
      // check if either a universal affirmative already exists or if a particular affirmative exists
    if (queryPA(univ->term, b)) {
      return true;
    }
    univ = univ->next;
  }
  return false;
}

// query a particular affirmative
bool queryPA(Term a, Term b) {
  return queryPAHelper(a, b, true);
}

// query a particular negative
bool queryPN(Term a, Term b) {
  // if there is a universal negative with either, then it's true.
  if (queryUN(a, b) || queryUN(b, a)) return true;
  // loop through the particular negatives, returning if there is an exact match for particular negatives
  List list = a->inferences->nextNegParticular;
  while (list != NULL) {
    Term term = list->term;
    if (term == b) {
      return true;
    }
    list = list->next;
  }

  // check the universal negatives
  List univNeg = a->inferences->nextNegUniversal;
  while (univNeg != NULL) {
    // if a universal negative exists between the universal negative and the term, then a partial negative exists.
    if (queryUN(univNeg->term, b)) {
      return true;
    }
    univNeg = univNeg->next;
  }
  return false;
}
