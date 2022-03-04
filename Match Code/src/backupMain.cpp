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

void turnAuton20pt()
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
	//chassis->turnAngle(87_deg);//38
	chassis->turnAngle(82_deg);//38
	chassis->moveDistance(4.2_ft);
	moveclamp(2, false);

	// turning, moving and preparing to stack mobile goal
	moveFourBar(1.5, true); // turn 4-bar before turning to avoid interference
	chassis->turnAngle(38_deg);
	moveFourBar(1, true);	
	chassis->moveDistance(2_ft);
	chassis->turnAngle(-10_deg);
	chassis->moveDistance(0.8_ft);
	chassis->turnAngle(-13_deg);

	//stacking mobile goal and moving away from platform
	moveFourBar(0.8, false);
	moveclamp(1, true);
	moveFourBar(0.5, true);

	//prepping mogo and moving back
	movemogo(1, false);
	chassis->moveDistanceAsync(-1_ft);
	moveclamp(1, false);
	moveFourBar(2, false);

	//turning bot to face right mogo
	chassis->turnAngle(96_deg);

	//letting go of back mogo and pushing middle neutral to other side
	movemogo(1, false);
	moveclamp(2, true);
	chassis->moveDistance(6_ft);

	//turn and push neutral mogo to other side
	chassis->turnAngle(25_deg);
	movemogo(2, true);
	chassis->moveDistance(-5_ft);

}

void autonomous()
{
	//auton20pt();
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