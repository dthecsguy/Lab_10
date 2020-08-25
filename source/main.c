/*	Author: lab
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab #  Exercise #
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

#include "keypad.h"
#include "timer.h"

static unsigned char outtieKP = 0;
const unsigned char tasksNum = 1;
const unsigned long periodKP = 200;
const unsigned long tasksPeriodGCD = 200;

typedef struct task{
	int state;
	unsigned long period;
	unsigned long elapsedTime;
	int (*TickFct)(int);
} task;

task tasks[1];

void set_out(){ PORTB = outtieKP;}
unsigned char decKey(unsigned char key);


enum KP_State {WAIT, PRESS, HOLD, RELEASE};

int KP_tick(int state);

int main(void) {
    /* Insert DDR and PORT initializations */
	DDRB = 0xFF;	PORTB = 0x00;
	DDRC = 0xF0;	PORTC = 0x0F;
	
	tasks[0].state = WAIT;
	tasks[0].period = 200;
	tasks[0].elapsedTime = 0;
	tasks[0].TickFct = &KP_tick;
	
	TimerSet(tasksPeriodGCD);
	TimerOn();
	
    /* Insert your solution below */
    while (1) {
		
		for(unsigned char i = 0; i < tasksNum; i++){
			if (tasks[i].elapsedTime >= tasks[i].period){
				tasks[i].state = tasks[i].TickFct(tasks[i].state);
				tasks[i].elapsedTime = 0;
			}
			
			tasks[i].elapsedTime += tasksPeriodGCD;
		}
		
		set_out();
		
		while(!TimerFlag){}
		TimerFlag = 0;
    }
    return 1;
}

unsigned char decKey(unsigned char key){
	unsigned char tmp;
	
	switch(key){				
		case '1': 
			tmp = 0x01;
			break;
			
		case '2':
			tmp = 0x02;
			break;
			
		case '3':
			tmp = 0x03;
			break;
			
		case '4':
			tmp = 0x04;
			break;
			
		case '5':
			tmp = 0x05;
			break;
			
		case '6':
			tmp = 0x06;
			break;
			
		case '7':
			tmp = 0x07;
			break;
			
		case '8':
			tmp = 0x08;
			break;
			
		case '9':
			tmp = 0x09;
			break;
			
		case 'A':
			tmp = 0x0A;
			break;
			
		case 'B':
			tmp = 0x0B;
			break;
			
		case 'C':
			tmp = 0x0C;
			break;
			
		case 'D':
			tmp = 0x0D;
			break;
			
		case '*':
			tmp = 0x0E;
			break;
			
		case '0':
			tmp = 0x00;
			break;
			
		case '#':
			tmp = 0x0F;
			break;
			
		default:
			tmp = 0x1B;
			break;
	}
	return tmp;
}


int KP_tick(int state){
	
	unsigned char key = GetKeypadKey();
	
	switch(state){ //transitions
		case WAIT:
			state = (key == '\0') ? WAIT : PRESS;
			break;
			
		case PRESS:
			state = (key == '\0') ? WAIT : HOLD;
			break;
		
		case HOLD:
			state = (key == '\0') ? RELEASE : HOLD;
			break;
			
		case RELEASE:
			state = WAIT;
			break;
	}
	
	switch(state){ //transitions
		case WAIT:
			outtieKP = 0;
			break;
			
		case PRESS:
			outtieKP = 0x80;
			break;
		
		case HOLD:
			outtieKP = 0x80;
			break;
			
		case RELEASE:
			outtieKP = 0;
			break;
	}
	
	outtieKP |= decKey(key);
	
	return state;
}