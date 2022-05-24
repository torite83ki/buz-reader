/*
 *  avr_main.c
 *  device = at90s2313
 *
 *  used interrupts
 *  TIM0
 *  
 *  coded by H.Takenobu
 *  2020.12.13 rev.0
 *
 */



#include <avr/io.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include <avr/cpufunc.h>
#include <util/delay.h>
#include <avr/sleep.h>

/* 
 * user defined constant
 */


/* led is connected as sink-source */
#define LED1_PIN	2
#define LED2_PIN	3
#define LED3_PIN	6
#define LED4_PIN	7


#define HIGH	0x01
#define LOW	0x00


/*
 * interrupt handlers
 */
ISR(TIMER0_OVF0_vect);
ISR(INT0_vect);

/*
 * sub routines
 */
void init_device(void);

/*
 * sram static values
 */

/* main routine for avr */
int main(void){

	cli();
	//init_device();

#include	"raw_blink_4_debug.h"
	
	//sei();
	for(;;) {
		sleep_mode();
	}

} /* end of main routine */



// sub routines

/* initialize io peripherals */
#define T0_OVF	247			/* Timer 0 overflow value -> 1ms */
void init_device(void) {
	// io ports
	DDRB = _BV(DDB0) | _BV(DDB1) | _BV(DDB2) | _BV(DDB3) | _BV(DDB4) | _BV(DDB5) | _BV(DDB6) | _BV(DDB7);				/* PORTB: set 1 as output , all ports are output*/ 
	PORTB = 0xAA;				/* set LED pattern */
	DDRD = _BV(DDD4) | _BV(DDD5) | _BV(DDD6);	/* PORTD: set 1 as output ,  port4, 5 and 6 are output*/ 
	PORTD = 0xAA;				/* set LED pattern */

	// Timer 
	/* Timer 0 setting */
	TCCR0 = _BV(CS02) | _BV(CS00);		/* set prescaler 1/1024 */
	TCNT0 = T0_OVF;				/* set overflow count */
	TIMSK |= _BV(TOIE0);			/* set timer overflow interrupt enable */

	// External INT
	/* INT0 */
	MCUCR = _BV(ISC01) | _BV(ISC00);		/* INT0 Rising edge to interrupt */
	GIMSK = _BV(INT0);				/* Enable INT0 interrupt */

}


static volatile uint16_t counter_ms = 0;		/* counter *ms  */
static volatile uint16_t counter_upperlimit = 500;		/* counter upper limit*/
ISR(TIMER0_OVF0_vect) {
	TCNT0 = T0_OVF;						/* set overflow value -> 1ms */
	if (++counter_ms >= counter_upperlimit) {
		counter_ms = 0;
		PORTB = ~PINB;
	}
}

ISR(INT0_vect){
	cli();
	counter_upperlimit += 100;
	if(counter_upperlimit > 1000) {
	       counter_upperlimit = 100;
	}
	sei();
}

