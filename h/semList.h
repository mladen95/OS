#ifndef _SEMLIST_H_
#define _SEMLIST_H_

#include "kernSem.h"
#include "intr.h"
#include <iostream.h>


class SemList {
private:
	class Element {
		KernelSem *kernSemaphore;
		Element *next;
		friend class SemList;
		Element(KernelSem *k){
			kernSemaphore=k;	next=0;
		}
	};

	Element *first, *last;

public:
	SemList(){
		first=0;	last=0;
	}
	void add(KernelSem *se){
		lock();
		Element *pom = new Element(se);
		if (first==0)
			first = pom;
		else
			last->next = pom;
		last = pom;
		unlock();
	}
	void remove(KernelSem *se){
		lock();
		Element *tek = first, *prev = 0;
		while(tek!=0 && tek->kernSemaphore!=se)	{	prev=tek;	tek = tek->next;	}
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
			delete del->kernSemaphore;
			delete del;
		}
		unlock();
	}
	~SemList(){
		lock();
		emptyList();
		unlock();
	}

	void ispis(){
		lock();
		Element *e = first;
		while(e!=0){
			cout<<"("<<e->kernSemaphore->val<<")";
			e=e->next;
		}
		unlock();
	}
};

#endif
