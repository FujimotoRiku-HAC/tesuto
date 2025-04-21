#include "PlayerOwnedState.h"

void PlayerAbilityBlowAttackState::Enter() {
	m_Owner->ChangeMotion(MotionAbilityBlowAttack, false);
	m_Enemy = m_Owner->FindTargetEnemy();
	m_Owner->ResetVelocity();
	m_Owner->SetNextAttackStateFrag(false);
}

void PlayerAbilityBlowAttackState::Execute(float delta_time) {

	if (m_Owner->IsMotionEnd(BlowMotionEndTime)) {
		//攻撃が当たったら次のステート
		if (m_Owner->IsAbilityReact() || m_Owner->IsSelect()) {
			m_Owner->ChangeState(PlayerOwnedState::AbilityBlowAttackReinforcement);
			return;
		}
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

void PlayerAbilityBlowAttackState::Exit() {
	m_Owner->ResetVelocity();
	m_Owner->SetNextAttackStateFrag(false);
}

void PlayerAbilityBlowAttackState::OnMessage(const std::string& message, void* param) {}

bool PlayerAbilityBlowAttackState::Distance()const{
	return (m_Owner->TargetDistance(m_Enemy) < StopDistance);
}