#include "GolemOwnedState.h"

void GolemWalkForwardState::Enter() {
	m_Owner->ChangeMotion(MotionGolemForwardWalk, true);
}

void GolemWalkForwardState::Execute(float delta_time) {
	if (m_Owner->IsMotionEnd()) {
		m_Owner->ChangeState(GolemOwnedState::Idle);
		return;
	}
	//�C���^�[�o���̌��Z
	m_Owner->DecrementInterval(delta_time);

	//��]�E�ړ�
	float angle = CLAMP(m_Owner->TargetSignedAngle(), -TurnAngle, TurnAngle) * delta_time;
	m_Owner->Transform().rotate(0.f, angle * delta_time, 0.f);
	m_Owner->Transform().translate(0.f, 0.f, m_Owner->Speed() * delta_time);
}

void GolemWalkForwardState::Exit() {}
