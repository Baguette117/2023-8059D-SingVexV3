#include "main.h"
#define kp 4
#define kd 8
//Note that values are set for degrees not centidegrees, must change when swapping to rotation sensor

void cataPID(void *ignore){
    Motor cata (cataPort, MOTOR_GEAR_GREEN, false, MOTOR_ENCODER_DEGREES);
    Rotation rotation (rotationPort);
    Controller master (CONTROLLER_MASTER);

    double err, targ = 0;
    while (true) {
        // if (master.get_digital_new_press(DIGITAL_R2)){
        //     cata.move(-127);
        //     delay(750);
        // } else if (master.get_digital(DIGITAL_R1)){
        //     cata.move(-50);
        // } else {
        //     err = 0 - rotation.get_position();

        //     cata.move(err*kp);
        // }

        if (master.get_digital_new_press(DIGITAL_R2)){
            targ += 1080;
        }
        
        if (master.get_digital(DIGITAL_R1)){
            cata.move(70);
            targ = cata.get_position();
        } else {
            err = targ - cata.get_position();
            cata.move(err*kp);
        }

        
        delay(15);
    }
}