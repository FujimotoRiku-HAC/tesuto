#include "StoneSoldierOwnedState.h"
#include "World/World.h"

void StoneSoldierSpawnState::Enter() {
	m_Owner->ChangeMotion(StoneSoldierSpawn, false);
	m_Owner->ResetVelocity();
}

void StoneSoldierSpawnState::Execute(float delta_time) {
	if (m_Owner->IsMotionEnd()) {
		m_Owner->ChangeState(StoneSoldierOwnedState::Idle);
		return;
	}
}

void StoneSoldierSpawnState::Exit() {
	m_Owner->ResetVelocity();
}