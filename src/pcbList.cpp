#include "pcbList.h"
#include "kernSem.h"
#include "intr.h"
#include "SCHEDULE.H"
#include <iostream.h>

class Element;

PCBList::PCBList(){
	first=0;
	last=0;
	counter=0;
}


void PCBList::addNewPCB(PCB *p){
	lock();
	Element *pom = new Element(p);
	if (first==0)
		first = pom;
	else
		last->next = pom;
	last = pom;
	counter++;
	unlock();
}

PCBList::Element* PCBList::getPCBID(ID i){
	Element* tek = first;
	while(tek!=0 || tek->pcb->getID()!=i)	tek = tek->next;
	if (tek!=0)
		return tek;
	return 0;
}


PCB* PCBList::getPCBbyID(ID id){
	Element *tek = first;
	while (tek!=0 && tek->pcb->getID()!=id)	tek = tek->next;
	if (tek!=0)
		return tek->pcb;
	return 0;

}

void PCBList::removePCB(PCB *p){
	lock();
	Element *tek = first, *prev = 0;
	while(tek!=0 && tek->pcb!=p)	{	prev=tek;	tek = tek->next;	}
	if (tek!=0){
		if (tek==first){
			first=first->next;
			if (first==0)
				last==0;
		}
		else if (tek==last){
			last = prev;
			prev->next=0;
		}
		else
			prev->next = tek->next;
		delete tek;
		counter--;
	}
	unlock();
}


void PCBList::ispis(){
	Element *tek = first;
	while(tek!=0){
		cout<<tek->pcb->getID();
		tek = tek->next;
	}
}



Thread* PCBList::getThread(ID id){
	Element *tek = first;
	while(tek!=0 &&  tek->pcb->getID()!=id)	tek = tek->next;
	if (tek!=0)
		return tek->pcb->myThread;
	return 0;
}


PCBList::~PCBList(){
	Element* del;
	while(first!=0){
		del=first;
		first=first->next;
		delete del;
	}
}


PCBAll::~PCBAll(){
	lock();
	Element* del = first;
	while(first!=0){
		del = first;
		first=first->next;
		delete del->pcb;
		delete del;
	}
	unlock();
}



void PCBWaiting::notifyAll(){
	lock();
	Element *tek = first;
	while(tek!=0){
		tek->pcb->status=1;
		Scheduler::put((PCB*)tek->pcb);
		PCB::numberOfReady++;
		tek = tek->next;
	}
	unlock();
}


void PCBSleep::addNewPCB(KernelSem *s, int *r, PCB *p, int sTime){
	lock();
	Element *pom = new Element(p);
	pom->sem = s;
	pom->r = r;
	if (first == 0){	//-----	AKO JE LISTA PRAZNA, UBACITI NA POCETAK	-----
		first = pom;
		pom->sleepTime = sTime;
		last = pom;
	}
	else {	//-----	AKO LISTA NIJE PRAZNA	-----
		Element *tek = first, *prev = 0;
		int s = 0;
		//-----	tek NAM JE MESTO KOD KOG SMO PREKORACILI sTime	-----
		while ((s+=tek->sleepTime)<sTime && tek->next!=0){	prev=tek;	tek = tek->next;	}
		if (tek == first){
			if (sTime<tek->sleepTime){	//-----	UBACIVANJE PRE first ELEMENTA	-----
				first = pom;
				first->next = tek;
				first->sleepTime = sTime;
				tek->sleepTime -= sTime;
			}
			else {	//-----	UBACIVANJE POSLE first ELEMENTA	-----
				pom->next = tek->next;
				tek->next = pom;
				pom->sleepTime = sTime-s;
				if (last==first)
					last = pom;
			}
		}
		else if (tek == last && s<sTime){	//-----	UBACIVANJE POSLE LAST ELEMENTA	-----
			last->next = pom;
			last = pom;
			pom->sleepTime = sTime-s;
		}
		else {	//-----	UBACIVANJE U SREDINI LISTE	-----
			prev->next = pom;
			pom->next = tek;
			pom->sleepTime = sTime-(s-tek->sleepTime);
			if (tek!=0)
				tek->sleepTime -= pom->sleepTime;
		}
	}
	p->status = 3;
	counter++;
	unlock();
	//dispatch();
}


void PCBSleep::ispis(){

	Element *tek = first;
	while(tek!=0){
		cout<<tek->pcb->getID()<<"("<<tek->sleepTime<<")";
		tek = tek->next;
	}
}


void PCBSleep::tickUpdate(){	//POZIVA SE U TIMERU
	if (first!=0){
		first->sleepTime--;
		Element *tek = first;
		while(tek->sleepTime==0 && tek!=0){
			tek->pcb->status = 1;
			Scheduler::put(tek->pcb);
			PCB::numberOfReady++;
			first = tek->next;
			if (tek->sem!=0){
				*(tek->r)=1;
				tek->sem->tickSignal();
				tek->sem->blockedList->removePCB(tek->pcb);
			}
			delete tek;
			tek = first;
			counter--;
		}
		if (first==0)
			last = 0;
	}
}


void PCBSleep::wakePCB(PCB *p){
	Element *tek = first, *prev=0;
	int s=0;
	while(tek!=0 && tek->pcb!=p){
		s+=tek->sleepTime;
		prev=tek;
		tek=tek->next;
	}
	if (tek!=0){
		if (tek!=first){
			prev->next = tek->next;
			if (prev->next!=0)
				prev->next->sleepTime += tek->sleepTime;	//UPDAJTOVANJE VREMENA SLEDECEG CLANA
			else
				last = prev;
		}
		else {
			first = tek->next;
			if (first!=0)
				first->sleepTime += tek->sleepTime;
			else
				last=0;
		}
		delete tek;
	}
}




void PCBBlocked::addNewPCB(KernelSem *s, int *res, PCB *p, int sTime){
	lock();
	Element *newEl = new Element(p);
	newEl->sleepTime = sTime;
	newEl->r = res;
	if (first==0){
		first = newEl;
		last = newEl;
	}
	else {
		last->next = newEl;
		last = newEl;
	}
	if (sTime>0)
		PCB::listSleep->addNewPCB(s, res, p, sTime);
	unlock();
}


void PCBBlocked::deblockS(){
	lock();
	if (first!=0){
		*(first->r) = 2;	//ODBLOKIRAN POMOCU SIGNAL
		PCB* p = first->pcb;
		Element *del = first;
		first = first->next;
		if (first==0)
			last=0;
		p->status=1;
		Scheduler::put(p);
		if (del->sleepTime>0)
			PCB::listSleep->wakePCB(del->pcb);
		delete del;
	}
	unlock();
}

