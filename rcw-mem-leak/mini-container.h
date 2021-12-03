
#ifndef _MINI_CONTAINER_H
#define _MINI_CONTAINER_H

typedef struct MiniContainer {
	int capacity;
	int count;
	void **items;
} MiniContainer;

MiniContainer *
mini_container_new (int capacity);

int
mini_container_add (MiniContainer *container, void *item);

void *
mini_container_get (MiniContainer *container, int index);


typedef void (*mini_container_foreach_func)(void *elem, void *user_data);

void
mini_container_foreach (MiniContainer *container, mini_container_foreach_func func, void *user_data);

void
mini_container_destroy (MiniContainer *container, mini_container_foreach_func destroy_func);



#endif
