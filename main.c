#ifndef F_CPU
#define F_CPU 16000000UL //set frequency
#endif

#include <avr/io.h>
#include <util/delay.h>

void init_pins() {
    DDRB &= ~_BV(DDB2); //set pin 10 to input (Right Sensor)
    DDRB &= ~_BV(DDB3); //set pin 11 to input (Left Sensor)
    DDRB &= ~_BV(DDB4); //set pin 12 to input (Center Sensor)

    DDRD |= _BV(DDD2); //set pin 2 to output (Right Backward)
    DDRD |= _BV(DDD3); //set pin 3 to output (Right Forward)
    DDRD |= _BV(DDD4); //set pin 4 to output (Left Backward)
    DDRD |= _BV(DDD5); //set pin 5 to output (Left Forward)

    DDRD |= _BV(DDD6); //set pin 6 to output (Right 2 Backward)
    DDRD |= _BV(DDD7); //set pin 7 to output (Right 2 Forward)
    DDRB |= _BV(DDB1); //set pin 9 to output (Left 2 Backward)
    DDRB |= _BV(DDB0); //set pin 8 to output (Left 2 Forward)
}

void rotate_right() {
    PORTD &= ~_BV(PORTD5); //set pin 5 to low
    PORTD |= _BV(PORTD4); //set pin 4 to high
    PORTD |= _BV(PORTD3); //set pin 3 to high
    PORTD &= ~_BV(PORTD2); //set pin 2 to low

    PORTB &= ~_BV(PORTB0); //set pin 8 to low
    PORTB |= _BV(PORTB1); //set pin 9 to high
    PORTD |= _BV(PORTD7); //set pin 7 to high
    PORTD &= ~_BV(PORTD6); //set pin 6 to low
}

void rotate_left() {
    PORTD |= _BV(PORTD5); //set pin 5 to high
    PORTD &= ~_BV(PORTD4); //set pin 4 to low
    PORTD &= ~_BV(PORTD3); //set pin 3 to low
    PORTD |= _BV(PORTD2); //set pin 2 to high

    PORTB |= _BV(PORTB0); //set pin 8 to high
    PORTB &= ~_BV(PORTB1); //set pin 9 to low
    PORTD &= ~_BV(PORTD7); //set pin 7 to low
    PORTD |= _BV(PORTD6); //set pin 6 to high 
}

void stop() {
    PORTD &= ~_BV(PORTD5); //set pin 5 to low
    PORTD &= ~_BV(PORTD4); //set pin 4 to low
    PORTD &= ~_BV(PORTD3); //set pin 3 to low
    PORTD &= ~_BV(PORTD2); //set pin 2 to low

    PORTB &= ~_BV(PORTB0); //set pin 8 to low
    PORTB &= ~_BV(PORTB1); //set pin 9 to low
    PORTD &= ~_BV(PORTD7); //set pin 7 to low
    PORTD &= ~_BV(PORTD6); //set pin 6 to low
}

void forward() {
    PORTD |= _BV(PORTD5); //set pin 5 to high
    PORTD &= ~_BV(PORTD4); //set pin 4 to low
    PORTD |= _BV(PORTD3); //set pin 3 to high
    PORTD &= ~_BV(PORTD2); //set pin 2 to low

    PORTB |= _BV(PORTB0); //set pin 8 to high
    PORTB &= ~_BV(PORTB1); //set pin 9 to low
    PORTD |= _BV(PORTD7); //set pin 7 to high
    PORTD &= ~_BV(PORTD6); //set pin 6 to low
}
 
int main() {
    int state = 0;
    int isStarting = 1;

    init_pins();

    for(;;) {
        if(isStarting) {
            isStarting = 0;
            _delay_ms(5000);
        }

        switch(state) {
            case 1:
                //detected black line at left sensor
                //rotate right
                rotate_right();
                //until center sensor detects black line
                if((PINB & _BV(PB4)) != 0) {
                    stop();
                    state = 0;
                }
            break;
            default:
                //just go forward
                forward();

                //check left sensor (detect black line)
                if((PINB & _BV(PB3)) != 0) {
                    stop();
                    state = 1;
                }

                //check right sensor (detect black line)
                if((PINB & _BV(PB2)) != 0) {
                    stop();
                    state = 2;
                }
        }
    }

    return 0;
}