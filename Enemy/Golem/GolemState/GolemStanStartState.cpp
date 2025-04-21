#include "GolemOwnedState.h"

void GolemStanStartState::Enter() {
	m_Owner->ChangeMotion(MotionGolemStanStart, false);
	m_Owner->ResetVelocity();
	m_Owner->StanCheck(true);
}

void GolemStanStartState::Execute(float delta_time) {
	if (m_Owner->IsMotionEnd()) {
		m_Owner->ChangeState(GolemOwnedState::StanLoop);
		return;
	}
}

void GolemStanStartState::Exit() {}

