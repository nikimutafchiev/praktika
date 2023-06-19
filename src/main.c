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
#include <ctype.h>
#include "stories.h"
#include "cipher.h"


void create_new_user(char *name, char *password, const char *filename)
{
	FILE *file = fopen(filename, "a");
	if (file == NULL)
	{
		printf("Failed to open the file: %s\n", filename);
		perror("fopen");
		return;
	}

	fprintf(file, "%s\n%s\n", name, password);
	fclose(file);
}

void add_story(char *title, const char *user, const char *date, const char *story, const char *filename, char *password, CBC cbc)
{
	FILE *file = fopen(filename, "a");
	if (file == NULL)
	{
		printf("Failed to open the file: %s\n", filename);
		perror("fopen");
		return;
	}

	encrypt(cbc, story, password, strlen(story));

	fprintf(file, "%s\n%s\n%s\n%s\n", user, date, title, story);
	fclose(file);
}

struct stories_packet *put_in_structs(const char *filename, char *password, CBC cbc)
{
	FILE *file = fopen(filename, "r");
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
		size_t size = size_from_iv(cbc, story);
		decrypt(cbc, story, password, size, 0);
		story[size] = 0;
		packet->size++;
		if (packet->size > packet->capacity)
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

	free(title);
	free(user);
	free(date);
	free(story);

	fclose(file);
	return packet;
}


int validate_user(const char *username, const char *password, const char *filename)
{
	FILE *file = fopen(filename, "r");
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

void print_story(const char *title, const char *date, struct stories_packet *packet)
{
	struct stories *found_story = NULL;

	if (*title != NULL)
		found_story = search_by_title(packet, title);
	else if (*date != NULL)
		found_story = search_by_date(packet, date);

	if (found_story != NULL)
		printf("\nStory:\n%s\n\n", found_story->story);
	else
		printf("\nStory not found\n");
}

void lowercase_words(char *str)
{
	for (int i = 0; str[i]; i++)
		str[i] = tolower(str[i]);
}

void sign_up()
{
	char new_username[USRMAX], new_password[PSWRDMAX];
	do
	{
		printf("\n\nEnter username(50 chars limit): "); gets(new_username);
		printf("\nEnter password(20 chars limit): "); gets(new_password);
	} while (!strlen(new_username) > USRMAX || !strlen(new_password) > PSWRDMAX || !strcmp(new_password, "") || !strcmp(new_username, ""));
	create_new_user(new_username, new_password, "users.txt");
	printf("\nThe user was successfully created");
}

void log_in(CBC cbc)
{
	int k = 10, i = 0;
	char choice[20], date_title_choice[20], view_choice[20];
	char input_pswrd[PSWRDMAX], input_username[USRMAX];
	char story[STRYMAX], name[USRMAX], date[DATEMAX];

	do
	{
		printf("\nEnter username: "); gets(input_username);
		if (!strcmp(input_username, "exit"))
			goto end;
		printf("\nEnter password: "); gets(input_pswrd);
		
	} while (!validate_user(input_username, input_pswrd, "users.txt"));

	do
	{
		printf("\n1.Write story \n2.View stories \nExit: \n"); gets(choice);
		lowercase_words(choice);
		if (!strcmp(choice, "1"))
		{
			do
			{
				printf("\nEnter title of the story: "); gets(name);
				printf("\nEnter date that you wanna set: "); gets(date);
				printf("\nNow write your story: \n");  gets(story);
			} while (strlen(name) > USRMAX || strlen(date) > DATEMAX || strlen(story) > STRYMAX);
			add_story(name, input_username, date, story, "stories.txt", input_pswrd, cbc);
			printf("\nThe story was written successfully");
		}
		if (!strcmp(choice, "2"))
		{
			struct stories_packet **user_stories = stories_by_user(put_in_structs("stories.txt", input_pswrd, cbc), input_username);
			struct stories_packet *title_stories = user_stories[0];
			struct stories_packet *date_stories = user_stories[1];

			i = 0;
			k = 10;
			do
			{
				if (k > date_stories->size)
					k = date_stories->size;

				for (; i < k; i++) {
					if(date_stories->buff[i]!=NULL)
						printf("\nTitle: %s\t\tDate: %s", date_stories->buff[i]->title, date_stories->buff[i]->date);
				}

				printf("\nTo view more stories - view more, to view all stories - view all or exit for exit: "); gets(view_choice);
				lowercase_words(view_choice);
				if (!strcmp(view_choice, "view more"))
				{
					k += 10;
					if (k > date_stories->size)
						k = date_stories->size;
				}
				if (!strcmp(view_choice, "view all"))
					k = date_stories->size;
			} while (strcmp(view_choice, "exit"));

			printf("\nChoose to search for the stories by title or by date: \n"); gets(date_title_choice);
			lowercase_words(date_title_choice);

			if (!strcmp(date_title_choice, "date"))
			{
				*name = NULL;
				printf("\nEnter date: "); gets(date);
				print_story(name, date, date_stories);
			}
			if (!strcmp(date_title_choice, "title"))
			{
				*date = NULL;
				printf("\nEnter title: "); gets(name);
				print_story(name, date, title_stories);
			}

			destroy_packet(date_stories);
			free(user_stories);
		}
	} while (strcmp(choice, "exit"));
end:;
}

int main()
{
	CBC cbc = load_cbc(DEFAULT_FILENAME, 1);

	char choice[10];

	do
	{
		printf("\nLog in or Sign up: "); gets(choice);
		lowercase_words(choice);

		if (!strcmp(choice, "sign up"))
			sign_up();
		else if (!strcmp(choice, "log in"))
			log_in(cbc);
		if (strcmp(choice, "sign up") && strcmp(choice, "log in") && strcmp(choice, "exit"))
			printf("\nWrong input\n");
	}
	while (strcmp(choice, "exit"));

	save_cbc(DEFAULT_FILENAME, cbc);

	return 0;
}
