// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include "frc2/command/RepeatCommand.h"

#include <wpi/sendable/SendableBuilder.h>

using namespace frc2;

RepeatCommand::RepeatCommand(std::unique_ptr<Command>&& command) {
  CommandScheduler::GetInstance().RequireUngrouped(command.get());
  m_command = std::move(command);
  m_command->SetComposed(true);
  AddRequirements(m_command->GetRequirements());
  SetName(std::string{"Repeat("}.append(m_command->GetName()).append(")"));
}

void RepeatCommand::Initialize() {
  m_ended = false;
  m_command->Initialize();
}

void RepeatCommand::Execute() {
  if (m_ended) {
    m_ended = false;
    m_command->Initialize();
  }
  m_command->Execute();
  if (m_command->IsFinished()) {
    // restart command
    m_command->End(false);
    m_ended = true;
  }
}

bool RepeatCommand::IsFinished() {
  return false;
}

void RepeatCommand::End(bool interrupted) {
  m_command->End(interrupted);
}

bool RepeatCommand::RunsWhenDisabled() const {
  return m_command->RunsWhenDisabled();
}

Command::InterruptionBehavior RepeatCommand::GetInterruptionBehavior() const {
  return m_command->GetInterruptionBehavior();
}

void RepeatCommand::InitSendable(wpi::SendableBuilder& builder) {
  CommandBase::InitSendable(builder);
  builder.AddStringProperty(
      "command", [this] { return m_command->GetName(); }, nullptr);
}
