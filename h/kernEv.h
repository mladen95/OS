#ifndef _KERNEV_H_
#define _KERNEV_H_

#include "event.h"

class PCB;

typedef void interrupt (*pInterrupt)(...);

class KernelEv {
public:

	Event *myEvent;
	volatile int value;
	PCB *creator;
	IVTNo ivtNo;
	pInterrupt oldR;

	KernelEv(IVTNo ivtN);
	void wait();
	void signal();
	~KernelEv();
};


class IVTEntry {
public:
	static KernelEv *event[256];
	static IVTEntry *entry[256];

	IVTEntry(IVTNo ivtN, pInterrupt newRoutine);
	~IVTEntry();
	void signal();
	void callOld();

	pInterrupt newR;
	int ivtNo;
};


#define PREPAREENTRY(_num,_callOld);\
	void interrupt inter##_num(...){\
	if(IVTEntry::entry[(unsigned char)_num]){\
		(IVTEntry::entry[(unsigned char)_num])->signal();\
		if(_callOld==1) (IVTEntry::entry[(unsigned char)_num])->callOld();\
	}}\
	IVTEntry newIVTEntry##num((unsigned char)_num, (pInterrupt)inter##_num);

#endif
