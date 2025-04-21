#include "GolemOwnedState.h"

void GolemDieState::Enter() {
	m_Owner->ChangeMotion(MotionGolemDie, true);
}

void GolemDieState::Execute(float delta_time) {}

void GolemDieState::Exit() {}
