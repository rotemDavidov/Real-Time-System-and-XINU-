#include <conf.h>
#include <kernel.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

/*
------------------------------- varible info ----------------------------------
BarberShop[3]-> array that contains the id of the barber process
N -> number of costumers
sumCnum -> sums all the capcity barbers can handle 
mutex1,mutex2 -> for critical code 
Sumsem -> semaphore initlize with sumCnum
amount -> how many process enter the costumer method
lastCostumer -> the id of the last costumer
maxCnum -> array that include the maximum each barber can handle
counterCnum -> array that include the number of free sits in each barber shop
-------------------------------------------------------------------------------
*/

int BarberShop[3];
int N=6,SumCnum=4,mutex1,mutex2,Sumsem,amount,lastCostumer;
int maxCnum[3] = {1,1,2}; 
int counterCnum[3]={1,1,2}; 

xmain(){

int Barber(), costumer();
int i;
	//create semaphore with the size of the sum of the capacitys
	mutex1= screate(1);
	mutex2= screate(1);
	Sumsem = screate(SumCnum);
	//create 3 barbers
	if(N!=0){ //if there is no costumers we dont need to open the Barber shops
	for(i=0;i<3;i++)
		BarberShop[i]=create(Barber,INITSTK,INITPRIO+2,"processBarber",2,maxCnum[i],i); 
	}
	//create N costumers
	for(i=0;i<N;i++)
		resume(create(costumer,INITSTK,INITPRIO+1,"processCostumer",0)); 
	
	
}

Barber(int capacity,int i){
	int costumerPid,amount ;
	
	wait(mutex1); //protecte critical code
	while(capacity-counterCnum[i]>0){ //if another costumer choose the same Barber we dont want the process will end
	costumerPid = receive(); //the barber shop get the id of the costumer that enter to the barber shop
	printf("proc %d is handled by proc %d\n\n",costumerPid,BarberShop[i]);
	printf("proc %d : number of costumers : %d\n\n",BarberShop[i],capacity-counterCnum[i]);
	signal(mutex1);
	suspend(getpid());
	}	
}

costumer(){
	int index,Bpid,i;

	printf("proc %d start\n\n",getpid());
	
	//for closing the barber shop and send the workers home
	wait(mutex2);
	amount++;
	if(amount==N)
		lastCostumer=getpid();
	signal(mutex2);
	
	//costumer wants to take haircut
	printf("proc %d waiting in queue\n\n",getpid());
	wait(Sumsem);
	
	//search for a barber shop
	while(1){ //its guaranteed that a process wont take a lot of cpu time beacuse Sumsem let him enter - he must find a place 
		index = rand()%3; //each process will have is own index
		wait(mutex1); //start of critical code
		if(counterCnum[index]>0 && counterCnum[index]<=maxCnum[index]){
			break; //costumre stop to search for a barber shop
		}	
		signal(mutex1);
	}
	
	//costumer enter the chosen barber shop
	counterCnum[index]--; 
	Bpid = BarberShop[index];
	signal(mutex1); //end of critical code
	
	//let barber handle the costumer that want to be handled
	send(Bpid,getpid());
	resume(Bpid); 
	
	sleep(1); 
	
	//the costumer finish to take a hair cut 
	wait(mutex1);
	counterCnum[index]++;
	signal(mutex1);
	
	signal(Sumsem); //the sit is vacant for another costumer
	
	wait(mutex1);
	printf("proc %d end\n\n",getpid()); //dosent have to be inside critical code - for better printing 
	printf("proc %d : number of costumers : %d\n\n",Bpid,maxCnum[index]-counterCnum[index],getpid());
	signal(mutex1);
	
	//there is no costumer left , the Barbers dont need to wait(bcause of the suspend) they can go home//
	wait(mutex2);
	if(lastCostumer==getpid()) //in case of contaxt switch , between incrsing amount and kill the peocess
		for(i=0;i<3;i++)
			kill(BarberShop[i]);
	signal(mutex2);

}
