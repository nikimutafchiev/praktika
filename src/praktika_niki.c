#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
struct stories
{
	char *title, *date, *user, *story;
};
struct stories_packet
{
	struct stories **buff;
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
//хеширай тук за всеки user, добави и за дата хеширане

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



int main() {

}
