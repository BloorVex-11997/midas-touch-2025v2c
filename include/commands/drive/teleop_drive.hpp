#pragma once

#include "subsystems/drivetrain/drivetrain.hpp"
#include "uvlib/command_api.hpp"
#include "uvlib/input/controller.hpp"
#include "uvlib/input/joystick.hpp"
#include "uvlib/input_api.hpp"

class TeleopDrive : public uvl::CommandHelper<uvl::Command, TeleopDrive> {
public:
  TeleopDrive(Drivetrain *drivetrain, uvl::Controller *controller);

  void initialize() override;

  void execute() override;

  bool is_finished() override;

  void end(bool interrupted) override;

private:
  Drivetrain *m_drivetrain;
  uvl::Joystick m_left_joystick;
};