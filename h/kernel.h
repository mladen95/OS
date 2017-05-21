#ifndef _KERNEL_H_
#define _KERNEL_H_

#include "thread.h"
#include "intr.h"
#include "semaphore.h"
#include "event.h"
#include <iostream.h>
#include <dos.h>

void syncPrintf(char poruka[]){
	lock();
	cout<<poruka;
	unlock();
}

#endif
