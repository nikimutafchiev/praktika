#define _CRT_SECURE_NO_WARNINGS

#include "cipher.h"

void init(const char *iv)
{
	cipher.history = malloc(sizeof(char *));

	cipher.len = 1;

	cipher.history[0] = malloc(MAX_VECTOR + 1);
	strcpy(cipher.history[0], iv);
}

void push(char *iv)
{
	cipher.history = realloc(cipher.history, sizeof(char *) * ++cipher.len);

	cipher.history[cipher.len - 1] = malloc(MAX_VECTOR + 1);
	strcpy(cipher.history[cipher.len - 1], iv);
}

void pop(void)
{
	if (cipher.len == 1)
		return;

	free(cipher.history[cipher.len - 1]);

	cipher.history = realloc(cipher.history, sizeof(char *) * --cipher.len);
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

void encrypt(char *data, const char *key, size_t data_len)
{
	xor(data, cipher.history[cipher.len - 1], data_len);

	__encrypt(data, key, data_len);

	push(data, data_len);
}

void decrypt(char *data, const char *key, size_t data_len)
{
	if (cipher.len == 1)
		return;

	__decrypt(data, key, data_len);

	xor(data, cipher.history[cipher.len - 2], data_len);

	pop();
}

int main()
{
	init("INITIAL_VECTOR_");

	char buffer[256] = "Womp womp!";
	char b[256] = "Hello world";

	encrypt(buffer, "gotin key 123", sizeof buffer);
	encrypt(b, "blah blah", sizeof b);

	decrypt(b, "blah blah", sizeof b);
	decrypt(buffer, "gotin key 123", sizeof buffer);

	printf("%s\n", b);
	printf("%s\n", buffer);

	return 0;
}
