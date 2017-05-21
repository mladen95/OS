#include "event.h"
#include "kernEv.h"
#include "intr.h"
#include <iostream.h>

//#define _DEGUB_SEMAPHORE

Event::Event(IVTNo ivtNo){
	lock();
	myImpl = new KernelEv(ivtNo);
	unlock();
}

void Event::wait(){
	lock();
#ifdef _DEGUB_SEMAPHORE
	cout<<endl<<"EVENT WAIT"<<endl;
#endif
	myImpl->wait();
	unlock();
}

void Event::signal(){
	lock();
#ifdef _DEGUB_SEMAPHORE
	cout<<endl<<"EVENT SIGNAL"<<endl;
#endif
	myImpl->signal();
	unlock();
}


Event::~Event(){
	lock();
	delete myImpl;
	unlock();
}
