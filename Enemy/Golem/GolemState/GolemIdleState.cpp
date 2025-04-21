#include "GolemOwnedState.h"

void GolemIdleState::Enter() {}

void GolemIdleState::Execute(float delta_time) {
	m_Owner->DecrementInterval(delta_time);
	//�C���^�[�o�����ł͂Ȃ��Ȃ�
	if (!m_Owner->IsInterval()) {
		//���ړ�
		if (IsAsk()) {
			m_Owner->ThinkMove();
			return;
		}
		//��뉺����
		m_Owner->ChangeState(GolemOwnedState::BackWalk, false);
		return;
	}
	else {
		//�������s��
		if (IsLongDisAttack()) {
			m_Owner->ChangeState(GolemOwnedState::ChargeSpinAttack, false);
			return;
		}
		//�U��
		if (m_Owner->CanAttack(AttackDis)) {
			m_Owner->RandamAttack();
			return;
		}
		//����
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
