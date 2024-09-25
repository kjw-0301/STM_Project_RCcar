
#ifndef INC_MOTOR_H_
#define INC_MOTOR_H_

#include "tim.h"
#include "gpio.h"
#include <stdio.h>

#define INIT_SPEED  100

typedef enum {
    IDLE,
    FORWARD,
    RIGHT,
    BACKWARD,
    LEFT,
}CONTROLLER_SIGNAL;

void move(int cotrol_signal);
void idle();
void forward();
void backward();
void right();
void left();

void Speed(int pwm);

#endif /* INC_MOTOR_H_ */
