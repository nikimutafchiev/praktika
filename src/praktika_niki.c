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
struct stories_packet *init_packet(size_t max_size) {
	struct stories_packet *p = malloc(sizeof * p);
	p->buff = calloc(max_visualisation, sizeof(struct stories *));
	p->size = 0;
	p->capacity = max_size;
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
	for (size_t i = 0; i < packet->size; i++) {
		if (packet->buff[index] == NULL) {
			packet->buff[index] = story;
			packet->size++;
			return;
		}
		index = (index + 1) % packet->size;
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
	size_t hash_index = hash(packet->size, date);
	for (size_t i = 0; i < packet->size; i++) {
		if (!strcmp(packet->buff[hash_index]->date, date))
			return packet->buff[hash_index];
		hash_index = (hash_index + 1) % packet->size;
	}
	return NULL;
}
struct stories *search_by_title(struct stories_packet *packet, char *title) {
	size_t hash_index = hash(packet->size, title);
	for (size_t i = 0; i < packet->size; i++) {
		if (!strcmp(packet->buff[hash_index]->title, title))
			return packet->buff[hash_index];
		hash_index = (hash_index + 1) % packet->size;
	}
	return NULL;
}
struct stories_packet *stories_by_user(struct stories_packet *all, char *user) { 
	struct stories_packet *stories_of_user = init_packet(all->size);
	for (size_t i = 0; i < all->size; i++) {
		if(!strcmp(all->buff[i]->user, user))
		push_in_packet(stories_of_user, user, all->buff[i]);
	}
	return stories_of_user;
}




int main() {

}
