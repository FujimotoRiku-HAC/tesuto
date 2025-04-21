#include "PlayerOwnedState.h"

void PlayerIdleState::Enter() {
	m_Timer = 1.f;
	m_Owner->ChangeMotion(MotionPlayerIdle, true);

}

void PlayerIdleState::Execute(float delta_time) {
	//何もしていなければアイドル
	int motion = PlayerAnim::MotionPlayerIdle;
	if (m_Owner->IsSelect())return;
	//移動量の更新
	GSvector3 velocity = m_Owner->InputVelocity(InputManager::Instance().LeftAxis());
	//スピード取得
	float speed = m_Owner->PlayerSpeed();

	if (velocity != GSvector3::zero()) {
		motion = PlayerAnim::MotionFwdWalk;
		speed = m_Owner->PlayerSpeed();
	}
	//モーションの更新
	m_Owner->ChangeMotion(motion, true);

	velocity = velocity.normalized() * speed * delta_time;
	m_Owner->ChangeVelocity(velocity);
	m_Owner->Interpolation(velocity, delta_time);

	//攻撃ステートに遷移
	if (InputManager::Instance().IsBottonDown(InputManager::InputType::Attack)) {
		m_Owner->ChangeState(PlayerOwnedState::FirstAttack);
		return;
	}

	//回避
	if (InputManager::Instance().IsBottonDown(InputManager::InputType::Avoid) && m_Timer >= 1.0f) {
		m_Owner->ChangeState(PlayerOwnedState::AvoidanceForward);
		return;
	}
	m_Timer += delta_time;
}

void PlayerIdleState::Exit() {}

void PlayerIdleState::OnMessage(const std::string& message, void* param) {}