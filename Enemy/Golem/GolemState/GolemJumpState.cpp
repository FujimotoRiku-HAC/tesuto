#include "GolemOwnedState.h"

void GolemJumpState::Enter() {
	m_Owner->ChangeMotion(MotionGolemJump, false);
}

void GolemJumpState::Execute(float delta_time) {
	//�U���C���^�[�o�����Z
	m_Owner->DecrementInterval(delta_time);

	if (m_Owner->IsMotionEnd()) {
		m_Owner->ChangeState(GolemOwnedState::Idle);
		return;
	}
}

void GolemJumpState::Exit() {}
