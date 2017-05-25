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

// TODO: implement existence
struct term {
   String title;
   Set inferences;
   bool exists;
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
Term makeTerm(String title);

// add a universal affirmative
bool assertUA(Term a, Term b);
// add a universal negative
bool assertUN(Term a, Term b);
// add a particular affirmative
bool assertPA(Term a, Term b);
// add a particular negative
bool assertPN(Term a, Term b);

// Querying permises for the system

// query a universal affirmative
bool queryUA(Term a, Term b);
// query a universal negative
bool queryUN(Term a, Term b);
// query a particular affirmative
bool queryPA(Term a, Term b);
// query a particular negative
bool queryPN(Term a, Term b);
