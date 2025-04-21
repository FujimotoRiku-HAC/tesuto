#include "StoneSoldierOwnedState.h"
#include "World/World.h"

void StoneSoldierHitFrontBlowState::Enter() {
	m_Owner->ResetVelocity();
	m_Owner->ChangeMotion(StoneSoldierFrontBlowHit, false);
}

void StoneSoldierHitFrontBlowState::Execute(float delta_time) {
	//ˆê’èŽžŠÔ“®‚­
	if (m_Owner->GetAnimationTimer(HitBlowEnd)) {
		m_Owner->ChangeVelocity((m_Owner->Transform().position() - m_Owner->GetPlayer()->Transform().position()).normalized() * m_Owner->Speed() * HitBlowSpeed * delta_time);
		return;
	}
	if (m_Owner->IsMotionEnd()) {
		m_Owner->ChangeState(StoneSoldierOwnedState::GetUp);
		return;
	}
	//ˆÚ“®—Ê‰Šú‰»
	m_Owner->ResetVelocity();
}

void StoneSoldierHitFrontBlowState::Exit() {
	m_Owner->ResetVelocity();
}
