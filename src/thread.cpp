#include "thread.h"
#include "pcb.h"
#include "pcbList.h"
#include "intr.h"

extern volatile int zahtevana_promena_konteksta;

Thread::Thread(StackSize stackSize, Time timeSlice){
	lock();
	myPCB = new PCB(this, stackSize, timeSlice);
	unlock();
}

void Thread::start(){
	lock();
	if (myPCB->status==0){
		myPCB->status=1;
		Scheduler::put(myPCB);
		PCB::numberOfReady++;
	}
	unlock();
}





void dispatch(){
	asm{
		pushf
		cli
	}
	zahtevana_promena_konteksta = 1;
	timer();
	asm{
		popf
	}
}


void Thread::waitToComplete(){
	if (myPCB->status != 4){
		myPCB->listWaiting->addNewPCB((PCB*)PCB::running);
		PCB::running->status = 2;
		dispatch();
	}
}


ID Thread::getId(){
	return myPCB->getID();
}

ID Thread::getRunningId(){
	return ((PCB*)(PCB::running))->getID();
}

Thread* Thread::getThreadById(ID id){
	return PCB::listAll->getThread(id);
}


Thread::~Thread(){
	waitToComplete();
	lock();
	delete myPCB;
	unlock();
}
