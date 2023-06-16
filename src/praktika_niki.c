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
	int size;
};
truct stories_packet *init_packet(size_t max_visualisation) {
	struct stories_packet *p = malloc(sizeof * p);
	p->buff = calloc(max_visualisation, sizeof(struct stories *));
	p->size = 0;
	return p;
}
void push_in_packet(struct stories_packet *packet, size_t max_size, struct stories* story) {
	for (size_t i = 0; i < max_size; i++) {
		if (packet->buff[i] == NULL) {
			packet->buff[i] = story;
			packet->size++;
			return;
		}
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
	for (size_t i = 0; i < packet->size; i++) {
		if (!strcmp(packet->buff[i]->date, date))
			return packet->buff[i];
	}
	return NULL;
}
s
struct stories_packet *stories_by_user(struct stories_packet *all, char *user) { 
	struct stories_packet *stories_of_user = init_packet(all->size);
	for (size_t i = 0; i < all->size; i++) {
		if(!strcmp(all->buff[i]->user, user))
		push_in_packet(stories_of_user, user, all->buff[i]);
	}
	return stories_of_user;
}
struct stories *search_by_title(struct stories_packet *packet, char *title) {
	for (size_t i = 0; i < packet->size; i++) {
		if (!strcmp(packet->buff[i]->title, title))
			return packet->buff[i];
	}
	return NULL;
}



int main() {

}
