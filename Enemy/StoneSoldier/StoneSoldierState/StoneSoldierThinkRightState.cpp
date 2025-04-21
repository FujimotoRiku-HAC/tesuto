#include "StoneSoldierOwnedState.h"
#include "World/World.h"

void StoneSoldierThinkRightState::Enter() {
	m_Owner->ChangeMotion(StoneSoldierRightThink, false);
}

void StoneSoldierThinkRightState::Execute(float delta_time) {
	if (m_Owner->IsMotionEnd()) {
		m_Owner->ChangeState(StoneSoldierOwnedState::Idle);
		return;
	}
	//UŒ‚ƒCƒ“ƒ^[ƒoƒ‹Œ¸ŽZ
	m_Owner->DecrementInterval(delta_time);
	//‰ñ“]EˆÚ“®
	float angle = CLAMP(m_Owner->TargetSignedAngle(), -TurnAngle, TurnAngle);
	m_Owner->Transform().rotate(0.f, angle * delta_time, 0.f);
	m_Owner->Transform().translate(m_Owner->Speed() * SideBackWalk * delta_time, 0.f, 0.f);
}

void StoneSoldierThinkRightState::Exit() {}

