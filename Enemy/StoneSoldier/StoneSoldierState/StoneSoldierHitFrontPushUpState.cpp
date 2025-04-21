#include "StoneSoldierOwnedState.h"
#include "World/World.h"

void StoneSoldierHitFrontPushUpState::Enter() {
	m_Owner->ResetVelocity();
	m_Owner->ToUseOrNotUseGravity(false);
	m_Owner->ChangeMotion(StoneSoldierFrontPushUpHit, false);
}

void StoneSoldierHitFrontPushUpState::Execute(float delta_time) {
	//ˆê’èŽžŠÔ“®‚­
	if (m_Owner->GetAnimationTimer(HitPushEnd)) {
		m_Owner->ChangeVelocity((m_Owner->Transform().position() - m_Owner->GetPlayer()->Transform().position()).normalized() * m_Owner->Speed() * HitPushSpeed * delta_time);
		m_Owner->Velocity_Y(HitPushY);
		return;
	}
	if (m_Owner->IsMotionEnd()) {
		m_Owner->ChangeState(StoneSoldierOwnedState::CollapseAfterPushUp);
		return;
	}
	//ˆÚ“®—Ê‰Šú‰»
	m_Owner->ResetVelocity();
	//d—Í•t—^
	m_Owner->ToUseOrNotUseGravity(true);
}

void StoneSoldierHitFrontPushUpState::Exit() {
	m_Owner->ResetVelocity();
	m_Owner->ToUseOrNotUseGravity(true);
}
