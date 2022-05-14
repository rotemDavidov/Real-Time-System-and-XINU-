/* resched.c - resched */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <q.h>

//prioArr will be the oroginal prio of the process , extern varible from initailli
extern int prioArr[];

/*------------------------------------------------------------------------
 *  resched  --  reschedule processor to highest priority ready process
 *
 * Notes:	Upon entry, currpid gives current process id.
 *		Proctab[currpid].pstate gives correct NEXT state for
 *			current process if other than PRCURR.
 *------------------------------------------------------------------------
 */
int	resched()
{
	register struct	pentry	*optr;	/* pointer to old process entry */
	register struct	pentry	*nptr;	/* pointer to new process entry */
	register struct	pentry *min;	


	optr = &proctab[currpid];
	if ( optr->pstate == PRCURR ) 
         {
		/* no switch needed if current prio. higher than next	*/
		/* or if rescheduling is disabled ( pcxflag == 0 )	*/
		if ( sys_pcxget() == 0 ||(preempt > 0) ) /*CHANGE*/
			return;

		/* force context switch */
		optr->pstate = PRREADY;

		/*CHANGE*/
		if (currpid > 3) { //if you are not the zero process or the batlers
				if (optr->pprio > 1) { //protect process with 1 prio
					/*handle the prio*/
					if (q[rdyhead].qnext != q[rdytail].qprev) { //in case there are more then the zero process in the queue - we can extract the min process that doesnt equale to zero
						min = &proctab[q[q[rdyhead].qnext].qnext];
						optr->pprio = min->pprio - 1;
					}
				}

				if(optr->pprio ==1) { //protect process that became 1 prio from the prev condition
					optr->pprio = prioArr[currpid];
				}
		}
		insert(currpid,rdyhead,optr->pprio); 


	} /* if */ 
        else if ( sys_pcxget() == 0 ) 
            {
		kprintf("pid=%d state=%d name=%s",
			currpid,optr->pstate,optr->pname);
		panic("Reschedule impossible in this state");
	    } /* else if */

	/* remove highest priority process at end of ready list */

	nptr = &proctab[ (currpid = getlast(rdytail)) ];
	nptr->pstate = PRCURR;		/* mark it currently running	*/
	preempt = QUANTUM;		/* reset preemption counter	*/
	ctxsw(&optr->pregs,&nptr->pregs);

	/* The OLD process returns here when resumed. */
	return;
}
