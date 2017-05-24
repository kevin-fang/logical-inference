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

// test universal affirmatives
void testUA() {
    Term termA = calloc(1, sizeof(struct term));
    termA->title = "A";
    Term termB = calloc(1, sizeof(struct term));
    termB->title = "B";
    Term termC = calloc(1, sizeof(struct term));
    termC->title = "C";
    Term termD = calloc(1, sizeof(struct term));
    termC->title = "D";

    assertUA(termA, termB); // A -> B
    assertUA(termB, termC); // B -> C
    assertUA(termA, termD); // A -> D
    assert(queryUA(termA, termB)); // A -> B
    printf("A -> B good\n");
    assert(queryUA(termB, termC)); // B -> C
    printf("B -> C good\n");
    assert(queryUA(termA, termD)); // A -> D
    printf("A -> D good\n");
    assert(queryUA(termA, termC)); // A -> C
    printf("A -> C good\n");
    assert(!queryUA(termB, termD)); // B -> D
    printf("B -!> D good\n");

    assertUA(termB, termD);
    assert(queryUA(termB, termD));
    printf("B -> D good\n");
    printf("Successful!\n");

}

void testUN() {
}

void testPN() {

}

void testPA() {
  
}

int main() {
  testUA();
}
