#include "main.h"

// motor ports
const int LEFT_BACK_DRIVE_PORT = 5;
const int LEFT_FRONT_DRIVE_PORT = 2;
const int RIGHT_BACK_DRIVE_PORT = 10;
const int RIGHT_FRONT_DRIVE_PORT = 3;
const int LEFT_2BAR = 4;
const int RIGHT_2BAR = 7;
const int MOGO = 1;
const int CLAMP = 6;

// controller initialize
Controller joystick(ControllerId::master);

Motor wheelLB(LEFT_BACK_DRIVE_PORT, true, AbstractMotor::gearset::green, AbstractMotor::encoderUnits::degrees);
Motor wheelLF(LEFT_FRONT_DRIVE_PORT, true, AbstractMotor::gearset::green, AbstractMotor::encoderUnits::degrees);
Motor wheelRB(RIGHT_BACK_DRIVE_PORT, false, AbstractMotor::gearset::green, AbstractMotor::encoderUnits::degrees);
Motor wheelRF(RIGHT_FRONT_DRIVE_PORT, false, AbstractMotor::gearset::green, AbstractMotor::encoderUnits::degrees);

// Chassis controller 
std::shared_ptr<okapi::ChassisController> chassis = ChassisControllerBuilder()
	.withMotors(
		{wheelLF, wheelLB},
		{wheelRF, wheelRB}
	).withDimensions(
		AbstractMotor::gearset::green,
		{
			{4_in, 18_in},
			static_cast<int32_t>(imev5GreenTPR * 1.5)
		}
	).build();


void initialize() {
	pros::lcd::initialize();
	pros::lcd::set_text(1, "Running and Active");
	leftFourBar.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
	rightFourBar.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
	mogoLift.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
	clamp.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
	// function to move the mogo lift for auton 
	// auton for diff sides of the field 
}

void movemogo(int x, bool b)
{
	//x is time delayed after moving mogo lift, b is if mogo lift moves up or down (up is true, down is false)
	if (b)
	{
		mogoLift.move_velocity(-50);
		pros::delay(x);
	}
	else if (!b)
	{
		mogoLift.move_velocity(50);
		pros::delay(x);
	}
}

void edit() {
	movemogo(200, false);
	chassis->moveDistanceAsync(-75_in);
	pros::delay(2200);
	movemogo(200, false);
}

void basic(){
	chassis->moveDistanceAsync(5.75_ft);
	movemogo(200, false);
	chassis->waitUntilSettled();
	movemogo(200,true);
	chassis->moveDistanceAsync(-2_ft);
	movemogo(200, false);
	chassis->waitUntilSettled();
}

void autonomous() 
{
	basic();
}


void opcontrol() 
{
	while (true) 
	{

		leftFourBar.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
		rightFourBar.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
		mogoLift.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
		clamp.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);

		setDriveMotors();

		setFourBar();

		setClampMotors();

		setLiftMotors();

	}
      
}


void disabled() {}
void competition_initialize() {
}
