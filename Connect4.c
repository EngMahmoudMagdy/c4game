// SpaceInvaders.c
// Runs on LM4F120/TM4C123
// Jonathan Valvano and Daniel Valvano
// This is a starter project for the edX Lab 15
// In order for other students to play your game
// 1) You must leave the hardware configuration as defined
// 2) You must not add/remove any files from the project
// 3) You must add your code only this this C file
// I.e., if you wish to use code from sprite.c or sound.c, move that code in this file
// 4) It must compile with the 32k limit of the free Keil

// April 10, 2014
// http://www.spaceinvaders.de/
// sounds at http://www.classicgaming.cc/classics/spaceinvaders/sounds.php
// http://www.classicgaming.cc/classics/spaceinvaders/playguide.php
/* This example accompanies the books
   "Embedded Systems: Real Time Interfacing to Arm Cortex M Microcontrollers",
   ISBN: 978-1463590154, Jonathan Valvano, copyright (c) 2013

   "Embedded Systems: Introduction to Arm Cortex M Microcontrollers",
   ISBN: 978-1469998749, Jonathan Valvano, copyright (c) 2013

 Copyright 2014 by Jonathan W. Valvano, valvano@mail.utexas.edu
    You may use, edit, run or distribute this file
    as long as the above copyright notice remains
 THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 VALVANO SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL,
 OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 For more information about my classes, my research, and my books, see
 http://users.ece.utexas.edu/~valvano/
 */
// ******* Required Hardware I/O connections*******************
// Slide pot pin 1 connected to ground
// Slide pot pin 2 connected to PE2/AIN1
// Slide pot pin 3 connected to +3.3V
// fire button connected to PE0
// special weapon fire button connected to PE1
// 8*R resistor DAC bit 0 on PB0 (least significant bit)
// 4*R resistor DAC bit 1 on PB1
// 2*R resistor DAC bit 2 on PB2
// 1*R resistor DAC bit 3 on PB3 (most significant bit)
// LED on PB4
// LED on PB5

// Blue Nokia 5110
// ---------------
// Signal        (Nokia 5110) LaunchPad pin
// Reset         (RST, pin 1) connected to PA7
// SSI0Fss       (CE,  pin 2) connected to PA3
// Data/Command  (DC,  pin 3) connected to PA6
// SSI0Tx        (Din, pin 4) connected to PA5
// SSI0Clk       (Clk, pin 5) connected to PA2
// 3.3V          (Vcc, pin 6) power
// back light    (BL,  pin 7) not connected, consists of 4 white LEDs which draw ~80mA total
// Ground        (Gnd, pin 8) ground

// Red SparkFun Nokia 5110 (LCD-10168)
// -----------------------------------
// Signal        (Nokia 5110) LaunchPad pin
// 3.3V          (VCC, pin 1) power
// Ground        (GND, pin 2) ground
// SSI0Fss       (SCE, pin 3) connected to PA3
// Reset         (RST, pin 4) connected to PA7
// Data/Command  (D/C, pin 5) connected to PA6
// SSI0Tx        (DN,  pin 6) connected to PA5
// SSI0Clk       (SCLK, pin 7) connected to PA2
// back light    (LED, pin 8) not connected, consists of 4 white LEDs which draw ~80mA total

#include "tm4c123gh6pm.h"
#include "Nokia5110.h"
#include "Random.h"
#include "TExaS.h"
#include <string.h>
#include <stdlib.h>
#include <math.h>


// *************************** Images ***************************
// width=16 x height=10

#define BOARD_ROWS 6
#define BOARD_COLS 7

const unsigned char logo1[] ={
 0x42, 0x4D, 0x4A, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x76, 0x00, 0x00, 0x00, 0x28, 0x00, 0x00, 0x00, 0x38, 0x00, 0x00, 0x00, 0x23, 0x00, 0x00, 0x00, 0x01, 0x00, 0x04, 0x00, 0x00, 0x00,
 0x00, 0x00, 0xD4, 0x03, 0x00, 0x00, 0xC4, 0x0E, 0x00, 0x00, 0xC4, 0x0E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80,
 0x00, 0x00, 0x00, 0x80, 0x80, 0x00, 0x80, 0x00, 0x00, 0x00, 0x80, 0x00, 0x80, 0x00, 0x80, 0x80, 0x00, 0x00, 0x80, 0x80, 0x80, 0x00, 0xC0, 0xC0, 0xC0, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0xFF,
 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x88, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x88, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x88, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x88, 0x80, 0x00, 0x00, 0x07, 0x77, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x88, 0x80, 0x00, 0x00, 0x08, 0xF8, 0xF7, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x08, 0x88, 0x80, 0x00, 0x00, 0x88, 0x88, 0x88, 0x00, 0x03, 0x00, 0x00, 0x08, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x88, 0x88,
 0x88, 0x00, 0x88, 0x88, 0x8F, 0x00, 0x8F, 0xF7, 0x00, 0x0F, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x88, 0x88, 0x88, 0x80, 0x78, 0x88,
 0xFF, 0x07, 0xFF, 0xFF, 0x00, 0x08, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x88, 0x88, 0x88, 0x80, 0x07, 0x8F, 0xF0, 0x07, 0xF8, 0x7F,
 0x00, 0x88, 0x80, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x88, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0xF7, 0x0F, 0x00, 0x88, 0x30, 0x88,
 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x88, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0xF7, 0x08, 0x00, 0x88, 0x88, 0x8F, 0x70, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x88, 0x88, 0x00, 0x00, 0x00, 0x07, 0x88, 0x70, 0x0F, 0xF0, 0x08, 0x08, 0x80, 0x0F, 0x87, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x88, 0x88, 0x80, 0x00, 0x00, 0x78, 0x88, 0x88, 0x0F, 0xF0, 0x0F, 0x08, 0x80, 0x07, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x88, 0x88, 0x88, 0x88, 0x00, 0xFF, 0x88, 0x8F, 0x00, 0x00, 0x08, 0x08, 0x80, 0x07, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x88, 0x88,
 0x88, 0x00, 0x88, 0x88, 0xF8, 0x00, 0x00, 0x00, 0x08, 0x88, 0x7F, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x88,
 0xF7, 0x00, 0x00, 0x00, 0x00, 0x88, 0x88, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x77, 0x70, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x28, 0x87, 0x70, 0x00, 0x00, 0x00, 0x00, 0x08, 0x80, 0x07, 0x07, 0x70, 0x08, 0x70,
 0x7F, 0x8F, 0x70, 0x07, 0x78, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x78, 0x88, 0x88, 0xF8, 0x70, 0x00, 0x00, 0x00, 0x08, 0x80, 0x08, 0x07, 0x80, 0x88, 0x70, 0x7F, 0x87, 0x70, 0x88,
 0x88, 0x80, 0x00, 0xF7, 0x00, 0x00, 0x00, 0x00, 0x07, 0xFF, 0xFF, 0x77, 0x78, 0x70, 0x00, 0x88, 0x73, 0x0F, 0x80, 0x88, 0x07, 0x70, 0x88, 0x70, 0x78, 0x00, 0x08, 0xF8, 0x77, 0x80, 0x00, 0xF7,
 0x00, 0x00, 0x00, 0x00, 0x07, 0xF8, 0x87, 0x00, 0x07, 0x70, 0x08, 0x88, 0x88, 0x08, 0x88, 0x88, 0x07, 0x78, 0x88, 0x70, 0x7F, 0x88, 0x08, 0xF7, 0x00, 0x00, 0x00, 0xF7, 0x00, 0x00, 0x00, 0x00,
 0x77, 0x88, 0x87, 0x00, 0x00, 0x70, 0x88, 0x88, 0x88, 0x08, 0x88, 0x88, 0x07, 0x88, 0x87, 0x70, 0x7F, 0xF8, 0x08, 0xF7, 0x00, 0x00, 0x00, 0xF7, 0x00, 0x00, 0x00, 0x00, 0x68, 0x88, 0x70, 0x00,
 0x00, 0x00, 0x88, 0x88, 0x88, 0x08, 0x88, 0x88, 0x07, 0x88, 0x00, 0x70, 0x88, 0x70, 0x08, 0xF7, 0x00, 0x20, 0x00, 0xF7, 0x00, 0x00, 0x00, 0x00, 0x68, 0x88, 0x70, 0x00, 0x00, 0x00, 0x08, 0x88,
 0x87, 0x08, 0x88, 0x08, 0x08, 0x87, 0x00, 0x80, 0xF8, 0x77, 0x08, 0xF7, 0x33, 0x00, 0x08, 0xF8, 0x70, 0x00, 0x00, 0x00, 0x68, 0x88, 0x70, 0x00, 0x00, 0x00, 0x00, 0xF8, 0x00, 0x08, 0x80, 0x08,
 0x07, 0xF7, 0x03, 0x80, 0x7F, 0x88, 0x00, 0xFF, 0x78, 0x00, 0x8F, 0xFF, 0xF8, 0x00, 0x00, 0x00, 0x78, 0x88, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x08, 0x00, 0x80, 0x00, 0x00,
 0x8F, 0xFF, 0x70, 0x8F, 0xFF, 0x00, 0xFF, 0x88, 0xF7, 0x00, 0x00, 0x00, 0x08, 0x8F, 0xF7, 0x00, 0x06, 0x70, 0x00, 0x08, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x77, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xFF, 0xF7, 0x00, 0x07, 0x70, 0x78, 0x8F, 0x87, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x07, 0xFF, 0x8E, 0x70, 0x68, 0x00, 0x78, 0x88, 0x88, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x0F, 0x8E, 0x88, 0xF8, 0x00, 0x78, 0x88, 0x88, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x78, 0x87,
 0x70, 0x00, 0x7F, 0x88, 0x8F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0xFF,
 0x84, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF,

};


void printBoard(char *board);
int takeTurn(char *board, int player, const char*);
int takeTurnRemote(char *board, int player, const char*);
int takeTurnAI(char *board, int player, const char*);
int changeBoard(char *board,int player , const char *PIECES , int col );
int checkWin(char *board);
int checkFour(char *board, int, int, int, int);
int horizontalCheck(char *board);
int verticalCheck(char *board);
int diagonalCheck(char *board);
void Delay100ms(unsigned long count); // time delay in 0.1 seconds


#define GPIO_PORTF_DATA_R       (*((volatile unsigned long *)0x400253FC))
#define GPIO_PORTF_DIR_R        (*((volatile unsigned long *)0x40025400))
#define GPIO_PORTF_AFSEL_R      (*((volatile unsigned long *)0x40025420))
#define GPIO_PORTF_PUR_R        (*((volatile unsigned long *)0x40025510))
#define GPIO_PORTF_DEN_R        (*((volatile unsigned long *)0x4002551C))
#define GPIO_PORTF_LOCK_R       (*((volatile unsigned long *)0x40025520))
#define GPIO_PORTF_CR_R         (*((volatile unsigned long *)0x40025524))
#define GPIO_PORTF_AMSEL_R      (*((volatile unsigned long *)0x40025528))
#define GPIO_PORTF_PCTL_R       (*((volatile unsigned long *)0x4002552C))
#define SYSCTL_RCGC2_R          (*((volatile unsigned long *)0x400FE108))
#define SYSCTL_RCGC2_GPIOF      0x00000020  // port F Clock Gating Control


unsigned long SW1,SW2;  // input from PF4,PF0
unsigned long Out;      // outputs to PF3,PF2,PF1 (multicolor LED)
void PortF_Init(void);

void UARTB_init(void);
char UARTB_InChar(void) ;
void UARTB_OutChar( char data);
void UARTB_outString(char* buffer) ;
int selectMode(void); 
void startingScreen(void);


int r = 0 , rr = 0 , uturn = 0 , f = 0 , i = 0 , j = 0 ;
int turn=0, done = 0;
const char *PIECES = "XO";
char board1[(BOARD_ROWS * BOARD_COLS)+1];
unsigned int seed = 25 ; 

//AI CODE HERE : 
int provocation = 0; // used to display a provocative screen
char input[43]; // There are 42 places to play in the board .. this array represent them
int PlayOut = 0;
int EVA = 0;

int winning(void);
int GetValue(int t);
int AIManager(void);
int NegaMax(int Depth);
void Board(void);

void clean()
{
	int i; 
    provocation = 0;
    for(i = 0 ; i<= 43 ; i++)
        input[i]=' ';
}

int GetValue(int column) // pass this function a column that you want to play in and it will return its value in input array ..
{
		int i; 
		int n;
    if(column > 7)
        return 0;
    
    for(i = 0 ; i<= 6 ; i++)
    {
        if( input[column+7*i] == ' '  )
        {
            n = column+7*i;
            break;
        }
    }
    if ( n > 42 )
        return 0;
    return n;
}

void Board() // Draw board
{

    int j = 42;
		int i , k , h = 0 ;
    for( i = 0 ; i<= 23 ; i++)
    {		
			 if(i%4!=0)
        {
            if( (i - 2) % 4 == 0)
            {
                j=42-(0.25*i+0.5)*6-((0.25*i+0.5)-1) ;
                for(k = 0 ; k<=6 ; k++)
                {
                        Nokia5110_SetCursorChar(k,h,input[j]);
                        j++;
                }
								h++;
            }
        }
    }
    
}


int winning() // Winning algorithm
{
    int temp=0;
		int i ;
    for( i = 1 ; i<= 42 ; i++)
    {
        if(input[i] != ' ')
        {
            temp++;
            if( i - floor((i-1)/7) * 7 <= 4  )
                if( input[i] == input [i+1] && input[i] == input[i+2] && input[i] == input[i+3] )
								{if(input[i] == 'X' )
                        return 1 ;
                    else
                        return 2;}
            if( i <= 21 )
                if ( input[i] == input[i+7] && input[i] == input[i+14] && input[i] == input[i+21]  )
								{if(input[i] == 'X' )
                        return 1 ;
                    else
                        return 2;}
            if( i - floor((i-1)/7) * 7 <= 4 && i<=18  )
                if(input[i] == input[i+8] && input[i] == input[i+16] && input[i]==input[i+24])
								{if(input[i] == 'X' )
                        return 1 ;
                    else
                        return 2;}
            if( i - floor((i-1)/7) * 7 >= 4 && i <= 21   )
                if(input[i] == input[i+6] && input[i] == input[i+12] && input[i]==input[i+18])
								{if(input[i] == 'X' )
										{return 1 ;}
                    else
										{return 2;}}
        }

    }
    if (temp == 42)
        return 3;
    return 0;
}


void PlayPosition(char XO) // Function that asks you to enter where you like to play
{
    int col , g;
	
    while(1)
    {
			SW1 = GPIO_PORTF_DATA_R&0x10;     // read PF4 into SW1
		 Nokia5110_SetCursorChar( col ,0,XO);
		 if(!SW1)
		 {
				while (!(GPIO_PORTF_DATA_R&0x10)){
			 Delay100ms(1);
				}
				col++;
			 if(col>6)
				 col = 0 ;
			Nokia5110_SetCursorChar( col ,0,XO);
			  g = col==0?6:col-1;
			 Nokia5110_SetCursorChar( g,0,input[g]);
		 }
		 SW2 = GPIO_PORTF_DATA_R&0x01;     // read PF4 into SW2
		 Delay100ms(1);
		 if(!SW2)
		 {
			 while (!(GPIO_PORTF_DATA_R&0x01))
			 {
				 Delay100ms(1);
			 }
				col=GetValue(col);
        if( col != 0 )
        {
            input[col] = XO;
            break ;
        }
		 }
        
        
    }
}

int AIManager() // AI Algorithm
{
		int column; 
		float temp;
		int PlayNumber;
    float chance[2] = {9999999 , 0 };
    for( column = 1 ; column <= 7 ; column ++)
    {
        PlayOut = 0;
        EVA=0;
         PlayNumber = GetValue(column);
        if( PlayNumber != 0 )
        {

            input[PlayNumber] = 'O';
            if(winning()==2)
               {
                   input[PlayNumber]=' ';
                   return PlayNumber ;
               }
             temp = -(100*NegaMax(1));
            if(PlayOut != 0)
                temp -= ((100*EVA)/PlayOut);
            if(-temp >= 100)
                provocation = 1;
            if( chance[0] > temp  )
            {
                chance[0] = temp  ;
                chance[1] = PlayNumber;
            }
             //  cout<<endl<<-temp<<"   "<<EVA << "   " <<PlayOut;
            input[PlayNumber] = ' ';
        }
    }
    return chance[1];
}
int NegaMax(int Depth) // MiniMax algorithm in NegaMax form
{
    char XO;
		int column;
    int PlayNumber[8] = {0,0,0,0,0,0,0,0}; // The values of the input[] for every column
    int chance=0;
    if(Depth % 2 != 0)
        XO='X';
    else
        XO='O';
    for( column = 1 ; column <= 7 ; column ++)
        PlayNumber[column]=GetValue(column);
    for( column = 1 ; column <= 7 ; column++)
    {
        if(PlayNumber[column] != 0)
        {
            input[PlayNumber[column]]=XO;
            if( winning() != 0 )
            {
                PlayOut ++;
                if(XO=='O')
                    EVA ++;
                else
                    EVA --;
                input[PlayNumber[column]]=' ';
                return -1;
            }
            input[PlayNumber[column]]=' ';
        }
    }
    if(Depth <= 6)
    {

        for( column = 1 ; column <= 7 ; column ++)
        {
            int temp=0;
            if( PlayNumber[column] != 0 )
            {
                input[PlayNumber[column]]=XO;
                if( winning() != 0 )
                {
                    PlayOut++;
                    if(XO=='O')
                        EVA++;
                    else
                        EVA--;
                    input[PlayNumber[column]]=' ';
                    return -1;
                }
                temp = NegaMax(Depth+1);
                if(column == 1)
                    chance = temp;
                if(chance < temp)
                    chance = temp;
                input[PlayNumber[column]]=' ';
            }
        }
    }
    return -chance;
}


void PortF_Init(void){ volatile unsigned long delay;
  SYSCTL_RCGC2_R |= 0x00000020| 0x00000002;     // 1) B & F clock
  delay = SYSCTL_RCGC2_R;           // delay
  GPIO_PORTF_LOCK_R = 0x4C4F434B;   // 2) unlock PortF PF0
  GPIO_PORTF_CR_R = 0x1F;           // allow changes to PF4-0
  GPIO_PORTF_AMSEL_R = 0x00;        // 3) disable analog function
  GPIO_PORTF_PCTL_R = 0x00000000;   // 4) GPIO clear bit PCTL
  GPIO_PORTF_DIR_R = 0x0E;          // 5) PF4,PF0 input, PF3,PF2,PF1 output
  GPIO_PORTF_AFSEL_R = 0x00;        // 6) no alternate function
  GPIO_PORTF_PUR_R = 0x11;          // enable pullup resistors on PF4,PF0
  GPIO_PORTF_DEN_R = 0x1F;          // 7) enable digital pins PF4-PF0
}

int main(void){
  //TExaS_Init(SSI0_Real_Nokia5110_Scope);  // set system clock to 80 MHz
  // initialization goes here
	int mode ; 
	PortF_Init();        // Call initialization of port PF4, PF3, PF2, PF1, PF0
	for(i = 0 ; i < BOARD_COLS*BOARD_ROWS +1;i++)
	{
		board1[i] = ' ';
		input[i]=' ';
	}
	
  Nokia5110_Init();
	UARTB_init();
  Nokia5110_Clear();
	//startingScreen();
	mode = selectMode(); 
	srand(seed);
	Nokia5110_Clear();
	if(mode)
	{
	r = (rand()%9)+'0';
	UARTB_OutChar(r);
	rr = UARTB_InChar();
	
	if(r<rr)
	{
		uturn = 1 ;
	}

	printBoard(board1);
	for(turn = 0; turn < (BOARD_ROWS * BOARD_COLS) && !done; turn++){ // 42
      
		if(uturn)
		{
			do{
         printBoard(board1);
      }
			while(!takeTurnRemote(board1, (turn) % 2, PIECES));
			done = checkWin(board1);
			turn++;
			if(done) break;
			do{
         printBoard(board1);
      }
			while(!takeTurn(board1, (turn) % 2, PIECES));
			done = checkWin(board1);
		}
		else
		{
      do{
         printBoard(board1);
      }
			while(!takeTurn(board1, (turn) % 2, PIECES));
			done = checkWin(board1);
			turn++;
			if(done) break;
			do{
         printBoard(board1);
      }
			while(!takeTurnRemote(board1, (turn) % 2, PIECES));
			done = checkWin(board1);
		}
   }
	printBoard(board1);
	 if(turn == BOARD_ROWS * BOARD_COLS && !done){
     Nokia5110_OutString("It's a tie!");
   } else {
     turn--;
		 Nokia5110_Clear();
		 Nokia5110_SetCursor(1,1);
		Nokia5110_OutString("Player");
		Nokia5110_SetCursor(8,1);
		Nokia5110_OutString(turn%2==0?"X":"O");
		 Nokia5110_SetCursor(3,2);
		 Nokia5110_OutString("wins!");
		 Nokia5110_SetCursor(1,4);
		 Nokia5110_OutString("GAME OVER");
   }
	}
	else
	{
		int winningtemp ;
		while(1)
    {
        input[AIManager()]='O';
				Board();
         winningtemp = winning();
        if(winningtemp!=0)
        {
						Nokia5110_Clear();
            if(winningtemp == 1)
						{
						  Nokia5110_SetCursor(1,1);
							Nokia5110_OutString("Player");
							Nokia5110_SetCursor(8,1);
							Nokia5110_OutString(turn%2==0?"X":"O");
						  Nokia5110_SetCursor(3,2);
						  Nokia5110_OutString("wins!");
						}
            else if (winningtemp == 2)
						{
						  Nokia5110_SetCursor(1,1);
							Nokia5110_OutString("Player");
							Nokia5110_SetCursor(8,1);
							Nokia5110_OutString(turn%2==0?"X":"O");
						  Nokia5110_SetCursor(3,2);
						  Nokia5110_OutString("wins!");
						}
            else if (winningtemp == 3)
						{
							Nokia5110_SetCursor(1,1);
							Nokia5110_OutString("IT is TIE");
						}
						 Nokia5110_SetCursor(1,4);
						 Nokia5110_OutString("GAME OVER");
        }
        else
            PlayPosition('X');
    }
	}
	
}
void startingScreen()
{
	int w; 
	Nokia5110_SetCursor(0,0);
	Nokia5110_ClearBuffer();
	Nokia5110_PrintBMP(10, 35, logo1, 2);
	Nokia5110_DisplayBuffer();
	Nokia5110_SetCursor(0,5);
	Nokia5110_OutString("Welcome :D!");	
	Delay100ms(40);
	for(w=0 ; w<7 ; w++)
	{
		Nokia5110_SetCursor(0,w);
		Nokia5110_OutString("           ");
		Delay100ms(5);
	}

}

int selectMode(){ // here is selecting if the mode is P1 VS AI or PI vs P2 
	int k = 0 ; 
	Nokia5110_SetCursor(0,0);
	Nokia5110_OutString("select");
	Nokia5110_SetCursor(7,0);
	Nokia5110_OutString("mode");
	Nokia5110_SetCursor(3,2);
	Nokia5110_OutString("P1 VS AI");
	Nokia5110_SetCursor(3,4);
	Nokia5110_OutString("P1 VS P2");
	Nokia5110_SetCursor(0,k+2);	
	Nokia5110_OutString(">>");
	
	while(1)
	{
		SW1 = GPIO_PORTF_DATA_R&0x10;     // read PF4 into SW1
		if(!SW1)
		 {
				while (!(GPIO_PORTF_DATA_R&0x10)){
			 Delay100ms(1);
				}
				Nokia5110_SetCursor(0,k+2);	
				Nokia5110_OutString("  ");				
				k+=2;
				if(k>2)
					k=0;
				Nokia5110_SetCursor(0,k+2);	
				Nokia5110_OutString(">>");
		 }
	SW2 = GPIO_PORTF_DATA_R&0x01;     // read PF4 into SW2
		 Delay100ms(1);
		 if(!SW2)
		 {
			 while (!(GPIO_PORTF_DATA_R&0x01))
			 {
				 Delay100ms(1);
				 seed++; 
			 }
			 Nokia5110_SetCursor(3,k+2);
			 Nokia5110_OutString("        ");	
			Delay100ms(5);			 
			 Nokia5110_SetCursor(3,k+2);
			 Nokia5110_OutString(k?"P1 VS P2":"P1 VS AI");	
			Delay100ms(5);			 
			 Nokia5110_SetCursor(3,k+2);
			 Nokia5110_OutString("        ");		
			 Delay100ms(5);
			 Nokia5110_SetCursor(3,k+2);
			 Nokia5110_OutString(k?"P1 VS P2":"P1 VS AI");		
			 
				break ;
		 }
		 seed++;
	 }
	return  k ; 
}

void printBoard(char *board){
   int row, col;
	Nokia5110_ClearBuffer();
   for(row = 0; row < BOARD_ROWS; row++){
      for(col = 0; col < BOARD_COLS; col++){
				Nokia5110_SetCursorChar( col ,row,board[BOARD_COLS*row+col]);
      }
   }
}


	// take turn for the Kit player and PIECES can be X or O
int takeTurn(char *board, int player, const char *PIECES){
   int  col = 0;
		int g ;
   while(1){

		SW1 = GPIO_PORTF_DATA_R&0x10;     // read PF4 into SW1
		 Nokia5110_SetCursorChar( col ,0,PIECES[player]);
		 if(!SW1)
		 {
				while (!(GPIO_PORTF_DATA_R&0x10)){
			 Delay100ms(1);
				}
				col++;
			 if(col>6)
				 col = 0 ;
			Nokia5110_SetCursorChar( col ,0,PIECES[player]);
			  g = col==0?6:col-1;
			 Nokia5110_SetCursorChar( g,0,board[g]);
		 }
		 SW2 = GPIO_PORTF_DATA_R&0x01;     // read PF4 into SW2
		 Delay100ms(1);
		 if(!SW2)
		 {
			 while (!(GPIO_PORTF_DATA_R&0x01))
			 {
				 Delay100ms(1);
			 }
				break ;
		 }
   }
	 UARTB_OutChar(col+1+'0');
   return changeBoard(board , player , PIECES , col );
}
//For AI 
int takeTurnAI(char *board, int player, const char*PIECES)
{
	int col = 0;
	
	return changeBoard(board , player , PIECES , col );
}
//For UART move
int takeTurnRemote(char *board, int player, const char *PIECES){
  int col = 0;
	col = UARTB_InChar();
	col=col-1-'0' ;
	if(col>6)
	{
		col = 0 ;
	}
   return changeBoard(board , player , PIECES , col );
}
int changeBoard(char *board,int player , const char *PIECES , int col  )
{
	int row ;
   for(row = BOARD_ROWS - 1; row >= 0; row--){
      if(board[BOARD_COLS * row + col] == ' '){
         board[BOARD_COLS * row + col] = PIECES[player];
         return 1;
      }
   }
	 return 0 ;
}

int checkWin(char *board){
    return (horizontalCheck(board) || verticalCheck(board) || diagonalCheck(board));
}
int checkFour(char *board, int a, int b, int c, int d){
    return (board[a] == board[b] && board[b] == board[c] && board[c] == board[d] && board[a] != ' ');
}

int horizontalCheck(char *board){
    int row, col, idx;
    const int WIDTH = 1;

    for(row = 0; row < BOARD_ROWS; row++){
       for(col = 0; col < BOARD_COLS - 3; col++){
          idx = BOARD_COLS * row + col;
          if(checkFour(board, idx, idx + WIDTH, idx + WIDTH * 2, idx + WIDTH * 3)){
             return 1;
          }
       }
    }
    return 0;

}
int verticalCheck(char *board){
    int row, col, idx;
    const int HEIGHT = 7;

    for(row = 0; row < BOARD_ROWS - 3; row++){
       for(col = 0; col < BOARD_COLS; col++){
          idx = BOARD_COLS * row + col;
          if(checkFour(board, idx, idx + HEIGHT, idx + HEIGHT * 2, idx + HEIGHT * 3)){
              return 1;
          }
       }
    }
    return 0;

}
int diagonalCheck(char *board){
   int row, col, idx, count = 0;
   const int DIAG_RGT = 6, DIAG_LFT = 8;

   for(row = 0; row < BOARD_ROWS - 3; row++){
      for(col = 0; col < BOARD_COLS; col++){
         idx = BOARD_COLS * row + col;
         if((count <= 3 && checkFour(board, idx, idx + DIAG_LFT, idx + DIAG_LFT * 2, idx + DIAG_LFT * 3)) || (count >= 3 && checkFour(board, idx, idx + DIAG_RGT, idx + DIAG_RGT * 2, idx + DIAG_RGT * 3))){
            return 1;
         }
         count++;
      }
      count = 0;
   }
   return 0;

}
//
void UARTB_init()
{

	SYSCTL_RCGCUART_R |= (1<<1);
	GPIO_PORTB_AFSEL_R |= 0x03;
	GPIO_PORTB_DEN_R |= 0x03;
	GPIO_PORTB_PCTL_R |= 0x11;
	UART1_CTL_R &= ~(1<<0)  ;
	UART1_IBRD_R = 8 ;
	UART1_FBRD_R = 44 ;
	// IBRD = int(16,000,000 / (16 * 115200)) = int(8.6805)
  // FBRD = int(0.6805 * 64 + 0.5) = 44
	UART1_LCRH_R = (3<<5);
	UART1_CC_R = 0 ;
	UART1_CTL_R |= (1<<0) | (1<<8)| (1<<9);

}
 char UARTB_InChar(void){
// as part of Lab 11, modify this program to use UART0 instead of UART1
  while((UART1_FR_R&UART_FR_RXFE) != 0);
  return(( char)(UART1_DR_R));
}

void UARTB_OutChar( char data){
// as part of Lab 11, modify this program to use UART0 instead of UART1
  while((UART1_FR_R&(1<<5)) != 0);
  UART1_DR_R = data;
}

void UARTB_outString(char* buffer)
{
	while(*buffer != 0 )
	{
		UARTB_OutChar(*buffer) ;
		buffer++;

	}

}
/*

void UARTB_init()
{

	SYSCTL_RCGCUART_R |= (1<<0);
	GPIO_PORTA_AFSEL_R |= 0x03;
	GPIO_PORTA_DEN_R |= 0x03;
	GPIO_PORTA_PCTL_R |= 0x11;
	UART0_CTL_R &= ~(1<<0)  ;
	UART0_IBRD_R = 104 ;
	UART0_FBRD_R = 11;
	// IBRD = int(80,000,000 / (16 * 9600)) = int(43.4027)
  // FBRD = int(0.4027 * 64 + 0.5) = 26
	UART0_LCRH_R = (3<<5);
	UART0_CC_R = 0 ;
	UART0_CTL_R |= (1<<0) | (1<<8)| (1<<9);

}
 char UARTB_InChar(void){
// as part of Lab 11, modify this program to use UART0 instead of UART1
  while((UART0_FR_R&UART_FR_RXFE) != 0);
  return(( char)(UART0_DR_R));
}

void UARTB_OutChar( char data){
// as part of Lab 11, modify this program to use UART0 instead of UART1
  while((UART0_FR_R&(1<<5)) != 0);
  UART0_DR_R = data;
}

void UARTB_outString(char* buffer)
{
	while(*buffer != 0 )
	{
		UARTB_OutChar(*buffer) ;
		buffer++;

	}

}
//*/

void Delay100ms(unsigned long count){unsigned long volatile time;
  while(count){
    time = 72724;  // 0.1sec at 80 MHz
    while(time){
	  	time--;
    }
    count--;
  }
}
