#include "PlayerOwnedState.h"
#include "World/World.h"


void PlayerDieLoopState::Enter() {
	m_Owner->ChangeMotion(MotionDieLoop, true);
}

void PlayerDieLoopState::Execute(float delta_time) {}

void PlayerDieLoopState::Exit() {}

void PlayerDieLoopState::OnMessage(const std::string& message, void* param) {}