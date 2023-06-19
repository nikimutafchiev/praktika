#define _CRT_SECURE_NO_WARNINGS

#include "cipher.h"
#include <stdio.h>
#include <string.h>


CBC init_cbc(const char *iv)
{
	CBC new_cbc = malloc(sizeof *new_cbc);

	new_cbc->head = new_cbc->tail = NULL;
	new_cbc->len = 0;

	//push_back(new_cbc, iv, 0);

	return new_cbc;
}

// if iv is NULL, create a random one
Block init_block(const char *iv, size_t data_len)
{
	Block new_block = malloc(sizeof * new_block);

	if (iv == NULL)
	{
		new_block->iv = random_str(VECTOR_LEN);
	}
	else
	{
		new_block->iv = strndup(iv, VECTOR_LEN);
	}

	new_block->data_len = data_len;
	new_block->next = new_block->prev = NULL;

	return new_block;
}

void free_cbc(CBC *cbc)
{
	Block iter = (*cbc)->head;

	while (iter)
	{
		Block to_del = iter;

		iter = iter->next;

		free_block(&to_del);
	}

	free(*cbc);
	*cbc = NULL;
}

void free_block(Block *block)
{
	free((*block)->iv);
	free(*block);
	*block = NULL;
}

void push_back(CBC cbc, const char *iv, size_t data_len)
{
	Block new_block = init_block(iv, data_len);

	if (cbc->len == 0)
	{
		cbc->head = cbc->tail = new_block;
	}
	else
	{
		cbc->tail->next = new_block;
		new_block->prev = cbc->tail;

		cbc->tail = new_block;
	}

	cbc->len += 1;
}

void pop_back(CBC cbc)
{
	if (cbc->len == 0)
		return;

	Block to_del = cbc->tail;

	if (cbc->len == 1)
	{
		cbc->head = cbc->tail = NULL;
	}
	else
	{
		cbc->tail = cbc->tail->prev;
	}

	free_block(&to_del);

	cbc->len -= 1;
}

size_t size_from_iv(CBC cbc, const char *iv)
{
	for (Block iter = cbc->head; iter; iter = iter->next)
	{
		if (strncmp(iter->iv, iv, 5))
			return iter->data_len;
	}
}

CBC load_cbc(const char *filename)
{
	FILE *file = fopen(filename, "r");
	char buffer[VECTOR_LEN + 1];
	int temp;
	CBC cbc;
	strcpy(buffer, "initial___vector");
	//fscanf(file, "%s", buffer);

	cbc = init_cbc(buffer);

	while (fscanf(file, "%s%d", buffer, &temp)==2)
	{
		push_back(cbc, buffer, temp);
	}

	fclose(file);

	return cbc;
}

void save_cbc(const char *filename, CBC cbc)
{
	FILE *file = fopen(filename, "w");
	Block iter = cbc->head;

	while (iter != NULL) {
		fprintf(file, "%s %d ", iter->iv, iter->data_len);
		iter = iter->next;
	}

	fprintf(file, "\n");

	fclose(file);

	free_cbc(&cbc);
}

char *xor(char *data, char *vector, size_t data_len)
{
	int i = 0;

	for (; i < data_len; ++i)  // was  i < data_len && data[i]
	{
		data[i] ^= vector[i % VECTOR_LEN];
	}

	data[i] = '\0';

	return data;
}

char *vigenere(char *data, const char *key, size_t data_len, char op)
{
	size_t key_len = strlen(key);
	int i = 0;

	for (; i < data_len; ++i)  // was  i < data_len && data[i]
	{
		if (op == ENCRYPT)
		{
			data[i] += key[i % key_len];
		}
		else if (op == DECRYPT)
		{
			data[i] -= key[i % key_len];
		}
	}

	data[i] = 0;

	return data;
}

void encrypt(CBC cbc, char *plaintext, const char *key, size_t plaintext_len)
{
	int ok = 0;
	if (cbc->len == 0) {
		push_back(cbc, "initial___vector", plaintext_len);
		ok = 1;
	}
	

	xor(plaintext, cbc->tail->iv, plaintext_len);

	vigenere(plaintext, key, plaintext_len, ENCRYPT);


	if (!ok) {
		push_back(cbc, plaintext, plaintext_len);
		cbc->tail->data_len = plaintext_len;
	}
}


/* IF pop IS 1 LAST BLOCK GETS POPPED */
void decrypt(CBC cbc, char *ciphertext, const char *key, size_t ciphertext_len, char pop)
{
	if (cbc->len == 0)
		return;

	Block remember_block = cbc->tail->prev;

	for (Block iter = cbc->tail; iter != NULL; iter = iter->prev)
	{
		if (strncmp(ciphertext, iter->iv, 5) == 0)
		{
			remember_block = iter;
			break;
		}
	}
	if (remember_block == NULL)
		remember_block = cbc->tail;
	vigenere(ciphertext, key, ciphertext_len, DECRYPT);

	xor(ciphertext, remember_block->iv, ciphertext_len);

	if (pop == POP_LAST)
		pop_back(cbc);
}

/* ----- UTILITY ----- */
char *strndup(const char *string, int count)
{
	char *temp = malloc(count + 1);
	
	memcpy(temp, string, count);
	temp[count] = 0;

	return temp;
}

// allocates memory !
char *random_str(size_t len)
{
	srand(time(NULL));

	char *new_str = malloc(len + 1);

	for (int i = 0; i < len; ++i)
		new_str[i] = rand() % 27 + 97;  // random printable ascii char [32; 127]

	new_str[len] = 0;

	return new_str;
}
