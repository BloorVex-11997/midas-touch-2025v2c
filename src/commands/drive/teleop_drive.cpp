#include "commands/drive/teleop_drive.hpp"
#include "subsystems/drivetrain/drivetrain.hpp"
#include "uvlib/input/controller.hpp"

TeleopDrive::TeleopDrive(Drivetrain *drivetrain, uvl::Controller *controller)
    : m_left_joystick(controller->left_joystick()), m_drivetrain(drivetrain) {

  add_requirements({drivetrain});
}

void TeleopDrive::initialize() { m_drivetrain->set_voltage(0); }

void TeleopDrive::execute() {
  int input_y = m_left_joystick.get_y();
  m_drivetrain->set_voltage(input_y);
}

bool TeleopDrive::is_finished() { return false; }

void TeleopDrive::end(bool interrupted) { m_drivetrain->set_voltage(0); }