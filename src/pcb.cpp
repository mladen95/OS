#include "pcb.h"
#include <dos.h>
#include "pcbList.h"
#include "evList.h"
#include "semList.h"
#include "sigList.h"
#include "intr.h"
#include "kernSem.h"
#include <iostream.h>



volatile PCB* PCB::running=0;
PCBAll* PCB::listAll=0;
PCBSleep* PCB::listSleep=0;
PCB* PCB::idleThread = 0;
ID PCB::GEN_ID = 0;
volatile int PCB::numberOfActive = 0;


int PCB::globalSignalStates[16] = {0};

void zeroSignalFunction(){
	PCB::running->listWaiting->notifyAll();
	delete PCB::running->myEvent;
	if (PCB::running->sem!=0)
		PCB::running->sem->signal();
	PCB::running->status = 4;

	if (PCB::running->owner!=0)	// SINGAL 1 - SLANJE KREATORU DA JE NIT ZAVRSILA
		PCB::running->owner->myThread->signal(1);

	PCB::running->signalsArrived->emptyList();
	PCB::running->signalsArrived->addSignal(2);		//	SIGNAL 2 - OBRADA SIGNALA
	PCB::running->signalsArrived->signalRoutine();

	unlock();
	dispatch();
}

SignalHandler PCB::defaultZeroSignal = zeroSignalFunction;
// KRAJ DRUGI ZADATAK

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


#ifndef BCC_BLOCK_IGNORE
	stack[stackSz-6] = FP_SEG(PCB::wrapper);
	stack[stackSz-7] = FP_OFF(PCB::wrapper);
	//stack[stackSz-7] = FP_OFF(stack+stackSz-4);	// SACUVAN BP

	stack[stackSz-8] = 0x200;
	stack[stackSz-9] = FP_SEG(PCB::signalRoutine);
	stack[stackSz-10] = FP_OFF(PCB::signalRoutine);

	/*sp = FP_OFF(stack+stackSz-16);
	ss = FP_SEG(stack+stackSz-16);*/
	sp = FP_OFF(stack+stackSz-19);
	ss = FP_SEG(stack+stackSz-19);
	stack[stackSz-19] = FP_OFF(stack+stackSz-5);
#endif
	bp = sp;

	status=0;
	listWaiting = new PCBWaiting();
	listSemaphore = new SemList();
	PCB::listAll->addNewPCB(this);
	PCB::numberOfActive++;

	// DRUGI ZADATAK
	owner = (PCB*)PCB::running;
	if (owner == 0){
		signalStates[0]=0;
		handlers[0] = PCB::defaultZeroSignal;
		for(int i=1; i<16; i++){
			signalStates[i]=0;
			handlers[i]=0;
		}
	}
	else {
		for(int i=0; i<16; i++){
			signalStates[i] = owner->signalStates[i];
			handlers[i] = owner->handlers[i];
		}
	}
	signalsArrived = new SignalList();
	runRoutine=1;
	signalStateChanged=0;
	// KRAJ DRUGI ZADATAK
	myEvent=0;
	sem=0;
	unlock();
}


void PCB::wrapper(){
	PCB::running->myThread->run();
	lock();
	PCB::running->listWaiting->notifyAll();
	PCB::running->status = 4;
	if (PCB::running->owner!=0)	// SINGAL 1 - SLANJE KREATORU DA JE NIT ZAVRSILA
		PCB::running->owner->myThread->signal(1);

	PCB::running->signalsArrived->emptyList();
	PCB::running->signalsArrived->addSignal(2);		//	SIGNAL 2 - OBRADA SIGNALA
	PCB::running->signalsArrived->signalRoutine();
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


void PCB::signalRoutine(){

	PCB::running->signalsArrived->signalRoutine();
	lock();
	PCB::running->runRoutine=0;
	unlock();
}
