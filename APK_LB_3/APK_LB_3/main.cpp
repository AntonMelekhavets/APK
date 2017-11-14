#include <dos.h>
#include <stdio.h>

struct VIDEO
{
	unsigned char symb;
	unsigned char attr;
};

void get_reg();         	//get data from registres
void print(int);    //fill in the screen

//IRQ0-7
void interrupt (*int8) (...);
void interrupt (*int9) (...);

// new interrupt handlers
void interrupt  int60(...) { get_reg(); int8(); }
void interrupt  int61(...) { get_reg(); int9(); }

void print(int val)
{
	char temp;
	int i;
	VIDEO far* screen = (VIDEO far *)MK_FP(0xB800, 0);

	for(i=7;i>=0;i--)        //find bits
	{
		temp=val%2;
		val/=2;         //fill the screen
		screen->symb=temp+'0';
		screen->attr=0x5E;
		screen++;
	}
}

// get data from registers
void get_reg()
{
	outp(0x20, 0x0B);//master service register
	print(inp(0x20));
}

void init()
{
	//IRQ0-7
	int8 = getvect(0x08); // timer
	int9 = getvect(0x09); // keyboard

	setvect(0x70, int60); // 70-77
	setvect(0x71, int61);

	_disable(); // disable interrupts handling (cli)

	/* MASTER */
	outp(0x20, 0x11);	//ICW1
	outp(0x21, 0x70);       //ICW2
	outp(0x21, 0x04);	//ICW3
	outp(0x21, 0x01);	//ICW4

	_enable(); // enable interrupt handling (sti)
}

int main()
{
	unsigned far *fp;
	init();

	FP_SEG (fp) = _psp; // segment
	FP_OFF (fp) = 0x2c; // offset
	_dos_freemem(*fp);

	_dos_keep(0,(_DS -_CS)+(_SP/16)+1);//TSR
	return 0;
}
}