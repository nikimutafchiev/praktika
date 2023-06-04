#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include "encryption.h"

void create_new_user(char* name, char* password, const char* filename)
{
	FILE* file = fopen(filename, "a");
	if (file == NULL) {
		printf("Failed to open the file: %s\n", filename);
		perror("fopen");
		return;
	}

	fprintf(file, "%s\n%s\n", name, password);
	fclose(file);
}

void add_story(char* name, const char* user, const char* date, const char* story, const char* filename)
{
	FILE* file = fopen(filename, "a");
	if (file == NULL) {
		printf("Failed to open the file: %s\n", filename);
		perror("fopen");
		return;
	}

	fprintf(file, "%s\n%s\n%s\n%s\n", user, date, name, story);
	fclose(file);
}
struct stories
{
	char* name, * date, * user, * story;
};
struct stories_packet
{
	struct stories* *buff;
	int size;
};

struct stories_packet* put_in_structs(const char* filename)
{
	FILE* file = fopen(filename, "r");
	if (file == NULL) {
		printf("Failed to open the file: %s\n", filename);
		perror("fopen");
		return NULL;
	}

	struct stories_packet* packet = malloc(sizeof(struct stories_packet));
	packet->size = 0;
	packet->buff = NULL;
	char* name = malloc(sizeof(char) * 30);  // Replace MAX_LENGTH with the maximum expected length of the string
	char* user = malloc(sizeof(char) * 100);
	char* date = malloc(sizeof(char) * 10);
	char* story = malloc(sizeof(char) * 10000);
	while (fscanf(file, "%[^\n]\n%[^\n]\n%[^\n]\n%[^\n]\n", user, date, name, story) == 4) {
		packet->size++;
		packet->buff = realloc(packet->buff, packet->size * sizeof(struct stories*));
		packet->buff[packet->size - 1] = malloc(sizeof(struct stories));
		packet->buff[packet->size - 1]->date = date;
		packet->buff[packet->size - 1]->user = user;
		packet->buff[packet->size - 1]->name = name;
		packet->buff[packet->size - 1]->story = story;
	}

	fclose(file);
	return packet;
}

int main()
{
	char *msg = "Hello, World!";
	char *buffer[256];

	encrypt_vigenere(msg, "ipvyqbweqsngq", buffer);

	printf("%s", buffer);

	decrypt_vigenere(buffer, "ipvyqbweqsngq", buffer);

	printf("%s", buffer);

	/*create_new_user("Stefan Georgiev", "asdasdasd", "users.txt");
	add_story("How i met your mother", "Josh", "29.2.2022", "uvu vue vue unicue veu veu u sans", "stories.txt");
	add_story("How i met your mother", "Josh", "29.2.2022", "uvu vue vue unicue veu veu u sans", "stories.txt");
	add_story("How i met your mother", "Josh", "29.2.2022", "uvu vue vue unicue veu veu u sans", "stories.txt");
	add_story("How i met your mother", "Josh", "29.2.2022", "uvu vue vue unicue veu veu u sans", "stories.txt");
	put_in_structs("stories.txt");*/

	return 0;
}
