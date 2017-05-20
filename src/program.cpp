#include <iostream.h>
#include "pcbList.h"
#include "thread.h"
#include "pcb.h"
#include "ex.h"
#include "intr.h"
#include "kernThr.h"


int main(int argc, char* argv[]){
	inic();
	int i;
	MainT *tr = new MainT(8192,15,&i,argc,argv);
	tr->start();
	cout<<"\nKERNEL NIT START\n";
	tr->waitToComplete();

	restore();
	cout<<"\nKERNEL NIT STOP\n";
	return i;
}
