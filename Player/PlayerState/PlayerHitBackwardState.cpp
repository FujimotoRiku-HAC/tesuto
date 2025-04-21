#include "PlayerOwnedState.h"

void PlayerHitBackwardState::Enter() {
	m_Owner->ChangeMotion(MotionHitBackward, false);
	m_Owner->ResetVelocity();
}

void PlayerHitBackwardState::Execute(float delta_time) {
	if (m_Owner->IsMotionEnd()) {
		m_Owner->ChangeState(PlayerOwnedState::Idle);
		return;
	}
}

void PlayerHitBackwardState::Exit() {
	m_Owner->ResetVelocity();
}

void PlayerHitBackwardState::OnMessage(const std::string& message, void* param) {}