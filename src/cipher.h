#pragma once

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_VECTOR 16

void init(const char *);
void push(char *);
void pop(void);
void xor(char *, const char *, size_t);
void __encrypt(char *, const char *, size_t);
void __decrypt(char *, const char *, size_t);
void encrypt(char *, const char *, size_t);
void decrypt(char *, const char *, size_t);

struct cipher_t
{
	char **history;
	size_t len;

} cipher;
