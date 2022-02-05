#include "main.h"

void setDrive(double leftBackW, double rightBackW, double leftFrontW, double rightFrontW) {
    leftBack = leftBackW;
    leftFront = leftFrontW;
    rightBack = rightBackW;
    rightFront = rightFrontW;
}

void setDriveMotors() {

    double leftJoystickY = controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y) * (200/127);
    double rightJoystickX = controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X) * (200/127);

    if (abs(leftJoystickY) < 20) leftJoystickY = 0;
    if (abs(rightJoystickX) < 20) rightJoystickX = 0;

    //int lB, lF, rB, rF = 0;
    double lB = 0;
    double lF = 0;
    double rB = 0;
    double rF = 0;

    if (leftJoystickY != 0 && rightJoystickX == 0) {
        lB = -leftJoystickY;
        lF = -leftJoystickY;
        rB = leftJoystickY;
        rF = leftJoystickY;
    }

    if (rightJoystickX != 0 && leftJoystickY == 0) {
        lB = -rightJoystickX;
        lF = -rightJoystickX;
        rB = -rightJoystickX;
        rF = -rightJoystickX;
    }

    if (leftJoystickY != 0 && rightJoystickX != 0){
        if (leftJoystickY > 0) {
            if (rightJoystickX > 0) {
                lB = -rightJoystickX;
                lF = -rightJoystickX;
                rB = rightJoystickX * 0.5;
                rF = rightJoystickX * 0.5;
            }
            if (rightJoystickX < 0) {
                lB = rightJoystickX * 0.5;
                lF = rightJoystickX * 0.5;
                rB = -rightJoystickX;
                rF = -rightJoystickX;
            }
        }
        if (leftJoystickY < 0) {
            if (rightJoystickX > 0) {
                lB = -rightJoystickX;
                lF = -rightJoystickX;
                rB = rightJoystickX * 0.5;
                rF = rightJoystickX * 0.5;
            }
            if (rightJoystickX < 0) {
                lB = rightJoystickX * 0.5;
                lF = rightJoystickX * 0.5;
                rB = -rightJoystickX;
                rF = -rightJoystickX;
            }
        }
    }

    setDrive(lB, rB, lF, rF);

}