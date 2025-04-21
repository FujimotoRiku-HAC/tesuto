#include "GolemOwnedState.h"

void GolemThinkLeftMoveState::Enter() {
	m_Owner->ChangeMotion(MotionGolemLeftWalk, true);
}

void GolemThinkLeftMoveState::Execute(float delta_time) {
	if (m_Owner->IsMotionEnd()) {
		m_Owner->ChangeState(GolemOwnedState::Idle);
		return;
	}
	//�C���^�[�o�����Z
	m_Owner->DecrementInterval(delta_time);

	//��]�E�ړ�
	float angle = CLAMP(m_Owner->TargetSignedAngle(), -TurnAngle, TurnAngle) * delta_time;
	m_Owner->Transform().rotate(0.f, angle * delta_time, 0.f);
	m_Owner->Transform().translate(m_Owner->Speed() * ThinkSpeed * delta_time, 0.f, 0.f);
}

void GolemThinkLeftMoveState::Exit() {}
