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
	void run();
	~Ex1(){
		waitToComplete();
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
