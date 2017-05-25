#ifndef _PCBLIST_H_
#define _PCBLIST_H_

#include "pcb.h"

class PCB;
class KernelSem;

class PCBList {
public:
	class Element {
			friend class PCBAll;
			friend class PCBList;
			friend class PCBWaiting;
			friend class PCBSleep;
			friend class PCBBlocked;
			PCB *pcb;
			int sleepTime;
			KernelSem *sem;	//KERNELSEM KLASA
			int *r;		//POKAZIVAC NA res, DA LI JE ODBLOKIRANA TIMEROM (1), ILI SA SIGNAL(2)
			Element *next;
			Element* getE(){	return this;	}
			Element(PCB *p){
				pcb=p;	next=0;	sleepTime=0;	sem=0;	r=0;
			}
		};

	Element *first, *last;
	int counter;

	PCBList();
	void addNewPCB(PCB *p);
	Element* getPCBID(ID i);
	void removePCB(PCB *p);

	PCB* getPCBbyID(ID id);


	Thread* getThread(ID id);

	virtual void ispis();

	virtual ~PCBList();
};


class PCBAll : public PCBList {
public:
	virtual ~PCBAll();
};

class PCBWaiting : public PCBList {
public:

	void notifyAll();
};

class PCBSleep : public PCBList {
public:

		void addNewPCB(KernelSem *s, int *r, PCB *p, int sTime);
		virtual void ispis();
		void tickUpdate();
		void wakePCB(PCB *p);
};



class PCBBlocked : public PCBList {
public:
	void addNewPCB(KernelSem *s, int *res, PCB *p, int sTime);
	void deblockS();
};




#endif
