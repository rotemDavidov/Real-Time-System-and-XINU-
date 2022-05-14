//rotem davidov 
//or yehuda ben shimol


#include <stdio.h>
#include <dos.h>
#include <time.h>

//global variables 
volatile unsigned char scan;
volatile unsigned int scan_code;

volatile int speed=1;
volatile int flag;
volatile int enter=0;
volatile int p_press=0;
volatile int shift_l_pressed=0;
volatile int shift_r_pressed=0;
volatile int time_shift=0;

//pointer to function of type interrupt
void interrupt (*save9) (void);
void interrupt (*save8) (void);

//regular functions 
void slowPrg();

//take over interrupt functions
void interrupt my9func(void) {
	
		//caling the ols routine
		save9();
	
		//getting the scan code from the port
		asm{
			in al,60h    
			mov scan, al  
		}	
		scan_code = (unsigned int)scan;
		
		//if the user wants to slow the program
		if (scan_code == 45 || scan_code ==80){
			if(speed<16)
				speed = speed*2;
			printf("the speed is : 1/%d \n",speed);		
		}
		
		//if the user wants to hurry the program
		else if (scan_code == 17 || scan_code ==72){
			if(speed>1)
				speed = speed/2;
			printf("the speed is : 1/%d \n",speed);	
		}
		
		//if the user wants to stop or resume the program
		else if(scan_code == 25){
			p_press=!p_press;
			asm{sti}
			while(p_press);
			asm{cli}	
		}
			
		//if the user want to go back to reguar running
		else if(scan_code ==19 && !p_press){ //!p_press for protect - the user cant go back to regular run when the prog on pause 
			//p_press=!p_press; -> potential adding - in case we allowed the user go from pause to regular
			setvect(9,save9);
			setvect(8,save8);
		}
		
		//if the right shift was being pressed
		else if(scan_code==54)
			shift_r_pressed=1;
		
		//if the left shift was being pressed
		else if(scan_code==42)
			shift_l_pressed=1;
		
		//if the left shift was being realese
		else if(scan_code==182){
			shift_r_pressed=0;
			time_shift=0;
		}
		
		//if the left shift was being realese
		else if(scan_code==170){
			shift_l_pressed=0;
			time_shift=0;
		}
			
}

void interrupt my8func(void) {
	save8();
	
	//in case both of the shifts pressed we count the secondes
	if(shift_r_pressed && shift_l_pressed)
		time_shift++;
	
	//in case we pressed 4 sec we go back to dos and exit the program
	if(time_shift>=18.2*4){
		printf("slowPrg is canceled");
		setvect(9,save9);
		setvect(8,save8);
		exit(0);	
	}
	
	enter++; 
	if(enter%speed!=0){ //decide which interrupt will be stuck in a busy wait and which not
		flag=1;
		asm{sti}
	}
	else
		flag=0;

	while(flag);
	
}
		


void slowPrg() {
	save9 = getvect(9);
	setvect(9, my9func);
	save8=getvect(8);
	setvect(8,my8func);
}



void main() {
	unsigned long int i=0, j=0;
	long long int	counter = 0;
	time_t t1, t2;
	slowPrg();
	while (1) {
		(void)time(&t1);
		j = 0;
		while (j < 10) {
			i++;
			counter++;
			if ((i % 10000) == 0) {
				printf("counter = %lld\n", counter);
				j++;
			}
		}// while
		(void)time(&t2);
		printf("\nTotal  Iteration  Time -  = %d seconds\n", (int)(t2 - t1));
	}
}


	
	






