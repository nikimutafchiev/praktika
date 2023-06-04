#pragma once

char *generate_key(unsigned long);

void __vigenere(char *, char *, char *, char);
void encrypt_vigenere(char *, char *, char *);
void decrypt_vigenere(char *, char *, char *);
