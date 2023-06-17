#pragma once

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define MAX_VECTOR 16

typedef struct cipher_t
{
	char **history;
	size_t len;

} Cipher;

Cipher* init(const char *);
void push(Cipher *, char *);
void pop(Cipher *);
void xor(char *, const char *, size_t);
void __encrypt(char *, const char *, size_t);
void __decrypt(char *, const char *, size_t);
void encrypt(Cipher *, char *, const char *, size_t);
void decrypt(Cipher *, char *, const char *, size_t);
