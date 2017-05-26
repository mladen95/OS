#include "sigList.h"
#include "intr.h"
#include <iostream.h>

SignalList::SignalList(){
	lock();
	first=0;	last=0;
	unlock();
}


void SignalList::addSignal(int n){
	lock();
	Element *pom = new Element(n);
	if (first==0)
		first = pom;
	else
		last->next = pom;
	last = pom;
	unlock();
}


SignalList::~SignalList(){
	lock();
	Element *del;
	while(first!=0){
		del=first;
		first = first->next;
		delete del;
	}
	unlock();
}

void SignalList::removeSignal(Element *e){
	lock();
	Element *tek = first, *prev = 0;
	while(tek!=0 && tek!=e)	{	prev=tek;	tek = tek->next;	}
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
	}
	unlock();
}


void SignalList::signalRoutine(){
	Element *tek = first;
	while(tek!=0){
		lock();
		/*if (PCB::running->signalStateChanged==1){
			tek=first;
			PCB::running->signalStateChanged=0;
		}*/
		if (PCB::running->signalStates[tek->number]==0 && PCB::running->globalSignalStates[tek->number]==0 && PCB::running->handlers[tek->number]!=0){
			PCB::running->handlers[tek->number]();
			Element *del = tek;
			tek=tek->next;
			removeSignal(del);
		}
		else
			tek=tek->next;
		unlock();
	}
}


void SignalList::emptyList(){
	lock();
	Element *del;
	while(first!=0){
		del=first;
		first = first->next;
		delete del;
	}
	first=last=0;
	unlock();
}


void SignalList::ispis(){
	lock();
	Element *e = first;
	while(e!=0){
		cout<<"("<<e->number<<")";
		e=e->next;
	}
	unlock();
}
