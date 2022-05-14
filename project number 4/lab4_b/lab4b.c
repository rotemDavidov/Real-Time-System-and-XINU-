//rotem davidov 
//or yehuda ben shimol


#include <stdio.h>
#include <dos.h>
#include <time.h>

//global variables 
volatile unsigned char scan;
volatile unsigned int scan_code;
volatile unsigned int enter = 0;
volatile int index = 0;
volatile int speed = 100;

unsigned char chosen_color;
unsigned char colors[] = { 0x00,0x10,0x20,0x30,0x40,0x50,0x60,0x70 };




//pointer to function of type interrupt
void interrupt (*save9) (void);
void interrupt (*save8) (void);

//regular functions 
void initlize();
void backToNormal();

//take over interrupt functions
void interrupt my9func(void) {
	
		//calling the old routine
		save9();
	
		//getting the scan code from the port
		asm{
			in al,60h    
			mov scan, al  
		}	
		scan_code = (unsigned int)scan;
		
		//if the user wants to hurry the program
		if (scan_code == 80){
			if(speed!=1) 
				speed--;
			
		}
		
		//if the user wants to slow the program
		else if (scan_code == 72){
			speed++;
			
		}

		
			
}

void interrupt my8func(void) {
	save8();
	enter++;
	
	/*if we reched to speed interupt per sec
	its the time to change the color*/
	if (enter >= speed) {
		/*Changing colors in a circular motion*/
		if (index == 7)
			index = 0;
		else
			index++;
		chosen_color = colors[index]; //getting the wanted color
		asm{
				MOV AX,0B800h
				MOV ES,AX
				MOV DI, 0
				XOR AX,AX
				MOV AL,' '
				MOV AH,chosen_color
				MOV CX,2000
				CLD
				REP STOSW
		}
		enter = 0;
	} //end_of_if
	
}
		


void initlize() {
	//run over interupt methods
	save9 = getvect(9);
	setvect(9, my9func);
	save8=getvect(8);
	setvect(8,my8func);

	//Changes the amount of interruptions to 100 per second
	//this change help us monitor the count of the interupt prsicely
	asm{
	MOV AL,36H    
	OUT 43H,AL
	MOV BX,11930 // = 100
	MOV AL,BL               
	OUT 40H,AL
	MOV AL,BH                
	OUT 40H,AL
	} // asm

	//Initialize the display adapter
		asm{
		MOV  AH, 0
		MOV  AL, 1
		INT  10H
	}
}

void backToNormal() {
	/* change the latch back to 18.2 interupts per second*/
	asm{
		MOV AL,36H    
		OUT 43H,AL
		MOV BX,0
		MOV AL,BL               
		OUT 40H,AL
		MOV AL,BH                
		OUT 40H,AL
	} 
	/* return the old routine to the interupt vector*/
	setvect(8, save8); 
	setvect(9, save9); 
	/*go from the existing background color to the original*/
	asm{
		mov ax,3
		INT 10h
	}
}



void main() {
	initlize();
	while (scan_code != 1);
	backToNormal();
}


	
	






