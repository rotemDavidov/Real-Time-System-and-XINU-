/* ex2.c - xmain, prA, prB */

#include <conf.h>
#include <kernel.h>
#include <new.h>

/*------------------------------------------------------------------------
 *  xmain  --  example of creating processes in PC-Xinu
 *------------------------------------------------------------------------
 */


void setCreateDelay() {
	asm{
	   MOV AL,36H // TIMER COMMAND: SELECT CHANNEL 0;
			   //                READ / WRITE LSB-MSB
			   //                MODE 3: BINARY
	OUT 43H,AL
	MOV BX,1193  // עבור אלף פסיקות שעון בשניה, יהיה לי 1500 פסיקות בשניה וחצי
	MOV AL,BL                 //TRANSFER THE LSB 1ST
	OUT 40H,AL
	MOV AL,BH                 //TRANSFER THE MSB SECOND 
	OUT 40H,AL
	} // asm
}

void back2normal() {
	asm{
		MOV AL,36H 
		MOV BX,0 
		MOV AL,BL                 
		OUT 40H,AL
		MOV AL,BH                 
		OUT 40H,AL
	} // asm

}

xmain()
{
	int prA(), prB();
	setCreateDelay();
	resume( create(prA, INITSTK, INITPRIO, "proc 1", 0) );
	resume( create(prB, INITSTK, INITPRIO, "proc 2", 0) );
	back2normal();
}

/*------------------------------------------------------------------------
 *  prA  --  repeatedly print 'A' without ever terminating
 *------------------------------------------------------------------------
 */
prA()
{
	int i;
	for(i=0;i<10;i++)
		putc(CONSOLE, 'A');
}

/*------------------------------------------------------------------------
 *  prB  --  repeatedly print 'B' without ever terminating
 *------------------------------------------------------------------------
 */
prB()
{
	int i;
	for (i = 0; i < 10; i++)
		putc(CONSOLE, 'B');
}

