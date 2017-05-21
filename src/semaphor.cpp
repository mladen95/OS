#include "semaphor.h"
#include "intr.h"
#include "kernSem.h"
#include <iostream.h>

//#define _DEBUG_

Semaphore::Semaphore(int init){
	lock();
#ifdef _DEBUG_
	cout<<endl<<"SEMAPHORE STARTED"<<endl;
#endif
	myImpl = new KernelSem(this, init);
	unlock();
}

int Semaphore::wait(Time maxTimeToWait){
	lock();
#ifdef _DEBUG_
	cout<<endl<<"SEMAPHORE WAIT"<<endl;
#endif
	int pom = myImpl->wait(maxTimeToWait);
	unlock();
	return pom;
}

void Semaphore::signal(){
	lock();
#ifdef _DEBUG_
	cout<<endl<<"SEMAPHORE SIGNAL"<<endl;
#endif
	myImpl->signal();
	unlock();
}

int Semaphore::val() const{
	lock();
#ifdef _DEBUG_
	cout<<endl<<"SEMAPHORE DELETED"<<endl;
#endif
	int pom = myImpl->getVal();
	unlock();
	return pom;
}

Semaphore::~Semaphore(){
	lock();
	delete myImpl;
	unlock();
}
