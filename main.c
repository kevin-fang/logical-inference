#include <stdio.h>
#include <stdlib.h>
#include "logical.h"
#include <assert.h>
#include <string.h>

#define printInstructions() printf("Instructions: \nASSERT ALL A are B, QUERY ALL A are B\nASSERT NO A are B, QUERY NO A are B\nASSERT SOME A are B, QUERY SOME A are B\nASSERT NOTSOME A are B, QUERY NOTSOME A are B\n")
#define DEFAULT_ALLOC_SIZE 10

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
  assert(!queryUA(apple, banana)); // therefore, falsely, all apples are bananas
  // assertUA(apple, vegetable); -> this should cause an assertion error as it's false.

  printf(GRN "Combined test successful\n" RESET);
}

char *getLine() {
	int size = DEFAULT_ALLOC_SIZE;
	char *str = malloc(size);
	if (!str) return NULL;

	int i = 0;
	char c;
	while ((c = getchar()) != '\n' && c != EOF) {
		if (i == size - 2) {
			size = size * 2;
			char *temp = realloc(str, size);
			if (!temp) {
				*str = '\0';
				return str;
			}
			else {
				str = temp;
			}
		}
		str[i] = c;
		i++;
	}
	str[i] = '\0';
    if (i == 0) {
        free(str);
        str = NULL;
    }
    return str;
}

#define ASSERT "ASSERT"
#define QUERY "QUERY"
#define ALL "ALL"
#define SOME "SOME"
#define NOTSOME "NOTSOME"
#define NO "NO"

// returns ALL, SOME, or NO
String getKeyword(String line, String query) {
  line = line + strlen(query) + 1;
  int index = strchr(line, ' ') - line + 1;
  String word = malloc(sizeof(char) * index + 1);
  strncpy(word, line, index - 1);
  word[index] = '\0';
  return word;
}

// returns the title of the first object
String getFirstTitle(String line, String query) {
  line = line + strlen(query) + 1;
  int index = strchr(line, ' ') - line + 1;
  int secondIndex = strchr(line + index + 1, ' ') - line;
  String word = malloc(sizeof(char) * (secondIndex - index) + 1);
  strncpy(word, line + index, secondIndex - index);
  word[secondIndex - index] = '\0';
  return word;
}

// returns the title of the second object
String getSecondTitle(String line, String query) {
  line = line + strlen(query) + 1;
  int index = strchr(line, ' ') - line + 1;
  int secondIndex = strchr(line + index + 1, ' ') - line;
  int thirdIndex = strchr(line + secondIndex + 1, ' ') - line;
  int fourthIndex = strchr(line + thirdIndex, ' ') - line;
  String word = malloc(sizeof(char) * (fourthIndex - index) + 1);
  strcpy(word, line + fourthIndex + 1);
  word[fourthIndex] = '\0';
  return word;
}

bool inList(List list, String title) {
  while (list != NULL) {
    if (strcmp(list->term->title, title) == 0) {
      return true;
    }
    list = list->next;
  }
  return false;
}
// TODO: allow input from user
int main() {
  /*
  testUA();
  testUN();
  testPA();
  testPN();
  testCombined();
  printf(GRN "\nEverything Successful!\n" RESET); */

  printInstructions();
  printf("> ");
  String curLine;
  /* Instructions for use:
   * ASSERT ALL A are B, QUERY ALL A are B, meaning all A are B
   * ASSERT NO A are B, QUERY NO A are B, meaning no A are B
   * ASSERT SOME A are B, QUERY SOME A are B, meaning some A are B
   * ASSERT NOTSOME A are B, QUERY NOTSOME A are B, meaning some A are not B
   */

  // list of created terms
  List terms = NULL;
  while ((curLine = getLine()) != NULL) {
    //printf("%s\n", curLine);
    //curLine = "ASSERT ALL AB are B";
    if (strncmp(ASSERT, curLine, strlen(ASSERT)) == 0) { // asserting
      String firstWord = getFirstTitle(curLine, ASSERT);
      String keyWord = getKeyword(curLine, ASSERT);
      String secondWord = getSecondTitle(curLine, ASSERT);
      printf("keyword: %s, firstword: %s, second: %s\n", keyWord, firstWord, secondWord);
      if (!inList(terms, firstWord)) {

      }
      if (!inList(terms, secondWord)) {
        
      }
    } else if (strncmp(QUERY, curLine, strlen(QUERY)) == 0) { // querying
      String firstWord = getFirstTitle(curLine, QUERY);
      String keyWord = getKeyword(curLine, QUERY);
      String secondWord = getSecondTitle(curLine, QUERY);
      printf("keyword: %s, firstword: %s, second: %s\n", keyWord, firstWord, secondWord);
    } else {
      printInstructions();
    }

    printf("> ");
  }
}
