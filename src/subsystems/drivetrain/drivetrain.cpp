#include "subsystems/drivetrain/drivetrain.hpp"
#include "constants.hpp"

Drivetrain::Drivetrain()
    : m_voltage(0), m_left_motor(Constants::LEFT_MOTOR_ID),
      m_right_motor(Constants::RIGHT_MOTOR_Id) {}

void Drivetrain::set_voltage(int voltage) {
  m_voltage = voltage;
  m_left_motor.move(voltage);
  m_right_motor.move(-voltage);
}

int Drivetrain::get_voltage() const { return m_voltage; }