#include "StoneSoldierOwnedState.h"
#include "World/World.h"

void StoneSoldierDieState::Enter() {
	m_Owner->ChangeMotion(StoneSoldierDie, true);
	m_Owner->HaveTicket(false);
	m_Owner->DeadCheck(true);
}

void StoneSoldierDieState::Execute(float delta_time) {
	//Á‚¦‚Ä‚¢‚­
	m_Owner->ExtinctionDie(delta_time);
}

void StoneSoldierDieState::Exit() {}


