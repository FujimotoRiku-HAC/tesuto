#include "PlayerOwnedState.h"

void PlayerAbilityRotatingAttackState::Enter() {
	m_Owner->ChangeMotion(MotionAbilityRotatingAttack, false);
	m_Enemy = m_Owner->FindTargetEnemy();
	m_Owner->ResetVelocity();
}

void PlayerAbilityRotatingAttackState::Execute(float delta_time) {

	if (m_Owner->IsMotionEnd()) {
		m_Owner->ChangeState(PlayerOwnedState::Idle);
		return;
	}
}

void PlayerAbilityRotatingAttackState::Exit() {
	m_Owner->ResetVelocity();
}

void PlayerAbilityRotatingAttackState::OnMessage(const std::string& message, void* param) {}