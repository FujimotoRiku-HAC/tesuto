#include "StoneSoldierOwnedState.h"
#include "World/World.h"

void StoneSoldierGetUpState::Enter() {
	m_Owner->ChangeMotion(StoneSoldierGetUp, false);
}

void StoneSoldierGetUpState::Execute(float delta_time) {
	if (m_Owner->IsMotionEnd()) {
		m_Owner->ChangeState(StoneSoldierOwnedState::Idle);
		return;
	}
}

void StoneSoldierGetUpState::Exit() {}

