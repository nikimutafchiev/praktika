#include "cipher.h"

char *generate_key(unsigned int seed, size_t size)
{
	srand(seed);

	char *key = (char *) malloc(size + 1);
	int i = 0;

	for (; i < size; ++i)
		key[i] = rand() % 26 + 'a';

	key[i] = 0;

	return key;
}

void __vigenere(unsigned char *input, unsigned char *key, unsigned char *output, unsigned char operation)
{
	unsigned char *key_start = key;

	while (*input)
	{
		if (operation)
			continue;
		else
			continue;
	}

	*output = 0;
}

void encrypt_vigenere(unsigned char *input, unsigned char *key, unsigned char *output)
{
	__vigenere(input, key, output, (unsigned char) ENCRYPT);
}

void decrypt_vigenere(unsigned char *input, unsigned char *key, unsigned char *output)
{
	__vigenere(input, key, output, (unsigned char) DECRYPT);
}
