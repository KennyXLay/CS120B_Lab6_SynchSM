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
#include <avr/interrupt.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

volatile unsigned char TimerFlag = 0;

unsigned long _avr_timer_M = 1;
unsigned long _avr_timer_cntcurr = 0;
unsigned char tmpB = 0x00;

void TimerOn(){
	TCCR1B = 0x0B;
	OCR1A = 125;
	TIMSK1 = 0x02;
	TCNT1 = 0;
	_avr_timer_cntcurr = _avr_timer_M;
	SREG |= 0x80;
}
void TimerOff(){
	TCCR1B = 0x00;
}
void TimerISR(){
	TimerFlag = 1;
}
ISR(TIMER1_COMPA_vect){
	_avr_timer_cntcurr--;
	if(_avr_timer_cntcurr == 0){
		TimerISR();
		_avr_timer_cntcurr = _avr_timer_M;
	}
}
void TimerSet(unsigned long M){
	_avr_timer_M = M;
	_avr_timer_cntcurr = _avr_timer_M;	
}

enum States{start, init, led1, led2, led3, led2s, buttonPress, wait1} state;

void Tick(){
	unsigned char tmpA = ~PINA;
	switch(state){
		case start:
			state = init;
			break;
		case init:
			if(tmpA == 0x01){
				state = buttonPress;
			}
			else{
				state = led1;
			}
			break;
		case led1:
			if(tmpA == 0x01){
                                state = buttonPress;
                        }
                        else{
				state = led2;
			}
			break;
		case led2:
			if(tmpA == 0x01){
                                state = buttonPress;
                        }
                        else{
				state = led3;
			}	
			break;
		case led3:
			if(tmpA == 0x01){
                                state = buttonPress;
                        }
                        else{
				state = led2s;
			}
			break;
		case led2s:
			if(tmpA == 0x01){
                                state = buttonPress;
                        }
                        else{
				state = led1;
			}
			break;
		case buttonPress:
			if(tmpA == 0x01){
				state = buttonPress;
			}
			else{
				state = wait1;
			}
			break;
		case wait1:
			if(tmpA == 0x01){
				state = init;
			}
			else{
				state = wait1;
			}
			break;
		default:
			state = start;
			break;	
	}
	switch(state){
		case led1:
			tmpB = 0x01;
			break;
		case led2:
			tmpB = 0x02;
			break;
		case led2s:
			tmpB = 0x02;
			break;
		case led3:
			tmpB = 0x04;
			break;
		
		default:
			break;
	}
	PORTB = tmpB;
}


int main(void) {
    /* Insert DDR and PORT initializations */
	DDRA = 0x00;
	PORTA = 0xFF;
	DDRB = 0xFF; 
	PORTB = 0x00;
	TimerSet(300);
	TimerOn();
	//unsigned char tmpB = 0x00;
	state = init;
    /* Insert your solution below */
    while (1) {
	//tmpB = ~tmpB;
	//PORTB = tmpB;
	Tick();
	while(!TimerFlag);
	TimerFlag = 0;
    }
    return 1;
}
