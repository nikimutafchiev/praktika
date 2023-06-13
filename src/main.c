#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <malloc.h>

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
	char* title, * date, * user, * story;
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
	char* title = malloc(sizeof(char) * 100);  // Replace MAX_LENGTH with the maximum expected length of the string
	char* user = malloc(sizeof(char) * 100);
	char* date = malloc(sizeof(char) * 100);
	char* story = malloc(sizeof(char) * 10000);
	while (fscanf(file, "%[^\n]\n%[^\n]\n%[^\n]\n%[^\n]\n", user, date, title, story) == 4) {
		packet->size++;
		packet->buff = realloc(packet->buff, packet->size * sizeof(struct stories*));
		packet->buff[packet->size - 1] = malloc(sizeof(struct stories)); 
		packet->buff[packet->size - 1]->title = malloc(sizeof(char) * 30);
		packet->buff[packet->size - 1]->user = malloc(sizeof(char) * 100);
		packet->buff[packet->size - 1]->date = malloc(sizeof(char) * 10);
		packet->buff[packet->size - 1]->story = malloc(sizeof(char) * 10000);
		strcpy(packet->buff[packet->size - 1]->date, date);
		strcpy(packet->buff[packet->size - 1]->user, user);
		strcpy(packet->buff[packet->size - 1]->title, title);
		strcpy(packet->buff[packet->size - 1]->story, story);
	}

	fclose(file);
	return packet;
}

int main()
{
	create_new_user("Stefan Georgiev", "asdasdasd", "users.txt");
	add_story("How i met your mother1", "1", "29.2.2022", "1uvu vue vue unicue veu veu u sans", "stories.txt");
	add_story("How i met your mother23", "2", "29.2.2022", "2uvu vue vue unicue veu veu u sans", "stories.txt");
	add_story("How i met your mother3", "3", "29.2.2022", "3uvu vue vue unicue veu veu u sans", "stories.txt");
	add_story("How i met your mother4", "4", "29.2.2022", "4uvu vue vue unicue veu veu u sans", "stories.txt");
	put_in_structs("stories.txt");
	return 0;
}
