#include "main.h"
#include "joystick.hpp"
#include "odom.hpp"


// motors
pros::Motor chassis_fl(1, pros::E_MOTOR_GEARSET_18, false, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor chassis_fr(10, pros::E_MOTOR_GEARSET_18, true, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor chassis_bl(20, pros::E_MOTOR_GEARSET_18, false, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor chassis_br(11, pros::E_MOTOR_GEARSET_18, true, pros::E_MOTOR_ENCODER_DEGREES);


// joystick
Joystick controller;


float clamp(float val, float min, float max) {
  if (val < min) return min;
  if (val > max) return max;
  return val;
}


void opcontrol() {

  pros::delay(1000);
  odom::x = 0;
  odom::y = 0;

  float left = 0;
  float right = 0;

  while (true) {

    // update joystick
    controller.update();
    float l = controller.analog_left_y * 12000;
    float r = controller.analog_right_y * 12000;

    left += clamp(l - left, -18, 18);
    right += clamp(r - right, -18, 18);

    // update motors
    chassis_fl.move_voltage(left);
    chassis_fr.move_voltage(right);
    chassis_bl.move_voltage(left);
    chassis_br.move_voltage(right);

    // odom
    odom::update();
    pros::lcd::print(1, "Left:  %f      X: %f", odom::left, odom::x);
    pros::lcd::print(2, "Right: %f      Y: %f", odom::right, odom::y);
    pros::lcd::print(3, "Side : %f  Angle: %f", odom::side, odom::angle / units::DEGREES);

    chassis_fl.move_voltage(12000);
    chassis_bl.move_voltage(12000);
    chassis_fr.move_voltage(12000);
    chassis_br.move_voltage(12000);

    pros::delay(1);
	}
}
