#include "main.h"

void setDrive(double leftBackW, double rightBackW, double leftFrontW, double rightFrontW) 
{
    leftBack = leftBackW;
    leftFront = leftFrontW;
    rightBack = rightBackW;
    rightFront = rightFrontW;
}

void setDriveMotors() 
{
    double leftJoystickY = -controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y) * (200/127);
    double rightJoystickX = -controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X) * (200/127);

    if (abs(leftJoystickY) < 20) leftJoystickY = 0;
    if (abs(rightJoystickX) < 20) rightJoystickX = 0;

    //int lB, lF, rB, rF = 0;
    double lB = 0;
    double lF = 0;
    double rB = 0;
    double rF = 0;

    lB = (leftJoystickY + rightJoystickX) * (200/127);
    lF = (leftJoystickY + rightJoystickX) * (200/127);
    rB = (-leftJoystickY + rightJoystickX) * (200/127);
    rF = (-leftJoystickY + rightJoystickX) * (200/127);

    setDrive(lB, rB, lF, rF);
}