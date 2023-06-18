#pragma once

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define LEN(x) ( sizeof (x) / sizeof (*x) )
#define DEFAULT_FILENAME "cbc.txt"
#define VECTOR_LEN 16  // in bytes
#define ENCRYPT 1
#define DECRYPT 0
#define POP_LAST 1

typedef struct block_t* Block;
typedef struct cbc_t *CBC;

struct block_t
{
	char *iv;

	Block next;
	Block prev;
};

struct cbc_t
{
	Block head;
	Block tail;
	size_t len;
};

/*
         always here                   essential part

          ___/\___        ___________________/\__________________
         /        \      /                                       \
          ________        ________        ________        ________
         |        |      |        |      |        |      |        |
         |   IV   |      | OWN_IV |      | OWN_IV |      | OWN_IV |
         |  NEXT  | ---> |  NEXT  | ---> |  NEXT  | ---> |  NEXT  | --> 0
   0 <-- |  PREV  | <--- |  PREV  | <--- |  PREV  | <--- |  PREV  |
         |________|      |________|      |________|      |________|
*/

static int cbc_count = 0;

CBC init_cbc(const char *);
Block init_block(const char *);

void free_cbc(CBC);
void free_block(Block);

void push_front(CBC, const char *);
void push_back(CBC, const char *);
void pop_front(CBC);
void pop_back(CBC);
void clear(CBC);

int is_empty(CBC);

CBC load_cbc(const char *, int);  // from file
void save_cbc(const char *, CBC);  // to file

char *xor(char *, char *, size_t);
char *vigenere(char *, const char *, size_t, char);
void encrypt(CBC, char *, const char *, size_t);
void decrypt(CBC, char *, const char *, size_t, char);

/* ----- UTILITY ----- */
char *strndup(const char *, int);
char *random_str(size_t);
