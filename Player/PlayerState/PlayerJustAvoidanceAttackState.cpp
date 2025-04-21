#include "PlayerOwnedState.h"

void PlayerJustAvoidanceAttackState::Enter() {

	m_Owner->ChangeMotion(MotionJustAvoidAttack, false);
	m_Enemy = m_Owner->FindTargetEnemy();
	m_Owner->JustAttackFrag(true);
	m_Owner->AvoidanceFrag(false);
	m_Speed = 1.f;
}

void PlayerJustAvoidanceAttackState::Execute(float delta_time) {
	if (m_Owner->IsMotionEnd()) {
		m_Owner->ChangeState(PlayerOwnedState::Idle);
		return;
	}
	if (m_Enemy != nullptr) {
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
		//敵の方まで移動
		GSvector3 target_vec = (to_target).normalized();
		m_Owner->Transform().translate(0.f,target_vec.y * m_Speed * delta_time, m_Speed *delta_time);
		if (Distance()) {
			m_Speed = 0.f;
			return;
		}
		return;
	}
}

void PlayerJustAvoidanceAttackState::Exit() {
	m_Owner->JustAttackFrag(false);
}

void PlayerJustAvoidanceAttackState::OnMessage(const std::string& message, void* param) {}

bool PlayerJustAvoidanceAttackState::Distance() const{
	return (m_Owner->TargetDistance(m_Enemy) < StopDistance);
}