#include "PlayerOwnedState.h"

void PlayerDieState::Enter() {
	m_Owner->ChangeMotion(MotionDie, false);
}

void PlayerDieState::Execute(float delta_time) {
	if (m_Owner->IsMotionEnd()) {
		m_Owner->ChangeState(PlayerOwnedState::DieLoop);
		return;
	}
}

void PlayerDieState::Exit() {}

void PlayerDieState::OnMessage(const std::string& message, void* param) {}