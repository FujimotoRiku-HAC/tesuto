#include "PlayerOwnedState.h"

void PlayerAbilityWaveWildDanceAttackState_2::Enter() {
	m_Owner->ChangeMotion(MotionAbilityWaveWildDance_2, false);
	m_Owner->ResetVelocity();
	m_Owner->SetNextAttackStateFrag(false);
	m_Enemy = m_Owner->FindTargetEnemy();
}

void PlayerAbilityWaveWildDanceAttackState_2::Execute(float delta_time) {

	if (m_Owner->IsMotionEnd(WaveMotionEndTime)) {
		m_Owner->ChangeState(PlayerOwnedState::AbilityWaveWildDance_3);
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
		return;
	}
}

void PlayerAbilityWaveWildDanceAttackState_2::Exit() {
	m_Owner->ResetVelocity();
	m_Owner->SetNextAttackStateFrag(false);

}

void PlayerAbilityWaveWildDanceAttackState_2::OnMessage(const std::string& message, void* param) {}