#include "GolemOwnedState.h"

void GolemStanEndState::Enter() {
	m_Owner->ChangeMotion(MotionGolemStanEnd, false);
}

void GolemStanEndState::Execute(float delta_time) {
	if (m_Owner->IsMotionEnd()) {
		m_Owner->ChangeState(GolemOwnedState::Idle);
		return;
	}
}

void GolemStanEndState::Exit() {
	m_Owner->ResetStanValue();
	m_Owner->StanCheck(false);
}

