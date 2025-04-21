#include "PlayerOwnedState.h"
#include "World/World.h"


void PlayerAvoidForwardState::Enter() {
	m_Owner->ChangeMotion(MotionForwardAvoidance, false);
	m_Owner->ResetVelocity();
}

void PlayerAvoidForwardState::Execute(float delta_time) {
	//‘O‚Éi‚ñ‚ÅUŒ‚
	if (m_Owner->IsMotionTime(AvoidAnimMotionEnd)) {
		m_Owner->ChangeVelocity(m_Owner->Transform().forward().normalized() * m_Owner->PlayerSpeed() * AvoidSpeed * delta_time);
		return;
	}
	if (m_Owner->IsMotionEnd(AvoidMotionEnd)) {
		m_Owner->ChangeState(PlayerOwnedState::Idle);
		return;
	}
	m_Owner->ResetVelocity();
}

void PlayerAvoidForwardState::Exit() {
	m_Owner->ResetVelocity();
	m_Owner->AvoidanceFrag(false);
	world_->DefaultTime();
}

void PlayerAvoidForwardState::OnMessage(const std::string& message, void* param) {}
