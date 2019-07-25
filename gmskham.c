#include <y:\picc\examples\16c63a.h>
#include <y:\picc\examples\ctype.h>

#use delay (clock=20000000)
#use rs232 (RESTART_WDT,ERRORS, baud=9600, BITS=8,xmit=PIN_C6,rcv=PIN_C7)
#USE FIXED_IO (c_outputs=PIN_C5)

#define CTS	PIN_C5
#define RTS	PIN_C4

#define CLOCK	PIN_A0
#define DATOS_TX PIN_A1

void main (void)
{
	int n;
	char dato;

	output_bit (DATOS_TX,FALSE);	//Bit START

	while (TRUE)
	{
		if (input (RTS) && !input (CTS)) //Activan RTS y está CTS desactivado
		{
			delay_ms (3);
			output_bit (CTS,TRUE);
		}

		if (!input (RTS) && input(CTS)) //Desactivan RTS y está CTS activado
		{
			output_bit (CTS,FALSE);
		}

		if (kbhit() && input (CTS))	//Llega byte de la UART y el CTS está activo
		{
			dato=getch();

			output_bit (DATOS_TX,FALSE);	//Bit START
			while (input (CLOCK));
			while (!input(CLOCK));
			for (n=0;n<8;n++)
			{
				output_bit (DATOS_TX,shift_right(&dato,1,0));
				while (input (CLOCK));
				while (!input(CLOCK));
			}
			output_bit (DATOS_TX,1);	//STop      
			while (input (CLOCK));
			while (!input (CLOCK));
		}
	}
}