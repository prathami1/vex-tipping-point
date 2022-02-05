#include "main.h"

void setFourBarMotors(int motor) {
    leftFourBar = motor;
    rightFourBar = motor;
}

void setFourBar() {
    
    int up = controller.get_digital(pros::E_CONTROLLER_DIGITAL_L1);
    int down = controller.get_digital(pros::E_CONTROLLER_DIGITAL_L2);

    int m = 0;

    if (up != 0 && down == 0) {
        m = 200;

        setFourBarMotors(m);
    }

    if (down != 0 && up == 0) {
        m = -100;

        setFourBarMotors(m);
    }

    if (down == 0 && up == 0) {
        leftFourBar.move_velocity(0);
        rightFourBar.move_velocity(0);
    }

}