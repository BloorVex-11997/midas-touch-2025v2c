#pragma once

#include "pros/motors.hpp"
#include "uvlib/input/controller.hpp"
#include "uvlib/subsystem.hpp"

class Drivetrain : public uvl::Subsystem {
public:
  Drivetrain();

  void set_voltage(int voltage);

  int get_voltage() const;

private:
  pros::Motor m_left_motor;
  pros::Motor m_right_motor;

  int m_voltage;
};