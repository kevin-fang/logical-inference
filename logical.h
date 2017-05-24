#include <stdio.h>
#include <stdbool.h>

/* There are four relationships that can occur between two terms:
 * Universal Affirmative: All A are B
 * Universal Negative: No A are B
 * Particular Affirmative: Some A are B
 * Particular Negative: Some A are not B
 */

typedef char* String;
typedef struct term *Term;
typedef struct sets *Set;
typedef struct node *List;

struct term {
   String title;
   Set inferences;
};

struct node {
  Term term;
  List next;
};

struct sets {
  List nextPosUniversal;
  List nextNegUniversal;
  List nextPosParticular;
  List nextNegParticular;
};



// Adding premises to the system:

// add a universal affirmative
void assertUA(Term a, Term b);
// add a universal negative
void assertUN(Term a, Term b);
// add a particular affirmative
void assertPA(Term a, Term b);
// add a particular negative
void assertPN(Term a, Term b);

// Querying permises for the system

// query a universal affirmative
bool queryUA(Term a, Term b);
// query a universal negative
bool queryUN(Term a, Term b);
// query a particular affirmative
bool queryPA(Term a, Term b);
// query a particular negative
bool queryPN(Term a, Term b);
