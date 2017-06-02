#include "event.h"
#include "kernEv.h"
#include "intr.h"
#include <iostream.h>


Event::Event(IVTNo ivtNo){
	lock();
	myImpl = new KernelEv(ivtNo);
	unlock();
}

void Event::wait(){
	lock();
	myImpl->wait();
	unlock();
}

void Event::signal(){
	lock();
	myImpl->signal();
	unlock();
}


Event::~Event(){
	lock();
	delete myImpl;
	unlock();
}
