#include "main.h"

void setClamp(int motor) {
    clamp = motor;
}

void setClampMotors() {
    
    int up = controller.get_digital(pros::E_CONTROLLER_DIGITAL_A);
    int down = controller.get_digital(pros::E_CONTROLLER_DIGITAL_B);

    int m = 0;

    if (up != 0 && down == 0) {
        m = 150;

        setClamp(m);
    }

    if (down != 0 && up == 0) {
        m = -150;

        setClamp(m);
    }

    if (down == 0 && up == 0) {
        clamp.move_velocity(0);
    }

}