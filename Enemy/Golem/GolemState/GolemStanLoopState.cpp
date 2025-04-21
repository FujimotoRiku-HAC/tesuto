#include "GolemOwnedState.h"

void GolemStanLoopState::Enter() {
	m_Owner->ChangeMotion(MotionGolemStanLoop, true);
	m_Return = ReturnTimer;
}

void GolemStanLoopState::Execute(float delta_time) {
	m_Return -= delta_time;
	//���^�[���l���Ȃ��Ȃ�����ʏ�
	if (m_Return <= 0.f) {
		m_Owner->ChangeState(GolemOwnedState::StanEnd);
		return;
	}
}

void GolemStanLoopState::Exit() {}

