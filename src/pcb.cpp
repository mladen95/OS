#include "pcb.h"
#include <dos.h>
#include "pcbList.h"
#include "intr.h"



volatile PCB* PCB::running=0;
PCBAll* PCB::listAll=0;
PCBSleep* PCB::listSleep=0;
PCB* PCB::idleThread = 0;
ID PCB::GEN_ID = 0;
volatile int PCB::numberOfActive = 0;
volatile int PCB::numberOfReady = 0;

PCB::PCB(Thread* t, StackSize stackSize, Time timeSlice){
	lock();
	myID = ++PCB::GEN_ID;
	sp=0;	ss=0;	bp=0;
	myThread = t;
	kvant = timeSlice;
	if (stackSize > 65535)
		stackSize = 65535;
	stackSize /= sizeof(unsigned);

	stack = new unsigned[stackSize];
	stackSz = stackSize;

	stack[stackSz-5] = 0x200;
#ifndef BCC_BLOCK_IGNORE
	stack[stackSz-6] = FP_SEG(PCB::wrapper);
	stack[stackSz-7] = FP_OFF(PCB::wrapper);
	sp = FP_OFF(stack+stackSz-16);
	ss = FP_SEG(stack+stackSz-16);
#endif
	bp = sp;

	status=0;
	listWaiting = new PCBWaiting();
	PCB::listAll->addNewPCB(this);
	PCB::numberOfActive++;
	unlock();
}


void PCB::wrapper(){
	PCB::running->myThread->run();
	lock();
	PCB::running->listWaiting->notifyAll();
	PCB::running->status = 4;
	unlock();
	dispatch();
}


ID PCB::getID(){
	return myID;
}

PCB* PCB::getPCBById(ID id){
	return PCB::listAll->getPCBbyID(id);
}


PCB::~PCB(){
	lock();
	if (stack!=0)
		delete[] stack;
	delete listWaiting;
	PCB::listAll->removePCB(this);
	PCB::numberOfActive--;
	unlock();
}
