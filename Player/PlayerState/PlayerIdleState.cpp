#include "PlayerOwnedState.h"

void PlayerIdleState::Enter() {
	m_Timer = 1.f;
	m_Owner->ChangeMotion(MotionPlayerIdle, true);

}

void PlayerIdleState::Execute(float delta_time) {
	//�������Ă��Ȃ���΃A�C�h��
	int motion = PlayerAnim::MotionPlayerIdle;
	if (m_Owner->IsSelect())return;
	//�ړ��ʂ̍X�V
	GSvector3 velocity = m_Owner->InputVelocity(InputManager::Instance().LeftAxis());
	//�X�s�[�h�擾
	float speed = m_Owner->PlayerSpeed();

	if (velocity != GSvector3::zero()) {
		motion = PlayerAnim::MotionFwdWalk;
		speed = m_Owner->PlayerSpeed();
	}
	//���[�V�����̍X�V
	m_Owner->ChangeMotion(motion, true);

	velocity = velocity.normalized() * speed * delta_time;
	m_Owner->ChangeVelocity(velocity);
	m_Owner->Interpolation(velocity, delta_time);

	//�U���X�e�[�g�ɑJ��
	if (InputManager::Instance().IsBottonDown(InputManager::InputType::Attack)) {
		m_Owner->ChangeState(PlayerOwnedState::FirstAttack);
		return;
	}

	//���
	if (InputManager::Instance().IsBottonDown(InputManager::InputType::Avoid) && m_Timer >= 1.0f) {
		m_Owner->ChangeState(PlayerOwnedState::AvoidanceForward);
		return;
	}
	m_Timer += delta_time;
}

void PlayerIdleState::Exit() {}

void PlayerIdleState::OnMessage(const std::string& message, void* param) {}