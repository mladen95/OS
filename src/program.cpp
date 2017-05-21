#include <iostream.h>
#include "pcbList.h"
#include "thread.h"
#include "pcb.h"
#include "intr.h"
#include "kernThr.h"

#include "SCHEDULE.H"


int main(int argc, char* argv[]){
	inic();
	int i;
	MainT *tr = new MainT(8192,15,&i,argc,argv);
	tr->start();
	cout<<"\nKERNEL NIT START\n";
	delete tr;

	restore();
	cout<<"\nKERNEL NIT STOP\nPovratna vrednost: "<<i;
	return i;
}
