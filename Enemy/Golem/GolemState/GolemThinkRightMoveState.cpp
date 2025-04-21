#include "GolemOwnedState.h"

void GolemThinkRightMoveState::Enter() {
	m_Owner->ChangeMotion(MotionGolemRightWalk, true);
}

void GolemThinkRightMoveState::Execute(float delta_time) {
	if (m_Owner->IsMotionEnd()) {
		m_Owner->ChangeState(GolemOwnedState::Idle);
		return;
	}
	//インターバル減算
	m_Owner->DecrementInterval(delta_time);

	//回転・移動
	float angle = CLAMP(m_Owner->TargetSignedAngle(), -TurnAngle, TurnAngle) * delta_time;
	m_Owner->Transform().rotate(0.f, angle * delta_time, 0.f);
	m_Owner->Transform().translate(m_Owner->Speed() * -ThinkSpeed * delta_time, 0.f, 0.f);
}

void GolemThinkRightMoveState::Exit() {}
