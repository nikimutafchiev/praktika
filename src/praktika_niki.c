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
	for (size_t i = 0; i < packet->size; i++) {
		if (!strcmp(packet->buff[i]->date, date))
			return packet->buff[i];
	}
}
struct stories_packet *init_packet(size_t max_visualisation) {
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

struct stories_packet *stories_by_user(struct stories_packet *all, char *user) { 
	struct stories_packet *stories_of_user = init_packet(all->size);
	for (size_t i = 0; i < all->size; i++) {
		if(!strcmp(all->buff[i]->user, user))
		push_in_packet(stories_of_user, user, all->buff[i]);
	}
	return stories_of_user;
}
struct stories *search_by_name(struct stories_packet *packet, char *name) {
	for (size_t size = 0; size < packet->size; size++) {
		if (!strcmp(packet->buff[size]->name, name))
			return packet->buff[size];
	}
}
int main() {

}
