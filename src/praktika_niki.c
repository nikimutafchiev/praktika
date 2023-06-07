#include <stdio.h>
#include <stdlib.h>
#include <string.h>
struct stories
{
	char *name, *date, *user, *story;
};
struct stories_packet
{
	struct stories **buff;
	int size;
};
struct stories *search_by_date(struct stories_packet *packet, char *date) {
	for (size_t size = 0; size < packet->size; size++) {
		if (!strcmp(packet->buff[size]->date, date))
			return packet->buff[size];
	}
}
struct stories_packet *init_packet(size_t max_visualisation) {
	struct stories_packet *p = malloc(sizeof * p);
	p->buff = calloc(max_visualisation, sizeof(struct stories *));
	p->size = 0;
	return p;
}
void push_in_packet(struct stories_packet *packet, size_t max_visualisation, struct stories* story) {
	for (size_t size = 0; size < max_visualisation; size++) {
		if (packet->buff[size] == NULL) {
			packet->buff[size] = story;
			packet->size++;
			return;
		}
	}
}
struct stories *search_by_user(struct stories_packet *packet, char *user, size_t last_visualisated) {
	for (size_t size = last_visualisated; size < packet->size; size++) {
		if (!strcmp(packet->buff[size]->user, user))
			return packet->buff[size];
	}
}
struct stories_packet *stories_by_user(struct stories_packet *all, char *user, size_t max_visualisation) {
	//НЕ работи напълно
	struct stories_packet *stories_of_user = init_packet(max_visualisation);
	for (size_t i = 0; i < max_visualisation &&  i < all->size; i++) {
		push_in_packet(stories_of_user, user, search_by_user(all, user, i));
	}
	return stories_of_user;
}
struct stories *search_by_name(struct stories_packet *packet, char *name) {
	for (size_t size = 0; size < packet->size; size++) {
		if (!strcmp(packet->buff[size]->name, name))
			return packet->buff[size];
	}
}