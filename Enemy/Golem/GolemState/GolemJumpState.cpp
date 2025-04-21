#include "GolemOwnedState.h"

void GolemJumpState::Enter() {
	m_Owner->ChangeMotion(MotionGolemJump, false);
}

void GolemJumpState::Execute(float delta_time) {
	//UŒ‚ƒCƒ“ƒ^[ƒoƒ‹Œ¸ŽZ
	m_Owner->DecrementInterval(delta_time);

	if (m_Owner->IsMotionEnd()) {
		m_Owner->ChangeState(GolemOwnedState::Idle);
		return;
	}
}

void GolemJumpState::Exit() {}
