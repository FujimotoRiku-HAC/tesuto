#include "StoneSoldierOwnedState.h"
#include "World/World.h"

void StoneSoldierBeforePounceAttackState::Enter() {
	m_Owner->AttackStart(true);
	m_Owner->ResetVelocity();
	m_Owner->ChangeMotion(StoneSoldierBeforePounceAttack, false);
}

void StoneSoldierBeforePounceAttackState::Execute(float delta_time) {
	//ˆê’èŽžŠÔ“®‚­
	if (m_Owner->GetAnimationTimer(BeforePounceTimerEnd, BeforePounceTimerStart)) {
		m_Owner->ChangeVelocity(m_Owner->Transform().forward().normalized() * -m_Owner->Speed() * BackDistance * delta_time);
		return;
	}
	if (m_Owner->IsMotionEnd()) {
		m_Owner->ChangeState(StoneSoldierOwnedState::PounceAttack);
		return;
	}
	//ˆÚ“®—Ê‰Šú‰»
	m_Owner->ResetVelocity();
}

void StoneSoldierBeforePounceAttackState::Exit() {
	m_Owner->ResetVelocity();
}
