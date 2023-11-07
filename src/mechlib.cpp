#include "main.h"
#include "pros/misc.h"
#define kp .08
#define kd .12
//Note that values are set for degrees not centidegrees, must change when swapping to rotation sensor

bool fire = false;


void shoot(){
    fire = true;
}

void cataPID(void *ignore){
    Motor cata (cataPort, MOTOR_GEAR_RED, true, MOTOR_ENCODER_DEGREES);
    Rotation rotation (rotationPort);
    Controller master (CONTROLLER_MASTER);

    double err, prevErr, deriv, targ = 6000;
    while (true) {
        if (master.get_digital_new_press(DIGITAL_R1)){
            cata.move(127);
            delay(250);
        } else if (master.get_digital(DIGITAL_R2)){
            cata.move(50);
        } else {
            err = targ - rotation.get_position();
            deriv = prevErr - err;

            cata.move(err*kp + deriv*kd);
            printf("err: %f power: %f rotationSensor: %d\n", err, err*kp + deriv*kd, rotation.get_position());

            prevErr = err;
        }

        // if (master.get_digital_new_press(DIGITAL_R1) || fire == true){
        //     targ += 1080;
        //     fire = false;
        // }
        
        // if (master.get_digital(DIGITAL_R2)){
        //     cata.move(70);
        //     targ = cata.get_position();
        // } else {
        //     err = targ - cata.get_position();
        //     cata.move(err*kp);
        // }

        
        delay(15);
    }
}