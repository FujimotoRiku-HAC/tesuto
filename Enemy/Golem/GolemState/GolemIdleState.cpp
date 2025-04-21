#include "GolemOwnedState.h"

void GolemIdleState::Enter() {}

void GolemIdleState::Execute(float delta_time) {
	m_Owner->DecrementInterval(delta_time);
	//インターバル中ではないなら
	if (!m_Owner->IsInterval()) {
		//横移動
		if (IsAsk()) {
			m_Owner->ThinkMove();
			return;
		}
		//後ろ下がり
		m_Owner->ChangeState(GolemOwnedState::BackWalk, false);
		return;
	}
	else {
		//遠距離行動
		if (IsLongDisAttack()) {
			m_Owner->ChangeState(GolemOwnedState::ChargeSpinAttack, false);
			return;
		}
		//攻撃
		if (m_Owner->CanAttack(AttackDis)) {
			m_Owner->RandamAttack();
			return;
		}
		//歩く
		m_Owner->ChangeState(GolemOwnedState::ForwardWalk, false);
		return;
		
	}
}

void GolemIdleState::Exit() {}

bool GolemIdleState::IsAsk() const{
	return (m_Owner->TargetDistance() > AskDis);
}

bool GolemIdleState::IsLongDisAttack() const{
	return (m_Owner->TargetDistance() > LongAttackDis);
}
