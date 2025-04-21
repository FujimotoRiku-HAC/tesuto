#include "PlayerOwnedState.h"

void PlayerAttackThirdState::Enter() {
	m_Owner->ChangeMotion(MotionThirdAttack, false);
	m_Owner->ResetVelocity();
	m_Owner->SetNextAttackStateFrag(false);
	m_Enemy = m_Owner->FindTargetEnemy();
}

void PlayerAttackThirdState::Execute(float delta_time) {
	if (m_Owner->IsMotionEnd()) {
		m_Owner->ChangeState(PlayerOwnedState::Idle);
		return;
	}

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
		if (Distance()) m_Owner->ResetVelocity();
		return;
	}

	//���
	if (InputManager::Instance().IsBottonDown(InputManager::InputType::Avoid)) {
		m_Owner->SetNextAttackStateFrag(false);
		m_Owner->DirectionOfAvoidance(InputManager::Instance().LeftAxis());
		return;
	}
}

void PlayerAttackThirdState::Draw() const{}

void PlayerAttackThirdState::Exit() {
	m_Owner->ResetVelocity();
	m_Owner->SetNextAttackStateFrag(false);
}

void PlayerAttackThirdState::OnMessage(const std::string& message, void* param) {}

bool PlayerAttackThirdState::Distance() const {
	return (m_Owner->TargetDistance(m_Enemy) < StopDistance);
}