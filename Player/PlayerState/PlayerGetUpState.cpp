#include "PlayerOwnedState.h"

void PlayerGetUpState::Enter() {
	m_Owner->ChangeMotion(MotionGetUp, false);
}

void PlayerGetUpState::Execute(float delta_time) {
	if (m_Owner->IsMotionEnd()) {
		m_Owner->ChangeState(PlayerOwnedState::Idle);
		return;
	}
}

void PlayerGetUpState::Exit() {}

void PlayerGetUpState::OnMessage(const std::string& message, void* param) {}