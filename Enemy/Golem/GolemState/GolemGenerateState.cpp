#include "GolemOwnedState.h"

void GolemGenerateState::Enter() {
	m_Owner->ChangeMotion(MotionGolemGenerate, false);
}

void GolemGenerateState::Execute(float delta_time) {
	if (m_Owner->IsMotionEnd()) {
		m_Owner->ChangeState(GolemOwnedState::Idle);
		return;
	}
}

void GolemGenerateState::Exit() {}

