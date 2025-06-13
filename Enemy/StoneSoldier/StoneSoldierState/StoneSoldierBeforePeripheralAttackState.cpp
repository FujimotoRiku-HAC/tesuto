#include "StoneSoldierOwnedState.h"
#include "World/World.h"

void StoneSoldierBeforePeripheralAttackState::Enter() {
	m_Owner->AttackStart(true);
	m_Owner->ResetVelocity();
	m_Owner->ChangeMotion(StoneSoldierBeforePeripheralAttack, true);
}

void StoneSoldierBeforePeripheralAttackState::Execute(float delta_time) {
	//一定時間動く
	if (m_Owner->GetAnimationTimer(BeforePeripheralTimeEnd, BeforePeripheralTimeStart)) {
		m_Owner->ChangeVelocity(m_Owner->Transform().forward().normalized() * m_Owner->Speed() * BackDistance * delta_time);
		return;
	}
	if (m_Owner->IsMotionEnd()) {
		m_Owner->ChangeState(StoneSoldierOwnedState::PeripheralAttack);
		return;
	}
	//移動量初期化
	m_Owner->ResetVelocity();
}

void StoneSoldierBeforePeripheralAttackState::Exit() {
	m_Owner->ResetVelocity();
}


