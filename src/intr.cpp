#include "intr.h"
#include "pcb.h"
#include "pcbList.h"
#include "kernThr.h"
#include "debug.h"

class PCB;

volatile int zahtevana_promena_konteksta = 0;
volatile int brojac=0;
volatile unsigned int locked=0;

void lock(){
	asm{
		pushf
		cli
	}
	locked++;
	asm popf;
}

void unlock(){
	asm{
		pushf
		cli
	}
	if (locked>0)
		locked--;
	asm popf;
}


unsigned oldTimerOFF, oldTimerSEG;
// postavlja novu prekidnu rutinu
void inictimer(){
#ifndef BCC_BLOCK_IGNORE
	asm{
		cli
		push es
		push ax

		mov ax,0   //  ; inicijalizuje rutinu za tajmer
		mov es,ax

		mov ax, word ptr es:0022h //; pamti staru rutinu
		mov word ptr oldTimerSEG, ax
		mov ax, word ptr es:0020h
		mov word ptr oldTimerOFF, ax

		mov word ptr es:0022h, seg timer	 //postavlja
		mov word ptr es:0020h, offset timer //novu rutinu

		mov ax, oldTimerSEG	 //	postavlja staru rutinu
		mov word ptr es:0182h, ax //; na int 60h
		mov ax, oldTimerOFF
		mov word ptr es:0180h, ax

		pop ax
		pop es
		sti
	}
#endif
}

// vraca staru prekidnu rutinu
void restoretimer(){
#ifndef BCC_BLOCK_IGNORE
	asm {
		cli
		push es
		push ax

		mov ax,0
		mov es,ax


		mov ax, word ptr oldTimerSEG
		mov word ptr es:0022h, ax
		mov ax, word ptr oldTimerOFF
		mov word ptr es:0020h, ax

		pop ax
		pop es
		sti
	}
#endif
}

void tick();

unsigned tbp, tsp, tss;
void interrupt timer(){	// prekidna rutina
	if (!zahtevana_promena_konteksta){
		if (brojac>0)
			brojac--;
		PCB::listSleep->tickUpdate();	//	SLEEP PCB LISTA - BUDJENJE PCB-OVA KOJIMA JE ISTEKAO SLEEP TIME
	}
	if (brojac == 0 || zahtevana_promena_konteksta || PCB::running->status==5) {
		if (locked==0){
			#ifndef BCC_BLOCK_IGNORE
				asm{
					mov tsp, sp
					mov tss, ss
					mov tbp, bp
				}
			#endif

				PCB::running->sp = tsp;
				PCB::running->ss = tss;
				PCB::running->bp = tbp;

#ifdef DEBUG_THREAD
		cout<<endl<<"NIT "<<((PCB*)PCB::running)->getID()<<" NAPUSTA KONTEKST";
#endif

				if (PCB::running->status==1){
					Scheduler::put((PCB*)PCB::running);

				}


				PCB::running = Scheduler::get();
				if (PCB::running==0)
					PCB::running = PCB::idleThread;

				tsp = PCB::running->sp;
				tss = PCB::running->ss;
				tbp = PCB::running->bp;
				brojac = (PCB::running->kvant > 0) ? PCB::running->kvant : -1;
			#ifndef BCC_BLOCK_IGNORE
				asm{
					mov sp, tsp
					mov ss, tss
					mov bp, tbp
				}
			#endif

#ifdef DEBUG_THREAD
		cout<<endl<<"NIT "<<((PCB*)PCB::running)->getID()<<" DOBIJA KONTEKST";
#endif
		}
	}

	// poziv stare prekidne rutine koja se
     // nalazila na 08h, a sad je na 60h
     // poziva se samo kada nije zahtevana promena
     // konteksta – tako se da se stara
     // rutina poziva samo kada je stvarno doslo do prekida
if(!zahtevana_promena_konteksta) {
	tick();
	asm int 60h;
}

	zahtevana_promena_konteksta = 0;
}


void inic(){
	lock();
	inictimer();
	PCB::listAll = new PCBAll();
	PCB::listSleep = new PCBSleep();
	PCB::kernelPCB = new PCB(0, 4096, 0);
	PCB::running = PCB::kernelPCB;
	IdleThread *idle = new IdleThread(4096, 0);
	PCB::idleThread = PCB::getPCBById(idle->getId());
	PCB::idleThread->status=5;
	unlock();
}

void restore(){
	lock();
	restoretimer();
	delete PCB::listAll;
	delete PCB::listSleep;
	delete PCB::idleThread;
	delete PCB::running;
	//delete PCB::kernelPCB;
	unlock();
}
