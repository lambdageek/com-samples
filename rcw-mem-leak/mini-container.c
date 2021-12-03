#include <stdlib.h>

#include "mini-container.h"

MiniContainer *
mini_container_new (int cap)
{
	MiniContainer *c = malloc (sizeof (MiniContainer));
	c->items = malloc (sizeof (void*) * cap);
	c->capacity = cap;
	c->count = 0;
	return c;
}

int
mini_container_add (MiniContainer *c, void *item)
{
	if (c->count + 1 == c->capacity) {
		int new_cap = (int)((long)c->capacity * 3 / 2);
		c->items = realloc (c->items, sizeof (void*) * new_cap);
		c->capacity = new_cap;
	}
	int idx = c->count++;
	c->items [idx] = item;
	return idx;
}

void *
mini_container_get (MiniContainer *container, int i)
{
	if (i >= container->count)
		abort();
	return container->items [i];
}

void
mini_container_foreach (MiniContainer *container, mini_container_foreach_func func, void *user_data)
{
	for (int i = 0; i < container->count; ++i) {
		func (container->items [i], user_data);
	}
}

void
mini_container_destroy (MiniContainer *container, mini_container_foreach_func destroy_func)
{
	mini_container_foreach (container, destroy_func, (void*)0);
	free (container->items);
	free (container);
}
