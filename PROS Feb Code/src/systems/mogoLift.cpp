#include "main.h"

void setLift(int motor) {
    mogoLift = motor;
}

void setLiftMotors() {
    
    int up = controller.get_digital(pros::E_CONTROLLER_DIGITAL_R2);
    int down = controller.get_digital(pros::E_CONTROLLER_DIGITAL_R1);

    int m = 0;

    if (up != 0 && down == 0) {
        m = 150;

        setLift(m);
    }

    if (down != 0 && up == 0) {
        m = -150;

        setLift(m);
    }

    if (down == 0 && up == 0) {
        mogoLift.move_velocity(0);
    }

}