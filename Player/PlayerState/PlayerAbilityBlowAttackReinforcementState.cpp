#include "PlayerOwnedState.h"

void PlayerAbilityBlowAttackReinforcementState::Enter() {
	m_Owner->ChangeMotion(MotionAbilityBlowAttackReinforcement, false);
	m_Enemy = m_Owner->FindTargetEnemy();
	m_Owner->ResetVelocity();
	m_Owner->SetNextAttackStateFrag(false);
}

void PlayerAbilityBlowAttackReinforcementState::Execute(float delta_time) {
	if (m_Owner->IsMotionEnd()) {
		m_Owner->ChangeState(PlayerOwnedState::Idle);
		return;
	}

	//フラグが立っていないかつ敵がいたら
	if (!m_Owner->CanAttackNextState() && m_Enemy != nullptr) {
		//ターゲット方向のベクトルを調べる
		GSvector3 to_target = m_Enemy->Transform().position() - m_Owner->Transform().position();
		//前方向を調べる
		GSvector3 forward = m_Owner->Transform().forward();
		//y成分は無効
		forward.y = 0.f;
		to_target.y = 0.f;
		//前方向のベクトルとターゲット方向のベクトルの角度差を求める
		float angle = GSvector3::signedAngle(forward, to_target);
		//向き変更
		m_Owner->Transform().rotate(0.f, angle, 0.f);
		//近すぎたら止まる
		if (Distance()) m_Owner->ResetVelocity();
		return;
	}
}

void PlayerAbilityBlowAttackReinforcementState::Exit() {
	m_Owner->SetNextAttackStateFrag(false);
	m_Owner->ResetVelocity();
}

void PlayerAbilityBlowAttackReinforcementState::OnMessage(const std::string& message, void* param) {}

bool PlayerAbilityBlowAttackReinforcementState::Distance()const {
	return (m_Owner->TargetDistance(m_Enemy) < StopDistance);
}