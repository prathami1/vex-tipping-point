#include "main.h"

bool enableDriverPID = true;

double kP = 0.0;
double kI = 0.0;
double kD = 0.0;

double tP = 0.0;
double tI = 0.0;
double tD = 0.0;


int desiredValue = 200;

int desiredTurnValue = 0;

int error;
int prevError = 0;
int d;
int totalError = 0;

int turnError;
int turnPrevError = 0;
int turnd;
int totalTurnError = 0;

int drivePID(std::shared_ptr<okapi::ChassisController>  chassis) {

    while (enableDriverPID) {

        int leftBackPos = leftBack.get_position();
        int rightBackPos = rightBack.get_position();
        int rightFrontPos = rightFront.get_position();
        int leftFrontPos = leftFront.get_position();

        int avgPosition = (leftBackPos + leftFrontPos + rightBackPos + rightFrontPos) / 4;

        error = avgPosition - desiredValue;

        d = error - prevError;

        totalError += error;

        double lateralMotorPower = error * kP + d * kD + totalError* kI;

        int turnDiff = (leftBackPos + rightBackPos - rightFrontPos - rightBackPos)/4;

        

        turnError = turnDiff - desiredTurnValue;

        turnd = turnError - turnPrevError;

        totalTurnError += turnError;

        double turnMotorPower = turnError * tP + turnd * tD + totalError * tI;

        // leftBack.move(std::forward, lateralMotorPower + turnMotorPower);
        // leftFront.move(std::forward, lateralMotorPower + turnMotorPower);
        // rightBack.move(std::forward, lateralMotorPower + turnMotorPower);
        // rightFront.move(std::forward, lateralMotorPower + turnMotorPower);

        leftBack.move(lateralMotorPower + turnMotorPower);
        leftFront.move(lateralMotorPower + turnMotorPower);
        rightBack.move(lateralMotorPower + turnMotorPower);
        rightFront.move(lateralMotorPower + turnMotorPower);
    }

    return 1;
}
