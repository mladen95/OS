#include "kernEv.h"
#include "intr.h"
#include "pcb.h"
#include "SCHEDULE.H"
#include <dos.h>

KernelEv* IVTEntry::event[256] = {0};
IVTEntry* IVTEntry::entry[256] = {0};

KernelEv::KernelEv(IVTNo ivtN){
	lock();
	creator = (PCB*)PCB::running;
	value=0;
	ivtNo = ivtN;
	IVTEntry::event[ivtN] = this;
#ifndef BCC_BLOCK_IGNORE
	oldR = getvect(ivtN);
	setvect(ivtN, IVTEntry::entry[ivtN]->newR);
#endif
	unlock();
}

KernelEv::~KernelEv(){
	lock();
	IVTEntry::event[ivtNo] = 0;
#ifndef BCC_BLOCK_IGNORE
	setvect(ivtNo, oldR);
#endif
	unlock();
}


void KernelEv::signal(){
	if (value==0){
		if (creator->status==2){
			creator->status=1;
			Scheduler::put(creator);
			unlock();
			dispatch();
			//lock();
		}
		else{
			value=1;
			//unlock();
		}

	}
}

void KernelEv::wait(){
	if (creator == PCB::running){
		if (value==1){
			value=0;
		}
		else {
			creator->status = 2;
			unlock();
			dispatch();
			//lock();
		}
	}
}


IVTEntry::IVTEntry(IVTNo ivtN, pInterrupt newRoutine){
	lock();
	ivtNo = ivtN;
	newR = newRoutine;
	IVTEntry::entry[ivtNo] = this;
	unlock();
}

void IVTEntry::signal(){
	if(IVTEntry::event[ivtNo])
		IVTEntry::event[ivtNo]->signal();
}

void IVTEntry::callOld(){
	if(IVTEntry::event[ivtNo])
			IVTEntry::event[ivtNo]->oldR();
}

IVTEntry::~IVTEntry(){
	IVTEntry::entry[ivtNo]=0;
}
