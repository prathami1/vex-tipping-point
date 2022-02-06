#include "main.h"

// motor ports
const int LEFT_BACK_DRIVE_PORT = 2;
const int LEFT_FRONT_DRIVE_PORT = 1;
const int RIGHT_BACK_DRIVE_PORT = 3;
const int RIGHT_FRONT_DRIVE_PORT = 10;
const int LEFT_2BAR = 4;
const int RIGHT_2BAR = 7;
const int MOGO2 = 9;
const int CLAMP = 6;
// controller initialize
Controller joystick(ControllerId::master);

// motors and buttons
Motor wheelLB(LEFT_BACK_DRIVE_PORT, true, AbstractMotor::gearset::green, AbstractMotor::encoderUnits::degrees);
Motor wheelLF(LEFT_FRONT_DRIVE_PORT, true, AbstractMotor::gearset::green, AbstractMotor::encoderUnits::degrees);
Motor wheelRB(RIGHT_BACK_DRIVE_PORT, false, AbstractMotor::gearset::green, AbstractMotor::encoderUnits::degrees);
Motor wheelRF(RIGHT_FRONT_DRIVE_PORT, false , AbstractMotor::gearset::green, AbstractMotor::encoderUnits::degrees);
ControllerButton btnUp(ControllerDigital::R1);
ControllerButton btnDown(ControllerDigital::R2);
ControllerButton mogoUP(ControllerDigital::L1);
ControllerButton mogoDOwn(ControllerDigital::L2); 
ControllerButton ClampUp(ControllerDigital::X);
ControllerButton CLampDown(ControllerDigital::B); 
ControllerButton switchDriveF(ControllerDigital::Y); 
ControllerButton switchDriveB(ControllerDigital::A); 
Motor LtwoBar (LEFT_2BAR);
Motor RtwoBar (LEFT_2BAR);
Motor mogoMotor2(MOGO2);
Motor Clamp(CLAMP);
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
	pros::lcd::set_text(1, "hello");
	LtwoBar.setBrakeMode(AbstractMotor::brakeMode::hold);
	RtwoBar.setBrakeMode(AbstractMotor::brakeMode::hold);
	mogoMotor2.setBrakeMode(AbstractMotor::brakeMode::hold);
	}
// function to move the mogo lift for auton 
void movemogo(int x, bool b){
	if (b)
	{
	mogoMotor2.moveVelocity(-50);
	pros::delay(x);
	}else if (!b)
	{
	mogoMotor2.moveVelocity(50);
	pros::delay(x);
	}
}
// auton for diff sides of the field 
void Left(){
	chassis->moveDistanceAsync(-2_m);
	movemogo(200, false);
	chassis->waitUntilSettled();
	movemogo(200,true);
	chassis->moveDistanceAsync(1.5_m);
	movemogo(200, false);
	chassis->waitUntilSettled();
}
void basic(){
	chassis->moveDistanceAsync(1.8_m); //moves bot forward asyncronously
	movemogo(200, false); //deploys mobile goal
	chassis->waitUntilSettled(); //waits until drive is settled
	movemogo(200, true); //drops mobile goal on the ground
	chassis->moveDistanceAsync(-.2_m); //moves backward with mobile goal picked up
	movemogo(200, false); //leaves mobile goal alone
	chassis->moveDistanceAsync(-.2_m); //moves away from mobile goal
	chassis->waitUntilSettled(); //waits until drive is settled
}
void Right(){
	
}

void autonomous() {
	Left();
}


void opcontrol() {
	int goalHeight = 0;
	int goalHeight2 =0;
	
	while (true) {
		chassis->setMaxVelocity(150);
		chassis->getModel()->arcade(
			joystick.getAnalog(ControllerAnalog::leftY),
			joystick.getAnalog(ControllerAnalog::rightX)
		);
		if (ClampUp.isPressed()){
			Clamp.moveVelocity(-150);
		}else if(CLampDown.isPressed()){
			Clamp.moveVelocity(150);
		}else{
			Clamp.moveVelocity(0);
		}
		if (btnUp.isPressed()) {
			LtwoBar.moveVelocity(200);
			RtwoBar.moveVelocity(200);
		}else if (btnDown.isPressed()) {
      		LtwoBar.moveVelocity(-200);
			RtwoBar.moveVelocity(-200);
    	}else{
			LtwoBar.moveVelocity(0);
			RtwoBar.moveVelocity(0);
		}
		if(mogoUP.isPressed()){
			mogoMotor2.moveVelocity(-100);
		}else if(mogoDOwn.isPressed()){
			mogoMotor2.moveVelocity(100);
		}else{
			mogoMotor2.moveVelocity(0);
		}
		if(switchDriveF.isPressed())
		{
			autonomous();
		}
		pros::delay(20);
	}
}


void disabled() {}
void competition_initialize() {
}
