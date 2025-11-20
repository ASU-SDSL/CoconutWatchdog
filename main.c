/*
 * File:   main.c
 * Author: Tyler Nielsen
 *
 * Created on September 24, 2025, 12:58 AM
 */

// config defines
// internal 8MHz clock is divided by 8
#define F_CPU 1000000UL  // used for util/delay.h

#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <util/delay.h>

// normal defines
#define HEARTBEAT_PIN PB1
#define HEARTBEAT_PIN_INTERRUPT PCINT1
#define HEARTBEAT_PIN_INPUT PINB1

#define RESET_PIN_DIR DDB0
#define RESET_PIN PB0
#define LED_BLINK_PIN_DIR DDB4
#define LED_BLINK_PIN PB4

// local configs
// #define MS_WAIT_AFTER_BOOT (1000UL * 10)  // 10 sec
// #define MS_WAIT_IN_OP (1000UL * 5)        // 5 sec

#define MS_WAIT_AFTER_BOOT (1000UL * 60)  // 60 sec
#define MS_WAIT_IN_OP (1000UL * 60)       // 60 sec

volatile bool beat = false;

ISR(PCINT0_vect) {
  if ((PINB & _BV(HEARTBEAT_PIN_INPUT)) != 0) {
    beat = true;
  }
}
/*
 *
 */
int main(void) {
  // setup for blink
  // set up pin as output
  DDRB |= _BV(LED_BLINK_PIN_DIR);
  // initialize PB4 to LOW
  PORTB &= ~_BV(LED_BLINK_PIN);

  // setup for being a watchdog
  // set reseting pin to output, low
  PORTB &= ~_BV(RESET_PIN);
  DDRB |= _BV(RESET_PIN_DIR);
  // set pin to input
  DDRB |= ~_BV(HEARTBEAT_PIN);
  // any logical change interrupt
  MCUCR &= ~(_BV(ISC01) | _BV(ISC00));
  MCUCR |= _BV(ISC00);
  // enable Pin Change interrupt
  GIMSK |= _BV(PCIE);
  // enable SREG
  sei();  // macro - memory barrier?
  // enable pins
  PCMSK |= _BV(HEARTBEAT_PIN_INTERRUPT);

  // setup for internal watchdog

  _delay_ms(MS_WAIT_AFTER_BOOT);  // wait 10s to start checking
  while (1) {
    _delay_ms(MS_WAIT_IN_OP);  // check every period
    // toggle pin with PINxn
    //        PINB |= _BV(LED_BLINK_PIN);

    if (beat == false) {  // if either beat hasn't changed
      // one of the tasks if frozen, reset
      PORTB |= _BV(RESET_PIN);
      PORTB |= _BV(LED_BLINK_PIN);
      _delay_ms(500);
      PORTB &= ~_BV(RESET_PIN);
      PORTB &= ~_BV(LED_BLINK_PIN);
      _delay_ms(MS_WAIT_AFTER_BOOT);  // give the pico time to setup again
    }
    // clear beats
    beat = false;
  }

  return 0;
}

/**
 #define HEARTBEAT_PIN_0 1
#define HEARTBEAT_PIN_0_INTERRUPT PCINT1
#define HEARTBEAT_PIN_1 2
#define HEARTBEAT_PIN_1_INTERRUPT PCINT2
#define RESET_PIN 0

volatile bool beat0 = false;
volatile bool beat1 = false;

ISR(PCINT0_vect) {
  if (digitalRead(HEARTBEAT_PIN_0) == HIGH) {
    beat0 = true;
  }
  if (digitalRead(HEARTBEAT_PIN_1) == HIGH) {
    beat1 = true;
  }
}

// 1 minute before looking for heartbeat
#define MS_WAIT_AFTER_BOOT 60000 * 5
#define MS_WAIT_IN_OP 60000

void setup() {
  // setup
  pinMode(HEARTBEAT_PIN_0, INPUT);
  pinMode(HEARTBEAT_PIN_1, INPUT);
  pinMode(RESET_PIN, OUTPUT);
  digitalWrite(RESET_PIN, LOW);

  // any logical change interrupt
  MCUCR &= ~((1 << ISC01) | (1 << ISC00));
  MCUCR |= (1 << ISC00);
  // enable Pin Change interrupt
  GIMSK |= (1 << PCIE);
  // enable SREG, I think the arduino sdk will do that

  // enable pins
  PCMSK |= (1 << HEARTBEAT_PIN_0_INTERRUPT) | (1 << HEARTBEAT_PIN_1_INTERRUPT);

  delay(MS_WAIT_AFTER_BOOT);  // wait 10s to start checking
}

void loop() {
  delay(MS_WAIT_IN_OP);             // check every period
  if ((beat0 && beat1) == false) {  // if either beat hasn't changed
    // one of the tasks if frozen, reset
    digitalWrite(RESET_PIN, HIGH);
    delay(500);
    digitalWrite(RESET_PIN, LOW);
    delay(MS_WAIT_AFTER_BOOT);  // give the pico time to setup again
    // clear beats
  }
  beat0 = false;
  beat1 = false;
}


 */
