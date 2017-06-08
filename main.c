#include <stdio.h>
#include <stdlib.h>
#include "logical.h"
#include <assert.h>
#include <string.h>
#include "test.h"

#define printInstructions() printf("Instructions: \nASSERT ALL A are B, QUERY ALL A are B\nASSERT NO A are B, QUERY NO A are B\nASSERT SOME A are B, QUERY SOME A are B\nASSERT NOTSOME A are B, QUERY NOTSOME A are B\n\n")
#define DEFAULT_ALLOC_SIZE 10

//#define GRN "\x1B[32m"
#define GRN ""
//#define RESET "\x1B[0m"
#define RESET ""
/* There are four relationships that can occur between two terms:
 * Universal Affirmative: All A are B
 * Universal Negative: No A are B
 * Particular Affirmative: Some A are B
 * Particular Negative: Some A are not B
 *
 * Universal is denoted using ->
 * Particular is denoted using ~>
 */

#define ALL "ALL"
#define SOME "SOME"
#define NOTSOME "NOTSOME"
#define NO "NO"

char *getLine() {
	int size = DEFAULT_ALLOC_SIZE;
	String str = malloc(size);
	if (!str) return NULL;

	int i = 0;
	char c;
	while ((c = getchar()) != '\n' && c != EOF) {
		if (i == size - 2) {
			size = size * 2;
			String temp = realloc(str, size);
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

// returns ALL, SOME, NO, or NOTSOME
String getKeyword(String line, String keyWord) {
	String loc;
	int queryLen;
	if ((loc = strstr(line, ALL)) != NULL) {
		queryLen = strlen(ALL);
	} else if ((loc = strstr(line, SOME)) != NULL) {
		queryLen = strlen(SOME);
	} else if ((loc = strstr(line, NO)) != NULL) {
		queryLen = strlen(NO);
	} else if ((loc = strstr(line, NOTSOME)) != NULL) {
		queryLen = strlen(NOTSOME);
	}

	String toReturn = malloc(sizeof(char) * queryLen + 1);
	strncpy(toReturn, loc, queryLen + 1);
	toReturn[queryLen] = '\0';
	return toReturn;
}

// returns the title of the first object
String getFirstTitle(String line, String keyWord) {
	String index = strstr(line, keyWord) + strlen(keyWord) + 1;
	String finishIndex = strstr(line, "are") - 1;
	int diff = finishIndex - index;

	String toReturn = malloc(sizeof(char) * diff + 1);
	strncpy(toReturn, index, diff);
	toReturn[diff] = '\0';
	return toReturn;
}

#define ARE "are"
// returns the title of the second object
String getSecondTitle(String line, String keyWord) {
	String index = strstr(line, ARE) + strlen(ARE) + 1;
	return index;
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

bool stringInList(List list, String title) {
  return findInList(list, title) != NULL;
}

int main() {
	// perform tests
  testCombined();
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
      //printInstructions();
    } else if (strstr(curLine, ASSERT) != NULL) { // asserting
      String keyWord = getKeyword(curLine, ASSERT);
	    String firstWord = getFirstTitle(curLine, keyWord);
      String secondWord = getSecondTitle(curLine, keyWord);
			//printf("assert first word:%s, second word:%s, keyword:%s;", firstWord, secondWord, keyWord);
      Term firstTerm;
      Term secondTerm;
      if (!stringInList(terms, firstWord)) {
        List list = malloc(sizeof(struct node));
        firstTerm = makeTerm(firstWord);
        list->term = firstTerm;
        list->next = terms;
        terms = list;
      } else {
        firstTerm = findInList(terms, firstWord);
      }
      if (!stringInList(terms, secondWord)) {
        List list = malloc(sizeof(struct node));
        secondTerm = makeTerm(secondWord);
        list->term = secondTerm;
        list->next = terms;
        terms = list;
      } else {
        secondTerm = findInList(terms, secondWord);
      }
      if (strstr(curLine, ALL) != NULL) {
        if (!assertUA(firstTerm, secondTerm)) {
          printf("Presents a contradiction.\n");
        };
      } else if (strstr(curLine, NO) != NULL) {
        if (!assertUN(firstTerm, secondTerm)) {
					printf("Presents a contradiction.\n");
				}
      } else if (strstr(keyWord, SOME) != NULL) {
        if (!assertPA(firstTerm, secondTerm)) {
					printf("Presents a contradiction\n");
				}
      } else if (strstr(keyWord, NOTSOME) != NULL) {
        if (!assertPN(firstTerm, secondTerm)) {
					printf("Presents a contradiction.\n");
				}
      } else {
        printf("Unknown keyword: %s\n", keyWord);
        //printInstructions();
      }
    } else if (strstr(curLine, QUERY) != NULL) { // querying
      String keyWord = getKeyword(curLine, QUERY);
			String firstWord = getFirstTitle(curLine, keyWord);
      String secondWord = getSecondTitle(curLine, keyWord);

			//printf("query first word:%s, second word:%s, keyword:%s;", firstWord, secondWord, keyWord);
      Term firstTerm;
      Term secondTerm;
      if (!stringInList(terms, firstWord)) {
        printf("Bad query: %s\n", firstWord);
        //printInstructions();
        printf("> ");
        continue;
      } else {
        firstTerm = findInList(terms, firstWord);
      }
      if (!stringInList(terms, secondWord)) {
        printf("Bad query: %s\n", secondWord);
        //printInstructions();
        printf("> ");
        continue;
      } else {
        secondTerm = findInList(terms, secondWord);
      }

      if (strstr(keyWord, ALL) != NULL) {
        printf("%s\n", getTrueFalse(queryUA(firstTerm, secondTerm)));
      } else if (strstr(keyWord, NO) != NULL) {
        printf("%s\n", getTrueFalse(queryUN(firstTerm, secondTerm)));
      } else if (strstr(keyWord, SOME) != NULL) {
        printf("%s\n", getTrueFalse(queryPA(firstTerm, secondTerm)));
      } else if (strstr(keyWord, NOTSOME) != NULL) {
        printf("%s\n", getTrueFalse(queryPN(firstTerm, secondTerm)));
      } else {
        printf("Bad keyword: %s\n", keyWord);
        //printInstructions();
			}
    } else {
      printf("Bad input: %s\n", curLine);
      //printInstructions();
    }
    printf("> ");
  }
	free(curLine);
}
