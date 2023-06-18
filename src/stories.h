#pragma once

#include <stdlib.h>


struct stories
{
	char *title, *date, *user, *story;
};

struct stories_packet
{
	struct stories **buff;
	int size, capacity;
};

struct stories_packet *init_packet(size_t);
struct stories *resize_packet(struct stories_packet *);
int hash(size_t, char *);
void push_in_packet(struct stories_packet *, size_t, struct stories *);
void destroy_packet(struct stories_packet *);

struct stories *search_by_date(struct stories_packet *, char *);
struct stories *search_by_title(struct stories_packet *, char *);
struct stories_packet **stories_by_user(struct stories_packet *, char *);
