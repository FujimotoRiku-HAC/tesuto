#include "Golem.h"
#include "Collision/AttackCollider.h"
#include "GolemState/GolemOwnedState.h"

Golem::Golem(IWorld* world, const GSvector3& position, const GSvector3& rotation) {
	//ワールド設定
	m_World = world;
	//タグ名設定
	m_Tag = st_Enemy;
	//名前設定
	m_Name = st_Golem;
	//衝突判定球の設定
	m_Collider = BoundingSphere{ GolemRadius,GSvector3{0.f,GolemHeight,0.f} };
	//メッシュ設定
	m_Mesh = AnimatedMesh{ Mesh_Golem,NULL,true };
	//座標の初期化
	m_Transform.position(position);
	//回転初期化
	m_Transform.rotate(rotation);
	//ワールド変換行列の初期化
	m_Mesh.Transform(m_Transform.localToWorldMatrix());
	//体力設定
	m_MaxHealth = GolemHealth;
	m_Health = m_MaxHealth;
	//スタンゲージ設定
	m_MaxStanPoint = GolemStanPoint;
	m_StanPoint = m_MaxStanPoint;
	//スピード設定
	m_Speed = GolemRunSpeed;
	//出現フラグを立てる
	m_IsGenerate = true;
	//体力バー設定
	m_HPSlider = Slider(HPSliderPos, HPSliderScale, 0, m_Health, HPSliderColor);
	//スタンポイントバー設定
	m_SPSlider = Slider(SPSliderPos, SPSliderScale, 0, m_StanPoint, SPSliderColor);
	//プレイヤー検索
	m_Player = m_World->FindActor(st_Player);
	//イベント追加
	SetAnimationEvent();
	//ステート追加
	AddState();
	//初期モーション設定
	m_StateMachine.SetCurrentState(GolemOwnedState::Generate);
}

void Golem::Update(float delta_time) {
	//共通アップデート
	EnemyUpdate(delta_time);
	//ステートマシン更新
	m_StateMachine.Update(delta_time * m_SlowMagnification);
	//メッシュを更新
	m_Mesh.Update(delta_time * GolemAnimSpeed * m_SlowMagnification);
	//ルートモーション適用
	m_Mesh.ApplyRootMotion(m_Transform);
	//行列を設定
	m_Mesh.Transform(m_Transform.localToWorldMatrix());
	//出現タイマーの更新
	GenerateTimer(delta_time);
	//HP設定
	m_HPSlider.SetValue(m_Health);
	//スタンポイント設定
	m_SPSlider.SetValue(m_StanPoint);

	//スタンできるなら
	if (m_StanPoint <= 0.f && !m_IsStan) {
		gsPlaySE(Enemy_Finish_SE);
		ChangeState(GolemOwnedState::StanStart, false);
		return;
	}
}

void Golem::Draw() const {
	//描画
	MeshDraw();
}

void Golem::DrawGui() const {
	m_HPSlider.Draw();	//HP描画
	m_SPSlider.Draw();	//SP描画
}

//衝突処理
void Golem::React(Actor& other) {
	//プレイヤー攻撃タグに当たったら
	if (other.Tag() == st_PlayerAttackTag) {
		GSvector3 intersect = other.Collider().m_Center * m_Transform.worldToLocalMatrix();
		gsPlaySE(React_SE);
		//移動停止
		ResetVelocity();
		//エフェクト処理
		m_IsDamage = true;
		//プレイヤーにメッセージ送信
		m_Player->HandleMessage(st_Message_Hit, nullptr);
		//ダメージ量の取得
		int damage = static_cast<AttackCollider*>(&other)->DamageValue();
		int stan_damage = static_cast<AttackCollider*>(&other)->StanPointValue();
		//ダメージ描画
		DrawDamage(m_Transform.localToWorldMatrix().transform(intersect), damage);

		//体力減少
		m_Health -= damage;
		if (m_Health <= 0) {
			m_EnableCollider = false;
			gsPlaySE(Enemy_Finish_SE);
			ChangeState(GolemOwnedState::Die, false);
			return;
		}
		//Sp減算
		m_StanPoint -= stan_damage;

		//3回攻撃を受けてたらジャンプ
		if (!IsInterval() && !m_IsStan) {
			if (m_HitCount >= HitNum) {
				ChangeState(GolemOwnedState::Jump, false);
				m_HitCount = 0;
				return;
			}
			m_HitCount++;
		}
	}
}

void Golem::HandleMessage(const std::string& message, void* param) {
	if (message == st_Message_Hit) m_IsHitStop = true;
}


void Golem::ResetStanValue(){
	m_StanPoint = m_MaxStanPoint;
}

void Golem::ChangeState(int state, bool restate) {
	m_StateMachine.Change_State(state, restate);
}

void Golem::ThinkMove() {
	//0度以上だったら右に動く
	if (TargetSignedAngle() >= 0.f) {
		ChangeState(GolemOwnedState::RightThink);
		return;
	}
	else {
		ChangeState(GolemOwnedState::LeftThink);
		return;
	}
}

void Golem::RandamAttack() {
	int random = gsRandf(0, 6);
	//ランダム攻撃
	switch (random)
	{
	case 0:
		ChangeState(GolemOwnedState::TripleAttack, false);
		break;
	case 1:
		ChangeState(GolemOwnedState::ForAttack, false);
		break;
	case 2:
		ChangeState(GolemOwnedState::JumpingAttack, false);
		break;
	case 3:
		ChangeState(GolemOwnedState::WildDanceRotatingAttack, false);
		break;
	case 4:
		ChangeState(GolemOwnedState::ChargeDanceAttack, false);
		break;
	case 5:
		ChangeState(GolemOwnedState::PushAttack, false);
		break;
	default:
		break;
	}
}

void Golem::AddState() {
	m_StateMachine.AddState(GolemOwnedState::Idle, new GolemIdleState{ this,m_World });
	m_StateMachine.AddState(GolemOwnedState::ForwardWalk, new GolemWalkForwardState{ this,m_World });
	m_StateMachine.AddState(GolemOwnedState::BackWalk, new GolemWalkBackWardState{ this,m_World });
	m_StateMachine.AddState(GolemOwnedState::RightThink, new GolemThinkRightMoveState{ this,m_World });
	m_StateMachine.AddState(GolemOwnedState::LeftThink, new GolemThinkLeftMoveState{ this,m_World });
	m_StateMachine.AddState(GolemOwnedState::TripleAttack, new GolemAttackTriple{ this,m_World });
	m_StateMachine.AddState(GolemOwnedState::ForAttack, new GolemAttackForState{ this,m_World });
	m_StateMachine.AddState(GolemOwnedState::ChargeSpinAttack, new GolemAttackChargeSpin{ this,m_World });
	m_StateMachine.AddState(GolemOwnedState::JumpingAttack, new GolemAttackJumpingState{ this,m_World });
	m_StateMachine.AddState(GolemOwnedState::WildDanceRotatingAttack, new GolemAttackWildDanceRotationState{ this,m_World });
	m_StateMachine.AddState(GolemOwnedState::ChargeDanceAttack, new GolemAttackChargedDanceState{ this,m_World });
	m_StateMachine.AddState(GolemOwnedState::PushAttack, new GolemAttackPushState{ this,m_World });
	m_StateMachine.AddState(GolemOwnedState::Generate, new GolemGenerateState{ this,m_World });
	m_StateMachine.AddState(GolemOwnedState::Jump, new GolemJumpState{ this,m_World });
	m_StateMachine.AddState(GolemOwnedState::StanStart, new GolemStanStartState{ this,m_World });
	m_StateMachine.AddState(GolemOwnedState::StanLoop, new GolemStanLoopState{ this,m_World });
	m_StateMachine.AddState(GolemOwnedState::StanEnd, new GolemStanEndState{ this,m_World });
	m_StateMachine.AddState(GolemOwnedState::Die, new GolemDieState{ this,m_World });

}



void Golem::SetAnimationEvent() {
	m_Mesh.AddEvent(GolemMotion::MotionGolemGenerate, 1.0f, [=] {	gsPlaySE(Golem_Skill_SE); });

	//ジャンプ
	m_Mesh.AddEvent(GolemMotion::MotionGolemJump, 10.0f, [=] {ToUseOrNotUseGravity(false); });
	m_Mesh.AddEvent(GolemMotion::MotionGolemJump, 10.0f, [=] { m_Velocity.y += 0.19f; });
	m_Mesh.AddEvent(GolemMotion::MotionGolemJump, 15.0f, [=] { m_Velocity = m_Transform.forward().normalized() * 0.35f; });
	m_Mesh.AddEvent(GolemMotion::MotionGolemJump, 30.0f, [=] {ToUseOrNotUseGravity(true); });
	m_Mesh.AddEvent(GolemMotion::MotionGolemJump, 35.0f, [=] { ResetVelocity(); });
	//回転して飛んでくる攻撃
	m_Mesh.AddEvent(GolemMotion::MotionGolemChargeSpinAttack, 36.0f, [=] { ChangeVelocity(Transform().forward().normalized() * m_Speed * 1.5f); });
	m_Mesh.AddEvent(GolemMotion::MotionGolemChargeSpinAttack, 36.0f, [=] { GenerateAttackCollider(st_EnemyAttack, GolemNormalDamage,3.f,3.f); });
	m_Mesh.AddEvent(GolemMotion::MotionGolemChargeSpinAttack, 36.0f, [=] { gsPlaySE(Golem_Attack_SE); });
	m_Mesh.AddEvent(GolemMotion::MotionGolemChargeSpinAttack, 46.0f, [=] { GenerateAttackCollider(st_EnemyAttack, GolemNormalDamage,3.f,3.f); });
	m_Mesh.AddEvent(GolemMotion::MotionGolemChargeSpinAttack, 46.0f, [=] { gsPlaySE(Golem_Attack_SE); });
	m_Mesh.AddEvent(GolemMotion::MotionGolemChargeSpinAttack, 56.0f, [=] { GenerateAttackCollider(st_EnemyAttack, GolemNormalDamage,3.f,3.f); });
	m_Mesh.AddEvent(GolemMotion::MotionGolemChargeSpinAttack, 56.0f, [=] { gsPlaySE(Golem_Attack_SE); });
	m_Mesh.AddEvent(GolemMotion::MotionGolemChargeSpinAttack, 66.0f, [=] { GenerateAttackCollider(st_EnemyAttack, GolemNormalDamage,3.f,3.f); });
	m_Mesh.AddEvent(GolemMotion::MotionGolemChargeSpinAttack, 66.0f, [=] { gsPlaySE(Golem_Attack_SE); });
	m_Mesh.AddEvent(GolemMotion::MotionGolemChargeSpinAttack, 76.0f, [=] { GenerateAttackCollider(st_EnemyAttack, GolemNormalDamage,3.f,3.f); });
	m_Mesh.AddEvent(GolemMotion::MotionGolemChargeSpinAttack, 76.0f, [=] { gsPlaySE(Golem_Attack_SE); });
	m_Mesh.AddEvent(GolemMotion::MotionGolemChargeSpinAttack, 86.0f, [=] { GenerateAttackCollider(st_EnemyAttack, GolemNormalDamage,3.f,3.f); });
	m_Mesh.AddEvent(GolemMotion::MotionGolemChargeSpinAttack, 86.0f, [=] { gsPlaySE(Golem_Attack_SE); });
	m_Mesh.AddEvent(GolemMotion::MotionGolemChargeSpinAttack, 90.0f, [=] { ResetVelocity(); });
	//ジャンプ攻撃
	m_Mesh.AddEvent(GolemMotion::MotionGolemJumpingAttack, 20.0f, [=] { GenerateAttackCollider(st_EnemyAttack, GolemNormalDamage); });
	m_Mesh.AddEvent(GolemMotion::MotionGolemJumpingAttack, 20.0f, [=] { gsPlaySE(Golem_Attack_SE); });
	m_Mesh.AddEvent(GolemMotion::MotionGolemJumpingAttack, 35.0f, [=] { GenerateAttackCollider(st_EnemyAttack, GolemNormalDamage); });
	m_Mesh.AddEvent(GolemMotion::MotionGolemJumpingAttack, 35.0f, [=] { gsPlaySE(Golem_Attack_SE); });
	m_Mesh.AddEvent(GolemMotion::MotionGolemJumpingAttack, 100.0f, [=] { GenerateAttackCollider(st_EnemyAttack, GolemNormalDamage,2.5f); });
	m_Mesh.AddEvent(GolemMotion::MotionGolemJumpingAttack, 100.0f, [=] { gsPlaySE(Golem_Attack_SE); });
	m_Mesh.AddEvent(GolemMotion::MotionGolemJumpingAttack, 120.0f, [=] { GenerateAttackCollider(st_EnemyAttack, GolemNormalDamage,2.5f); });
	m_Mesh.AddEvent(GolemMotion::MotionGolemJumpingAttack, 120.0f, [=] { gsPlaySE(Golem_Attack_SE); });
	m_Mesh.AddEvent(GolemMotion::MotionGolemJumpingAttack, 150.0f, [=] { m_EnableCollider = false; });
	m_Mesh.AddEvent(GolemMotion::MotionGolemJumpingAttack, 165.0f, [=] { m_EnableCollider = true; });
	m_Mesh.AddEvent(GolemMotion::MotionGolemJumpingAttack, 165.0f, [=] { GenerateAttackCollider(st_EnemyStrongAttack, GolemStrongDamage,2.5f); });
	m_Mesh.AddEvent(GolemMotion::MotionGolemJumpingAttack, 165.0f, [=] { gsPlaySE(Golem_Attack3_SE); });
	//回転斬り
	m_Mesh.AddEvent(GolemMotion::MotionGolemWildDanceRotatingAttack, 18.0f, [=] { GenerateAttackCollider(st_EnemyAttack, GolemNormalDamage); });
	m_Mesh.AddEvent(GolemMotion::MotionGolemWildDanceRotatingAttack, 18.0f, [=] { gsPlaySE(Golem_Attack3_SE); });
	m_Mesh.AddEvent(GolemMotion::MotionGolemWildDanceRotatingAttack, 33.0f, [=] { GenerateAttackCollider(st_EnemyAttack, GolemNormalDamage); });
	m_Mesh.AddEvent(GolemMotion::MotionGolemWildDanceRotatingAttack, 33.0f, [=] { gsPlaySE(Golem_Attack3_SE); });
	m_Mesh.AddEvent(GolemMotion::MotionGolemWildDanceRotatingAttack, 50.0f, [=] { GenerateAttackCollider(st_EnemyAttack, GolemNormalDamage); });
	m_Mesh.AddEvent(GolemMotion::MotionGolemWildDanceRotatingAttack, 50.0f, [=] { gsPlaySE(Golem_Attack3_SE); });
	m_Mesh.AddEvent(GolemMotion::MotionGolemWildDanceRotatingAttack, 63.0f, [=] { GenerateAttackCollider(st_EnemyAttack, GolemNormalDamage); });
	m_Mesh.AddEvent(GolemMotion::MotionGolemWildDanceRotatingAttack, 63.0f, [=] { gsPlaySE(Golem_Attack3_SE); });
	m_Mesh.AddEvent(GolemMotion::MotionGolemWildDanceRotatingAttack, 88.0f, [=] { GenerateAttackCollider(st_EnemyAttack, GolemNormalDamage, 3.f); });
	m_Mesh.AddEvent(GolemMotion::MotionGolemWildDanceRotatingAttack, 88.0f, [=] { gsPlaySE(Golem_Attack3_SE); });
	m_Mesh.AddEvent(GolemMotion::MotionGolemWildDanceRotatingAttack, 102.0f, [=] { GenerateAttackCollider(st_EnemyAttack, GolemNormalDamage, 3.f); });
	m_Mesh.AddEvent(GolemMotion::MotionGolemWildDanceRotatingAttack, 102.0f, [=] { gsPlaySE(Golem_Attack3_SE); });
	m_Mesh.AddEvent(GolemMotion::MotionGolemWildDanceRotatingAttack, 113.0f, [=] { GenerateAttackCollider(st_EnemyAttack, GolemNormalDamage, 3.f); });
	m_Mesh.AddEvent(GolemMotion::MotionGolemWildDanceRotatingAttack, 113.0f, [=] { gsPlaySE(Golem_Attack3_SE); });
	m_Mesh.AddEvent(GolemMotion::MotionGolemWildDanceRotatingAttack, 123.0f, [=] { GenerateAttackCollider(st_EnemyAttack, GolemNormalDamage, 3.f); });
	m_Mesh.AddEvent(GolemMotion::MotionGolemWildDanceRotatingAttack, 123.0f, [=] { gsPlaySE(Golem_Attack3_SE); });
	m_Mesh.AddEvent(GolemMotion::MotionGolemWildDanceRotatingAttack, 133.0f, [=] { GenerateAttackCollider(st_EnemyAttack, GolemNormalDamage, 3.f); });
	m_Mesh.AddEvent(GolemMotion::MotionGolemWildDanceRotatingAttack, 133.0f, [=] { gsPlaySE(Golem_Attack3_SE); });
	m_Mesh.AddEvent(GolemMotion::MotionGolemWildDanceRotatingAttack, 143.0f, [=] { GenerateAttackCollider(st_EnemyAttack, GolemNormalDamage, 3.f); });
	m_Mesh.AddEvent(GolemMotion::MotionGolemWildDanceRotatingAttack, 143.0f, [=] { gsPlaySE(Golem_Attack3_SE); });
	//溜め乱舞
	m_Mesh.AddEvent(GolemMotion::MotionGolemChargeDanceAttack, 45.0f, [=] { GenerateAttackCollider(st_EnemyAttack, GolemNormalDamage,1.5f,5.f); });
	m_Mesh.AddEvent(GolemMotion::MotionGolemChargeDanceAttack, 45.0f, [=] { gsPlaySE(Golem_Attack3_SE); });
	m_Mesh.AddEvent(GolemMotion::MotionGolemChargeDanceAttack, 55.0f, [=] { GenerateAttackCollider(st_EnemyAttack, GolemNormalDamage,1.5f,5.0f); });
	m_Mesh.AddEvent(GolemMotion::MotionGolemChargeDanceAttack, 55.0f, [=] { gsPlaySE(Golem_Attack3_SE); });
	m_Mesh.AddEvent(GolemMotion::MotionGolemChargeDanceAttack, 69.0f, [=] { GenerateAttackCollider(st_EnemyAttack, GolemNormalDamage,1.5f,5.0f); });
	m_Mesh.AddEvent(GolemMotion::MotionGolemChargeDanceAttack, 69.0f, [=] { gsPlaySE(Golem_Attack3_SE); });
	m_Mesh.AddEvent(GolemMotion::MotionGolemChargeDanceAttack, 79.0f, [=] { GenerateAttackCollider(st_EnemyAttack, GolemNormalDamage,1.5f,5.0f); });
	m_Mesh.AddEvent(GolemMotion::MotionGolemChargeDanceAttack, 79.0f, [=] { gsPlaySE(Golem_Attack3_SE); });
	m_Mesh.AddEvent(GolemMotion::MotionGolemChargeDanceAttack, 94.0f, [=] { GenerateAttackCollider(st_EnemyStrongAttack, GolemStrongDamage,1.5f,5.0f); });
	m_Mesh.AddEvent(GolemMotion::MotionGolemChargeDanceAttack, 94.0f, [=] { gsPlaySE(Golem_Attack3_SE); });
	//プッシュ攻撃
	m_Mesh.AddEvent(GolemMotion::MotionGolemPushAttack, 20.0f, [=] { GenerateAttackCollider(st_EnemyAttack, GolemNormalDamage,2.5f); });
	m_Mesh.AddEvent(GolemMotion::MotionGolemPushAttack, 20.0f, [=] { gsPlaySE(Golem_Attack2_SE); });
	m_Mesh.AddEvent(GolemMotion::MotionGolemPushAttack, 45.0f, [=] { GenerateAttackCollider(st_EnemyAttack, GolemNormalDamage,2.5f); });
	m_Mesh.AddEvent(GolemMotion::MotionGolemPushAttack, 45.0f, [=] { gsPlaySE(Golem_Attack2_SE); });
	m_Mesh.AddEvent(GolemMotion::MotionGolemPushAttack, 70.0f, [=] { GenerateAttackCollider(st_EnemyAttack, GolemNormalDamage,2.5f); });
	m_Mesh.AddEvent(GolemMotion::MotionGolemPushAttack, 70.0f, [=] { gsPlaySE(Golem_Attack2_SE); });
	m_Mesh.AddEvent(GolemMotion::MotionGolemPushAttack, 90.0f, [=] { GenerateAttackCollider(st_EnemyStrongAttack, GolemStrongDamage,2.5f); });
	m_Mesh.AddEvent(GolemMotion::MotionGolemPushAttack, 90.0f, [=] { gsPlaySE(Golem_Attack2_SE); });
	//３連攻撃
	m_Mesh.AddEvent(GolemMotion::MotionGolemTripleAttack, 20.0f, [=] { GenerateAttackCollider(st_EnemyAttack, GolemNormalDamage,2.5f); });
	m_Mesh.AddEvent(GolemMotion::MotionGolemTripleAttack, 20.0f, [=] { gsPlaySE(Golem_Attack3_SE); });
	m_Mesh.AddEvent(GolemMotion::MotionGolemTripleAttack, 35.0f, [=] { GenerateAttackCollider(st_EnemyAttack, GolemNormalDamage); });
	m_Mesh.AddEvent(GolemMotion::MotionGolemTripleAttack, 35.0f, [=] { gsPlaySE(Golem_Attack3_SE); });
	m_Mesh.AddEvent(GolemMotion::MotionGolemTripleAttack, 70.0f, [=] { GenerateAttackCollider(st_EnemyStrongAttack, GolemStrongDamage,2.5f); });
	m_Mesh.AddEvent(GolemMotion::MotionGolemTripleAttack, 70.0f, [=] { gsPlaySE(Golem_Attack4_SE); });
	//ため攻撃
	m_Mesh.AddEvent(GolemMotion::MotionGolemForAttack, 55.0f, [=] { GenerateAttackCollider(st_EnemyStrongAttack, GolemStrongDamage,2.5f); });
	m_Mesh.AddEvent(GolemMotion::MotionGolemForAttack, 55.0f, [=] { gsPlaySE(Golem_Attack4_SE); });
	//
	m_Mesh.AddEvent(GolemMotion::MotionGolemDie, 105.0f, [=] { m_World->SetClearFrag(); });
}
