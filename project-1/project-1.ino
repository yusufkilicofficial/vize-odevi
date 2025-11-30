#include <avr/io.h>
#include <util/delay.h>

#define BUTTON_UP   (1 << PB2)
#define BUTTON_DOWN (1 << PB3)

const uint8_t SEGMENT_PATTERNS[10] = {
  0b00111111,
  0b00000110,
  0b01011011,
  0b01001111,
  0b01100110,
  0b01101101,
  0b01111101,
  0b00000111,
  0b01111111,
  0b01101111
};

volatile uint8_t counter = 0;
uint8_t lastButtonState_up = 1;
uint8_t lastButtonState_down = 1;

void setup_ports(void);
void display_digit(uint8_t digit);
uint8_t read_button_up(void);
uint8_t read_button_down(void);

int main(void) {
  setup_ports();
  display_digit(counter);
  
  while(1) {
    uint8_t button_up = read_button_up();
    if (button_up == 0 && lastButtonState_up == 1) {
      _delay_ms(20);
      button_up = read_button_up();
      if (button_up == 0) {
        counter++;
        if (counter > 9) {
          counter = 0;
        }
        display_digit(counter);
        
        while(read_button_up() == 0) {
          _delay_ms(10);
        }
      }
    }
    lastButtonState_up = button_up;
    
    uint8_t button_down = read_button_down();
    if (button_down == 0 && lastButtonState_down == 1) {
      _delay_ms(20);
      button_down = read_button_down();
      if (button_down == 0) {
        if (counter == 0) {
          counter = 9;
        } else {
          counter--;
        }
        display_digit(counter);
        
        while(read_button_down() == 0) {
          _delay_ms(10);
        }
      }
    }
    lastButtonState_down = button_down;
    
    _delay_ms(10);
  }
  
  return 0;
}

void setup_ports(void) {
  DDRD |= 0b01111111;
  PORTD &= 0b10000000;
  
  DDRB &= ~(1 << PB2);
  DDRB &= ~(1 << PB3);
  
  PORTB |= (1 << PB2);
  PORTB |= (1 << PB3);
}

void display_digit(uint8_t digit) {
  if (digit > 9) digit = 0;
  
  uint8_t pattern = SEGMENT_PATTERNS[digit];
  
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

uint8_t read_button_up(void) {
  return (PINB & BUTTON_UP) ? 1 : 0;
}

uint8_t read_button_down(void) {
  return (PINB & BUTTON_DOWN) ? 1 : 0;
}
