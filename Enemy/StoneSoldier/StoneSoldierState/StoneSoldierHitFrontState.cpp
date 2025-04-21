#include "StoneSoldierOwnedState.h"
#include "World/World.h"

void StoneSoldierHitFrontState::Enter() {
	m_Owner->ChangeMotion(StoneSoldierFrontHit, false);
}

void StoneSoldierHitFrontState::Execute(float delta_time) {
	if (m_Owner->IsMotionEnd()) {
		m_Owner->ChangeState(StoneSoldierOwnedState::Idle);
		return;
	}
}

void StoneSoldierHitFrontState::Exit() {}
