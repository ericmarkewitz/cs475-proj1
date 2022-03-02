/* newqueue.c - newqueue */

#include <xinu.h>
#include <stdlib.h>

/**
 * Allocate and initialize a queue
 * @return pointer to queue structure
 */
struct queue *newqueue(void)
{
	// TODO - allocate memory for a new Queue structure (required to be on heap)
	struct queue *nqueue = (struct queue*) malloc(sizeof(struct queue));

	// TODO - initialize the structure
	nqueue->head = NULL;
	nqueue->tail = NULL;
	nqueue->size = 0;

	// TODO - return pointer to the structure
	return nqueue;
}
