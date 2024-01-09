#include "main.h"
#include "pros/misc.h"
#include "pros/motors.h"
#include <algorithm>
#define akp 2
#define akd 2
#define akt 3.30196850394 //Degrees of wheel turn to turn base 1 degree
#define akm 22.9183118053 //Degrees of wheel turn to 1 inch

Motor leftFront (leftFrontPort, MOTOR_GEAR_GREEN, false, MOTOR_ENCODER_DEGREES);
Motor leftMid (leftMidPort, MOTOR_GEAR_GREEN, false, MOTOR_ENCODER_DEGREES);
Motor leftBack (leftBackPort, MOTOR_GEAR_GREEN, false, MOTOR_ENCODER_DEGREES);
Motor rightFront (rightFrontPort, MOTOR_GEAR_GREEN, true, MOTOR_ENCODER_DEGREES);
Motor rightMid (rightMidPort, MOTOR_GEAR_GREEN, true, MOTOR_ENCODER_DEGREES);
Motor rightBack (rightBackPort, MOTOR_GEAR_GREEN, true, MOTOR_ENCODER_DEGREES);
Motor intake (intakePort, MOTOR_GEAR_GREEN, false, MOTOR_ENCODER_DEGREES);
ADIDigitalOut wing(wingPort, false);
ADIDigitalOut grabber(grabberPort, false);

bool voltControl = false, targReach = false, auton = true;
double speedCap = 0, targLeft = 0, targRight = 0, errorLeft, errorRight, preverrorLeft = 0, preverrorRight = 0, derivLeft, derivRight, left, right;

void autonPID(void *ignore){
    leftMid.tare_position();
    rightBack.tare_position();

    while (auton) {
        if (!voltControl){
            errorLeft = targLeft - leftMid.get_position();
            errorRight = targRight - rightBack.get_position();

            derivLeft = errorLeft - preverrorLeft;
            derivRight = errorRight - preverrorRight;

            left = errorLeft*akp + derivLeft*akd;
            right = errorRight*akp + derivRight*akd;
            if (speedCap != 0){
                if (left < -speedCap){
                    left = -speedCap;
                } else if (left > speedCap){
                    left = speedCap;
                }

                if (right < -speedCap){
                    right = -speedCap;
                } else if (right > speedCap) {
                    right = speedCap;
                }
            }

            leftFront.move(left);
            leftMid.move(left);
            leftBack.move(left);
            rightFront.move(right);
            rightMid.move(right);
            rightBack.move(right);

            preverrorLeft = errorLeft;
            preverrorRight = errorRight;
        } else {
            delay(5);
        }

        targReach = fabs(errorLeft) < 7.5 && fabs(errorRight) < 7.5;

        printf("posLeft: %.2f posRight: %f targLeft: %f targRight: %f, errorLeft: %f, errorRight: %f\n", leftFront.get_position(), rightFront.get_position(), targLeft, targRight, errorLeft, errorRight);
        delay(15);
    }
}

void move(double inches, double time = 0){
    targLeft += inches*akm;
    targRight += inches*akm;
    printf("Move: %f\n", inches);

    delay(500);

    if (time > .5){
        printf("Delaying %f", time);
        delay(time*1000 - 500);
    } else {
        while (!targReach) {
            printf("Moving\n");
            delay(20);
        }
    }
}

void turn(double degrees, double time = 0){
    targLeft += degrees*akt;
    targRight -= degrees*akt;
    printf("Turning: %f\n", degrees);

    delay(500);

    if (time > .5){
        printf("Delaying %f", time);
        delay(time*1000 - 500);
    } else {
        while (!targReach) {
            printf("Turning\n");
            delay(20);
        }
    }
}

void calibration(pathEnumT path){
    Task autonPIDTask (autonPID, (void*)"BALLS", TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "autonPIDTask");
    
    switch(path){
        case pathEnum_MOVE:
            move(24);
            break;
        case pathEnum_TURN:
            turn(90);
            break;
        case pathEnum_All:
            move(24);
            turn(90);
            break;
    }

    autonPIDTask.remove();
}

void preload(){
    Task autonPIDTask (autonPID, (void*)"BALLS", TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "autonPIDTask");
    
    move(-48, 3);
    move(48, 3);

    // turn(-135);

    // move(-48);

    delay(500);

    autonPIDTask.remove();
}

void matchload(){
    Task autonPIDTask (autonPID, (void*)"BALLS", TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "autonPIDTask");

    move(15, 3);
    move(-20, 3);

    turn(135);

    move(48);
}

void full(){
    Task autonPIDTask (autonPID, (void*)"BALLS", TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "autonPIDTask");

    speedCap = 80;
    move(-15);
    grabber.set_value(true);
    speedCap = 50;
    move(-10, 1.5);
    speedCap = 60;
    turn(80, 2);
    turn(-40);
    grabber.set_value(false);
    speedCap = 0;
    turn(20);
    move(-24, 1.5);
    move(7);
    turn(-25);
    move(33);
    turn(-45);
    move(28);
    delay(500);

    autonPIDTask.remove();
}

void singleShot(){
    shoot = true;
}
