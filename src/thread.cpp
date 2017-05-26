#include "thread.h"
#include "pcb.h"
#include "pcbList.h"
#include "sigList.h"
#include "intr.h"
#include <iostream.h>
#include <dos.h>

extern volatile int zahtevana_promena_konteksta;
extern volatile int locked;

void stackImplementSignalRoutine(PCB *p);

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
	lock();
	if (myPCB->status != 4){
		myPCB->listWaiting->addNewPCB((PCB*)PCB::running);
		PCB::running->status = 2;
		unlock();
		dispatch();
	}
	else
		unlock();
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



//  ----- DRUGI ZADATAK
void Thread::signal(SignalId signal){
	lock();
	if (myPCB->signalStates[signal]!=2 && PCB::globalSignalStates[signal]!=2){
		myPCB->signalsArrived->addSignal(signal);
		if ((myPCB->signalStates[signal]==0 && PCB::globalSignalStates[signal]==0) && myPCB->status==6){
			myPCB->status = 1;
			Scheduler::put(myPCB);
		}

		if (myPCB->runRoutine==0){
			long int spAddress, stackAddress, spOff;
			spAddress = 0x10*(unsigned long)myPCB->ss + (unsigned long)myPCB->sp;
#ifndef BCC_BLOCK_IGNORE
			stackAddress = 0x10*(unsigned long)FP_SEG(myPCB->stack) + (unsigned long)FP_OFF(myPCB->stack);
#endif
			spOff = (spAddress-stackAddress)/sizeof(unsigned);

			for(int i=0; i<12; i++)
				myPCB->stack[spOff+i-2] = myPCB->stack[spOff+i];
			myPCB->stack[spOff+10] = myPCB->stack[spOff+7];
			myPCB->stack[spOff+11] = myPCB->stack[spOff+8];
#ifndef BCC_BLOCK_IGNORE
			myPCB->stack[spOff+8] = FP_SEG(PCB::signalRoutine);
			myPCB->stack[spOff+7] = FP_OFF(PCB::signalRoutine);
			spOff-=2;
			myPCB->sp = FP_OFF(myPCB->stack+spOff);
			myPCB->ss = FP_SEG(myPCB->stack+spOff);
#endif
			myPCB->bp = myPCB->sp;
			myPCB->runRoutine=1;
		}

	}
	unlock();
	dispatch();
}



void Thread::registerHandler(SignalId signal, SignalHandler handler){
	lock();
	myPCB->handlers[signal] = handler;
	unlock();
}

SignalHandler Thread::getHandler(SignalId signal){
	lock();
	return myPCB->handlers[signal];
	unlock();
}

void Thread::pause(){
	lock();
	PCB::running->status = 6;
	unlock();
	dispatch();
}


void Thread::maskSignal(SignalId signal){
	lock();
	myPCB->signalStates[signal] = 2;
	unlock();
}

void Thread::unmaskSignal(SignalId signal){
	lock();
	myPCB->signalStates[signal] = 0;
	unlock();
}


void Thread::blockSignal(SignalId signal){
	lock();
	myPCB->signalStates[signal] = 1;
	unlock();
}

void Thread::unblockSignal(SignalId signal){
	lock();
	myPCB->signalStates[signal] = 0;
	myPCB->signalStateChanged=1;
	stackImplementSignalRoutine(myPCB);
	unlock();
}


void Thread::maskSignalGlobally(SignalId signal){
	lock();
	PCB::running->globalSignalStates[signal] = 2;
	unlock();
}

void Thread::unmaskSignalGlobally(SignalId signal){
	lock();
	PCB::running->globalSignalStates[signal] = 0;
	unlock();
}


void Thread::blockSignalGlobally(SignalId signal){
	lock();
	PCB::running->globalSignalStates[signal] = 1;
	unlock();
}

void Thread::unblockSignalGlobally(SignalId signal){
	lock();
	PCB::running->globalSignalStates[signal] = 0;
	PCB::listAll->signalStateChangedUpdate();
	unlock();
}


void stackImplementSignalRoutine(PCB *p){
	if (p->runRoutine==0){
		long int spAddress, stackAddress, spOff;
					spAddress = 0x10*(unsigned long)p->ss + (unsigned long)p->sp;
		#ifndef BCC_BLOCK_IGNORE
					stackAddress = 0x10*(unsigned long)FP_SEG(p->stack) + (unsigned long)FP_OFF(p->stack);
		#endif
					spOff = (spAddress-stackAddress)/sizeof(unsigned);

					for(int i=0; i<12; i++)
						p->stack[spOff+i-2] = p->stack[spOff+i];
					p->stack[spOff+10] = p->stack[spOff+7];
					p->stack[spOff+11] = p->stack[spOff+8];
		#ifndef BCC_BLOCK_IGNORE
					p->stack[spOff+8] = FP_SEG(PCB::signalRoutine);
					p->stack[spOff+7] = FP_OFF(PCB::signalRoutine);
					spOff-=2;
					p->sp = FP_OFF(p->stack+spOff);
					p->ss = FP_SEG(p->stack+spOff);
		#endif
					p->bp = p->sp;
					p->runRoutine=1;
	}
}
