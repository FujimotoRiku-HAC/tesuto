#include "StoneSoldierOwnedState.h"
#include "World/World.h"

void StoneSoldierHitBackPushUpState::Enter() {
	m_Owner->ResetVelocity();
	m_Owner->ToUseOrNotUseGravity(false);
	m_Owner->ChangeMotion(StoneSoldierBackPushUpHit, false);
}

void StoneSoldierHitBackPushUpState::Execute(float delta_time) {
	//一定時間動く
	if (m_Owner->GetAnimationTimer(HitPushEnd)) {
		m_Owner->ChangeVelocity((m_Owner->Transform().position() - m_Owner->GetPlayer()->Transform().position()).normalized() * m_Owner->Speed() * HitPushSpeed * delta_time);
		m_Owner->Velocity_Y(HitPushY);
		return;
	}
	if (m_Owner->IsMotionEnd()) {
		m_Owner->ChangeState(StoneSoldierOwnedState::CollapseAfterPushUp);
		return;
	}
	//移動量初期化
	m_Owner->ResetVelocity();
	//重力付与
	m_Owner->ToUseOrNotUseGravity(true);
}

void StoneSoldierHitBackPushUpState::Exit() {
	m_Owner->ResetVelocity();
	m_Owner->ToUseOrNotUseGravity(true);
}
