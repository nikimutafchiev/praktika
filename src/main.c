#define _CRT_SECURE_NO_WARNINGS
#define PSWRDMAX 20+1
#define USRMAX 50+1
#define TITLEMAX 30+1
#define DATEMAX 10+1
#define STRYMAX 10000+1

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>

void create_new_user(char* name, char* password, const char* filename)
{
	FILE* file = fopen(filename, "a");
	if (file == NULL) 
	{
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
	if (file == NULL) 
	{
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
	struct stories** buff;
	int size,capacity;
};
struct stories_packet *init_packet(size_t capacity) {
	struct stories_packet *p = malloc(sizeof * p);
	p->buff = calloc(capacity, sizeof(struct stories *));
	p->size = 0;
	p->capacity = capacity;
	return p;
}
struct stories *resize_packet(struct stories_packet *p) {
	p->capacity *= 2;
	p->buff = realloc(p->buff, p->capacity * sizeof(struct stories *));
	return p->buff;
}
int hash(size_t size, char *s) {
	return *s % size;
}
void push_in_packet(struct stories_packet *packet, size_t index, struct stories *story) {
	for (size_t i = 0; i < packet->capacity; i++) {
		if (packet->buff[index] == NULL) {
			packet->buff[index] = story;
			packet->size++;
			return;
		}
		index = (index + 1) % packet->capacity;
	}
}
void destroy_packet(struct stories_packet *packet) {
	for (size_t i = 0; i < packet->size; i++) {
		free(packet->buff[i]->date);
		free(packet->buff[i]->user);
		free(packet->buff[i]->story);
		free(packet->buff[i]->title);
		free(packet->buff[i]);
	}
}
struct stories *search_by_date(struct stories_packet *packet, char *date) {
	size_t hash_index = hash(packet->capacity, date);
	for (size_t i = 0; i < packet->capacity&&packet->buff[hash_index]!=NULL; i++) {
		if (!strcmp(packet->buff[hash_index]->date, date))
			return packet->buff[hash_index];
		hash_index = (hash_index + 1) % packet->capacity;
	}
	return NULL;
}
struct stories *search_by_title(struct stories_packet *packet, char *title) {
	size_t hash_index = hash(packet->capacity, title);
	for (size_t i = 0; i < packet->capacity&&packet->buff[hash_index]!=NULL; i++) {
		if (!strcmp(packet->buff[hash_index]->title, title))
			return packet->buff[hash_index];
		hash_index = (hash_index + 1) % packet->capacity;
	}
	return NULL;
}

struct stories_packet **stories_by_user(Cipher*cbc,struct stories_packet *all, char *user) {
	struct stories_packet **stories_of_user = malloc(2*sizeof(struct stories_packet*));
	stories_of_user[0] = init_packet(all->size);
	stories_of_user[1] = init_packet(all->size);
	for (size_t i = 0; i < all->size; i++) {
		if (!strcmp(all->buff[i]->user, user)) {
			decrypt(cbc, all->buff[i]->story, all->buff[i]->date, strlen(all->buff[i]->story));
			push_in_packet(stories_of_user[0], hash(all->size, all->buff[i]->title), all->buff[i]);
		}
	}
	for (size_t i = 0; i < all->size; i++) {
		if (!strcmp(all->buff[i]->user, user))
			push_in_packet(stories_of_user[1], hash(all->size, all->buff[i]->date), all->buff[i]);
	}
	return stories_of_user;
}
struct stories_packet* put_in_structs(const char* filename)
{
	FILE* file = fopen(filename, "r");
	if (file == NULL) 
	{
		printf("Failed to open the file: %s\n", filename);
		perror("fopen");
		return NULL;
	}

	struct stories_packet *packet = init_packet(4);
	char *title = malloc(sizeof(char) * TITLEMAX);  // Replace MAX_LENGTH with the maximum expected length of the string
	char *user = malloc(sizeof(char) * USRMAX);
	char *date = malloc(sizeof(char) * DATEMAX);
	char *story = malloc(sizeof(char) * STRYMAX);
	while (fscanf(file, "%[^\n]\n%[^\n]\n%[^\n]\n%[^\n]\n", user, date, title, story) == 4)
	{
		packet->size++;
		if(packet->size>packet->capacity)
			packet->buff = resize_packet(packet);
		packet->buff[packet->size - 1] = malloc(sizeof(struct stories));
		packet->buff[packet->size - 1]->title = malloc(sizeof(char) * TITLEMAX);
		packet->buff[packet->size - 1]->user = malloc(sizeof(char) * USRMAX);
		packet->buff[packet->size - 1]->date = malloc(sizeof(char) * DATEMAX);
		packet->buff[packet->size - 1]->story = malloc(sizeof(char) * STRYMAX);
		strcpy(packet->buff[packet->size - 1]->date, date);
		strcpy(packet->buff[packet->size - 1]->user, user);
		strcpy(packet->buff[packet->size - 1]->title, title);
		strcpy(packet->buff[packet->size - 1]->story, story);
	}

	fclose(file);
	return packet;
}


int validate_user(const char* username, const char* password, const char* filename)
{
	FILE* file = fopen(filename, "r");
	if (file == NULL) 
	{
		printf("Failed to open the file: %s\n", filename);
		perror("fopen");
		return NULL;
	}

	char file_username[USRMAX], file_password[PSWRDMAX];
	while (fscanf(file, "%s\n%s\n", file_username, file_password) == 2) 
	{
		if (strcmp(file_username, username) == 0 && strcmp(file_password, password) == 0) 
		{
			fclose(file);
			return 1;  
		}
	}

	fclose(file);
	return 0;  
}

int print_story(const char* name, const char* date, const char* filename)
{
	FILE* file = fopen(filename, "r");
	if (file == NULL) 
	{
		printf("Failed to open the file: %s\n", filename);
		perror("fopen");
		return NULL;
	}

	char name[TITLEMAX], date[DATEMAX], story[STRYMAX];
	while (fscanf(file, "%[^\n]\n%[^\n]\n%[^\n]\n", name, date, story) == 3) 
	{
		if (strcmp(name, name) == 0 && strcmp(date, date) == 0) 
		{
			printf("\nStory: %s\n", story);
			fclose(file);
			return;
		}
	}

	printf("\nStory not found\n");
	fclose(file);
}


void sign_up()
{
	char new_username[USRMAX], new_password[PSWRDMAX];
	do
	{
		printf("\n\nEnter username(50 chars limit): "); scanf("%s", new_username);
		printf("\nEnter password(20 chars limit): "); scanf("%s", new_password);
	} while (strlen(new_username) > USRMAX || strlen(new_password) > PSWRDMAX || strcmp(new_password, "") || strcmp(new_username, ""));
	create_new_user(new_username, new_password, "users.txt");
	printf("The user was successfully created");
}

void log_in()
{
	char choice[20];
	char input_pswrd[PSWRDMAX], input_username[USRMAX];
	char story[STRYMAX], name[USRMAX], date[10];

	do
	{
		printf("\nEnter username: "); scanf("%s", input_username);
		printf("\nEnter password: "); scanf("%s", input_pswrd);
	} while (!validate_user(input_username, input_pswrd, "users.txt"));
	do
	{
		printf("\n1.Write story \n2.View stories \nExit"); scanf("%s", choice);
		if (strcmp(choice, "1"))
		{
			do
			{
				printf("\nEnter title of the story: "); scanf("%s", name);
				printf("\nEnter date that you wanna set: "); scanf("%s", date);
				printf("\nNow write your story: \n");  scanf("%s", story);
			} while (strlen(name) > USRMAX || strlen(date) > 10 || strlen(story) > STRYMAX);
			add_story(name, input_username, date, story, "stories.txt");
			printf("The story was written successfully");
		}
		if (strcmp(choice, "2"))
		{
			struct stories_packet* packet = put_in_structs("stories.txt");
			for (int i = 0; i < packet->size; i++)
				printf("Title: %s\t\tDate: %s\n\n", packet->buff[i]->title, packet->buff[i]->date);
			printf("\nEnter date: "); scanf("%s", date);
			printf("\nEnter title: "); scanf("%s", date);
			print_story(name, date, "stories.txt");
		}
	} while (!strcmp(choice, "Exit") || !strcmp(choice, "exit"));
}

int main()
{
	char* buffer;
	char choice[10];

	while ((!strcmp(choice, "Exit") || !strcmp(choice, "exit")))
	{
		printf("\nLog in or Sign up"); scanf("%s", choice);
		if (!strcmp(choice, "sign up") || !strcmp(choice, "Sign up"))
			sign_up();
		else if (!strcmp(choice, "log in") || !strcmp(choice, "Log in"))
			log_in();
		else
			printf("Invalid input");
	}

	return 0;
}
