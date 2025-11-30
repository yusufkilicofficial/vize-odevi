#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>

#define SERVO_MIN    2000
#define SERVO_MAX    4000
#define SERVO_CENTER 3000

#define SERVO_STEP   5
#define SERVO_DELAY  10

#define ADC_LDR_LEFT   2
#define ADC_LDR_RIGHT  3

#define LIGHT_THRESHOLD  50

volatile uint16_t current_servo_pos = SERVO_CENTER;

void timer1_pwm_init(void) {
    DDRB |= (1 << PB1);
    
    TCCR1A = (1 << COM1A1) | (0 << COM1A0) | (1 << WGM11) | (0 << WGM10);
    
    TCCR1B = (1 << WGM13) | (1 << WGM12) | (0 << CS12) | (1 << CS11) | (0 << CS10);
    
    ICR1 = 39999;
    
    OCR1A = SERVO_CENTER;
    
    TCNT1 = 0;
}

void servo_move_to(uint16_t target_pos) {
    if (target_pos < SERVO_MIN) target_pos = SERVO_MIN;
    if (target_pos > SERVO_MAX) target_pos = SERVO_MAX;
    
    while (current_servo_pos != target_pos) {
        if (current_servo_pos < target_pos) {
            if (target_pos - current_servo_pos < SERVO_STEP) {
                current_servo_pos = target_pos;
            } else {
                current_servo_pos += SERVO_STEP;
            }
        } else {
            if (current_servo_pos - target_pos < SERVO_STEP) {
                current_servo_pos = target_pos;
            } else {
                current_servo_pos -= SERVO_STEP;
            }
        }
        
        OCR1A = current_servo_pos;
        
        _delay_ms(SERVO_DELAY);
    }
}

void adc_init(void) {
    ADMUX = (0 << REFS1) | (1 << REFS0);
    
    ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
}

uint16_t adc_read(uint8_t channel) {
    ADMUX = (ADMUX & 0xF0) | (channel & 0x0F);
    
    _delay_us(10);
    
    ADCSRA |= (1 << ADSC);
    
    while (ADCSRA & (1 << ADSC));
    
    uint16_t result = ADCL;
    result |= (ADCH << 8);
    
    return result;
}

void light_tracking(void) {
    uint16_t ldr_left  = adc_read(ADC_LDR_LEFT);
    uint16_t ldr_right = adc_read(ADC_LDR_RIGHT);
    
    int16_t difference = ldr_left - ldr_right;
    
    uint16_t target_pos = current_servo_pos;
    
    if (difference > LIGHT_THRESHOLD) {
        target_pos = SERVO_MIN;
    } 
    else if (difference < -LIGHT_THRESHOLD) {
        target_pos = SERVO_MAX;
    } 
    else {
        target_pos = SERVO_CENTER;
    }
    
    servo_move_to(target_pos);
}

int main(void) {
    timer1_pwm_init();
    
    adc_init();
    
    _delay_ms(1000);
    
    while (1) {
        light_tracking();
        
        _delay_ms(100);
    }
    
    return 0;
}
