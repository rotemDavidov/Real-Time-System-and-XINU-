

struct	savedProc {
		
	int* pVecArgs;		/*vector of parameters*/
	int	pnargs;			/* initial number of arguments	*/
	int	(*paddr)();		/* initial code address		*/
	int status;
	int seconds;
};

extern	struct	savedProc savedProcArr[];