/*
 *  raw_blink_4_debug.h 
 *  device = at90s2313
 *  
 *  Raw LED blink routine for debug usage
 *  especially for establish initial set-up 
 *  coded by H.Takenobu
 *  2021.01.11 rev.0
 * 
 *  required to include following libraries
 *  <util/delay.h>
 *  <stdio.h>
 *
 */

#ifndef _RAW_BLINK_4_DEBUG_H_
#define _RAW_BLINK_4_DEBUG_H_


#define LED_PORT_DIR DDRB
#define LED_PORT_DATA PORTB


	// Initialize
	LED_PORT_DIR = 0xFF;		// set all ports as output
	LED_PORT_DATA = 0xAA;		// set data
	
	// LED blink	
	uint8_t n;
	for(n = 0;n < 5;n++){
		LED_PORT_DATA = 0xAA;
		_delay_ms(100);
		_delay_ms(100);
		_delay_ms(100);
		LED_PORT_DATA = 0xCC;
		_delay_ms(100);
	}
#endif
