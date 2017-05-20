#ifndef _KERNTHR_H_
#define _KERNTHR_H_

#include "thread.h"
#include "pcb.h"
#include <iostream.h>
#include "pcbList.h"

int userMain(int argc, char* argv[]);

class MainT : public Thread {
private:
	unsigned long stek;
	unsigned int time;
	int argc;
	char** argv;
	int *ret;
public:
	MainT(unsigned long s = 4096, unsigned int t = defaultTimeSlice,int *ret,int argc, char* argv[]):Thread(s, t){
			stek=s;
			time=t;
			this->argc = argc;	this->argv=argv;	this->ret = ret;
		}
	virtual void run(){
		*ret = userMain(argc, argv);
	}

};

class IdleThread : public Thread {
public:
	IdleThread(StackSize stackSize = defaultStackSize, Time timeSlice = defaultTimeSlice):Thread(stackSize, timeSlice){

	}
	void run(){
		cout<<"IdleNIT";
		while(PCB::numberOfActive>0){
			//cout<<"Idle";
			//PCB::listSleep->ispis();
		}
	}
};


#endif
