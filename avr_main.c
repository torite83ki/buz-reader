/*
 *  avr_main.c
 *  device = at90s2313
 *
 *  used interrupts
 *  TIM0
 *  INT0
 *  TC1/PWM 
 *  coded by Nobu
 *  2022.05.27 rev.0
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


/* led is connected as sink */
#define LED1_PIN	PB0	// PB0 status LED
#define PR_LED_PIN	PB1	// PB1 photo-reflector LED
#define PR_TR_PIN		PINB2	// PB2 photo-reflector photo-tr
#define BUZZ_PIN	PB3	// PB3 Piezo speaker
#define SW1_PIN		PIND2	// PD2 Int0 Switch


#define HIGH	0x01
#define LOW	0x00


/*
 * interrupt handlers
 */
ISR(TIMER0_OVF0_vect);
ISR(TIMER1_OVF0_vect);
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
	init_device();

//#include	"raw_blink_4_debug.h"
	
	sei();
	for(;;) {
		sleep_mode();
	}

} /* end of main routine */



// sub routines

/* initialize io peripherals */
#define T0_OVF	247			/* Timer 0 overflow value -> 1ms @8MHz*/
void init_device(void) {
	// io ports
	DDRB = _BV(LED1_PIN) | _BV(PR_LED_PIN) | _BV(BUZZ_PIN);			/* PORTB: set 1 as output */ 
	PORTB = _BV(PR_TR_PIN);							/* set LED initial output - low, pull-up for input */
	DDRD = 0x00;												/* PORTD: set 1 as output, set 0 as input  */ 
	PORTD = _BV(SW1_PIN);								/* set pull-up for input */

	// Timer 
	/* Timer 0 setting */
	TCCR0 = _BV(CS02) | _BV(CS00);			/* set prescaler 1/1024 */
	TCNT0 = T0_OVF;											/* set overflow count */
	TIMSK |= _BV(TOIE0);								/* set timer overflow interrupt enable */
	/* Timer 1 setting */
	/* Set as PWM mode 
		 ftc1 = 8MHz/8
		 PWM freq = 1Mz/2046 = 489 Hz
	*/ 
#define COMP_TOP	511									/* PWM compare match value */
	TCCR1A = _BV(PWM11) | _BV(PWM10) |  _BV(COM1A1);	/* 10bit PWM */
	TCCR1B = _BV(CS11);		/* prescaler CK/8 */
	//OCR1A = COMP_TOP;												/* compare match TOP */
	OCR1AH = 0x00;												/* compare match TOP */
	OCR1AL = 0x7F;												/* compare match TOP */
	TCNT1H = 0x00;												/* Timer1 counter initialize */
	TCNT1L = 0x00;												/* Timer1 counter initialize */


	// External INT
	/* INT0 */
	MCUCR = _BV(ISC01);									/* INT0 falling edge to interrupt */
	GIMSK = _BV(INT0);									/* Enable INT0 interrupt */

}


static volatile uint16_t counter_ms = 0;		/* counter *ms  */
static volatile uint16_t counter_upperlimit = 500;		/* counter upper limit*/
ISR(TIMER0_OVF0_vect) {
	TCNT0 = T0_OVF;						/* set overflow value -> 1ms */
	if (++counter_ms >= counter_upperlimit) {
		counter_ms = 0;
		PORTB = PINB ^ _BV(LED1_PIN);
	}
}

ISR(TIMER1_OVF1_vect) {
	;
}

ISR(INT0_vect){
	cli();
	counter_upperlimit += 100;
	if(counter_upperlimit > 1000) {
	       counter_upperlimit = 100;
	}
	sei();
}

