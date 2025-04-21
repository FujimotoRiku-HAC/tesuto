#include "StoneSoldierOwnedState.h"
#include "World/World.h"

void StoneSoldierIdleState::Enter() {
	m_Owner->ResetVelocity();
}

void StoneSoldierIdleState::Execute(float delta_time) {
	m_Owner->DecrementInterval(delta_time);

	//�U��
	if (IsRandomAttack()) {
		m_Owner->RandomAttack();
		return;
	}
	//��뉺����
	if (IsBack()) {
		m_Owner->ChangeState(StoneSoldierOwnedState::BackwardWalk);
		return;
	}
	//�l�q����������
	if (IsAsk()) {
		m_Owner->ThinkMove();
		return;
	}
	//����
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
