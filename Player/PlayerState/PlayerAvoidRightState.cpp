#include "PlayerOwnedState.h"
#include "World/World.h"


void PlayerAvoidRightState::Enter() {
	m_Owner->ChangeMotion(MotionRightAvoidance, false);
	m_Owner->ResetVelocity();
}

void PlayerAvoidRightState::Execute(float delta_time) {
	//‘O‚Éi‚ñ‚ÅUŒ‚
	if (m_Owner->IsMotionTime(AvoidAnimMotionEnd)) {
		m_Owner->ChangeVelocity(m_Owner->Transform().right().normalized() * m_Owner->PlayerSpeed() * AvoidSpeed * delta_time);
		return;
	}
	if (m_Owner->IsMotionEnd(AvoidMotionEnd)) {
		m_Owner->ChangeState(PlayerOwnedState::Idle);
		return;
	}
	m_Owner->ResetVelocity();

}

void PlayerAvoidRightState::Exit() {
	m_Owner->ResetVelocity();
	m_Owner->AvoidanceFrag(false);
	world_->DefaultTime();
}

void PlayerAvoidRightState::OnMessage(const std::string& message, void* param) {}

