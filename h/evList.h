#ifndef _EVLIST_H_
#define _EVLIST_H_

#include "kernEv.h"
#include "intr.h"


class EvList {
private:
	class Element {
		KernelEv *kernEvent;
		Element *next;
		friend class EvList;
		Element(KernelEv *k){
			kernEvent=k;	next=0;
		}
	};

	Element *first, *last;

public:
	EvList(){
		first=0;	last=0;
	}
	void add(KernelEv *se){
		lock();
		Element *pom = new Element(se);
		if (first==0)
			first = pom;
		else
			last->next = pom;
		last = pom;
		unlock();
	}
	void remove(KernelEv *ev){
		lock();
		Element *tek = first, *prev = 0;
		while(tek!=0 && tek->kernEvent!=ev)	{	prev=tek;	tek = tek->next;	}
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
	void emptyList(){
		lock();
		Element *del;
		while(first!=0){
			del=first;
			first = first->next;
			delete del->kernEvent;
			delete del;
		}
		unlock();
	}
	~EvList(){
		lock();
		emptyList();
		unlock();
	}
};

#endif
