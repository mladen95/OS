#include "semaphor.h"
#include "intr.h"
#include "kernSem.h"

Semaphore::Semaphore(int init){
	lock();
	myImpl = new KernelSem(this, init);
	unlock();
}

int Semaphore::wait(Time maxTimeToWait){
	lock();
	int pom = myImpl->wait(maxTimeToWait);
	unlock();
	return pom;
}

void Semaphore::signal(){
	lock();
	myImpl->signal();
	unlock();
}

int Semaphore::val() const{
	lock();
	int pom = myImpl->getVal();
	unlock();
	return pom;
}

Semaphore::~Semaphore(){
	lock();
	delete myImpl;
	unlock();
}
