#include "main.h"

/**
 * A callback function for LLEMU's center button.
 *
 * When this callback is fired, it will toggle line 2 of the LCD text between
 * "I was pressed!" and nothing.
 */
void on_center_button() {}

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
	Motor leftFront (leftFrontPort, MOTOR_GEAR_GREEN, false, MOTOR_ENCODER_DEGREES);
	Motor leftMid (leftMidPort, MOTOR_GEAR_GREEN, false, MOTOR_ENCODER_DEGREES);
	Motor leftBack (leftBackPort, MOTOR_GEAR_GREEN, false, MOTOR_ENCODER_DEGREES);
	Motor rightFront (rightFrontPort, MOTOR_GEAR_GREEN, true, MOTOR_ENCODER_DEGREES);
	Motor rightMid (rightMidPort, MOTOR_GEAR_GREEN, true, MOTOR_ENCODER_DEGREES);
	Motor rightBack (rightBackPort, MOTOR_GEAR_GREEN, true, MOTOR_ENCODER_DEGREES);
	Motor intake (intakePort, MOTOR_GEAR_GREEN, false, MOTOR_ENCODER_DEGREES);
	Controller master (CONTROLLER_MASTER);

	Task cataPIDTask (cataPID, (void*)"BALLS", TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "cataPIDTask");
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {}

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */
void autonomous() {
	// calibration(pathEnum_MOVE);
	path1();
}

/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */
void opcontrol() {
	Motor leftFront (leftFrontPort, false);
	Motor leftMid (leftMidPort, false);
	Motor leftBack (leftBackPort,  false);
	Motor rightFront (rightFrontPort,  true);
	Motor rightMid (rightMidPort,  true);
	Motor rightBack (rightBackPort, true);
	Motor intake (intakePort, false);
	ADIDigitalOut wing(wingPort, LOW);
	Controller master (CONTROLLER_MASTER);

	leftFront.set_brake_mode(MOTOR_BRAKE_BRAKE);
	leftMid.set_brake_mode(MOTOR_BRAKE_BRAKE);
	leftBack.set_brake_mode(MOTOR_BRAKE_BRAKE);
	rightFront.set_brake_mode(MOTOR_BRAKE_BRAKE);
	rightMid.set_brake_mode(MOTOR_BRAKE_BRAKE);
	rightBack.set_brake_mode(MOTOR_BRAKE_BRAKE);

	bool invert = false, wingState = false;
	double left, right;

	while (true) {
		left = master.get_analog(ANALOG_LEFT_Y);
		right = master.get_analog(ANALOG_RIGHT_Y);

		if (master.get_digital_new_press(DIGITAL_UP)){
			invert = !invert;
		}

		if (invert){
			leftFront.move(-right*1.02);
			leftMid.move(-right*1.02);
			leftBack.move(-right*1.02);
			rightFront.move(-left);
			rightMid.move(-left);
			rightBack.move(-left);
		} else {
			leftFront.move(left*1.02);
			leftMid.move(left*1.02);
			leftBack.move(left*1.02);
			rightFront.move(right);
			rightMid.move(right);
			rightBack.move(right);
		}

		if (master.get_digital(DIGITAL_L1)){
			intake.move(-127);
		} else if (master.get_digital(DIGITAL_L2)){
			intake.move(127);
		} else {
			intake.move(0);
		}
		
		if (master.get_digital_new_press(DIGITAL_DOWN)){
			wingState = !wingState;
			wing.set_value(wingState);
			printf("wingState: %d\n", wingState);
		}

		delay(20);
	}
}
