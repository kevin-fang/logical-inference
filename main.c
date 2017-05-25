#include <stdio.h>
#include <stdlib.h>
#include "logical.h"
#include <assert.h>
#include <string.h>
#include "test.h"

#define printInstructions() printf("Instructions: \nASSERT ALL A are B, QUERY ALL A are B\nASSERT NO A are B, QUERY NO A are B\nASSERT SOME A are B, QUERY SOME A are B\nASSERT NOTSOME A are B, QUERY NOTSOME A are B\n\n")
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
  word[fourthIndex - index] = '\0';
  return word;
}

String getTrueFalse(bool b) {
  return b ? "True" : "False";
}

Term findInList(List list, String title) {
  while (list != NULL) {
    if (strcmp(list->term->title, title) == 0) {
      return list->term;
    }
    list = list->next;
  }
  return NULL;
}

bool inList(List list, String title) {
  return findInList(list, title) != NULL;
}

#define ALL "ALL"
#define SOME "SOME"
#define NOTSOME "NOTSOME"
#define NO "NO"

int main() {
  testCombined();

	//printf("LOGIC: %d\n", queryUA(cat, kevin));

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
    if (strstr(curLine, "are") == NULL) {
      printf("Missing keyword: are\n");
      printInstructions();
      printf("> ");
      continue;
    }
    //curLine = "ASSERT ALL AB are B";
    if (strncmp(ASSERT, curLine, strlen(ASSERT)) == 0) { // asserting
      String firstWord = getFirstTitle(curLine, ASSERT);
      String keyWord = getKeyword(curLine, ASSERT);
      String secondWord = getSecondTitle(curLine, ASSERT);
			//printf("assert first word:%s, second word:%s, keyword:%s;", firstWord, secondWord, keyWord);
      Term firstTerm;
      Term secondTerm;
      if (!inList(terms, firstWord)) {
        List list = malloc(sizeof(struct node));
        firstTerm = makeTerm(firstWord);
        list->term = firstTerm;
        list->next = terms;
        terms = list;
      } else {
        firstTerm = findInList(terms, firstWord);
      }
      if (!inList(terms, secondWord)) {
        List list = malloc(sizeof(struct node));
        secondTerm = makeTerm(secondWord);
        list->term = secondTerm;
        list->next = terms;
        terms = list;
      } else {
        secondTerm = findInList(terms, secondWord);
      }
      if (strcmp(keyWord, ALL) == 0) {
        if (!assertUA(firstTerm, secondTerm)) {
          printf("Presents a contradiction.\n> ");
					continue;
        };
      } else if (strcmp(keyWord, NO) == 0) {
        if (!assertUN(firstTerm, secondTerm)) {
					printf("Presents a contradiction.\n> ");
					continue;
				}
      } else if (strcmp(keyWord, SOME) == 0) {
        if (!assertPA(firstTerm, secondTerm)) {
					printf("Presents a contradiction\n> ");
					continue;
				}
      } else if (strcmp(keyWord, NOTSOME) == 0) {
        if (!assertPN(firstTerm, secondTerm)) {
					printf("Presents a contradiction.\n> ");
					continue;
				}
      } else {
        printf("Unknown keyword: %s\n", keyWord);
        printInstructions();
        printf("> ");
        continue;
      }

    } else if (strncmp(QUERY, curLine, strlen(QUERY)) == 0) { // querying
      String firstWord = getFirstTitle(curLine, QUERY);
      String keyWord = getKeyword(curLine, QUERY);
      String secondWord = getSecondTitle(curLine, QUERY);

			//printf("query first word:%s, second word:%s, keyword:%s;", firstWord, secondWord, keyWord);
      Term firstTerm;
      Term secondTerm;
      if (!inList(terms, firstWord)) {
        printf("Bad query: %s\n", firstWord);
        printInstructions();
        printf("> ");
        continue;
      } else {
        firstTerm = findInList(terms, firstWord);
      }
      if (!inList(terms, secondWord)) {
        printf("Bad query: %s\n", secondWord);
        printInstructions();
        printf("> ");
        continue;
      } else {
        secondTerm = findInList(terms, secondWord);
      }

      if (strcmp(keyWord, ALL) == 0) {
        printf("%s\n", getTrueFalse(queryUA(firstTerm, secondTerm)));
      } else if (strcmp(keyWord, NO) == 0) {
        printf("%s\n", getTrueFalse(queryUN(firstTerm, secondTerm)));
      } else if (strcmp(keyWord, SOME) == 0) {
        printf("%s\n", getTrueFalse(queryPA(firstTerm, secondTerm)));
      } else if (strcmp(keyWord, NOTSOME) == 0) {
        printf("%s\n", getTrueFalse(queryPN(firstTerm, secondTerm)));
      } else {
        printf("Bad keyword: %s\n", keyWord);
        printInstructions();
        printf("> ");
        continue;
      }
    } else {
      printf("Bad input: %s\n", curLine);
      printInstructions();
      printf("> ");
      continue;
    }
    printf("> ");
  }
}
