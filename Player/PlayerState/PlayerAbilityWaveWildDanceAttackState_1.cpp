#include "PlayerOwnedState.h"

void PlayerAbilityWaveWildDanceAttackState_1::Enter() {
	m_Owner->ChangeMotion(MotionAbilityWaveWildDance_1, false);
	m_Owner->ResetVelocity();
	m_Owner->SetNextAttackStateFrag(false);
	m_Enemy = m_Owner->FindTargetEnemy();
}

void PlayerAbilityWaveWildDanceAttackState_1::Execute(float delta_time) {

	if (m_Owner->IsMotionEnd(30.0f)) {
		m_Owner->ChangeState(PlayerOwnedState::AbilityWaveWildDance_2);
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
		return;
	}
}

void PlayerAbilityWaveWildDanceAttackState_1::Exit() {
	m_Owner->ResetVelocity();
	m_Owner->SetNextAttackStateFrag(false);
}

void PlayerAbilityWaveWildDanceAttackState_1::OnMessage(const std::string& message, void* param) {}