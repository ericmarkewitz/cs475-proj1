/* queue.c - enqueue, dequeue, isempty, nonempty, et al. */

#include <xinu.h>
#include <stdlib.h>

/**
 * Prints out contents of a queue
 * @param q	pointer to a queue
 */
void	printqueue(struct queue *q)
{
	//TODO - print all contents from head to tail
	kprintf("[");
	struct qentry *currNode = q->head;
	while(currNode != NULL){
		pid32 currpid = currNode->pid;
		kprintf("%d,",currpid);
		
		currNode = currNode->next;
	}
	kprintf("]");

	//TODO - format should be [(pid,key), (pid,key), ...]
}

/**
 * Checks whether queue is empty
 * @param q	Pointer to a queue
 * @return TRUE if true, FALSE otherwise
 */
bool8	isempty(struct queue *q)
{
	if(q->head==NULL) return 1;
	else return 0;
}

/**
 * Checks whether queue is nonempty
 * @param q	Pointer to a queue
 * @return TRUE if true, FALSE otherwise
 */
bool8	nonempty(struct queue *q)
{
	if(q->head != NULL) return 1;
	else return 0;
}


/**
 * Checks whether queue is full
 * @param q	Pointer to a queue
 * @return TRUE if true, FALSE otherwise
 */
bool8	isfull(struct queue *q)
{
	//TODO - check if there are at least NPROC processes in the queue
	if(q->size >= NPROC) return 1;
	else return 0;
}


/**
 * Insert a process at the tail of a queue
 * @param pid	ID process to insert
 * @param q	Pointer to the queue to use
 *
 * @return pid on success, SYSERR otherwise
 */
pid32 enqueue(pid32 pid, struct queue *q)
{
        //TODO - check if queue is full and if pid is illegal, and return SYSERR if either is true
		if(isfull(q) || isbadpid(pid)){
			return SYSERR;
		}

        //TODO - allocate space on heap for a new QEntry

		//TODO - initialize the new QEntry

        //TODO - insert into tail of queue

        //TODO - return the pid on success
		struct qentry *newQentry = (struct qentry*) malloc(sizeof(struct qentry));

		if(isempty(q)){
			newQentry->next = NULL;
			newQentry->prev = NULL;
			newQentry->pid = pid;
			q->head = newQentry;
			q->tail = newQentry;
			q->size = 1;
		}
		else{
			struct qentry *oldTail = q->tail;
			oldTail->next = newQentry;

			newQentry->pid = pid;
			newQentry->prev = oldTail;
			newQentry->next = NULL;

			q->tail = newQentry;
			q->size++;
		}

		return pid;

        
}


/**
 * Remove and return the first process on a list
 * @param q	Pointer to the queue to use
 * @return pid of the process removed, or EMPTY if queue is empty
 */
pid32 dequeue(struct queue *q)
{
	//TODO - return EMPTY if queue is empty
	if(isempty(q)) return EMPTY;

	//TODO - get the head entry of the queue
	struct qentry *oldHead = q->head;
	pid32 pid = oldHead->pid;



	//TODO - unlink the head entry from the rest
	if(q->size != 1){
		struct qentry *newHead = oldHead->next;
		newHead->prev = NULL;
		q->head = newHead;
	}
	else{
		q->tail = NULL;
		q->head = NULL;
	}

	//TODO - free up the space on the heap
	free(oldHead, sizeof(oldHead));

	//TODO - return the pid on success
	q->size--;
	return pid;
}


/**
 * Finds a queue entry given pid
 * @param pid	a process ID
 * @param q	a pointer to a queue
 * @return pointer to the entry if found, NULL otherwise
 */
struct qentry *getbypid(pid32 pid, struct queue *q)
{
	//TODO - return NULL if queue is empty or if an illegal pid is given
	if(isempty(q) || isbadpid(pid)){
			return NULL;
	}

	//TODO - find the qentry with the given pid
	//TODO - return a pointer to it

	struct qentry *currNode = q->head;
	while(currNode != NULL){
		pid32 currpid = currNode->pid;
		if(currpid == pid) return currNode;
		currNode = currNode->next;
	}

	return NULL;
}

/**
 * Remove a process from the front of a queue (pid assumed valid with no check)
 * @param q	pointer to a queue
 * @return pid on success, EMPTY if queue is empty
 */
pid32	getfirst(struct queue *q)
{
	//TODO - return EMPTY if queue is empty
	//TODO - remove process from head of queue and return its pid
	return dequeue(q);

}

/**
 * Remove a process from the end of a queue (pid assumed valid with no check)
 * @param q	Pointer to the queue
 * @return pid on success, EMPTY otherwise
 */
pid32	getlast(struct queue *q)
{
	//TODO - return EMPTY if queue is empty
	if(isempty(q)) return EMPTY;

	//TODO - remove process from tail of queue and return its pid

	//TODO - get the tail entry of the queue
	struct qentry *oldTail = q->tail;
	pid32 pid = oldTail->pid;


	//TODO - unlink the tail entry from the rest
	if(q->size != 1){
		struct qentry *newTail = oldTail->prev;
		newTail->next = NULL;
		q->tail = newTail;
	}
	else{
		q->tail = NULL;
		q->head = NULL;
	}


	//TODO - free up the space on the heap
	free(oldTail, sizeof(oldTail));

	//TODO - return the pid on success
	q->size--;
	return pid;
}



/**
 * Remove a process from an arbitrary point in a queue
 * @param pid	ID of process to remove
 * @param q	Pointer to the queue
 * @return pid on success, SYSERR if pid is not found
 */
pid32	remove(pid32 pid, struct queue *q)
{
	//TODO - return EMPTY if queue is empty
	if(isempty(q)) return EMPTY;

	//TODO - return SYSERR if pid is illegal
	if(isbadpid(pid)) return SYSERR;

	//TODO - if pid does not exist in the queue, return SYSERR
	if(getbypid(pid, q) == NULL){
		return SYSERR;
	}
	//TODO - remove process identified by pid parameter from the queue and return its pid
	
	if(q->size == 1){
		struct qentry *node = q->head;
		pid32 rpid = node->pid;
		q->head = NULL;
		q->tail = NULL;
		q->size = 0;

		return rpid;
	}
	else{
		struct qentry *currNode = q->head;
		while(currNode != NULL){
			pid32 rpid = currNode->pid;
			if(rpid == pid){
				//Node we are removing is the head
				if(currNode->prev == NULL){
					return dequeue(q);
				}
				else if(currNode->next == NULL){
					return getlast(q);
				}
				else{
					struct qentry *prevNode = currNode->prev;
					struct qentry *nextNode = currNode->next;

					prevNode->next = nextNode;
					nextNode->prev = prevNode;

					free(currNode, sizeof(currNode));

					return rpid;
				}
			}
		}
	}
	return SYSERR;
}
