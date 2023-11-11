typedef enum pathEnum {
	pathEnum_MOVE = 0,
    pathEnum_TURN = 1,
    pathEnum_All = 2,
} pathEnumT;

extern bool auton;
void autonPID(void *ignore);
void calibration(pathEnumT path);
void preload();
void matchload();
void full();
void singleShot();