#include "PlayerOwnedState.h"

void PlayerHitStrongState::Enter() {

	m_Owner->ChangeMotion(MotionStrongHit, false);
	m_Owner->ResetVelocity();

}

void PlayerHitStrongState::Execute(float delta_time) {
	if (m_Owner->IsMotionEnd()) {
		m_Owner->ChangeState(PlayerOwnedState::GetUp);
		return;
	}
}

void PlayerHitStrongState::Exit() {}

void PlayerHitStrongState::OnMessage(const std::string& message, void* param) {}