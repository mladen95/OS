#ifndef _EX_H_
#define _EX_H_

#include "thread.h"
#include "pcb.h"
#include <iostream.h>
#include "pcbList.h"
#include "semaphor.h"



class Ex1 : public Thread {
public:
	Ex1(StackSize stackSize = defaultStackSize, Time timeSlice = defaultTimeSlice):Thread(stackSize, timeSlice){}
	void run(){
		cout<<"\EX1 START\n";
		/*for (long k = 0; k<100000; ++k)
							for (long j = 0; j <10000; ++j);*/
		//PCB::listSleep->addNewPCB((PCB*)PCB::running, 10);


		cout<<"\nEX1 END\n";

	}
};


class Ex2 : public Thread {
public:
	Ex2(StackSize stackSize = defaultStackSize, Time timeSlice = defaultTimeSlice):Thread(stackSize, timeSlice){}
	void run();
};

class Ex3 : public Thread {
public:
	Ex3(StackSize stackSize = defaultStackSize, Time timeSlice = defaultTimeSlice):Thread(stackSize, timeSlice){}
	void run();
};


#endif
