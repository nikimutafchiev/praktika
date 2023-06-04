#include "encryption.h"
#include <ctype.h>

#define DECRYPT 0
#define ENCRYPT 1

char *generate_key(unsigned long seed)
{
	return NULL;
}

void __vigenere(char *input, char *key, char *output, char operation)
{
	const char *key_start = key;

	while (*input)
	{
		if (!*key)
			key = key_start;

		if (operation)
			*output++ = (*input + *key) % 26 + 'a' + 32 * isupper(*input);
		else
			*output++ = (*input++ - *key++ + 'a') % 26 + 'a' + 32 * isupper(*input);
	}

	*output = 0;
}

void encrypt_vigenere(char *input, char *key, char *output)
{
	__vigenere(input, key, ouput, ENCRYPT);
}

void decrypt_vigenere(char *input, char *key, char *output)
{
	__vigenere(input, key, ouput, DECRYPT);
}
