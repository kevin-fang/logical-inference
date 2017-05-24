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
    //assertUN(termA, termB); -> this should cause an assertion error, as it's a contradiction 
    assert(queryUA(termA, termB)); // A -> B
    printf("A -> B\n");
    assert(!queryUN(termA, termB));
    printf("not A -!> B\n");
    assert(queryUA(termB, termC)); // B -> C
    printf("B -> C\n");
    assert(queryUA(termA, termD)); // A -> D
    printf("A -> D\n");
    assert(queryUA(termA, termC)); // A -> C
    printf("A -> C\n");
    assert(!queryUA(termB, termD)); // B -> D
    printf("B -!> D\n");

    assertUA(termB, termD);
    assert(queryUA(termB, termD));
    printf("B -> D good\n");
    printf("UA Successful!\n\n");

}

// test universal negatives
void testUN() {
    Term termA = calloc(1, sizeof(struct term));
    termA->title = "A";
    Term termB = calloc(1, sizeof(struct term));
    termB->title = "B";
    Term termC = calloc(1, sizeof(struct term));
    termC->title = "C";
    Term termD = calloc(1, sizeof(struct term));
    termC->title = "D";

    assertUN(termA, termB); // A -!> B
    assertUN(termB, termC); // B -!> C
    assertUN(termA, termD); // A -!> D
    //assertUA(termA, termB); -> this should cause an assertion error, as it's a contradiction 
    assert(queryUN(termA, termB)); // A -> B
    printf("A -> B good\n");
    assert(queryUN(termB, termC)); // B -> C
    printf("B -> C good\n");
    assert(queryUN(termA, termD)); // A -> D
    printf("A -> D good\n");
    assert(queryUN(termA, termC)); // A -> C
    printf("A -> C good\n");
    assert(!queryUN(termB, termD)); // B -> D
    printf("B -!> D good\n");

    assertUA(termB, termD);
    assert(queryUA(termB, termD));
    printf("B -> D good\n");
    printf("UN successful!\n\n");
}

void testPN() {

}

void testPA() {
  
}

// TODO: implement particulars, allow input from user
int main() {
  testUA();
  testUN();
  printf("Successful!");
}
