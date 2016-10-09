/*
 * timer1.c
 *
 *  Created on: 6 okt. 2016
 *      Author: Acer
 */
#define OUTPUTPIN 0x00200000
#define NOTOUTPUTPIN 0xFFDFFFFF

#define FIO0DIR (*(unsigned int *)0x2009C000)	//GPIO direction
#define FIO0SET (*(unsigned int *)0x2009C018)	//GPIO set (can make pin high)
#define FIO0CLR (*(unsigned int *)0x2009C01C)	//GPIO clear (can make pin low)
#define FIO0PIN (*(unsigned int *)0x2009C014)	//GPIO value (can read and write pin) (write careful! Will effect whole port)

#define T0 0x40004000				//timer0
#define T0IR (*(int *)(T0))			//Interrupt register
#define T0CR (*(int *)(T0 + 0x04))	//control register
#define T0TC (*(int *)(T0 + 0x08))	//Timer Counter register
#define T0PR (*(int *)(T0 + 0x0C))	//Prescaler
#define T0MR0 (*(int *)(T0 + 0x18))	//Match register 0
#define T0MRC (*(int *)(T0 + 0x14))	//match control register
#define ISER0 (*(int *)(0xE000E100)) //Interrupt Set-Enable Register 0

/*
 * NOTE:
 * To use timer 3/4, you need to enable it.
 * This can be done by setting PCTIM2/3 in the PCONP
 */

void ledsInit() {
	size = 0;

	//set output pin to output mode
	FIO0DIR |= OUTPUTPIN;
}

void ledsSetData(unsigned char* data, int size) {
	for (int i = 0; i < size; ++i) {
		int line = data[i];
		for (int j = 0; j < 8; ++j) {
			FIO0PIN &= NOTOUTPUTPIN;
			if (line & 0x1) {
				//about 480ns
				for (int time = 0; time < 5; ++time) {
					asm("NOP");
					asm("NOP");
				}

				//pin LOW
				FIO0PIN |= OUTPUTPIN;

				//wait LOW time ()
				for (int time = 0; time < 0; ++time) {
					//asm("NOP");
				}

				//if last item, reserve some time to go to next item
				if (j < 7) {
					for (int time = 0; time < 3; ++time) {
						asm("NOP");
					}
				} else {
					for (int time = 0; time < 0; ++time) {
					}
				}

			} else {
				for (int time = 0; time < 1; ++time) {
					asm("NOP");
				}

				FIO0PIN |= OUTPUTPIN;

				for (int time = 0; time < 3; ++time) {
					asm("NOP");
				}

				//if last item, reserve some time to go to next item
				if (j < 7) {
					for (int time = 0; time < 3; ++time) {
						asm("NOP");
					}
				} else {
					for (int time = 0; time < 0; ++time) {
					}
				}
			}
			line >>= 1;
		}
	}
}

