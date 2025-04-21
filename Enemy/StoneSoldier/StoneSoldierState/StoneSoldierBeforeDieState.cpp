#include "StoneSoldierOwnedState.h"
#include "World/World.h"

void StoneSoldierBeforeDieState::Enter() {
	m_Owner->ChangeMotion(StoneSoldierBeforeDie, false);
}

void StoneSoldierBeforeDieState::Execute(float delta_time) {
	if (m_Owner->IsMotionEnd()) {
		m_Owner->ChangeState(StoneSoldierOwnedState::Die);
		return;
	}
}

void StoneSoldierBeforeDieState::Exit() {}


