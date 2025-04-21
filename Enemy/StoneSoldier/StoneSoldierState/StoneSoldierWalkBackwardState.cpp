#include "StoneSoldierOwnedState.h"
#include "World/World.h"

void StoneSoldierWalkBackwardState::Enter() {
	m_Owner->ChangeMotion(StoneSoldierBackwardWalk, true);
	m_Owner->ResetVelocity();
}

void StoneSoldierWalkBackwardState::Execute(float delta_time) {
	if (m_Owner->IsMotionEnd()) {
		m_Owner->ChangeState(StoneSoldierOwnedState::Idle);
		return;
	}
	//UŒ‚ƒCƒ“ƒ^[ƒoƒ‹Œ¸ŽZ
	m_Owner->DecrementInterval(delta_time);
	//‰ñ“]EˆÚ“®
	float angle = CLAMP(m_Owner->TargetSignedAngle(), -TurnAngle, TurnAngle);
	m_Owner->Transform().rotate(0.f, angle * delta_time, 0.f);
	m_Owner->Transform().translate(0.f, 0.f, (m_Owner->Speed() * -SideBackWalk) * delta_time);
}

void StoneSoldierWalkBackwardState::Exit() {
	m_Owner->ResetVelocity();
}