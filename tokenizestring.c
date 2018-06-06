/*
  tokenizestring.c

  String tokenizer.

  Build by:
  cl -TC tokenizestring.c /link -subsystem:console
  gcc tokenizestring.c

  Written by Ivan Avdonin, <xeus.main@gmail.com>
  Written at 16.04.2018.

  Revision history:
  - Created by Ivan Avdonin, 16.04.2018.
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#ifdef _DEBUG
#define ASSERT(exp) do {if (!(exp)) {static int *breakme; *breakme = 1;}} while(0)
#else
#define ASSERT(exp) do {} while(0)
#endif

char **
TokenizeString(const char *string, int *numtokens, const char *delim)
{
  ASSERT(string);
  ASSERT(delim);

  const char *p = string; // current position

  // iterate to count tokens
  int ntokens = 0;
  bool wasdelim = true;
  while (true) {
    if (strchr(delim, *p) || *p == 0) {
      if (!wasdelim)
	ntokens++;
      wasdelim = true;
    } else {
      wasdelim = false;
    }

    if (*p == 0)
      break;

    p++;
  }

  if (numtokens)
    *numtokens = ntokens;
  if (ntokens == 0)
    return 0;

  // allocate needed space
  char **tokenized = (char **)malloc(sizeof(char *) * ntokens);
  ASSERT(tokenized);

  // iterate all over again to capture tokens
  int i = 0;
  const char *l = string;
  p = string;
  wasdelim = true;
  while (true) {
    if (strchr(delim, *p) || *p == 0) {
      if (!wasdelim) {
	int diff = (int)(p - l);

	tokenized[i] = (char *)malloc(sizeof(char) * (diff + 1));
	ASSERT(tokenized[i]);
	
	strncpy(tokenized[i], l, diff);
	tokenized[i][diff] = 0;

	l = p + 1;
	i++;
      } else {
	l++;
      }

      wasdelim = true;
    } else {
      wasdelim = false;
    }

    if (*p == 0)
      break;

    p++;
  }

  return tokenized;
}

void
FreeTokens(char **tokens, int numtokens)
{
  ASSERT(tokens);

  if (!numtokens)
    return;

  for (int i = 0; i < numtokens; i++)
    free(tokens[i]);
  free(tokens);
}

int
main(int argc, char **argv)
{
  char teststring[] = "This is a test string";

  int numtokens;
  char **tokens = TokenizeString(teststring, &numtokens, " ");
  if (tokens) {
    printf("numtokens: %d\n", numtokens);
    for (int i = 0; i < numtokens; i++)
      printf("tokens[%d]: %s\n", i, tokens[i]);

    FreeTokens(tokens, numtokens);
  }

  return 0;
}
