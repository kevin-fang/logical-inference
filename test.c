#include <stdio.h>
#include "logical.h"
#include <assert.h>
#include "test.h"

//#define GRN "\x1B[32m"
#define GRN ""
//#define RESET "\x1B[0m"
#define RESET ""

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
    //printf("A -> B\n");
    assert(!queryUN(termA, termB));
    //printf("not A -!> B\n");
    assert(queryUA(termB, termC)); // B -> C
    //printf("B -> C\n");
    assert(queryUA(termA, termD)); // A -> D
    //printf("A -> D\n");
    assert(queryUA(termA, termC)); // A -> C
    //printf("A -> C\n");
    assert(!queryUA(termB, termD)); // B -> D
    //printf("B -!> D\n");

    assertUA(termB, termD);
    assert(queryUA(termB, termD));
    //printf("B -> D\n");
    printf(GRN "UA Successful!\n" RESET);

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
    //printf("A -> B\n");
    assert(queryUN(termB, termC)); // B -> C
    //printf("B -> C\n");
    assert(queryUN(termA, termD)); // A -> D
    //printf("A -> D\n");
    assert(queryUN(termA, termC)); // A -> C
    //printf("A -> C\n");
    assert(!queryUN(termB, termD)); // B -> D
    //printf("B -!> D\n");

    assertUA(termB, termD);
    assert(queryUA(termB, termD));
    //printf("B -> D\n");
    printf(GRN "UN successful!\n" RESET);
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
    //printf("A ~> B \n");
    assert(!queryUA(termA, termC));
    //printf("A -!>C \n");
    assert(!queryPA(termB, termD));
    //printf("B ~!> D \n");
    assert(!queryPA(termA, termC));
    //printf("A ~!> C \n");
    printf(GRN "PA successful!\n" RESET);
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
    //printf("A ~!> B \n");
    assert(!queryUN(termA, termC));
    //printf("A -!>C \n");
    assert(!queryPN(termB, termD));
    //printf("B ~!> D \n");
    assert(!queryPN(termA, termC));
    //printf("A ~!> C \n");
    assert(!queryPN(termB, termD));
    //printf("A ~!> B\n");
    printf(GRN "PN successful!\n" RESET);
}

void testCombined() {
  testUA();
  testUN();
  testPA();
  testPN();
  Term human = makeTerm("Human");
  Term animal = makeTerm("Animal");
  Term mammal = makeTerm("Mammal");
  Term reptile = makeTerm("Reptile");
  Term bird = makeTerm("Bird");
  Term villain = makeTerm("Villain");

  assertUA(mammal, animal); // all mammals are animals
  assertUA(human, mammal); // all humans are mammals
  assertUN(mammal, reptile); // no mammals are reptiles
  assertUA(bird, animal); // all birds are animals

  assert(!queryUN(reptile, mammal)); // assert that no reptiles are mammals
  //assertUA(human, reptile); // should not happen -- assertion error should occur
  assert(queryUA(human, animal)); // some humans are animals
  assert(!queryUA(human, bird)); // not all humans are birds - not enough information
  assert(!queryUA(bird, mammal)); // not all birds are mammals - not enough inforamtion

  assertPA(villain, human); // some villains are human

  assert(queryPA(villain, human)); // assert that some villains are human
  assert(!queryUA(villain, human)); // not all villains are human
  assert(!queryUN(villain, human)); // not all villains are not human
  assert(!queryPN(villain, human)); // not some villains are not human (some villains are human)

  // famous example
  Term mortal = makeTerm("Mortal");
  Term greek = makeTerm("Greek");
  assertUA(human, mortal); // all humans are mortal
  assertUA(greek, human); // all greeks are human
  assert(queryUA(greek, mortal)); // therefore, all greeks are mortal
  assert(!queryUA(human, greek)); // not all humans are greek

  // another good example
  Term apple = makeTerm("Apple");
  Term banana = makeTerm("Banana");
  Term fruit = makeTerm("Fruit");
  Term vegetable = makeTerm("Vegetable");
  assertUN(fruit, vegetable);
  assertUA(apple, fruit); // all apples are fruit
  assertUA(banana, fruit); // all bananas are fruit
  assert(!queryPA(apple, banana)); // that doesn't mean some apples are bananas
  assert(!queryUA(apple, banana)); // therefore, falsely, all apples are bananas
  // assertUA(apple, vegetable); -> this should cause an assertion error as it's false.

  Term orange = makeTerm("Orange");
  Term rottenFruit = makeTerm("Rotten Fruit");
  Term book = makeTerm("Book");
  Term rottenApple = makeTerm("Rotten apple");
  assertPN(orange, rottenFruit); // some oranges are rotten fruit
  assertUN(book, rottenFruit); // no books are rotten fruit
  assertUA(rottenApple, rottenFruit); // rotten apples are rotten fruit

  // no books are rotten apples,
  assert(queryUN(rottenApple, book));
  assert(!queryUN(rottenApple, orange));



  printf(GRN "Combined test successful\n" RESET);
}
