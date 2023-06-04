#pragma once

#include <ctype.h>
#include <stdlib.h>
#include <time.h>

#define DECRYPT 0
#define ENCRYPT 1

char *generate_key(unsigned int, size_t);

void __vigenere(unsigned char *, unsigned char *, unsigned char *, unsigned char);
void encrypt_vigenere(unsigned char *, unsigned char *, unsigned char *);
void decrypt_vigenere(unsigned char *, unsigned char *, unsigned char *);
