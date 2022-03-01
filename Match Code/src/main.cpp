#include "main.h"

// motor ports
const int LEFT_BACK_DRIVE_PORT = 3;
const int LEFT_FRONT_DRIVE_PORT = 9;
const int RIGHT_BACK_DRIVE_PORT = 2;
const int RIGHT_FRONT_DRIVE_PORT = 5;
const int LEFT_2BAR = 4;
const int RIGHT_2BAR = 7;
const int MOGO = 1;
const int CLAMP = 8;

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
			{4_in, 14.4_in},
			static_cast<int32_t>(imev5GreenTPR)
		}
	)
	.withGains(
		{0.001, 0, 0.0001}, // Distance controller gains
        {0.001, 0, 0.000}, // Turn controller gains
        {0.001, 0, 0.0001}  // Angle controller gains (helps drive straight)
	)
	.withMaxVelocity(125)
	.build();

void initialize() {
	pros::lcd::initialize();
	pros::lcd::set_text(1, "Running and Active");
	leftFourBar.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
	rightFourBar.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
	mogoLift.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
	clamp.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
	clamp.move_velocity(-50);
}

void movemogo(int x, bool b)
{
	// x is time delayed after moving mogo lift, b is if mogo lift moves up or down (up is true, down is false)
	if (b)
	{
		mogoLift.move_velocity(-100);
		pros::delay(x * 1000);
	}
	else if (!b)
	{
		mogoLift.move_velocity(100);
		pros::delay(x * 1000);
	}
	mogoLift.move_velocity(0);
}

void moveclamp(double x, bool b) 
{
	//down is false, up is true
	if (b) 
	{
		clamp.move_velocity(100);
		pros::delay(x * 1000);
		
	} else if (!b) 
	{
		clamp.move_velocity(-100);
		pros::delay(x * 1000);
	}
	clamp.move_velocity(0);
}

void moveFourBar(double x, bool b) 
{
	//down is false, up is true
	if (b) 
	{
		rightFourBar.move_velocity(100);
		pros::delay(x * 1000);
		
	} else if (!b) 
	{
		rightFourBar.move_velocity(-100);
		pros::delay(x * 1000);
	}
	rightFourBar.move_velocity(0);
}

void skills80pt()
{
	rightFourBar.move_velocity(-10);
	pros::delay(20);
	movemogo(2.7, false);
	chassis->moveDistanceAsync(-1.2_ft);
	//pros::delay(250);
	movemogo(1.4, true);
	movemogo(1, true);
	chassis->moveDistance(1_ft);
	chassis->turnAngle(80_deg);//38
	chassis->moveDistanceAsync(6_ft);
	pros::delay(3000);
	chassis->moveDistance(-0.5_ft);
	chassis->turnAngle(-45_deg);
	movemogo(1.8, false);
	pros::delay(5);
	chassis->moveDistance(-0.5_ft);
	chassis->moveDistanceAsync(01_ft);
	pros::delay(500);
	moveclamp(2, false);
	chassis->moveDistance(-0.1_ft);
	chassis->turnAngle(-50_deg);
	chassis->moveDistance(4_ft);
	moveclamp(2, false);
}

void skills()
{
	// lowering four bar for neutral mobile goal pickup
	rightFourBar.move_velocity(-10);
	pros::delay(20);

	// moving mobile goal before picking up 
	movemogo(2.7, false);
	chassis->moveDistance(-1.4_ft);
	//pros::delay(250);

	// picking up mobile goal and moving back, securing mobile goal in bot
	movemogo(1, true);
	chassis->moveDistanceAsync(1_ft);
	movemogo(1, true);

	// turning, moving, and clamping onto neutral mobile goal
	chassis->turnAngle(87_deg);//38
	chassis->moveDistance(4.2_ft);
	moveclamp(2, false);

	// turning, moving and preparing to stack mobile goal
	moveFourBar(1.5, true); // turn 4-bar before turning to avoid interference
	chassis->turnAngle(30_deg);
	moveFourBar(1, true);
	chassis->moveDistance(3_ft);
}

void turnRightAuton()
{
	//align the right side of bot with edge of first time
	rightFourBar.move_velocity(-10);
	pros::delay(20);
	chassis->moveDistance(1_ft);
	chassis->turnAngle(-32_deg);
	chassis->moveDistance(3.4_ft);
	//pros::delay(100);
	moveclamp(2, false);
	chassis->moveDistance(-3_ft);
	moveclamp(2, true);
	chassis->moveDistance(-0.5_ft);
}

void auton20pt()
{
	rightFourBar.move_velocity(-10);
	pros::delay(20);
	chassis->moveDistance(4_ft);
	moveclamp(1, false);
	chassis->moveDistance(-4.5_ft);
	// moveclamp(2, true);
	// chassis->moveDistance(-1_ft);
}

void PIDPractice()
{
	//degrees are off by 15 (they are 15 degrees more than what we code, so 90 degrees in code is 105 degrees for the bot turning)
	chassis->moveDistance(4_ft);
	chassis->turnAngle(75_deg);
	chassis->moveDistance(4_ft);
}

void autonomous()
{
	//auton20pt();
	//PIDPractice();
	skills(); 
}

void opcontrol() 
{
	// translate(500, 500);
	while (true) 
	{
		// leftFourBar.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
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

void competition_initialize() 
{
	mogoLift.set_brake_mode(MOTOR_BRAKE_HOLD);

}
