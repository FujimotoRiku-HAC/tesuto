#include "StoneSoldier.h"
#include "Collision/AttackCollider.h"
#include "StoneSoldierState/StoneSoldierOwnedState.h"

StoneSoldier::StoneSoldier(IWorld* world, const GSvector3& position,const GSvector3& rotation) {
	//ワールド設定
	m_World = world;
	//タグ名設定
	m_Tag = st_Enemy;
	//名前設定
	m_Name = st_Soldier;
	//衝突判定球の設定
	m_Collider = BoundingSphere{ SoldierRadius,GSvector3{0.f,SoldierHeight,0.f} };
	//メッシュ設定
	m_Mesh = AnimatedMesh{ Mesh_StoneSoldier,StoneSoldierSpawn,false };
	//座標の初期化
	m_Transform.position(position);
	//回転初期化
	m_Transform.rotate(rotation);
	//ワールド変換行列の初期化
	m_Mesh.Transform(m_Transform.localToWorldMatrix());
	//体力設定
	m_Health = SoldierHealth;
	//スピード設定
	m_Speed = SoldierRunSpeed;
	//体力バー設定
	m_HPSlider = Slider(m_Transform.position(), GSvector2{ m_Health * SoldierHPValue.x,SoldierHPValue.y }, 0, m_Health, EnemyHPColor);
	//プレイヤー検索
	m_Player = m_World->FindActor(st_Player);
	//アニメーションイベント追加
	SetAnimationEvent();
	//ステート追加
	AddState();
	//ステート設定
	m_StateMachine.SetCurrentState(StoneSoldierOwnedState::Spawn);
}

void StoneSoldier::Update(float delta_time) {
	//共通更新
	EnemyUpdate(delta_time);
	//メッシュを更新
	m_Mesh.Update(delta_time * m_SlowMagnification);
	//行列を設定
	m_Mesh.Transform(m_Transform.localToWorldMatrix());
	m_StateMachine.Update(delta_time * m_SlowMagnification);
	//体力の設定・値
	GSvector3 pos = m_Transform.position() + SoldierOffset;
	m_HPSlider.SetPos(pos);
	m_HPSlider.SetValue(m_Health);
}

void StoneSoldier::Draw() const {
	//描画
	MeshDraw();
}

void StoneSoldier::DrawGui() const{
	//カメラ内かつ体力があれば
	if (m_Health > 0.f && IsInErea()) m_HPSlider.Draw();
}

//衝突処理
void StoneSoldier::React(Actor& other) {
	//攻撃が当たったら
	if (other.Tag() == st_PlayerAttackTag) {
		gsPlaySE(React_SE);

		//移動停止
		ResetVelocity();
		//エフェクト処理
		m_IsDamage = true;
		//メッセージ送る
		m_Player->HandleMessage("Hit", nullptr);

		//ダメージ量の取得
		int damage = static_cast<AttackCollider*>(&other)->DamageValue();
		//ダメージ描画の位置
		GSvector3 pos = m_Transform.position();
		pos.y = 1.0f;
		GSvector3 intersect = other.Collider().m_Center * m_Transform.worldToLocalMatrix();
		//ダメージ描画
		DrawDamage(m_Transform.localToWorldMatrix().transform(intersect), damage);

		//体力減少
		m_Health -= damage;
		//攻撃を受けたら
		m_IsAttacking = false;

	    if (m_Health <= 0) {
			//死亡モーション追加する
			gsPlaySE(Enemy_Finish_SE);
			m_EnableCollider = false;
			ChangeState(StoneSoldierOwnedState::BeforeDie);
			return;
		}
		//強攻撃を打たれたら
		if (other.Name() == st_PlayerStrongAttack) {
			m_IsStrongHit = true;
		}
		//吹き飛ばす攻撃が当たったら
		if (other.Name() == st_PlayerBlowAttack) {
			m_IsBlowHit = true;
		}
		//空中攻撃が当たったら
		if (other.Name() == st_PlayerPushUpAttack) {
			m_IsPush = true;
			ToUseOrNotUseGravity(false);
		}
		//攻撃によって
		DecentHitMotion(other);
	}
}

void StoneSoldier::HandleMessage(const std::string& message, void* param) {
	//ヒットストップ
	if (message == st_Message_Hit)m_IsHitStop = true;
}

void StoneSoldier::ChangeState(int state, bool restate) {
	m_StateMachine.Change_State(state, restate);
}

void StoneSoldier::DecentHitMotion(Actor& other){
	//コライダ―の計算
	GSvector3 intersect = (Collider().m_Center + other.Collider().m_Center) * (Collider().m_Radius / (Collider().m_Radius + other.Collider().m_Radius));
	GSvector3 vec = (m_Transform.position() - intersect).getNormalized();
	vec.y = 0.f;
	GSvector3 f = m_Transform.forward();
	f.y = 0.f;
	float angle = GSvector3::signedAngle(vec, f);

	//ノックバックではなかったら
	if (!m_IsStrongHit && !m_IsBlowHit && !m_IsPush) {

		if (std::abs(angle) <= SoldierHitPosition) {
			ChangeState(StoneSoldierOwnedState::HitBack, false);
			return;
		}
		else {
			ChangeState(StoneSoldierOwnedState::HitFront, false);
			return;
		}
	}
	//強攻撃に当たったら
	else if(m_IsStrongHit){

		m_IsStrongHit = false;

		if (std::abs(angle) <= SoldierHitPosition) {
			ChangeState(StoneSoldierOwnedState::StrongHitBack, false);
			return;
		}
		else {
			ChangeState(StoneSoldierOwnedState::StrongHitFront, false);
			return;
		}
	}
	//ブロウ攻撃に当たったら
	else if (m_IsBlowHit) {

		m_IsStrongHit = false;

		if (std::abs(angle) <= SoldierHitPosition) {
			ChangeState(StoneSoldierOwnedState::BlowHitBack, false);
			return;
		}
		else {
			ChangeState(StoneSoldierOwnedState::BlowHitFront, false);
			return;
		}
	}
	//プッシュ攻撃に当たったら
	else if (m_IsPush) {

		m_IsPush = false;

		if (std::abs(angle) <= SoldierHitPosition) {
			ChangeState(StoneSoldierOwnedState::PushUpHitBack, false);
			return;
		}
		else {
			ChangeState(StoneSoldierOwnedState::PushUpHitFront, false);
			return;
		}
	}

}

void StoneSoldier::ThinkMove() {
	//0度以上だったら右歩き
	if (TargetSignedAngle() >= 0.f) {
		ChangeState(StoneSoldierOwnedState::RightWalk);
		return;
	}
	else {
		ChangeState(StoneSoldierOwnedState::LeftWalk);
		return;
	}
}

void StoneSoldier::RandomAttack(){
	//ランダムの攻撃
	int random = gsRand(0, SoldierAttackType);
	if (random == 0) {
		ChangeState(StoneSoldierOwnedState::BeforePeripheralAttack);
		return;
	}
	else {
		ChangeState(StoneSoldierOwnedState::BeforePounceAttack);
		return;
	}
}

//ステート追加
void StoneSoldier::AddState() {
	m_StateMachine.AddState(StoneSoldierOwnedState::Spawn, new StoneSoldierSpawnState{ this,m_World });
	m_StateMachine.AddState(StoneSoldierOwnedState::Idle, new StoneSoldierIdleState{ this,m_World });
	m_StateMachine.AddState(StoneSoldierOwnedState::ForwardWalk, new StoneSoldierWalkForwardState{ this,m_World });
	m_StateMachine.AddState(StoneSoldierOwnedState::BackwardWalk, new StoneSoldierWalkBackwardState{ this,m_World });
	m_StateMachine.AddState(StoneSoldierOwnedState::BeforePounceAttack, new StoneSoldierBeforePounceAttackState{ this,m_World });
	m_StateMachine.AddState(StoneSoldierOwnedState::BeforePeripheralAttack, new StoneSoldierBeforePeripheralAttackState{ this,m_World });
	m_StateMachine.AddState(StoneSoldierOwnedState::PeripheralAttack, new StoneSoldierPeripheralAttackState{ this,m_World });
	m_StateMachine.AddState(StoneSoldierOwnedState::PounceAttack, new StoneSoldierPounceAttackState{ this,m_World });
	m_StateMachine.AddState(StoneSoldierOwnedState::RightWalk, new StoneSoldierThinkRightState{ this,m_World });
	m_StateMachine.AddState(StoneSoldierOwnedState::LeftWalk, new StoneSoldierThinkLeftState{ this,m_World });
	m_StateMachine.AddState(StoneSoldierOwnedState::HitFront, new StoneSoldierHitFrontState{ this,m_World });
	m_StateMachine.AddState(StoneSoldierOwnedState::StrongHitFront, new StoneSoldierHitFrontStrongState{ this,m_World });
	m_StateMachine.AddState(StoneSoldierOwnedState::BlowHitFront, new StoneSoldierHitFrontBlowState{ this,m_World });
	m_StateMachine.AddState(StoneSoldierOwnedState::PushUpHitFront, new StoneSoldierHitFrontPushUpState{ this,m_World });
	m_StateMachine.AddState(StoneSoldierOwnedState::HitBack, new StoneSoldierHitBackState{ this,m_World });
	m_StateMachine.AddState(StoneSoldierOwnedState::StrongHitBack, new StoneSoldierHitBackStrongState{ this,m_World });
	m_StateMachine.AddState(StoneSoldierOwnedState::BlowHitBack, new StoneSoldierHitBackBlowState{ this,m_World });
	m_StateMachine.AddState(StoneSoldierOwnedState::PushUpHitBack, new StoneSoldierHitBackPushUpState{ this,m_World });
	m_StateMachine.AddState(StoneSoldierOwnedState::CollapseAfterPushUp, new StoneSoldierCollapseAfterPushUpState{ this,m_World });
	m_StateMachine.AddState(StoneSoldierOwnedState::GetUp, new StoneSoldierGetUpState{ this,m_World });
	m_StateMachine.AddState(StoneSoldierOwnedState::BeforeDie, new StoneSoldierBeforeDieState{ this,m_World });
	m_StateMachine.AddState(StoneSoldierOwnedState::Die, new StoneSoldierDieState{ this,m_World });
}

void StoneSoldier::SetAnimationEvent(){
	//近距離攻撃
	m_Mesh.AddEvent(StoneSoldierMotion::StoneSoldierPeripheralAttack, 25.0f, [=] { gsPlaySE(Soldier_Attack_SE); });
	m_Mesh.AddEvent(StoneSoldierMotion::StoneSoldierPeripheralAttack, 20.0f, [=] { GenerateAttackCollider(st_EnemyAttack, SoldierNormalDamage); });
	m_Mesh.AddEvent(StoneSoldierMotion::StoneSoldierPeripheralAttack, 60.0f, [=] { gsPlaySE(Soldier_Attack_SE); });
	m_Mesh.AddEvent(StoneSoldierMotion::StoneSoldierPeripheralAttack, 65.0f, [=] { GenerateAttackCollider(st_EnemyAttack, SoldierNormalDamage); });
	//とびかかり攻撃モーション
	m_Mesh.AddEvent(StoneSoldierMotion::StoneSoldierPounceAttack, 30.0f, [=] {GenerateAttackCollider(st_EnemyStrongAttack, SoldierStrongDamage,3.2f); });
	m_Mesh.AddEvent(StoneSoldierMotion::StoneSoldierPounceAttack, 30.0f, [=] {gsPlaySE(Soldier_Attack2_SE); });

}
