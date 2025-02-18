#include "main.h"
#include "commands/drive/teleop_drive.hpp"
#include "pros/misc.h"
#include "subsystems/drivetrain/drivetrain.hpp"
#include "uvlib/enums.hpp"
#include "uvlib/input/controller.hpp"
#include "uvlib/input/trigger.hpp"
#include "uvlib/scheduler.hpp"

Drivetrain drivetrain;
uvl::Controller master(pros::E_CONTROLLER_MASTER);

std::string routine_a_state = "invalid";
std::string routine_b_state = "invalid";

void noop() {}

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
  pros::lcd::initialize();

  drivetrain.set_default_command(TeleopDrive(&drivetrain, &master).to_ptr());

  int artificial_tick = 0;

  /* Debugging Command */
  uvl::Scheduler::get_instance().schedule_command(
      uvl::FunctionCommand(
          []() {},
          [&artificial_tick]() {
            pros::lcd::set_text(0, "Tick Number: " +
                                       std::to_string(artificial_tick));
            pros::lcd::set_text(
                1, "Scheduled Commands: " +
                       std::to_string(uvl::Scheduler::get_instance()
                                          .get_scheduled_commands()
                                          .size()));
            pros::lcd::set_text(
                2, "Scheduler Owned: " +
                       std::to_string(uvl::Scheduler::get_instance()
                                          .get_owned_commands()
                                          .size()));

            pros::lcd::set_text(
                3, "Active Subsystems: " +
                       std::to_string(uvl::Scheduler::get_instance()
                                          .get_active_subsystems()
                                          .size()));

            pros::lcd::set_text(
                4, std::string("Drivetrain Default: ") +
                       (drivetrain.get_default_command()->get()->is_alive()
                            ? "alive"
                            : "paused"));

            pros::lcd::set_text(5,
                                std::string("Routine A: ") + routine_a_state);

            pros::lcd::set_text(6,
                                std::string("Routine B: ") + routine_b_state);

            artificial_tick++;
          },
          []() { return false; }, [](bool) {}, {})
          .to_ptr());

  /* Initialize the scheduler */
  uvl::Scheduler::get_instance().initialize();
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
void autonomous() {}

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
  /*
pros::Controller master(pros::E_CONTROLLER_MASTER);
pros::MotorGroup left_mg({1, -2, 3});   // Creates a motor group with forwards
                                    // ports 1 & 3 and reversed port 2
pros::MotorGroup right_mg({-4, 5, -6}); // Creates a motor group with forwards
                                    // port 5 and reversed ports 4 & 6

while (true) {
pros::lcd::print(0, "%d %d %d",
               (pros::lcd::read_buttons() & LCD_BTN_LEFT) >> 2,
               (pros::lcd::read_buttons() & LCD_BTN_CENTER) >> 1,
               (pros::lcd::read_buttons() & LCD_BTN_RIGHT) >>
                   0); // Prints status of the emulated screen LCDs

// Arcade control scheme
int dir = master.get_analog(
  ANALOG_LEFT_Y); // Gets amount forward/backward from left joystick
int turn = master.get_analog(
  ANALOG_RIGHT_X);       // Gets the turn left/right from right joystick
left_mg.move(dir - turn);  // Sets left motor voltage
right_mg.move(dir + turn); // Sets right motor voltage
pros::delay(20);           // Run for 20 ms then update
}
          */

  uint32_t routine_a_start = 0;
  uvl::FunctionCommand routine_a(
      [&routine_a_start]() {
        routine_a_state = "initialized";
        routine_a_start = pros::millis();
      },
      []() {
        routine_a_state =
            "executing - " + std::to_string(pros::millis()) + " " +
            std::to_string(master.get_digital(uvl::TriggerButton::kA));
      },
      [&routine_a_start]() { return pros::millis() >= routine_a_start + 4; },
      [](bool interrupted) {
        routine_a_state = interrupted ? "interrupted" : "success";
      },
      {});

  uint32_t routine_b_start = 0;
  uvl::FunctionCommand routine_b(
      [&routine_b_start]() {
        routine_b_state = "initialized";
        routine_b_start = pros::millis();
      },
      []() {
        routine_b_state =
            "executing - " + std::to_string(pros::millis()) + " " +
            std::to_string(master.get_digital(uvl::TriggerButton::kB));
      },
      [&routine_b_start]() { return pros::millis() >= routine_b_start + 10; },
      [](bool interrupted) {
        routine_b_state = interrupted ? "interrupted" : "success";
      },
      {});

  master.get_trigger(uvl::TriggerButton::kA)
      .on_true(std::move(routine_a).to_ptr());

  master.get_trigger(uvl::TriggerButton::kB)
      .while_true(std::move(routine_b).to_ptr());

  uvl::Scheduler::get_instance().mainloop();
}