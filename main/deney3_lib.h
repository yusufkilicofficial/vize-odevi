#ifndef DENEY3_LIB_H
#define DENEY3_LIB_H

#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>

#define SERVO_MIN_D3    2000
#define SERVO_MAX_D3    4000
#define SERVO_CENTER_D3 3000
#define SERVO_STEP_D3   5
#define LIGHT_THRESHOLD_D3  50
#define ADC_LDR_LEFT_D3   2
#define ADC_LDR_RIGHT_D3  3

static volatile uint16_t current_servo_pos_d3 = SERVO_CENTER_D3;

void deney3_timer1_init(void) {
    DDRB |= (1 << PB1);
    TCCR1A = (1 << COM1A1) | (0 << COM1A0) | (1 << WGM11) | (0 << WGM10);
    TCCR1B = (1 << WGM13) | (1 << WGM12) | (0 << CS12) | (1 << CS11) | (0 << CS10);
    ICR1 = 39999;
    OCR1A = SERVO_CENTER_D3;
    TCNT1 = 0;
}

void deney3_servo_move_to(uint16_t target_pos) {
    if (target_pos < SERVO_MIN_D3) target_pos = SERVO_MIN_D3;
    if (target_pos > SERVO_MAX_D3) target_pos = SERVO_MAX_D3;
    
    while (current_servo_pos_d3 != target_pos) {
        if (current_servo_pos_d3 < target_pos) {
            if (target_pos - current_servo_pos_d3 < SERVO_STEP_D3) {
                current_servo_pos_d3 = target_pos;
            } else {
                current_servo_pos_d3 += SERVO_STEP_D3;
            }
        } else {
            if (current_servo_pos_d3 - target_pos < SERVO_STEP_D3) {
                current_servo_pos_d3 = target_pos;
            } else {
                current_servo_pos_d3 -= SERVO_STEP_D3;
            }
        }
        OCR1A = current_servo_pos_d3;
        _delay_ms(5);
    }
}

extern uint16_t deney2_adc_read(uint8_t channel);

static uint8_t servo_loop_counter_d3 = 0;

void deney3_setup(void) {
    deney3_timer1_init();
    _delay_ms(500);
}

void deney3_loop(void) {
    servo_loop_counter_d3++;
    if (servo_loop_counter_d3 >= 10) {
        servo_loop_counter_d3 = 0;
        uint16_t ldr_left  = deney2_adc_read(ADC_LDR_LEFT_D3);
        uint16_t ldr_right = deney2_adc_read(ADC_LDR_RIGHT_D3);
        int16_t difference = ldr_left - ldr_right;
        uint16_t target_pos = current_servo_pos_d3;
        
        if (difference > LIGHT_THRESHOLD_D3) {
            target_pos = SERVO_MIN_D3;
        } else if (difference < -LIGHT_THRESHOLD_D3) {
            target_pos = SERVO_MAX_D3;
        } else {
            target_pos = SERVO_CENTER_D3;
        }
        deney3_servo_move_to(target_pos);
    }
}

#endif
