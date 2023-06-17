#define _CRT_SECURE_NO_WARNINGS

#include "cipher.h"

Cipher* init(const char *iv)
{
	Cipher *cbc = (Cipher *) malloc(sizeof (Cipher));

	cbc->len = 1;

	cbc->history = malloc(sizeof(char *));
	cbc->history[0] = malloc(MAX_VECTOR + 1);
	memcpy(cbc->history[0], iv, MAX_VECTOR + 1);

	return cbc;
}

void push(Cipher *cbc, char *iv)
{
	cbc->history = realloc(cbc->history, sizeof(char *) * ++cbc->len);

	cbc->history[cbc->len - 1] = malloc(MAX_VECTOR + 1);
	memcpy(cbc->history[cbc->len - 1], iv, MAX_VECTOR + 1);
}

void pop(Cipher *cbc)
{
	if (cbc->len == 1)
		return;

	free(cbc->history[cbc->len - 1]);

	cbc->history = realloc(cbc->history, sizeof(char *) * --cbc->len);
}

void xor(char *data, const char *vector, size_t data_len)
{
	int i = 0;

	for (; i < data_len && data[i]; ++i)
	{
		data[i] ^= vector[i % MAX_VECTOR];
	}

	data[i] = 0;
}

void __encrypt(char *data, const char *key, size_t data_len)
{
	size_t key_len = strlen(key);
	int i = 0;

	for (; i < data_len && data[i]; ++i)
	{
		data[i] += key[i % key_len];
	}

	data[i] = 0;
}

void __decrypt(char *data, const char *key, size_t data_len)
{
	size_t key_len = strlen(key);
	int i = 0;

	for (; i < data_len && data[i]; ++i)
	{
		data[i] -= key[i % key_len];
	}

	data[i] = 0;
}

void encrypt(Cipher *cbc, char *data, const char *key, size_t data_len)
{
	xor(data, cbc->history[cbc->len - 1], data_len);

	__encrypt(data, key, data_len);

	push(cbc, data);
}

void decrypt(Cipher *cbc, char *data, const char *key, size_t data_len)
{
	if (cbc->len == 1)
		return;

	__decrypt(data, key, data_len);

	xor(data, cbc->history[cbc->len - 2], data_len);

	pop(cbc);
}

int main()
{
	Cipher *cbc = init("*INITIAL_VECTOR*");

	char msg[] = "Hello, World!";

	encrypt(cbc, msg, "coolkey", sizeof msg);

	printf("%s\n", msg);

	decrypt(cbc, msg, "coolkey", sizeof msg);

	printf("%s\n", msg);

	return 0;
}
