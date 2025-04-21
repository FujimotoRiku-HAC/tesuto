#include "StoneSoldierOwnedState.h"
#include "World/World.h"

void StoneSoldierHitBackState::Enter() {
	m_Owner->ChangeMotion(StoneSoldierBackHit, false);
}

void StoneSoldierHitBackState::Execute(float delta_time) {
	if (m_Owner->IsMotionEnd()) {
		m_Owner->ChangeState(StoneSoldierOwnedState::Idle);
		return;
	}
}

void StoneSoldierHitBackState::Exit() {}
