#ifndef ODOM_H_
#define ODOM_H_

#include "main.h"
#include "units.hpp"

namespace odom {

  // robot configuration
  static const units::Distance WHEEL_DIAM = 2.8308 * units::INCHES;
  static const units::Distance PRIMARY_WHEEL_DIST = 4.4328 * units::INCHES;
  static const units::Distance SIDE_WHEEL_DIST = 7.4375 * units::INCHES;

  // encoder declarations
  extern pros::ADIEncoder encoder_left;
  extern pros::ADIEncoder encoder_right;
  extern pros::ADIEncoder encoder_side;

  // position
  extern units::Distance x;
  extern units::Distance y;
  extern units::Angle angle;
  extern units::Distance left;
  extern units::Distance right;
  extern units::Distance side;

  
  // velocity
  extern units::LinearVelocity xv;
  extern units::LinearVelocity yv;
  extern units::AngularVelocity anglev;
  extern units::LinearVelocity sidev;
  extern units::LinearVelocity leftv;
  extern units::LinearVelocity rightv;
  extern units::LinearVelocity sidev;

  // update
  void update();


}

#endif