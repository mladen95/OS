#ifndef _KERNSEM_H_
#define _KERNSEM_H_

#include "semaphor.h"
#include "pcbList.h"


class KernelSem {
public:
	Semaphore* mySem;
	int val;
	PCBBlocked *blockedList;

	KernelSem(Semaphore *sem, int init);
	int wait(Time maxTimeToWait);
	void signal();
	void tickSignal();
	int getVal();
	~KernelSem();
};

#endif
