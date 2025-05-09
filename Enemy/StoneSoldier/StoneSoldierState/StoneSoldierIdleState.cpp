#include "StoneSoldierOwnedState.h"
#include "World/World.h"

void StoneSoldierIdleState::Enter() {
	m_Owner->ResetVelocity();
}

void StoneSoldierIdleState::Execute(float delta_time) {
	m_Owner->DecrementInterval(delta_time);

	//攻撃
	if (IsRandomAttack()) {
		m_Owner->RandomAttack();
		return;
	}
	//後ろ下がり
	if (IsBack()) {
		m_Owner->ChangeState(StoneSoldierOwnedState::BackwardWalk);
		return;
	}
	//様子をうかがう
	if (IsAsk()) {
		m_Owner->ThinkMove();
		return;
	}
	//歩く
	m_Owner->ChangeState(StoneSoldierOwnedState::ForwardWalk);
	return;
}

void StoneSoldierIdleState::Exit() {
	m_Owner->ResetVelocity();
}

bool StoneSoldierIdleState::IsBack() const{
	return m_Owner->TargetDistance() < BackDistance;
}

bool StoneSoldierIdleState::IsAsk()const {
	return (m_Owner->TargetDistance() < AskDistance);
}

bool StoneSoldierIdleState::IsRandomAttack() const{
	return m_Owner->CanSoldierAttack(AttackDistance);
}
