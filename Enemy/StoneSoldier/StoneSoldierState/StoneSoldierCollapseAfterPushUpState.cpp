#include "StoneSoldierOwnedState.h"

void StoneSoldierCollapseAfterPushUpState::Enter() {
	m_Owner->ChangeMotion(StoneSoldierGetUpAfterPushUp, false);
}

void StoneSoldierCollapseAfterPushUpState::Execute(float delta_time) {
	if (m_Owner->IsMotionEnd()) {
		m_Owner->ChangeState(StoneSoldierOwnedState::GetUp);
		return;
	}
}

void StoneSoldierCollapseAfterPushUpState::Exit() {}

