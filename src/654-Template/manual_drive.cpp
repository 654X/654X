#include "manual_drive.h"
#include "util.h"
#include "vex.h"

void manual_drive::control_drivetrain()
{
  while (1)
  {
    float throttle = deadband(controller(primary).Axis3.value(), 5);
    float turn = deadband(controller(primary).Axis1.value(), 5);

    leftDrive.spin(fwd, percent_to_volt(throttle + turn), velocity_units::volt);
    rightDrive.spin(fwd, percent_to_volt(throttle - turn), velocity_units::volt);
  }
}

void manual_drive::control_drive_tasks()
{
  thread(control_drivetrain);
}