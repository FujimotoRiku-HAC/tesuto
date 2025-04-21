#include "StoneSoldierOwnedState.h"
#include "World/World.h"

void StoneSoldierIdleState::Enter() {
	m_Owner->ResetVelocity();
}

void StoneSoldierIdleState::Execute(float delta_time) {
	m_Owner->DecrementInterval(delta_time);

	//UŒ‚
	if (IsRandomAttack()) {
		m_Owner->RandomAttack();
		return;
	}
	//Œã‚ë‰º‚ª‚è
	if (IsBack()) {
		m_Owner->ChangeState(StoneSoldierOwnedState::BackwardWalk);
		return;
	}
	//—lŽq‚ð‚¤‚©‚ª‚¤
	if (IsAsk()) {
		m_Owner->ThinkMove();
		return;
	}
	//•à‚­
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
