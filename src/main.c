#define _CRT_SECURE_NO_WARNINGS
#define PSWRDMAX 20
#define USRMAX 50

#include <stdio.h>
#include <malloc.h>
#include <string.h>

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
	char* buffer;
	char choice[10], new_username[USRMAX], new_password[PSWRDMAX];
	char input_pswrd[PSWRDMAX], input_username[USRMAX];
	buffer = generate_key(time(NULL), 64);

	printf("%s\n", buffer);

	/*create_new_user("Stefan Georgiev", "asdasdasd", "users.txt");
	add_story("How i met your mother", "Josh", "29.2.2022", "uvu vue vue unicue veu veu u sans", "stories.txt");
	add_story("How i met your mother", "Josh", "29.2.2022", "uvu vue vue unicue veu veu u sans", "stories.txt");
	add_story("How i met your mother", "Josh", "29.2.2022", "uvu vue vue unicue veu veu u sans", "stories.txt");
	add_story("How i met your mother", "Josh", "29.2.2022", "uvu vue vue unicue veu veu u sans", "stories.txt");
	put_in_structs("stories.txt");*/

	while (1)
	{
		printf("\nLog in or Sign up"); scanf("%s", choice);
		if (!strcmp(choice, "sign up") || !strcmp(choice, "Sign up"))
		{
			do 
			{
				printf("\n\nEnter username(50 chars limit): "); scanf("%s", new_username);
				printf("\nEnter password(20 chars limit): "); scanf("%s", new_password);
			} while (strlen(new_username) > USRMAX || strlen(new_password) > PSWRDMAX || !strcmp(new_password, "") || !strcmp(new_username, ""));
			create_new_user(new_username, new_password, "users.txt");
		}
		else if (strcmp(choice, "log in") == 0 || strcmp(choice, "Log in") == 0)
		{
			do
			{
				printf("Enter username: "); scanf("%s", input_username);
				printf("Enter password: "); scanf("%s", input_pswrd);
				}while()
				//prints story
			//chech if pswd is true and if user exists 
		}
		else 
		{
			printf("Invalid input");
		}
	}
	
	return 0;
}
