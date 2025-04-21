#include "PlayerOwnedState.h"

void PlayerHitLeftState::Enter() {
	m_Owner->ChangeMotion(MotionHitLeft, false);
	m_Owner->ResetVelocity();
}

void PlayerHitLeftState::Execute(float delta_time) {
	if (m_Owner->IsMotionEnd()) {
		m_Owner->ChangeState(PlayerOwnedState::Idle);
		return;
	}
}

void PlayerHitLeftState::Exit() {
	m_Owner->ResetVelocity();
}

void PlayerHitLeftState::OnMessage(const std::string& message, void* param) {}