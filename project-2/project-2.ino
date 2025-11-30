#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>

#define LCD_RS PB4
#define LCD_E  PB5
#define LCD_D4 PD7
#define LCD_D5 PB0
#define LCD_D6 PC4
#define LCD_D7 PC5

#define LCD_CLEAR           0x01
#define LCD_HOME            0x02
#define LCD_ENTRY_MODE      0x06
#define LCD_DISPLAY_ON      0x0C
#define LCD_4BIT_2LINE      0x28
#define LCD_LINE1           0x80
#define LCD_LINE2           0xC0

#define ADC_CHANNEL_LDR  0

#define DHT11_PIN       PC1
#define DHT11_DDR       DDRC
#define DHT11_PORT      PORTC
#define DHT11_PIN_REG   PINC

typedef struct {
    uint8_t humidity_int;
    uint8_t humidity_dec;
    uint8_t temperature_int;
    uint8_t temperature_dec;
    uint8_t checksum;
    uint8_t valid;
} DHT11_Data;

void lcd_port_init(void) {
    DDRB |= (1 << LCD_RS) | (1 << LCD_E);
    PORTB &= ~((1 << LCD_RS) | (1 << LCD_E));
    
    DDRD |= (1 << LCD_D4);
    DDRB |= (1 << LCD_D5);
    DDRC |= (1 << LCD_D6) | (1 << LCD_D7);
    PORTD &= ~(1 << LCD_D4);
    PORTB &= ~(1 << LCD_D5);
    PORTC &= ~((1 << LCD_D6) | (1 << LCD_D7));
}

void lcd_enable_pulse(void) {
    PORTB |= (1 << LCD_E);
    _delay_us(1);
    PORTB &= ~(1 << LCD_E);
    _delay_us(50);
}

void lcd_write_nibble(uint8_t nibble) {
    if (nibble & 0x01) PORTD |= (1 << PD7); else PORTD &= ~(1 << PD7);
    if (nibble & 0x02) PORTB |= (1 << PB0); else PORTB &= ~(1 << PB0);
    if (nibble & 0x04) PORTC |= (1 << PC4); else PORTC &= ~(1 << PC4);
    if (nibble & 0x08) PORTC |= (1 << PC5); else PORTC &= ~(1 << PC5);
    
    lcd_enable_pulse();
}

void lcd_write_byte(uint8_t byte, uint8_t mode) {
    if (mode) {
        PORTB |= (1 << LCD_RS);
    } else {
        PORTB &= ~(1 << LCD_RS);
    }
    
    lcd_write_nibble(byte >> 4);
    lcd_write_nibble(byte & 0x0F);
    _delay_us(50);
}

void lcd_command(uint8_t cmd) {
    lcd_write_byte(cmd, 0);
}

void lcd_data(uint8_t data) {
    lcd_write_byte(data, 1);
}

void lcd_init(void) {
    lcd_port_init();
    _delay_ms(50);
    
    lcd_write_nibble(0x03);
    _delay_ms(5);
    lcd_write_nibble(0x03);
    _delay_us(150);
    lcd_write_nibble(0x03);
    _delay_us(150);
    lcd_write_nibble(0x02);
    _delay_us(150);
    
    lcd_command(LCD_4BIT_2LINE);
    lcd_command(LCD_DISPLAY_ON);
    lcd_command(LCD_CLEAR);
    _delay_ms(2);
    lcd_command(LCD_ENTRY_MODE);
}

void lcd_clear(void) {
    lcd_command(LCD_CLEAR);
    _delay_ms(2);
}

void lcd_gotoxy(uint8_t x, uint8_t y) {
    uint8_t address = (y == 0) ? (LCD_LINE1 + x) : (LCD_LINE2 + x);
    lcd_command(address);
}

void lcd_putc(char c) {
    lcd_data(c);
}

void lcd_puts(const char *str) {
    while (*str) {
        lcd_putc(*str++);
    }
}

void adc_init(void) {
    ADMUX = (1 << REFS0);
    ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
    adc_read(0);
}

uint16_t adc_read(uint8_t channel) {
    channel &= 0x07;
    ADMUX = (ADMUX & 0xF8) | channel;
    ADCSRA |= (1 << ADSC);
    while (ADCSRA & (1 << ADSC));
    ADCSRA |= (1 << ADIF);
    return ADC;
}

uint16_t read_ldr(void) {
    return adc_read(ADC_CHANNEL_LDR);
}

void dht11_start(void) {
    DHT11_DDR |= (1 << DHT11_PIN);
    DHT11_PORT &= ~(1 << DHT11_PIN);
    _delay_ms(20);
    DHT11_PORT |= (1 << DHT11_PIN);
    _delay_us(40);
    DHT11_DDR &= ~(1 << DHT11_PIN);
}

uint8_t dht11_check_response(void) {
    uint8_t timeout;
    
    timeout = 0;
    while (DHT11_PIN_REG & (1 << DHT11_PIN)) {
        _delay_us(1);
        if (++timeout > 100) return 0;
    }
    
    timeout = 0;
    while (!(DHT11_PIN_REG & (1 << DHT11_PIN))) {
        _delay_us(1);
        if (++timeout > 100) return 0;
    }
    
    timeout = 0;
    while (DHT11_PIN_REG & (1 << DHT11_PIN)) {
        _delay_us(1);
        if (++timeout > 100) return 0;
    }
    
    return 1;
}

uint8_t dht11_read_byte(void) {
    uint8_t byte = 0;
    
    for (uint8_t i = 0; i < 8; i++) {
        while (!(DHT11_PIN_REG & (1 << DHT11_PIN)));
        _delay_us(30);
        
        if (DHT11_PIN_REG & (1 << DHT11_PIN)) {
            byte |= (1 << (7 - i));
        }
        
        while (DHT11_PIN_REG & (1 << DHT11_PIN));
    }
    
    return byte;
}

DHT11_Data dht11_read(void) {
    DHT11_Data data = {0};
    
    dht11_start();
    
    if (!dht11_check_response()) {
        data.valid = 0;
        return data;
    }
    
    data.humidity_int = dht11_read_byte();
    data.humidity_dec = dht11_read_byte();
    data.temperature_int = dht11_read_byte();
    data.temperature_dec = dht11_read_byte();
    data.checksum = dht11_read_byte();
    
    uint8_t calculated_sum = data.humidity_int + data.humidity_dec + 
                             data.temperature_int + data.temperature_dec;
    
    data.valid = (calculated_sum == data.checksum) ? 1 : 0;
    
    return data;
}

void reverse_string(char *str, uint8_t length) {
    uint8_t start = 0;
    uint8_t end = length - 1;
    
    while (start < end) {
        char temp = str[start];
        str[start] = str[end];
        str[end] = temp;
        start++;
        end--;
    }
}

void int_to_string(uint16_t num, char *str) {
    uint8_t i = 0;
    
    if (num == 0) {
        str[0] = '0';
        str[1] = '\0';
        return;
    }
    
    while (num > 0) {
        str[i++] = (num % 10) + '0';
        num /= 10;
    }
    
    str[i] = '\0';
    reverse_string(str, i);
}

void uart_init(void) {
    UBRR0H = 0;
    UBRR0L = 103;
    UCSR0B = (1 << TXEN0);
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

void uart_putc(char c) {
    while (!(UCSR0A & (1 << UDRE0)));
    UDR0 = c;
}

void uart_puts(const char *str) {
    while (*str) {
        uart_putc(*str++);
    }
}

void uart_print_num(uint16_t num) {
    char buffer[8];
    int_to_string(num, buffer);
    uart_puts(buffer);
}

int main(void) {
    uart_init();
    lcd_init();
    adc_init();
    
    uart_puts("\r\n=== DENEY 2 DEBUG ===\r\n");
    
    lcd_clear();
    lcd_gotoxy(0, 0);
    lcd_puts("  DENEY 2");
    lcd_gotoxy(0, 1);
    lcd_puts("LCD & Sensor");
    _delay_ms(3000);
    lcd_clear();
    
    lcd_gotoxy(0, 0);
    lcd_puts("T:    C N:   %");
    lcd_gotoxy(0, 1);
    lcd_puts("L:");
    
    char buffer[8];
    uint8_t error_count = 0;
    
    while(1) {
        DHT11_Data dht = dht11_read();
        
        uint16_t light = read_ldr();
        
        uart_puts("\r\nDHT11: ");
        if (dht.valid) {
            uart_puts("VALID | T=");
            uart_print_num(dht.temperature_int);
            uart_putc('.');
            uart_print_num(dht.temperature_dec);
            uart_puts("C | H=");
            uart_print_num(dht.humidity_int);
            uart_putc('.');
            uart_print_num(dht.humidity_dec);
            uart_puts("% | CHK=");
            uart_print_num(dht.checksum);
        } else {
            uart_puts("INVALID");
        }
        uart_puts(" | Light=");
        uart_print_num(light);
        
        if (dht.valid) {
            error_count = 0;
            
            lcd_gotoxy(2, 0);
            if (dht.temperature_int == 0) {
                lcd_puts("0");
            } else {
                int_to_string(dht.temperature_int, buffer);
                lcd_puts(buffer);
            }
            lcd_puts("  ");
            
            lcd_gotoxy(10, 0);
            int_to_string(dht.humidity_int, buffer);
            lcd_puts(buffer);
            lcd_puts("  ");
            
        } else {
            error_count++;
            if (error_count >= 3) {
                lcd_gotoxy(2, 0);
                lcd_puts("ERR");
                lcd_gotoxy(10, 0);
                lcd_puts("ERR");
            }
        }
        
        lcd_gotoxy(2, 1);
        int_to_string(light, buffer);
        lcd_puts(buffer);
        lcd_puts("    ");
        
        _delay_ms(2000);
    }
    
    return 0;
}
