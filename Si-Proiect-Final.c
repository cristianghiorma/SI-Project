//On Dragon12+ board, the rows and columns of 4x4 keypad are connected to PORTA. 
//See page 26 of Dragon12+ User's Manual
//As you press any key the ASCII value for the key is placed on the LEDs of PORTB
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <hidef.h>            /* common defines and macros */
#include "mc9s12dg256.h"      /* derivative-specific definitions */
#define LCD_DATA PORTK
#define LCD_CTRL PORTK
#define RS 0x01
#define EN 0x02

void COMWRT4(unsigned char);
void DATWRT4(unsigned char);
void mSDelay(unsigned int);

const unsigned char keypad[4][4] =  //maparea tastaturii
{
'1','2','3','A', //A='+'
'4','5','6','B', //B='-'
'7','8','9','C', //C='/'
'*','0','#','D' //D='=', '#'='Clear'
};
unsigned char column,row;

void main(void){
   int number1=0;	
   int number2=0;	
   int rezultat=0;
   int buffer[10];
   int i=0,j=0,k;
   int aux=0;
   int iesire=0;	//conditie pentru cazuri speciale
   int negativ=0;	//indicator pentru rezultat negativ
   int adunare=0;	
   int scadere=0;	
   int inmultire=0;
   int impartire=0;
   
   DDRB = 0xFF;                           //MAKE PORTB OUTPUT
   DDRJ |= 0x02; 
   PTJ &= ~0x02;                           //ACTIVATE LED ARRAY ON PORT B
   DDRP |= 0x0F;                           //
   PTP |= 0x0F;                            //TURN OFF 7SEG LED
   DDRA = 0x0F;                           //MAKE ROWS INPUT AND COLUMNS OUTPUT 
   DDRK = 0xFF;   
   COMWRT4(0x33);   //reset sequence provided by data sheet
   mSDelay(1);
   COMWRT4(0x32);   //reset sequence provided by data sheet
   mSDelay(1);
   COMWRT4(0x28);   //Function set to four bit data length
                                         //2 line, 5 x 7 dot format
   mSDelay(1);
   COMWRT4(0x06);  //entry mode set, increment, no shift
   mSDelay(1);
   COMWRT4(0x0E);  //Display set, disp on, cursor on, blink off
   mSDelay(1);
   COMWRT4(0x01);  //Clear display
   mSDelay(1);
   COMWRT4(0x80);  //set start posistion, home position
   mSDelay(1);
   while(1){                              //OPEN WHILE(1)
		do{                                 //OPEN do1
			PORTA = PORTA | 0x0F;            //COLUMNS SET HIGH
			row = PORTA & 0xF0;              //READ ROWS
		}while(row == 0x00);                //WAIT UNTIL KEY PRESSED //CLOSE do1



		do{                                 //OPEN do2
			do{                              //OPEN do3
				mSDelay(1);                   //WAIT
				row = PORTA & 0xF0;           //READ ROWS
			}while(row == 0x00);             //CHECK FOR KEY PRESS //CLOSE do3
         
			mSDelay(15);                     //WAIT FOR DEBOUNCE
			row = PORTA & 0xF0;
			}while(row == 0x00);                //FALSE KEY PRESS //CLOSE do2

			while(1){                           //OPEN while(1)
				PORTA &= 0xF0;                   //CLEAR COLUMN
				PORTA |= 0x01;                   //COLUMN 0 SET HIGH
				row = PORTA & 0xF0;              //READ ROWS
				if(row != 0x00){                 //KEY IS IN COLUMN 0
					column = 0;
					break;                        //BREAK OUT OF while(1)
				}
			PORTA &= 0xF0;                   //CLEAR COLUMN
			PORTA |= 0x02;                   //COLUMN 1 SET HIGH
			row = PORTA & 0xF0;              //READ ROWS
			if(row != 0x00){                 //KEY IS IN COLUMN 1
				column = 1;
				break;                        //BREAK OUT OF while(1)
			}

			PORTA &= 0xF0;                   //CLEAR COLUMN
			PORTA |= 0x04;                   //COLUMN 2 SET HIGH
			row = PORTA & 0xF0;              //READ ROWS
			if(row != 0x00){                 //KEY IS IN COLUMN 2
				column = 2;
				break;                        //BREAK OUT OF while(1)
			}
			PORTA &= 0xF0;                   //CLEAR COLUMN
			PORTA |= 0x08;                   //COLUMN 3 SET HIGH
			row = PORTA & 0xF0;              //READ ROWS
			if(row != 0x00){                 //KEY IS IN COLUMN 3
				column = 3;
				break;                        //BREAK OUT OF while(1)
			}
			row = 0;                         //KEY NOT FOUND
			break;                              //step out of while(1) loop to not get stuck
			}                                   //end while(1)

			if(row == 0x10){						
				if (keypad[0][column]=='A') {
					adunare=1;
					number1=aux;
					aux=0;							// se verifica primul rand de taste + cazul apasarii tastei A pentru adunare, caz in care incepe sa se formeze termenul 2 
					DATWRT4('+');
					mSDelay(1);
				}else {
						aux=aux*10+(keypad[0][column]-'0');  	// crearea termenilor
						DATWRT4(keypad[0][column]);							// afisare tasta apasata
						mSDelay(1);
					}
 
				}
			else if(row == 0x20)
			{
				if (keypad[1][column]=='B') 
				{
					scadere=1;
					number1=aux;
					aux=0;	// se verifica al doilea rand de taste + cazul apasarii tastei B pentru scadere, caz in care incepe sa se formeze termenul 2
					DATWRT4('-');
					mSDelay(1);
				} 
				else 
				{
					aux=aux*10+(keypad[1][column]-'0');
					DATWRT4(keypad[1][column]);
					mSDelay(1);
				}
			}
			else if(row == 0x40){
				if (keypad[2][column]=='C') {
					impartire=1;
					number1=aux;
					aux=0;// se verifica al treilea rand de taste + cazul apasarii tastei C pentru impartire, caz in care incepe sa se formeze termenul 2
					DATWRT4('/');
					mSDelay(1);
				}else 
				{
					aux=aux*10+(keypad[2][column]-'0');
					DATWRT4(keypad[2][column]);
					mSDelay(1);
				}
			}
			else if(row == 0x80){
				if (keypad[3][column]=='D') 
				{
					number2=aux;
					DATWRT4('=');						// se verifica cazul apasarii tastei D pentru efectuarea operatiei, caz in care se formeaza rezultatul
					mSDelay(1);
					if(adunare==1)					// daca se indeplineste conditia adunare=1, operatia efectuata este cea de adunare
						rezultat=number1+number2;
					else if (scadere==1)				// daca se indeplineste conditia scadere=1, operatia efectuata este cea de scadere
					{
						rezultat=number1-number2;
						if (rezultat<0) {
							rezultat=rezultat*-1;		// daca rezultatul este negativ, se seteaza indicatorul pe 1 pentru a se afisa semnul '-' in fata rezultatului
							negativ=1;
						}
					}
					else if (inmultire==1)			// daca se indeplineste conditia inmultire=1, operatia efectuata este cea de inmultire
					{
						if (number1==0 || number2==0)				// se trateaza cazul in care unul dintre termeni este 0, se afiseaza 0
						{
							iesire=1;
							COMWRT4(0xC0);
							mSDelay(1);
							DATWRT4('0');
							mSDelay(1);
						}
						else
							rezultat=number1*number2;
					}
					else if (impartire==1)			// daca se indeplineste conditia impartire=1, operatia efectuata este cea de impartire
					{
						if (number2==0)						// daca al doilea termen este 0, atunci se afiseaza mesajul "EROARE"
						{
							iesire=1;
							COMWRT4(0xC0);
							mSDelay(1);
							DATWRT4('E');
							mSDelay(1);
							DATWRT4('R');
							mSDelay(1);
							DATWRT4('O');
							mSDelay(1);
							DATWRT4('A');
							mSDelay(1);
							DATWRT4('R');
							mSDelay(1);
							DATWRT4('E');
							mSDelay(1);
						}
						else
							rezultat=number1/number2;
					}
				if (iesire==0)
				{
					COMWRT4(0xC0);						//se seteaza afisarea pe a doua linie a display-ului LCD
					mSDelay(1);
					if (negativ==1)
					{
						DATWRT4('-');
						mSDelay(1);
					}
					if (rezultat==0)
					{
						DATWRT4('0');
						mSDelay(1);
					}
					while(rezultat!=0)					//se creaza vectorul pentru afisare
					{
						buffer[i++]=rezultat%10;
						rezultat=rezultat/10;
					}
					for (k=i-1; k>=0; k--)				//se afiseaza rezultatul
					{
						DATWRT4(buffer[k]+'0');
						mSDelay(1);
					}
				}
			}
			else if (keypad[3][column]=='*')
			{
				inmultire=1;
				number1=aux;
				aux=0;		// se verifica ultimul rand de taste + cazul apasarii tastei '*' pentru inmultire, caz in care incepe sa se formeze termenul 2
				DATWRT4(keypad[3][column]);
				mSDelay(1);
			}
			else if (keypad[3][column]=='#')
			{
				COMWRT4(0x01);  //Clear display
				mSDelay(1000);
				COMWRT4(0x80);  //set start posistion, home position
				number1=0;	// termenul 1
				number2=0;	// termenul 2 
				rezultat=0;	//rezultatul
				memset(&buffer[10], 0, sizeof(buffer));
				i=0,j=0,k;
				aux=0;
				iesire=0;	//conditie pentru cazuri speciale
				negativ=0;	//indicator pentru rezultat negativ
				adunare=0;	
				scadere=0;	//indicator pentru operatia efectuata
				inmultire=0;
				impartire=0;
			}
			else 
			{
				aux=aux*10+(keypad[3][column]-'0');
				DATWRT4(keypad[3][column]);
				mSDelay(1);
			}
		}
		do{
			mSDelay(15);
			PORTA = PORTA | 0x0F;         //columns set high
			row = PORTA & 0xF0;           //read rows
		}while(row != 0x00);              //check debounce
	}                                   
}                                       
void COMWRT4(unsigned char command)
{
        unsigned char x;
        
        x = (command & 0xF0) >> 2;         //shift high nibble to center of byte for Pk5-Pk2
		LCD_DATA =LCD_DATA & ~0x3C;          //clear bits Pk5-Pk2
        LCD_DATA = LCD_DATA | x;          //sends high nibble to PORTK
        mSDelay(1);
        LCD_CTRL = LCD_CTRL & ~RS;         //set RS to command (RS=0)
        mSDelay(1);
        LCD_CTRL = LCD_CTRL | EN;          //rais enable
        mSDelay(5);
        LCD_CTRL = LCD_CTRL & ~EN;         //Drop enable to capture command
        mSDelay(15);                       //wait
        x = (command & 0x0F)<< 2;          // shift low nibble to center of byte for Pk5-Pk2
        LCD_DATA =LCD_DATA & ~0x3C;         //clear bits Pk5-Pk2
        LCD_DATA =LCD_DATA | x;             //send low nibble to PORTK
        LCD_CTRL = LCD_CTRL | EN;          //rais enable
        mSDelay(5);
        LCD_CTRL = LCD_CTRL & ~EN;         //drop enable to capture command
        mSDelay(15);
}

void DATWRT4(unsigned char data)
{
	unsigned char x;
	x = (data & 0xF0) >> 2;
	LCD_DATA =LCD_DATA & ~0x3C;                     
	LCD_DATA = LCD_DATA | x;
	mSDelay(1);
	LCD_CTRL = LCD_CTRL | RS;
	mSDelay(1);
	LCD_CTRL = LCD_CTRL | EN;
	mSDelay(1);
	LCD_CTRL = LCD_CTRL & ~EN;
	mSDelay(5);
    x = (data & 0x0F)<< 2;
    LCD_DATA =LCD_DATA & ~0x3C;                     
    LCD_DATA = LCD_DATA | x;
    LCD_CTRL = LCD_CTRL | EN;
    mSDelay(1);\
	LCD_CTRL = LCD_CTRL & ~EN;
    mSDelay(15);
}

void mSDelay(unsigned int itime){
unsigned int i; unsigned int j;
   for(i=0;i<itime;i++)
      for(j=0;j<4000;j++);
}