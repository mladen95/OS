#ifndef _SIGLIST_H_
#define _SIGLIST_H_

#include "pcb.h"


class SignalList {
private:
	class Element {
		int number;
		Element *next;
		friend class SignalList;
		Element(int n){
			number=n;	next=0;
		}
	};

	Element *first, *last;

public:
	SignalList();
	void addSignal(int n);
	void signalRoutine();
	void removeSignal(Element *e);
	void ispis();
	void emptyList();
	~SignalList();
};

#endif
