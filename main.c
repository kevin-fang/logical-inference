#include <stdio.h>
#include <stdlib.h>
#include "logical.h"
#include <assert.h>

/* There are four relationships that can occur between two terms:
 * Universal Affirmative: All A are B
 * Universal Negative: No A are B
 * Particular Affirmative: Some A are B
 * Particular Negative: Some A are not B
 */

bool testUA() {
    Term termA = malloc(sizeof(struct term));
    termA->title = "A";
    Term termB = malloc(sizeof(struct term));
    termB->title = "B";
    Term termC = malloc(sizeof(struct term));
    termC->title = "C";

    assertUA(termA, termB);

    assertUA(termB, termC);

    return queryUA(termA, termC);
}

bool testUN() {
  return false;
}

bool testPN() {
  return false;
}

bool testPA() {
  return false;
}

int main() {
  printf("start");
  assert(testUA());
}
