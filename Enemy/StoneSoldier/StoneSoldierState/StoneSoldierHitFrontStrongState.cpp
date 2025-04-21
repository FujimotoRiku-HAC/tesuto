#include "StoneSoldierOwnedState.h"
#include "World/World.h"

void StoneSoldierHitFrontStrongState::Enter() {
	m_Owner->ChangeMotion(StoneSoldierFrontStrongHit, false);
	m_Owner->ResetVelocity();
}

void StoneSoldierHitFrontStrongState::Execute(float delta_time) {
	//ˆê’èŽžŠÔ“®‚­
	if (m_Owner->GetAnimationTimer(HitStrongEnd)) {
		m_Owner->ChangeVelocity((m_Owner->Transform().position() - m_Owner->GetPlayer()->Transform().position()).normalized() * m_Owner->Speed() * HitStrongSpeed * delta_time);
		return;
	}
	if (m_Owner->IsMotionEnd()) {
		m_Owner->ChangeState(StoneSoldierOwnedState::Idle);
		return;
	}
	//ˆÚ“®—Ê‰Šú‰»
	m_Owner->ResetVelocity();
}

void StoneSoldierHitFrontStrongState::Exit() {
	m_Owner->ResetVelocity();
}
