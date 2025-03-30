#include <stdio.h>

#include "ksj_gpio.h"

int main(void) {
    ksj_gpio_init();
    
    ksj_pinMode(5, INPUT);
    ksj_pinMode(6, OUTPUT);


    while (1)
    {
        ksj_digitalWrite(6, ksj_digitalRead(5));
    }
    
    /*
    ksj_pinMode(18, OUTPUT);
    ksj_pinMode(23, OUTPUT);

    while (1) {
        ksj_digitalWrite(18, HIGH);
        usleep(500000);
        ksj_digitalWrite(18, LOW);
        usleep(500000);
        ksj_digitalWrite(23, HIGH);
        usleep(500000);
        ksj_digitalWrite(23, LOW);
        usleep(500000);
    }*/
    
    return 0;
}