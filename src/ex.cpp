//#include "ex.h"
//#include "semaphor.h"
//
//Semaphore *s = new Semaphore(0);
//void userMain(){
//	cout<<"\nUSER MAIN START\n";
//	Ex1 *a = new Ex1(4096, 1);
//	Ex2 *b = new Ex2(4096, 5);
//	Ex3 *c = new Ex3(4096, 4);
//	a->start();		b->start();	c->start();
//
//	for(long i=0; i<100000000; i++)
//		i=i+1-1;
//	//PCB::running->listWaiting->ispis();
//	//Semaphore s = new Semaphore();
//
//
//	a->waitToComplete();
//	b->waitToComplete();
//	c->waitToComplete();
//
//	//PCB::listSleep->wakePCB(PCB::getPCBById(b->getId()));
//
//	cout<<"\n";
//	//PCB::listSleep->addNewPCB((PCB*)PCB::running, 35);
//	PCB::listSleep->ispis();
//	cout<<"\nUSER MAIN END\n";
//}
//
//////////////////////////////////////////////////////
//
//
//void Ex2::run(){
//	cout<<"\EX2 START\n";
//			/*for (long k = 0; k<100000; ++k)
//						for (long j = 0; j <10000; ++j);*/
//			//dispatch();
//			//PCB::listSleep->addNewPCB((PCB*)PCB::running, 25);
//			PCB::listSleep->ispis();
//			s->signal();
//			cout<<"\EX2 END\n";
//}
//
//
//void Ex3::run(){
//	cout<<"\EX3 START\n";
//			/*for (long k = 0; k<100000; ++k)
//						for (long j = 0; j <10000; ++j);*/
//			//dispatch();
//			//PCB::listSleep->addNewPCB((PCB*)PCB::running, 60);
//
//			cout<<s->wait(70);
//			cout<<"\EX3 END\n";
//}
