#include "kernSem.h"
#include "intr.h"
#include "thread.h"
#include "semList.h"
#include <iostream.h>

extern int locked;


KernelSem::KernelSem(Semaphore *sem, int init){
	lock();
	mySem = sem;
	val = init;
	blockedList = new PCBBlocked();
	unlock();
}

int KernelSem::getVal(){
	return val;
}


void KernelSem::signal(){
	lock();
	PCB::running->sem=0;
	if (val++<0){
		blockedList->deblockS();
	}
	unlock();
}

void KernelSem::tickSignal(){
	lock();
	val++;
	unlock();
}


int KernelSem::wait(Time maxTimeToWait){
	lock();
	PCB::running->sem=this;
	if (--val<0){
		int res=-1;
		blockedList->addNewPCB(this, &res, (PCB*)PCB::running, maxTimeToWait);
		PCB::running->status = 2;
		unlock();	//VEC POSTOJI JEDAN LOCK U SEMAPHORE::WAIT
		unlock();
		dispatch();
		lock();
		// NIT JE ODBLOKIRANA

		if (res==2)
			return 1;
		else if (res==1)
			return 0;
		else
			return -1;
	}
	else {
		unlock();
		return 1;
	}
}


KernelSem::~KernelSem(){
	lock();
	delete blockedList;
	unlock();
}
