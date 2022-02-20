#include "main.h"

void setFourBarMotors(int motor) {
    // leftFourBar = motor;
    rightFourBar = motor;
}

void setFourBar() {
    
    // int up = controller.get_digital(pros::E_CONTROLLER_DIGITAL_L1);
    // int down = controller.get_digital(pros::E_CONTROLLER_DIGITAL_L2);

    //Sri Controls
    int up = controller.get_digital(pros::E_CONTROLLER_DIGITAL_UP);
    int down = controller.get_digital(pros::E_CONTROLLER_DIGITAL_DOWN);

    int m = 0;

    if (up != 0 && down == 0) {
        m = 200;

        setFourBarMotors(m);
    }

    if (down != 0 && up == 0) {
        m = -100;

        // if (rightFourBar.get_position() > 1.5) m = 0;

        setFourBarMotors(m);
    }

    if (down == 0 && up == 0) 
    {
        // leftFourBar.move_velocity(0);
        rightFourBar.move_velocity(0);
    }
    else
    {
        clamp.move_velocity(-5);
    }

}