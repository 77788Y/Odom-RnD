#include "odom.hpp"
#include <cmath>

namespace odom {

  // robot configuration
  static const units::Distance TICK_INCHES_RATIO = WHEEL_DIAM * M_PI / 360.0;

  // encoder inits
  pros::ADIEncoder encoder_left('A', 'B');
  pros::ADIEncoder encoder_right('G', 'H');
  pros::ADIEncoder encoder_side('C', 'D');

  // position
  units::Distance x = 0;
  units::Distance y = 0;
  units::Angle angle = 0;
  units::Distance left = encoder_left.get_value();
  units::Distance right = encoder_right.get_value();
  units::Distance side = encoder_side.get_value();

  // velocity
  units::LinearVelocity xv = 0;
  units::LinearVelocity yv = 0;
  units::AngularVelocity anglev = 0;
  units::LinearVelocity leftv = 0;
  units::LinearVelocity rightv = 0;
  units::LinearVelocity sidev = 0;

  // time of last update
  units::Time last_update = pros::millis() * units::MS;

  // update
  void update() {

    // calculate delta t
    units::Time delta_t = pros::millis() * units::MS - last_update;

    // update side distances
    units::Distance absolute_l = -encoder_left.get_value() * TICK_INCHES_RATIO;
    units::Distance absolute_r = -encoder_right.get_value() * TICK_INCHES_RATIO;
    units::Distance absolute_s = -encoder_side.get_value() * TICK_INCHES_RATIO;
    units::Distance l = absolute_l - left;
    units::Distance r = absolute_r - right;
    units::Distance d = (l + r) / 2.0;
    units::Distance s = absolute_s - side;
    left = absolute_l;
    right = absolute_r;
    side = absolute_s;

    // update side velocities
    leftv = l / delta_t;
    rightv = r / delta_t;
    sidev = s / delta_t;

    // calculate angle
    units::Angle absolute_a = ((right - left) / (PRIMARY_WHEEL_DIST * 2)) * units::RADIANS;
    units::Angle a = absolute_a - angle;
    units::Angle prev_a = angle;
    angle = absolute_a;

    // update angular velocity
    anglev = angle / delta_t;

    units::Distance delta_x;
    units::Distance delta_y;

    // linear motion
    if (!a) {
      delta_x = d * cos(angle) + s * sin(angle);
      delta_y = d * sin(angle) - s * cos(angle);
    }

    // arc motion
    else {

      // calculate radii
      units::Distance radius_primary = d / a;
      units::Distance radius_side = s / a - SIDE_WHEEL_DIST;

      // calculate deltas
      float cos_a = cos(a);
      float sin_a = sin(a);
      units::Distance delta_x_primary = radius_primary * sin_a; // cos(x - 90 deg) == sin(x)
      units::Distance delta_y_primary = radius_primary * (1.0 - cos_a); // sin(x - 90 deg) == -cos(x), shift it up by radius0
      units::Distance delta_x_side = radius_side * (1.0 - cos_a);
      units::Distance delta_y_side = radius_side * -sin_a;

      // combine deltas
      delta_x = delta_x_primary + delta_x_side;
      delta_y = delta_y_primary + delta_y_side;

      // rotate point around bot to be field-centric
      cos_a = cos(prev_a);
      sin_a = sin(prev_a);
      delta_x = delta_x * cos_a - delta_y * sin_a;
      delta_y = delta_y * cos_a + delta_x * sin_a;
    }

    // add to position
    x += delta_x;
    y += delta_y;

    // update translational velocity
    xv = delta_x / delta_t;
    yv = delta_y / delta_t;

  }
}