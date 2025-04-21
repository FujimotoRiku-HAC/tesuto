#include "PlayerOwnedState.h"

void PlayerAbilityBlowAttackState::Enter() {
	m_Owner->ChangeMotion(MotionAbilityBlowAttack, false);
	m_Enemy = m_Owner->FindTargetEnemy();
	m_Owner->ResetVelocity();
	m_Owner->SetNextAttackStateFrag(false);
}

void PlayerAbilityBlowAttackState::Execute(float delta_time) {

	if (m_Owner->IsMotionEnd(BlowMotionEndTime)) {
		//�U�������������玟�̃X�e�[�g
		if (m_Owner->IsAbilityReact() || m_Owner->IsSelect()) {
			m_Owner->ChangeState(PlayerOwnedState::AbilityBlowAttackReinforcement);
			return;
		}
		m_Owner->ChangeState(PlayerOwnedState::Idle);
		return;
	}

	//�t���O�������Ă��Ȃ����G��������
	if (!m_Owner->CanAttackNextState() && m_Enemy != nullptr) {
		//�^�[�Q�b�g�����̃x�N�g���𒲂ׂ�
		GSvector3 to_target = m_Enemy->Transform().position() - m_Owner->Transform().position();
		//�O�����𒲂ׂ�
		GSvector3 forward = m_Owner->Transform().forward();
		//y�����͖���
		forward.y = 0.f;
		to_target.y = 0.f;
		//�O�����̃x�N�g���ƃ^�[�Q�b�g�����̃x�N�g���̊p�x�������߂�
		float angle = GSvector3::signedAngle(forward, to_target);
		//�����ύX
		m_Owner->Transform().rotate(0.f, angle, 0.f);
		//�߂�������~�܂�
		if (Distance()) m_Owner->ResetVelocity();
		return;
	}
}

void PlayerAbilityBlowAttackState::Exit() {
	m_Owner->ResetVelocity();
	m_Owner->SetNextAttackStateFrag(false);
}

void PlayerAbilityBlowAttackState::OnMessage(const std::string& message, void* param) {}

bool PlayerAbilityBlowAttackState::Distance()const{
	return (m_Owner->TargetDistance(m_Enemy) < StopDistance);
}