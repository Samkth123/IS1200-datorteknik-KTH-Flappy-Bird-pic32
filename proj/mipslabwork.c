/* mipslabwork.c

   This file written 2015 by F Lundevall
   Updated 2017-04-21 by F Lundevall

   This file should be changed by YOU! So you must
   add comment(s) here with your name(s) and date(s):

   This file modified 2017-04-31 by Ture Teknolog 

   For copyright and licensing, see file COPYING */

#include <stdint.h>	 /* Declarations of uint_32 and the like */
#include <pic32mx.h>	/* Declarations of system-specific addresses etc */
#include "mipslab.h"	/* Declatations for these labs */
//basically FPS, med 10 blir det trögt så vi kör 300
//by Sam
#define TMR2PERIOD ((80000000 / 256) / 300) /* 100ms */
#if TMR2PERIOD > 0xffff
#error "TimerPeriodIsTooBig"
#endif


//#define TMR2PERIOD ((80000000 / 256) / 10) //initialize tmr2period
//by Sam
int squareX = 15;
int squareY = 10;
//int squareXpixel = 23;
//int squareYpixel = 4;

int obsx = 128;
int obsy = 31;

int obsx1 = 180;
int obsy1 = 0;

int obsx2 =240;
int obsy2 = 0;

int obsx3 = 300;
int obsy3 = 31;
//by Robert
int wallx = 0;
int wally = 0;
//by Sam
int start = 1;

int prime = 1234567;
int mytime = 0x5957;
int timecount = 0;
//by Sam
char starttext[] = "flappycat ";
char starttext1[] = "BTN4 to start";
char starttext2[] = "BTN3 to jump ";
char starttext3[] = "BTN2--> BTN4<-- ";

char gameover1[] = "Game over, klick ";
char gameover2[] = "BTN4 to restart";

volatile int *porte = (volatile int *) 0xbf886110;


/* ISR*/
//kallas varje gång det är interrupt
//by Robert
void user_isr( void )
{
//när interupt kör den user
	IFSCLR(0) = 0x100;
	interupt = 1;
}


/* Lab-specific initialization goes here */
void labinit( void )
{
volatile int *trise = (volatile int*) 0xbf886100;

*trise =*trise &  0xFF00;

TRISD = TRISD & 0x0fe0;

PR2 = TMR2PERIOD;
TMR2 = 0; //kan lägga till vad som, börjar på 0
T2CONSET = 0X8070; //8 är bit för att starta, 70 är för att prescalingen


IEC(0) = 0x100; //set enable

IPC(2) = 0x1f; //set prioritty


enable_interrupt();
  return;
}

/* This function is called repetitively from the main program */
void labwork( void )
{

//by Sam
//start screen
while(start){

display_string(0, starttext);
display_string(1, starttext1);
display_string(2, starttext2);
display_string(3, starttext3);


display_update();

if(getbtns() == 4){
start = 0;
}
}
//by Robert
display_string(0, "");
display_string(3, "");



//by Sam
//standard cat
if(getbtns() != 2){
mark_square1(squareX, squareY);
}
//legs move cat
if(getbtns() == 2){
mark_square2(squareX, squareY);
}
//marking obstacles

//by Robert
mark_obs(obsx, obsy);

mark_obs1(obsx1, obsy1);

mark_obs2(obsx2, obsy2);
mark_obs3(obsx3, obsy3);

//by Robert
mark_wall(wallx, wally);

//sättr allt på standard plats
//by Robert
display_image(0, icon);
//display_update();
//by Sam
resetscreen();

//gravity
//by Robert
squareY = squareY + 1;

//delay annars går allt super snabbt
//by Robert
delay(30);

//obstacles rör sig till vänster hela tiden 
//by Robert
obsx = obsx - 1;
obsx1 = obsx1 - 1;
obsx2 = obsx2 - 1;
obsx3 = obsx3 - 1;




//Resets
//by Sam
if(obsx <= -128) 
{
	obsx = 128; 
}

if(obsx1 <= -128) 
{
	obsx1 = 128; 

}

if(obsx2 <= -128){ 
	obsx2 = 128; 

}

if(obsx3 <= -128){
	obsx3 = 128; 
}
//by Sam
//score
if(squareX == obsx){
	*porte = *porte + 1;
}
if(squareX == obsx1){
	*porte = *porte + 1;
}
if(squareX == obsx2){
	*porte = *porte + 1;
}
if(squareX == obsx3){
	*porte = *porte + 1;
}

//jump
//by Robert
//btn3
if(getbtns() == 2){
squareY = squareY - 3;
}
//by Sam
//move forawrd btn2
if(getbtns()==1){
	squareX = squareX + 1;
}
//by Sam
//move backwards btn 4
if(getbtns()==4){
	squareX = squareX - 1;
}

//by Sam
//collisions, första är ifall den slår i taket elr markeb, resten är obs
//hitboxen ligger på halsen längst fram, därför har vi squareY - 1 och squareX + 8

int whileloop = 1;
if(squareY >= 30 || squareY <= 4 || ( (squareY - 1) >= obsy-14 && (squareX+ 8) == obsx) || 
(((squareY - 1) <= (obsy1 + 10)) && ((squareX + 8) == obsx1)) || (((squareY -1)<=obsy2 + 19) && ((squareX +8)== obsx2)) 
  || (((squareY-1) >= (obsy3 -18)) &&  ((squareX + 8) == obsx3))){
	  //ifall vi har någon kollision in i loopen pga 1
while(whileloop){
		display_string(1, gameover1);
		display_string(2, gameover2);
//display update, annars kommer inte att displayed uppdateras och vi ser all text
		display_update();
		//reseta allt till deras originella positioner, och ut ur loop
		// btn 4
		if(getbtns() == 4){
			*porte = 0;
			
			whileloop = 0;
			squareY = 15;
			squareX = 10;
			obsx = 128;
			obsx1 = 180;
			obsx2 = 240;
			obsx3 = 300;

		}
	}
}

}


