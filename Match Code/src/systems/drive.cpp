#include "main.h"

void setDrive(double leftBackW, double rightBackW, double leftFrontW, double rightFrontW) 
{
    leftBack = leftBackW;
    leftFront = leftFrontW;
    rightBack = rightBackW;
    rightFront = rightFrontW;
}

void reset() {
    leftBack.tare_position();
    leftFront.tare_position();
    rightBack.tare_position();
    rightFront.tare_position();
}

double avgDriveEncoderValue() {
    pros::lcd::set_text(3, std::to_string(leftFront.get_position()));
    return (fabs(leftFront.get_position()) +
            fabs(leftBack.get_position()) + 
            fabs(rightBack.get_position()) +
            fabs(rightFront.get_position())) /4;
}

void setDriveMotors() 
{
    //power equation with voltage for drive motors
    double leftJoystickY = -controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y); // * (200/127);
    double rightJoystickX = -controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X); // * (200/127);

    if (abs(leftJoystickY) < 20) leftJoystickY = 0;
    if (abs(rightJoystickX) < 20) rightJoystickX = 0;

    //int lB, lF, rB, rF = 0;
    double lB = 0;
    double lF = 0;
    double rB = 0;
    double rF = 0;

    lB = (leftJoystickY + rightJoystickX) * (160/127); //200
    lF = (leftJoystickY + rightJoystickX) * (160/127); //200
    rB = (-leftJoystickY + rightJoystickX) * (160/127); //200
    rF = (-leftJoystickY + rightJoystickX) * (160/127); //200

    setDrive(lB, rB, lF, rF);
}

void translate(int units, int voltage){

    int direction = abs(units) / units;

    

    reset();

    pros::lcd::set_text(1, std::to_string(avgDriveEncoderValue()));
    while (avgDriveEncoderValue() < abs(units)) {
        
        setDrive(voltage * direction, voltage * direction, -voltage * direction, -voltage * direction);
        pros::delay(10);
    }

    setDrive(-10, -10, 10, 10);
    pros::delay(50);

    setDrive(0, 0, 0, 0);

}