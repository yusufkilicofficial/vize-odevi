#ifndef DENEY1_LIB_H
#define DENEY1_LIB_H

#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>

#define BUTTON_UP_D1   (1 << PB2)
#define BUTTON_DOWN_D1 (1 << PB3)

static const uint8_t SEGMENT_PATTERNS_D1[10] = {
  0b00111111, 0b00000110, 0b01011011, 0b01001111, 0b01100110,
  0b01101101, 0b01111101, 0b00000111, 0b01111111, 0b01101111
};

static volatile uint8_t segment_counter_d1 = 0;
static uint8_t lastButtonState_up_d1 = 1;
static uint8_t lastButtonState_down_d1 = 1;

void deney1_init(void) {
  DDRD |= 0b01111111;
  PORTD &= 0b10000000;
  DDRB &= ~(1 << PB2);
  DDRB &= ~(1 << PB3);
  PORTB |= (1 << PB2);
  PORTB |= (1 << PB3);
}

void deney1_display_digit(uint8_t digit) {
  if (digit > 9) digit = 0;
  uint8_t pattern = SEGMENT_PATTERNS_D1[digit];
  uint8_t portd_value = 0;
  if (pattern & (1<<2)) portd_value |= (1<<0);
  if (pattern & (1<<3)) portd_value |= (1<<1);
  if (pattern & (1<<4)) portd_value |= (1<<2);
  if (pattern & (1<<1)) portd_value |= (1<<3);
  if (pattern & (1<<0)) portd_value |= (1<<4);
  if (pattern & (1<<5)) portd_value |= (1<<5);
  if (pattern & (1<<6)) portd_value |= (1<<6);
  PORTD = (PORTD & 0b10000000) | portd_value;
}

uint8_t deney1_read_button_up(void) {
  return (PINB & BUTTON_UP_D1) ? 1 : 0;
}

uint8_t deney1_read_button_down(void) {
  return (PINB & BUTTON_DOWN_D1) ? 1 : 0;
}

void deney1_setup(void) {
  deney1_init();
  deney1_display_digit(segment_counter_d1);
}

void deney1_loop(void) {
  uint8_t button_up = deney1_read_button_up();
  if (button_up == 0 && lastButtonState_up_d1 == 1) {
    _delay_ms(20);
    button_up = deney1_read_button_up();
    if (button_up == 0) {
      segment_counter_d1++;
      if (segment_counter_d1 > 9) segment_counter_d1 = 0;
      deney1_display_digit(segment_counter_d1);
      while(deney1_read_button_up() == 0) _delay_ms(10);
    }
  }
  lastButtonState_up_d1 = button_up;
  
  uint8_t button_down = deney1_read_button_down();
  if (button_down == 0 && lastButtonState_down_d1 == 1) {
    _delay_ms(20);
    button_down = deney1_read_button_down();
    if (button_down == 0) {
      if (segment_counter_d1 == 0) segment_counter_d1 = 9;
      else segment_counter_d1--;
      deney1_display_digit(segment_counter_d1);
      while(deney1_read_button_down() == 0) _delay_ms(10);
    }
  }
  lastButtonState_down_d1 = button_down;
}

#endif
