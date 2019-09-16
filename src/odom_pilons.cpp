// #include "odom.hpp"
// #include <cmath>

// namespace odom {

//   // robot configuration
//   static const units::Distance TICK_INCHES_RATIO = WHEEL_DIAM * M_PI / 360.0;

//   // encoder inits
//   pros::ADIEncoder encoder_left('A', 'B');
//   pros::ADIEncoder encoder_right('G', 'H');
//   pros::ADIEncoder encoder_side('C', 'D');

//   // position
//   units::Distance x = 0;
//   units::Distance y = 0;
//   units::Angle angle = 0;
//   units::Distance left = encoder_left.get_value();
//   units::Distance right = encoder_right.get_value();
//   units::Distance side = encoder_side.get_value();

//   // velocity
//   units::LinearVelocity xv = 0;
//   units::LinearVelocity yv = 0;
//   units::AngularVelocity anglev = 0;
//   units::LinearVelocity leftv = 0;
//   units::LinearVelocity rightv = 0;
//   units::LinearVelocity sidev = 0;

//   // time of last update
//   units::Time last_update = pros::millis() * units::MS;

//   // update
//   void update() {

//     // calculate delta t
//     units::Time delta_t = pros::millis() * units::MS - last_update;

//     // update side distances
//     units::Distance absolute_l = -encoder_left.get_value() * TICK_INCHES_RATIO;
//     units::Distance absolute_r = -encoder_right.get_value() * TICK_INCHES_RATIO;
//     units::Distance absolute_s = encoder_side.get_value() * TICK_INCHES_RATIO;
//     units::Distance l = absolute_l - left;
//     units::Distance r = absolute_r - right;
//     units::Distance d = (l + r) / 2.0;
//     units::Distance s = absolute_s - side;
//     left = absolute_l;
//     right = absolute_r;
//     side = absolute_s;

//     // update side velocities
//     leftv = l / delta_t;
//     rightv = r / delta_t;
//     sidev = s / delta_t;

//     // calculate angle
//     units::Angle absolute_a = ((right - left) / (PRIMARY_WHEEL_DIST * 2)) * units::RADIANS;
//     units::Angle a = absolute_a - angle;
//     units::Angle prev_a = angle;
//     angle = absolute_a;

//     // update angular velocity
//     anglev = angle / delta_t;

//     units::Distance h_primary;
//     units::Angle i;
//     units::Distance h_side;

//     if (a) {
//       units::Distance radius_primary = r / a;
//       units::Distance radius_side = s / a;

//       i = a / 2.0;
//       float sin_i = sin(i);

//       h_primary = (radius_primary + PRIMARY_WHEEL_DIST) * sin_i * 2.0;
//       h_side = (radius_side + SIDE_WHEEL_DIST) * sin_i * 2.0;
//     }
//     else {
//       h_primary = d;
//       i = 0;
//       h_side = s;
//     }

//     units::Angle p = i + prev_a;
//     float cos_p = cos(p);
//     float sin_p = sin(p);

//     units::Distance delta_x = h_primary * cos_p + h_side * sin_p;
//     units::Distance delta_y = h_primary * sin_p + h_side * cos_p;

//     // add to position
//     x += delta_x;
//     y += delta_y;

//     // update translational velocity
//     xv = delta_x / delta_t;
//     yv = delta_y / delta_t;

//   }
// }