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
			{4_in, 18_in},
			static_cast<int32_t>(imev5GreenTPR)
		}
	).withMaxVelocity(200).build();


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

void moveclamp(int x, bool b) 
{
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

void tarunAuton()
{
	chassis->moveDistanceAsync(-5.75_ft);
	mogoLift.move_velocity(-200);
	chassis->waitUntilSettled();
	mogoLift.move_velocity(200);
	chassis->turnAngle(45_deg);
	chassis->moveDistanceAsync(1.5_ft);
	mogoLift.move_velocity(-200);
	chassis->waitUntilSettled();
	clamp.move_velocity(-50);
	rightFourBar.move_velocity(200);
	chassis->turnAngle(45_deg);
	rightFourBar.move_velocity(-200);
	clamp.move_velocity(50);
}


void skills20pt()
{
	chassis->moveDistance(6_ft);
	chassis->turnAngle(20_deg);
}

void skills40pt()
{
	chassis->moveDistance(2.25_ft);
	chassis->turnAngle(45_deg);
	chassis->moveDistance(7_ft);
}

void skills()
{

	// leftBack.set_voltage_limit(8);
	// leftFront.set_voltage_limit(8);
	// rightBack.set_voltage_limit(8);
	// rightFront.set_voltage_limit(8);

	movemogo(1.95, false);
	chassis->moveDistanceAsync(-1_ft);
	pros::delay(1000);
	movemogo(2, true);
	chassis->moveDistance(0.5_ft);
	chassis->turnAngle(65_deg);//38
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

void autonomous() 
{
	//basic();
	//skills20pt();
	//skills40pt();

	//drivePID(chassis);
	//skills();
}

void opcontrol() 
{
	//autonomous();
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
