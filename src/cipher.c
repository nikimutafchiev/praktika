#define _CRT_SECURE_NO_WARNINGS

#include "cipher.h"
#include <stdio.h>
#include <string.h>


CBC init_cbc(const char *iv)
{
	CBC new_cbc = malloc(sizeof *new_cbc);

	new_cbc->head = new_cbc->tail = NULL;
	new_cbc->len = 0;

	push_back(new_cbc, iv);

	cbc_count += 1;

	return new_cbc;
}

// if iv is NULL, create a random one
Block init_block(const char *iv)
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
	
	cbc_count -= 1;
}

void free_block(Block *block)
{
	free((*block)->iv);
	free(*block);
	*block = NULL;
}

void push_front(CBC cbc, const char *iv)
{
	Block new_block = init_block(iv);

	if (cbc->len == 0)
	{
		cbc->head = cbc->tail = new_block;
	}
	else
	{
		cbc->head->prev = new_block;
		new_block->next = cbc->head;
		cbc->head = new_block;
	}

	cbc->len += 1;
}

void push_back(CBC cbc, const char *iv)
{
	Block new_block = init_block(iv);

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

void pop_front(CBC cbc)
{
	if (cbc->len == 0)
		return;

	Block to_del = cbc->head;

	if (cbc->len == 1)
	{
		cbc->head = cbc->tail = NULL;
	}
	else
	{
		cbc->head = cbc->head->next;
	}

	free_block(&to_del);

	cbc->len -= 1;
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

void clear(CBC cbc)  // delete all except block with init vector
{
	Block iter = cbc->head->next;

	while (iter)
	{
		Block to_del = iter;

		iter = iter->next;

		free_block(&to_del);
	}
}

int is_empty(CBC cbc)
{
	return cbc->len == 1;
}

/*
FORMAT:
<num1> <init_vector1> <vector 1> <vector 2> ... <vector N> \n
<num2> <init_vector2> <vector 1> <vector 2> ... <vector N> \n
.
.
.

EX:
1 initial___vector 08dyoiphvaosbvnw 9a0-xsajkbdwrqwb x0-u9fsasplcagbh ...
2 coolvector123123 b0[8ghas[pkdvh=w zupxovg]b[qiwbrt ]a-[9xwqwelmna[n ...
.
.
.
*/
CBC load_cbc(const char *filename, int num)
{
	FILE *file = fopen(filename, "r");
	char buffer[4097];
	
	CBC cbc = init_cbc("-");
	pop_back(cbc);

	while (fgets(buffer, sizeof buffer, file) != NULL)
	{
		if (buffer[0] == '0' + num)  // find row
		{
			char *token = strtok(buffer, " ");
			char iv[VECTOR_LEN + 1];

			while ((token = strtok(NULL, " ")) != NULL)
			{
				memcpy(iv, token, VECTOR_LEN);
				iv[VECTOR_LEN] = 0;

				push_back(cbc, iv);
			}
		}
	}

	fclose(file);

	return cbc;
}

void save_cbc(const char *filename, CBC cbc)
{
	FILE *file = fopen(filename, "a");
	char buffer[4097];
	char *buffer_iter = buffer;

	strcpy(buffer_iter, (char[3]) { cbc_count + '0', ' ', '\0' });
	buffer_iter += 2;

	for (Block iter = cbc->head; iter != NULL; iter = iter->next)
	{
		strncpy(buffer_iter, iter->iv, VECTOR_LEN);
		buffer_iter[VECTOR_LEN] = ' ';
		buffer_iter += VECTOR_LEN + 1;
	}

	buffer_iter[-1] = '\n';
	*buffer_iter = 0;

	fprintf(file, "%s", buffer);
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
	xor(plaintext, cbc->tail->iv, plaintext_len);

	vigenere(plaintext, key, plaintext_len, ENCRYPT);

	push_back(cbc, plaintext);
}


/* IF pop IS 1 LAST BLOCK GETS POPPED */
void decrypt(CBC cbc, char *ciphertext, const char *key, size_t ciphertext_len, char pop)
{
	if (cbc->len == 1)
		return;

	Block remember_block = cbc->tail->prev;

	for (Block iter = cbc->tail; iter != cbc->head; iter = iter->prev)
	{
		if (strncmp(ciphertext, iter->iv, 5) == 0)
		{
			remember_block = iter->prev;
			break;
		}
	}

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
