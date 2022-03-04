/* ready.c - ready */

#include <xinu.h>

struct queue *readyqueue;	/** ready queue */


/**
 * Put process on ready queue
 * @param pid	ID of process to place on ready queue
 * @param resch	reschedule afterward?
 */
status	ready(pid32 pid, bool8 resch)
{
	

	register struct procent *prptr;

	if (isbadpid(pid))
		return(SYSERR);

	// set process state to indicate ready and add to ready list
	prptr = &proctab[pid];

	// TODO - set the process' state pointed by prptr to "ready"
	
	prptr->prstate = PR_READY; //POSSIBLE ISSUE HERE
	

	// TODO - enqueue the process
	pid32 rpid = enqueue(pid, readyqueue);
	
	if(rpid == SYSERR){
		kprintf("ERROR ON ENQUEUE");
	}

	if (resch == RESCHED_YES){
		resched();
	}
		
	return OK;
}
