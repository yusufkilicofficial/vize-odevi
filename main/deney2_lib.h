#ifndef DENEY2_LIB_H
#define DENEY2_LIB_H

#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>

#define LCD_RS_D2 PB4
#define LCD_E_D2  PB5
#define LCD_D4_D2 PD7
#define LCD_D5_D2 PB0
#define LCD_D6_D2 PC4
#define LCD_D7_D2 PC5
#define DHT11_PIN_D2 PC1

#define LCD_CLEAR_D2      0x01
#define LCD_HOME_D2       0x02
#define LCD_ENTRY_MODE_D2 0x06
#define LCD_DISPLAY_ON_D2 0x0C
#define LCD_4BIT_2LINE_D2 0x28
#define LCD_LINE1_D2      0x80
#define LCD_LINE2_D2      0xC0

void deney2_lcd_enable_pulse(void) {
    PORTB |= (1 << LCD_E_D2);
    _delay_us(1);
    PORTB &= ~(1 << LCD_E_D2);
    _delay_us(50);
}

void deney2_lcd_write_nibble(uint8_t nibble) {
    if (nibble & 0x01) PORTD |= (1 << LCD_D4_D2); else PORTD &= ~(1 << LCD_D4_D2);
    if (nibble & 0x02) PORTB |= (1 << LCD_D5_D2); else PORTB &= ~(1 << LCD_D5_D2);
    if (nibble & 0x04) PORTC |= (1 << LCD_D6_D2); else PORTC &= ~(1 << LCD_D6_D2);
    if (nibble & 0x08) PORTC |= (1 << LCD_D7_D2); else PORTC &= ~(1 << LCD_D7_D2);
    deney2_lcd_enable_pulse();
}

void deney2_lcd_write_byte(uint8_t byte, uint8_t mode) {
    if (mode) PORTB |= (1 << LCD_RS_D2);
    else PORTB &= ~(1 << LCD_RS_D2);
    deney2_lcd_write_nibble(byte >> 4);
    deney2_lcd_write_nibble(byte & 0x0F);
    _delay_us(50);
}

void deney2_lcd_command(uint8_t cmd) {
    deney2_lcd_write_byte(cmd, 0);
    if (cmd <= 0x02) _delay_ms(2);
}

void deney2_lcd_data(uint8_t data) {
    deney2_lcd_write_byte(data, 1);
}

void deney2_lcd_print(const char* str) {
    while (*str) deney2_lcd_data(*str++);
}

void deney2_lcd_print_number(uint16_t num) {
    char buffer[6];
    uint8_t i = 0;
    if (num == 0) {
        deney2_lcd_data('0');
        return;
    }
    while (num > 0) {
        buffer[i++] = (num % 10) + '0';
        num /= 10;
    }
    while (i > 0) deney2_lcd_data(buffer[--i]);
}

void deney2_adc_init(void) {
    ADMUX = (0 << REFS1) | (1 << REFS0);
    ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
}

uint16_t deney2_adc_read(uint8_t channel) {
    ADMUX = (ADMUX & 0xF0) | (channel & 0x0F);
    _delay_us(10);
    ADCSRA |= (1 << ADSC);
    while (ADCSRA & (1 << ADSC));
    uint16_t result = ADCL;
    result |= (ADCH << 8);
    return result;
}

uint8_t deney2_dht11_read(int8_t* temp, int8_t* humidity) {
    uint8_t data[5] = {0, 0, 0, 0, 0};
    DDRC |= (1 << DHT11_PIN_D2);
    PORTC &= ~(1 << DHT11_PIN_D2);
    _delay_ms(20);
    PORTC |= (1 << DHT11_PIN_D2);
    _delay_us(30);
    DDRC &= ~(1 << DHT11_PIN_D2);
    PORTC |= (1 << DHT11_PIN_D2);
    
    uint8_t timeout = 100;
    while ((PINC & (1 << DHT11_PIN_D2)) && timeout--) _delay_us(1);
    if (!timeout) return 0;
    timeout = 100;
    while (!(PINC & (1 << DHT11_PIN_D2)) && timeout--) _delay_us(1);
    if (!timeout) return 0;
    timeout = 100;
    while ((PINC & (1 << DHT11_PIN_D2)) && timeout--) _delay_us(1);
    if (!timeout) return 0;
    
    for (uint8_t i = 0; i < 40; i++) {
        timeout = 100;
        while (!(PINC & (1 << DHT11_PIN_D2)) && timeout--) _delay_us(1);
        if (!timeout) return 0;
        _delay_us(30);
        if (PINC & (1 << DHT11_PIN_D2)) {
            data[i / 8] |= (1 << (7 - (i % 8)));
        }
        timeout = 100;
        while ((PINC & (1 << DHT11_PIN_D2)) && timeout--) _delay_us(1);
        if (!timeout) return 0;
    }
    
    if (data[4] == ((data[0] + data[1] + data[2] + data[3]) & 0xFF)) {
        *humidity = data[0];
        *temp = data[2];
        return 1;
    }
    return 0;
}

void deney2_lcd_init(void) {
    DDRB |= (1 << LCD_RS_D2) | (1 << LCD_E_D2);
    PORTB &= ~((1 << LCD_RS_D2) | (1 << LCD_E_D2));
    DDRD |= (1 << LCD_D4_D2);
    DDRB |= (1 << LCD_D5_D2);
    DDRC |= (1 << LCD_D6_D2) | (1 << LCD_D7_D2);
    PORTD &= ~(1 << LCD_D4_D2);
    PORTB &= ~(1 << LCD_D5_D2);
    PORTC &= ~((1 << LCD_D6_D2) | (1 << LCD_D7_D2));
    
    _delay_ms(50);
    deney2_lcd_write_nibble(0x03);
    _delay_ms(5);
    deney2_lcd_write_nibble(0x03);
    _delay_us(150);
    deney2_lcd_write_nibble(0x03);
    deney2_lcd_write_nibble(0x02);
    deney2_lcd_command(LCD_4BIT_2LINE_D2);
    deney2_lcd_command(LCD_DISPLAY_ON_D2);
    deney2_lcd_command(LCD_CLEAR_D2);
    _delay_ms(2);
    deney2_lcd_command(LCD_ENTRY_MODE_D2);
}

static uint32_t deney2_millis_counter = 0;
static uint32_t deney2_last_update = 0;

void deney2_setup(void) {
    deney2_lcd_init();
    deney2_adc_init();
    deney2_lcd_command(LCD_LINE1_D2);
    deney2_lcd_print("3 DENEY HAZIR!");
    deney2_lcd_command(LCD_LINE2_D2);
    deney2_lcd_print("Baslatiliyor...");
    _delay_ms(2000);
    deney2_lcd_command(LCD_CLEAR_D2);
}

void deney2_loop(void) {
    deney2_millis_counter++;
    if (deney2_millis_counter - deney2_last_update > 2000) {
        deney2_last_update = deney2_millis_counter;
        uint16_t ldr_value = deney2_adc_read(0);
        int8_t temp = 0, humidity = 0;
        uint8_t dht_ok = deney2_dht11_read(&temp, &humidity);
        
        deney2_lcd_command(LCD_LINE1_D2);
        deney2_lcd_print("T:");
        if (dht_ok) {
            deney2_lcd_print_number(temp);
            deney2_lcd_data('C');
        } else {
            deney2_lcd_print("--");
        }
        deney2_lcd_print(" N:");
        if (dht_ok) {
            deney2_lcd_print_number(humidity);
            deney2_lcd_data('%');
        } else {
            deney2_lcd_print("--");
        }
        
        deney2_lcd_command(LCD_LINE2_D2);
        deney2_lcd_print("L:");
        deney2_lcd_print_number(ldr_value);
        deney2_lcd_print("   ");
    }
}

#endif
