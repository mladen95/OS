#ifndef _INTR_H_
#define _INTR_H_

void lock();
void unlock();

void inictimer();
void restoretimer();

void interrupt timer();

void inic();	//INICIJALIZACIJA KERNELA

void restore();	//GASENJE SISTEMA

#endif
