#include "ex.h"
#include "thread.h"
#include "pcb.h"
#include "sigList.h"
#include "event.h"
#include "semaphor.h"
#include "semList.h"
#include <iostream.h>
void fufu(){
	cout<<endl<<"PODNIT SE ZAVRSILA";
}
void fufu1(){
	cout<<endl<<"TRENUTNA NIT SE ZAVRSILA";
}
Semaphore *s = new Semaphore(1);
Ex1 *a = new Ex1();
	Ex2 *b = new Ex2();
void Ex1::run(){
	cout<<"\EX1 START\n";
			/*for (long k = 0; k<100000; ++k)
								for (long j = 0; j <10000; ++j);*/
			//PCB::listSleep->addNewPCB((PCB*)PCB::running, 10);
			//cout<<"ID :"<<getId();
	s->wait(0);
	dispatch();

	s->signal();
			cout<<"\nEX1 END\n";
}

void Ex2::run(){
	cout<<"\EX2 START\n";
				/*for (long k = 0; k<100000; ++k)
									for (long j = 0; j <10000; ++j);*/
				//PCB::listSleep->addNewPCB((PCB*)PCB::running, 10);
				//cout<<"ID :"<<getId();

		a->registerHandler(5, fufu1);
		a->signal(0);
		s->wait(0);
		cout<<"\nETTT\n";
		s->signal();
				cout<<"\nEX2 END\n";
}
/*
void tick(){

}
*/
void fun(){
		cout<<"ALLOOO1111";
	}
/*int userMain(int argc, char* argv[]){
	cout<<"USER\n";



	b->start(); a->start();

	delete a;
	//delete b;
	cout<<s->val();
	cout<<"END USER\n";
	return 15;
}*/
