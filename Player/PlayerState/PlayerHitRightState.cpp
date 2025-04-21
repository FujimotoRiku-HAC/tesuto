#include "PlayerOwnedState.h"

void PlayerHitRightState::Enter() {
	m_Owner->ChangeMotion(MotionHitRight, false);
	m_Owner->ResetVelocity();
}

void PlayerHitRightState::Execute(float delta_time) {
	if (m_Owner->IsMotionEnd()) {
		m_Owner->ChangeState(PlayerOwnedState::Idle);
		return;
	}
}

void PlayerHitRightState::Exit() {
	m_Owner->ResetVelocity();
}

void PlayerHitRightState::OnMessage(const std::string& message, void* param) {}