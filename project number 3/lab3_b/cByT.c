/* createByTime.c - create, newpid by time */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <mem.h>
#include <Targil3.h>


/*------------------------------------------------------------------------------
 *  create  --  create a process to start running a procedure after n seconds
 *------------------------------------------------------------------------------
 */

int loc = -1;

/*------------------------------------------------------------------------
 *  sleepAndExec  --  sleep for n sec and exec
 *------------------------------------------------------------------------
 */
void sleepAndExec(int loc)
{
	int i;
	sleep(savedProcArr[loc].seconds);
	xexecl(savedProcArr[loc].paddr,savedProcArr[loc].pVecArgs,loc);
}

SYSCALL createByTime(procaddr, ssize, priority, namep, seconds, nargs, args)
int (*procaddr)();		/* procedure address			*/
word ssize;			/* stack size in words			*/
short priority;			/* process priority > 0			*/
char* namep;			/* name (for debugging)			*/
int nargs;			/* number of args that follow		*/
int args;			/* arguments (treated like an array)	*/
{
	int	ps;			/* saved processor status	*/
	int i;
	int* a= (int*)NULL;
	

	//first step is to find a place to our saved proc
	disable(ps);
	if ((loc = newEntry()) == SYSERR) {
		restore(ps);
		return(SYSERR);
	}

	//second step is to initialize the entry we found
	
	savedProcArr[loc].pnargs = nargs;					/* number of arguments			   */
	savedProcArr[loc].paddr = procaddr;					/* pointer to saved proc		   */
	savedProcArr[loc].status = 1;						/* entry status 1 for not availble */
	savedProcArr[loc].seconds = seconds;				/* time to sleep			       */

	if (nargs != 0) {
		savedProcArr[loc].pVecArgs = (int*)getmem(2 * nargs);/* dinamic allocation*/
	}
		 
	a = &args;										/*a will be the location of args in the stack*/
	for (i = 0; i < nargs; i++) {
		savedProcArr[loc].pVecArgs[i] = a[i];		/*  copy from the stack to our "own stack"  */
	}

	restore(ps);
	//in this position we saved the environment of the proc we want to run after the sleep

	//now we can call to create with a intermediate function
	create(sleepAndExec, ssize, priority, namep, 1, loc);
	return OK;
}


/*------------------------------------------------------------------------
 *  newEntry  --  works like newpid() in create just for proc :)
 *------------------------------------------------------------------------
 */
LOCAL	newEntry()
{
	int	i;

	for (i = 1; i < NPROC; i++) {	/* check all NPROC slots	*/
		if (savedProcArr[i].status == 0)
			return i;
	}
	return(SYSERR);
}



