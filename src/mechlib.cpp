#include "main.h"
#define kp .180

void cataPID(void *ignore){
    Motor cata (cataPort, MOTOR_GEAR_GREEN, false, MOTOR_ENCODER_DEGREES);
    Rotation rotation (rotationPort);
    Controller master (CONTROLLER_MASTER);

    double err;
    while (true) {
        if (master.get_digital_new_press(DIGITAL_R2)){
            cata.move(-127);
            delay(750);
        } else if (master.get_digital(DIGITAL_R1)){
            cata.move(-50);
        } else {
            err = 0 - rotation.get_position();

            cata.move(err*kp);
        }

        delay(15);
    }
}