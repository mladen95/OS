#ifndef _PCB_H_
#define _PCB_H_

#include "thread.h"


class PCBAll;
class PCBWaiting;
class PCBSleep;
class EvList;
class SemList;
class KernelEv;
class KernelSem;
// DODATAK ZA DRUGI ZADATAK
class SignalList;
// KRAJ DODATKA

class PCB {
private:
	ID myID;
public:

	static volatile PCB* running;
	static PCBAll* listAll;
	static PCBSleep* listSleep;
	static PCB* idleThread, *kernelPCB;
	static ID GEN_ID;
	static volatile int numberOfActive;

	// DODATAK ZA DRUGI ZADATAK
	static void signalRoutine();
	static int globalSignalStates[16];
	static SignalHandler defaultZeroSignal;
	PCB* owner;
	SignalHandler handlers[16];
	int signalStates[16];		/*	0 - NEMASKIRAN		1 - BLOKIRAN	2 - MASKIRAN	*/
	SignalList *signalsArrived;
	volatile int runRoutine;	// DA LI JE UMETNUTA FUNKCIJA ZA OBRADU SVIH PRISTIGLIH SIGNALA
	volatile int signalStateChanged;	// DA LI JE U MEDJUVREMENU PROMENJENO STANJE SIGNALA
	// KRAJ DODATKA
	KernelEv *myEvent;
	KernelSem *sem;

	unsigned sp;
	unsigned ss;
	unsigned bp;
	Time kvant;
	unsigned *stack;
	int status;
	/*	0-new
	 * 	1-ready
	 * 	2-blocked
	 * 	3-sleep
	 * 	4-finished
	 */
	StackSize stackSz;
	Thread *myThread;
	PCBWaiting *listWaiting;
	SemList *listSemaphore;

	friend class Thread;


	PCB(Thread* t, StackSize stackSize = defaultStackSize, Time timeSlice = defaultTimeSlice);
	static void wrapper();
	static PCB* getPCBById(ID id);

	ID getID();
	~PCB();
};

#endif
