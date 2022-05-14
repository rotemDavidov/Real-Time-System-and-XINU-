 /* xexecl.c - xmain, prA, prB */

#include <conf.h>
#include <kernel.h>
#include <io.h>
#include <proc.h>
#include <sem.h>
#include <mem.h>
#include <q.h>
#include <bios.h>
#include <kbdio.h>
#include <Targil3.h>
#define INITF 0x0200

extern int INITRET();


int xexecl(void (*pf)(), int* parm,int loc)
{
struct pentry *pptr;
char *saddr;
int *sp1;
int ps;
int dummy;
int i;

disable(ps);
pptr = &proctab[currpid];
pptr->phasmsg = 0;
sp1 = (int *)(pptr->pbase + pptr->plen);

/* CHANGE */

pptr->pargs = savedProcArr[loc].pnargs;
for (i = pptr->pargs-1 ; i >= 0; i--) /*simulating a stack we will fill from the bottom to the top*/
	*(--sp1) = parm[i];

if (savedProcArr[loc].pnargs != 0) {
	if (freemem((char*)savedProcArr[loc].pVecArgs, 2 * savedProcArr[loc].pnargs) != OK) {
		restore(ps);
		return(SYSERR);
	}
}

/* END OF CHANGE */


*(--sp1) = (int) INITRET;
*(--sp1)= (int) pf;
--sp1;
*(--sp1) = INITF;
sp1 -= 2;
pptr->pregs = (char*)sp1;
pptr->paddr = pf;

ctxsw(&dummy, &pptr->pregs);
return 0;
} /* xexecl */


