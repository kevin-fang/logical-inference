#include <stdio.h>
#include <stdlib.h>
#include "logical.h"
#include <assert.h>

#define GRN "\x1B[32m"
#define RESET "\x1B[0m"
/* There are four relationships that can occur between two terms:
 * Universal Affirmative: All A are B
 * Universal Negative: No A are B
 * Particular Affirmative: Some A are B
 * Particular Negative: Some A are not B
 *
 * Universal is denoted using ->
 * Particular is denoted using ~>
 */

// test universal affirmatives
void testUA() {
    Term termA = makeTerm("A");
    Term termB = makeTerm("B");
    Term termC = makeTerm("C");
    Term termD = makeTerm("D");

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
    printf(GRN "UA Successful!\n\n" RESET);

}

// test universal negatives
void testUN() {
    Term termA = makeTerm("A");
    Term termB = makeTerm("B");
    Term termC = makeTerm("C");
    Term termD = makeTerm("D");

    assertUN(termA, termB); // A -!> B
    assertUN(termB, termC); // B -!> C
    assertUN(termA, termD); // A -!> D
    //assertUA(termA, termB); -> this should cause an assertion error, as it's a contradiction
    assert(queryUN(termA, termB)); // A -> B
    printf("A -> B\n");
    assert(queryUN(termB, termC)); // B -> C
    printf("B -> C\n");
    assert(queryUN(termA, termD)); // A -> D
    printf("A -> D\n");
    assert(queryUN(termA, termC)); // A -> C
    printf("A -> C\n");
    assert(!queryUN(termB, termD)); // B -> D
    printf("B -!> D\n");

    assertUA(termB, termD);
    assert(queryUA(termB, termD));
    printf("B -> D\n");
    printf(GRN "UN successful!\n\n" RESET);
}

void testPA() {
    Term termA = makeTerm("A");
    Term termB = makeTerm("B");
    Term termC = makeTerm("C");
    Term termD = makeTerm("D");

    // some A are B, some B are C, some A are D.
    // therefore, some A are B, not all A are C, not all B are D, not all A are C
    assertPA(termA, termB);
    assertPA(termB, termC);
    assertPA(termA, termD);
    assert(queryPA(termA, termB));
    printf("A ~> B \n");
    assert(!queryUA(termA, termC));
    printf("A -!>C \n");
    assert(!queryPA(termB, termD));
    printf("B ~!> D \n");
    assert(!queryPA(termA, termC));
    printf("A ~!> C \n");
    printf(GRN "PA successful!\n\n" RESET);
}

void testPN() {
    Term termA = makeTerm("A");
    Term termB = makeTerm("B");
    Term termC = makeTerm("C");
    Term termD = makeTerm("D");

    // some A are not B, some B are not C, some A are not D
    // therefore some A are not B, not all A are C, some B are not D, some A are not C
    assertPN(termA, termB);
    assertPN(termB, termC);
    assertPN(termA, termD);
    assert(queryPN(termA, termB));
    printf("A ~!> B \n");
    assert(!queryUN(termA, termC));
    printf("A -!>C \n");
    assert(!queryPN(termB, termD));
    printf("B ~!> D \n");
    assert(!queryPN(termA, termC));
    printf("A ~!> C \n");
    assert(!queryPN(termB, termD));
    printf("A ~!> B\n");
    printf(GRN "PN successful!\n\n" RESET);
}

// TODO: allow input from user
int main() {
  testUA();
  testUN();
  testPA();
  testPN();
  printf(GRN "Everything Successful!\n" RESET);
}
