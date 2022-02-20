#include "main.h"

void setClamp(int motor) 
{
    clamp = motor;
}

void clampMacro()
{
	clamp.move_velocity(15);
}

void setClampMotors() {
    
    int up = controller.get_digital(pros::E_CONTROLLER_DIGITAL_A);
    int down = controller.get_digital(pros::E_CONTROLLER_DIGITAL_B);

    //Sri Controls
    // int up = controller.get_digital(pros::E_CONTROLLER_DIGITAL_R1);
    // int down = controller.get_digital(pros::E_CONTROLLER_DIGITAL_R2);

    int m = 0;

    if (up != 0 && down == 0) {
        // m = 150;
        m = 50;

        setClamp(m);
    }

    if (down != 0 && up == 0) {
        // m = -150;
        m = -50;

        // if (clamp.get_position() < 0.05)
        // {
        //     m = 0;
        // }
        // else if(clamp.get_position() > 0.9)
        // {
        //     m = 0;
        // }

        setClamp(m);
    }

    if (down == 0 && up == 0) 
    {
        clamp.move_velocity(0);
    }

}