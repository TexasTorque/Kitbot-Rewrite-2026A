#include "subsystems/launcherSubsystem.hpp"
#include "frc/smartdashboard/SmartDashboard.h"
#include "frc2/command/CommandPtr.h"
#include "frc2/command/Commands.h"

#include "units/voltage.h"

launcherSubsystem::launcherSubsystem(std::unique_ptr<launcherIO> io)
    : m_io(std::move(io)),
      m_state(launcherStateEnum::Off, [this](const launcherStateEnum& newState) { ApplyState(newState); }) {
  SetName("launcherSubsystem");
}
void launcherSubsystem::ConfigureRollerMotor() {
  rev::spark::SparkMaxConfig config{};

  config.SmartCurrentLimit(60);

  m_rollerMotor.Configure(config, rev::ResetMode::kResetSafeParameters,
                          rev::PersistMode::kPersistParameters);
}

void launcherSubsystem::SetRollerVoltage(units::volt_t voltage) {
  m_rollerMotor.SetVoltage(voltage);
}

frc2::CommandPtr launcherSubsystem::RunLauncherRollerCommand() {
  return frc2::cmd::StartEnd([this] { SetRollerVoltage(7_V); },
                             [this] { SetRollerVoltage(0_V); }, {this});
}

void launcherSubsystem::Periodic() {
  frc::SmartDashboard::PutNumber("LauncherEncoder",
                                 m_rollerMotor.GetEncoder().GetPosition());
}

