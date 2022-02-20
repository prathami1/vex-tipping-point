#include "main.h"

pros::Controller master(pros::E_CONTROLLER_MASTER);
pros::Motor back_left_mtr(2);
pros::Motor back_right_mtr(3);
pros::Motor fwd_left_mtr(1);
pros::Motor fwd_right_mtr(10);
pros::Motor right_lift(7);
pros::Motor left_lift(4);
pros::Motor clamp(6);
pros::Motor mogoLift(9);

/**
 * A callback function for LLEMU's center button.
 *
 * When this callback is fired, it will toggle line 2 of the LCD text between
 * "I was pressed!" and nothing.
 */
void on_center_button() {
  static bool pressed = false;
  pressed = !pressed;
  if (pressed) {
    pros::lcd::set_text(2, "I was pressed!");
  } else {
    pros::lcd::clear_line(2);
  }
}

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
  pros::lcd::initialize();
  pros::lcd::set_text(1, "TWST");

  pros::lcd::register_btn1_cb(on_center_button);

  mogoLift.set_brake_mode(MOTOR_BRAKE_HOLD);
  left_lift.set_brake_mode(MOTOR_BRAKE_HOLD);
  right_lift.set_brake_mode(MOTOR_BRAKE_HOLD);
  clamp.set_brake_mode(MOTOR_BRAKE_HOLD);
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {}

/*void auton(){

std::shared_ptr<okapi::ChassisController> drive =
  okapi::ChassisControllerBuilder()
    .withMotors(okapi::MotorGroup({1, 2}), okapi::MotorGroup({10, 3}))
    .withDimensions(okapi::AbstractMotor::gearset::green, {{4_in, 17.5_in}, okapi::imev5BlueTPR})
    .build();

}*/
void movemogo(int x, bool b) {
  if (b) {
    mogoLift.move_velocity(x);
  } else {
    mogoLift.move_velocity(-x);
  }
}
/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */

void autonomous() {
  std::shared_ptr < okapi::ChassisController > drive =
    okapi::ChassisControllerBuilder()
    .withMotors(okapi::MotorGroup({
      1,
      2
    }), okapi::MotorGroup({
      3,
      10
    }))
    .withDimensions(okapi::AbstractMotor::gearset::blue, {
      {
        2.5_ in , 17.5_ in
      },
      okapi::imev5BlueTPR
    })
    .build();

  drive -> moveDistanceAsync(-6_ ft);
  movemogo(100, false);
  drive -> waitUntilSettled();
  drive -> moveDistance(6_ ft);

}

/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */
void opcontrol() {

  while (true) {

    //getting values from controller
    int horizontal = -master.get_analog(ANALOG_LEFT_Y);
    int forward = master.get_analog(ANALOG_RIGHT_X);

    int l1 = master.get_digital(DIGITAL_L1);
    int l2 = master.get_digital(DIGITAL_L2);

    int r1 = master.get_digital(DIGITAL_R1);
    int r2 = master.get_digital(DIGITAL_R2);

    int a = master.get_digital(pros::E_CONTROLLER_DIGITAL_B);
    int b = master.get_digital(pros::E_CONTROLLER_DIGITAL_X);

    //used to lower max speed of
    int multiplier = 1;

    back_left_mtr.move_velocity((forward - horizontal) / multiplier);
    fwd_left_mtr.move_velocity((forward - horizontal) / multiplier);
    back_right_mtr.move_velocity((forward + horizontal) / multiplier);
    fwd_right_mtr.move_velocity((forward + horizontal) / multiplier);

    //to move the back mogo lift
    //buttons: L1, L2
    if (r2) {
      mogoLift.move_velocity(100);
    } else if (r1) {
      mogoLift.move_velocity(-100);
    } else {
      mogoLift.move_velocity(0);
    }

    //to move the four bar
    //buttons: R1, R2
    if (l1) {
      left_lift.move_velocity(100);
      right_lift.move_velocity(-100);
    } else if (l2) {
      left_lift.move_velocity(-100);
      right_lift.move_velocity(100);
    } else {
      left_lift.move_velocity(0);
      right_lift.move_velocity(0);
    }

    //used to move the clamp
    //buttons: a, b
    if (a) {
      clamp.move_velocity(500);
    } else if (b) {
      clamp.move_velocity(-500);
    } else {
      clamp.move_velocity(0);
    }

    pros::delay(20);
  }
}