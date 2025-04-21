#include "PlayerOwnedState.h"

void PlayerHitForwardState::Enter() {

	m_Owner->ChangeMotion(MotionHitForward, false);
	m_Owner->ResetVelocity();

}

void PlayerHitForwardState::Execute(float delta_time) {
	if (m_Owner->IsMotionEnd()) {
		m_Owner->ChangeState(PlayerOwnedState::Idle);
		return;
	}
}

void PlayerHitForwardState::Exit() {
	m_Owner->ResetVelocity();

}

void PlayerHitForwardState::OnMessage(const std::string& message, void* param) {}