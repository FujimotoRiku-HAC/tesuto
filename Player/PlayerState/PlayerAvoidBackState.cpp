#include "PlayerOwnedState.h"
#include "World/World.h"

void PlayerAvoidBackState::Enter() {
	m_Owner->ChangeMotion(MotionBackAvoidance, false);
	m_Owner->ResetVelocity();

}

void PlayerAvoidBackState::Execute(float delta_time) {
	//ŽžŠÔ“à‚È‚ç“®‚­
	if (m_Owner->IsMotionTime(AvoidAnimMotionEnd, AvoidAnimMotionStart)) {
		m_Owner->ChangeVelocity(m_Owner->Transform().forward().normalized() * m_Owner->PlayerSpeed() * -AvoidSpeed * delta_time);
		return;
	}
	if (m_Owner->IsMotionEnd(AvoidMotionEnd)) {
		m_Owner->ChangeState(PlayerOwnedState::Idle);
		return;
	}
	m_Owner->ResetVelocity();
}

void PlayerAvoidBackState::Exit() {
	m_Owner->ResetVelocity();
	m_Owner->AvoidanceFrag(false);
	world_->DefaultTime();
}

void PlayerAvoidBackState::OnMessage(const std::string& message, void* param) {}