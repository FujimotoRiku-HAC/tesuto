#include "Player.h"
#include "Field/Field.h"
#include "Math/Line.h"
#include "Collision/PlayerAttackCollider/BasicPlayerAttackCollider.h"
#include "Collision/PlayerAttackCollider/WaveAttackCollider.h"
#include "Camera/DefalutCamera.h"
#include "Actor/Player/PlayerState/PlayerOwnedState.h"

//コンストラクタ
Player::Player(IWorld* world, const GSvector3& position){
	//ワールド設定
	m_World = world;
	//タグ名
	m_Tag = st_PlayerTag;
	//名前
	m_Name = st_Player;
	//メッシュ作成
	m_Mesh = { Mesh_Player,MotionPlayerIdle,true };
	//衝突判定級の設定
	m_Collider = BoundingSphere{ PlayerRadius,GSvector3{0.f,PlayerHeight,0.f} };
	//座標の初期化
	m_Transform.position(position);
	//メッシュの変換行列を初期化
	m_Mesh.Transform(m_Transform.localToWorldMatrix());
	//必殺技データのロード
	m_Ability.AbilityLoad("Data/ability_data.txt");
	//スキルロード
	m_Skill.SkillLoad("Data/skill_data.txt");
	//スキルセット
	m_Skill.Excute(this);
	//ステート追加
	AddState();
	//アニメーションイベント
	SetAnimationEvent();
	//初期ステート設定
	CurrentState(PlayerOwnedState::Idle);
	//無敵時間取得
	m_InvincibleTime = m_Status.InvincibleTime();
	//回避距離取得
	m_AvoidDistance = m_Status.AvoidDistance();
	//攻撃力初期化
	m_Status.SetAttackRate();
	//HPスライダー
	m_HPSlider = new Slider(PlayerHPSliderPos, GSvector2{ m_Status.MaxHealth() * PlayerHPSliderScale.x,PlayerHPSliderScale.y }, 0, m_Status.Health(), PlayerHPSliderColor);
}

Player::~Player() {
	delete m_HPSlider;
}

//更新
void Player::Update(float delta_time){
	//エネミー取得
	m_Enemy = m_World->FindActor(st_Enemy);
	//重力で下向きに加速
	m_Velocity.y += Gravity * delta_time;
	// velocityの分だけ移動する
	m_Transform.translate(m_Velocity * delta_time, GStransform::Space::World);
	//フィールドとの衝突判定
	CollideField();
	//ステートマシンの更新
	m_StateMachine.Update(delta_time* m_SlowMagnification);
	//セレクトシーンでは動かない
	if (m_IsSelect) m_Velocity = GSvector3::zero();
	//ステータス更新
	m_Status.Update(delta_time);
	m_HPSlider->SetValue(m_Status.Health());
	//アビリティ更新
	m_Ability.Update(m_World, delta_time);
	//アイテム更新
	m_Item.Update(m_World, delta_time);
	//メッシュ更新
	m_Mesh.Update(delta_time * m_SlowMagnification);
	//行列描画
	m_Mesh.Transform(m_Transform.localToWorldMatrix());
	//ジャスト回避
	JustAvoid();
	//ヒットストップ
	DecentHitStop();

	// フラッシュ時間をリセットする
	if (m_IsHitAvoidCollider|| Status().IsHealPortion() || Status().IsIsAttackPowerupPortion()) {

		if (m_FlashTimer <= 0.f) {
			m_FlashTimer = m_FlashTime;
			Status().UseHealPortion(false);
			return;
		}

		m_FlashTimer = std::max(m_FlashTimer - delta_time, 0.f);
	}
}

//描画
void Player::Draw() const{

	// リムライトの設定
	float transparency = gsGetRimLightExponent();
	GScolor current_secondary_color;
	//通常カラー
	gsSetRimLightExponent(usually_exponent);
	gsSetRimLightColor(&usually_color);
	if (!m_IsSelect) {
		//オートスロー状態だったら
		if (m_IsHitAvoidCollider)RimColor(just_avoidance_color, avoid_exponent);
		//ヒールポーションを飲んだら
		if (Status().IsHealPortion())RimColor(heal_color, heal_exponent);
		//攻撃上昇ポーションを飲んだら
		if (Status().IsIsAttackPowerupPortion())RimColor(power_color, power_exponent);
	}
	//メッシュ描画
	m_Mesh.Draw();
	//通常カラー
	gsSetRimLightExponent(transparency);
	gsSetRimLightColor(&current_secondary_color);
	//武器描画
	WeponDraw();
}

void Player::DrawGui() const{
	if (m_IsSelect)return;
	m_Skill.Draw();
	m_Item.Draw();
	m_Ability.Draw();
	m_HPSlider->Draw();
	//回避時のボタン描画
	if (m_IsJustAvoid)gsDrawSprite2D(X_Button_Texture, &XButtonPos, NULL, NULL, NULL, &XButtonSc, NULL);
}

//衝突リアクション
void Player::React(Actor& other){

	//回避判定に当たったら
	if (other.Tag() == st_AvoidTag) {
		//回避補助スキルを使っていたら
		if (m_Status.IsAvoidAuxiliary()) {
			static_cast<DefalutCamera*>(m_World->GetCamera())->ShakeOff();
			m_IsHitAvoidCollider = true;
			m_IsJustAvoid = true;
			return;
		}
		//回避中だったら
		if (IsInState(PlayerOwnedState::AvoidanceBack) || IsInState(PlayerOwnedState::AvoidanceForward)
		|| IsInState(PlayerOwnedState::AvoidanceLeft) || IsInState(PlayerOwnedState::AvoidanceRight)) {
			m_IsJustAvoid = true;
			return;
		}
	}
	//敵と衝突したか
	if (other.Tag() == st_Enemy) {
		//アクター同士が重ならないように補正
		CollideActor(other);
	}

	//敵の攻撃判定と衝突したか
	if (m_IsJustAvoid || m_IsJustAttack)return;
	if (other.Tag() == st_EnemyAttackTag) {
		//アビリティブロウ時に攻撃されたら
		if (IsInState(PlayerOwnedState::AbilityBlowAttack)) m_IsReact = true;
		else m_IsReact = false;

		//攻撃力の取得
		m_DamageValue = static_cast<AttackCollider*>(&other)->DamageValue();
		ResetVelocity();
		//ダメージ処理
		m_Status.HitDamage(m_DamageValue);
		//SE追加
		gsPlaySE(PlayerReact_SE);
		static_cast<DefalutCamera*>(m_World->GetCamera())->Shake();
		//敵全員にメッセージを送る
		for (auto& enemy : m_World->FindActorWithTag(st_Enemy)) {
			enemy->HandleMessage(st_Message_Hit,nullptr);
		}

		//死亡状態遷移
		if (m_Status.IsDead()) {
			gsStopBGM();
			gsPlaySE(Enemy_Finish_SE);
			m_EnableCollider = false;
			ChangeState(PlayerOwnedState::Die, false);
			return;
		}
		//ノックバック攻撃を食らったら
		if (IsKnockBack(other) && other.Name() == st_EnemyAttack) {
			DecentHitMotion(other);
			return;
		}
		//強攻撃に当たったら
		else if (other.Name() == st_EnemyStrongAttack) {
			float angle = TargetAngle();
			m_Transform.rotate(0.f, angle, 0.f);
			ChangeState(PlayerOwnedState::StrongHit, false);
			return;
		}
	}
	m_IsHitAvoidCollider = false;
	m_IsJustAvoid = false;
}

void Player::HandleMessage(const std::string& message, void* param){
	//セレクトシーン
	if (message == st_Message_SelectScene) m_IsSelect = true;
	//ヒットストップ用
	if (message == st_Message_Hit)m_IsHitStop = true;
	//ターゲット変更
	if (message == st_Message_Target) {
		m_TargetEnemyNum = *static_cast<int*>(param);
	}
}

PlayerStatus& Player::Status() const{
	return *const_cast<PlayerStatus*>(std::addressof(m_Status));
}

GSvector3 Player::InputVelocity(GSvector2 input){
	GSvector3 forward = m_World->GetCamera()->Transform().forward();
	forward.y = 0.f;
	GSvector3 right = m_World->GetCamera()->Transform().right();
	right.y = 0.f;
	//入力方向の移動量の算出
	GSvector3 velocity = forward * input.y + right * input.x;
	return velocity;
}

Actor* Player::FindTargetEnemy() const{
	std::vector<Actor*>enemys = m_World->FindActorWithTag(st_Enemy);
	if (enemys.empty())return nullptr;
	//ターゲットがいなくなったら次へ
	if (m_TargetEnemyNum < 0) {

		std::pair<float, Actor*>p = { 10000.0f,enemys.at(0) };

		for (auto& enemy : enemys) {
			float d = GSvector3::distance(m_Transform.position(), enemy->Transform().position());
			if (p.first > d) {
				p = { d,enemy };
			}
		}
		return p.second;
	}
	return enemys.at(m_TargetEnemyNum);
}



void Player::Interpolation(GSvector3 velocity, float delta_time){
	if (velocity.magnitude() == 0.f)return;

	GSquaternion rotation = GSquaternion::rotateTowards(
		m_Transform.rotation(),
		GSquaternion::lookRotation(velocity), RotateSpeed * delta_time);
	m_Transform.rotation(rotation);
}

void Player::DirectionOfAvoidance(GSvector2 input){
	//入力によって回避方向を決める
	if (std::abs(input.x) > std::abs(input.y)) {

		if (input.x > 0.f) {
			ChangeState(PlayerOwnedState::AvoidanceRight);
			return;
		}
		else {
			ChangeState(PlayerOwnedState::AvoidanceLeft);
			return;
		}
	}
	else {
		if (input.y < 0.f) {
			ChangeState(PlayerOwnedState::AvoidanceBack);
			return;
		}
		else {
			ChangeState(PlayerOwnedState::AvoidanceForward);
			return;
		}
	}
}

//フィールドとの衝突判定
void Player::CollideField() {
	//壁との衝突判定
	GSvector3 center;
	if (m_World->GetField()->Collide(Collider(), &center)) {
		//y座標は変更しない
		center.y = m_Transform.position().y;
		//補正後の座標に変更
		m_Transform.position(center);
	}
	//地面との衝突判定
	GSvector3 position = m_Transform.position();
	Line line;
	line.start = position + m_Collider.m_Center;
	line.end = position + GSvector3{ 0.f,FootOffset,0.f };
	GSvector3 intersect;//地面との交点
	//
	Actor* field_actor{ nullptr };
	//親をリセットしておく
	m_Transform.parent(nullptr);
	if (m_World->GetField()->Collide(line, &intersect,nullptr,&field_actor)) {
		//交点の位置からy座標のみ補正する
		position.y = intersect.y;
		//座標を変更
		m_Transform.position(position);
		//重力初期化
		m_Velocity.y = 0.f;

		//フィールド用のアクタークラスと衝突したか
		if (field_actor != nullptr) {
			//衝突したフィールド用アクターの親のトランスフォームとして設定
			m_Transform.parent(&field_actor->Transform());
		}
	}
}

//アクターとの衝突処理
void Player::CollideActor(Actor& other) {
	//y座標を除く座標を求める
	GSvector3 position = m_Transform.position();
	position.y = 0.f;
	GSvector3 target = other.Transform().position();
	target.y = 0.f;
	//相手との距離
	float distance = GSvector3::distance(position, target);
	//衝突判定級の半径同士を加えた長さを求める
	float length = m_Collider.m_Radius + other.Collider().m_Radius;
	//衝突判定級の重なっている長さを求める
	float overlap = length - distance;
	//重なっている部分の半分の距離だけ移動量を求める
	GSvector3 v = (position - target).getNormalized() * overlap * OverlapValue;
	m_Transform.translate(v, GStransform::Space::World);

	//フィールドとの衝突判定
	CollideField();
}

void Player::SetEffect(GSuint effect, bool continuation) {
	// 継続するか
	if (continuation) {
		// 今発動しているエフェクトを削除
		StopEffect();
		GSvector3 pos = m_Transform.position();
		m_EffectHandle = gsPlayEffect(effect, &pos);
	}
	else {
		PlayEffect(effect, GSvector3{ 0.0f,0.0f,0.0f }, GSvector3{ 0.0f,0.0f,0.0f }, GSvector3{ 1.0f,1.0f,1.0f });
	}
}

void Player::StopEffect() {
	gsStopEffect(m_EffectHandle);
}

float Player::TargetSignedAngle() const {
	//ターゲットがいなければ0を返す
	if (m_Enemy == nullptr)return 0.f;
	//ターゲット方向のベクトルを求める
	GSvector3 to_target = m_Enemy->Transform().position() - m_Transform.position();
	//前向き方向のベクトルを取得
	GSvector3 forward = m_Transform.forward();
	//ベクトルのy成分を無効に知る
	forward.y = 0.f;
	to_target.y = 0.f;
	//前向き方向にベクトルとターゲット方向のベクトルの角度差を求める
	return GSvector3::signedAngle(forward, to_target);
}

float Player::TargetAngle() const {
	return std::abs(TargetSignedAngle());
}

float Player::PlayerSpeed() const{
	return m_Status.MoveSpeed();
}

//ターゲットとの距離を求める
float Player::TargetDistance(Actor* actor) const {
	//ターゲットがいなければ最大距離を返す
	if (actor == nullptr)return FLT_MAX;//float型の最大値
	//ターゲットとの距離を計算する
	return GSvector3::distance(actor->Transform().position(), m_Transform.position());
}

//モーション変更
void Player::ChangeMotion(GSuint motion, bool loop, float speed) {
	m_Mesh.ChangeMotion(motion, loop, speed);
}

//ステート変更
void Player::ChangeState(int state, bool restate) {
	m_StateMachine.Change_State(state, restate);
}

//モーションが終了しているか
bool Player::IsMotionEnd(float time)const {
	return m_Mesh.MotionTime() >= m_Mesh.MotionEndTime() - time;
}

bool Player::CanAttackNextState() const{
	return m_NextAttackStateFrag;
}

bool Player::IsSelect() const{
	return m_IsSelect;
}

bool Player::IsKnockBack(Actor& other) const{
	bool is_super_armar = m_Status.IsSuperArmar();
	if (other.Name() == st_EnemyAttack && is_super_armar)return false;
	if (other.Name() == st_EnemyStrongAttack && !is_super_armar)return false;
	return true;
}

bool Player::IsAbilityReact() const{
	return m_IsReact;
}

bool Player::IsJustAttack() const{
	return m_IsJustAttack;
}

bool Player::IsAvoidance() const{
	return m_IsJustAvoid;
}

bool Player::IsInState(int state) const{
	return m_StateMachine.IsInState(state);
}

bool Player::IsMotionTime(float num, float num2) const{
	if (m_Mesh.MotionTime() > num)return false;
	if (m_Mesh.MotionTime() < num2)return false;
	return true;
}

void Player::DecentHitMotion(Actor& other){
	//コライダ―計算
	GSvector3 intersect = (Collider().m_Center + other.Collider().m_Center) * (Collider().m_Radius / (Collider().m_Radius + other.Collider().m_Radius));
	GSvector3 vec = (m_Transform.position() - intersect).getNormalized();
	vec.y = 0.f;
	GSvector3 f = m_Transform.forward();
	f.y = 0.f;

	float angle = GSvector3::signedAngle(vec, f);

	//30以下なら前
	if (std::abs(angle) <= HitForwardAngle) {
		ChangeState(PlayerOwnedState::HitForward, false);
		return;
	}
	//150以上なら後ろ
	else if (std::abs(angle) > HitBackwardAngle) {
		ChangeState(PlayerOwnedState::HitBackward, false);
		return;
	}
	else if (std::abs(angle) > 0.f) {
		ChangeState(PlayerOwnedState::HitRight, false);
		return;
	}
	else{
		ChangeState(PlayerOwnedState::HitLeft, false);
		return;
	}
}

void Player::AvoidanceFrag(bool state){
	m_IsJustAvoid = state;
}

void Player::JustAttackFrag(bool state){
	m_IsJustAttack = state;
}

void Player::SetNextAttackStateFrag(bool next) {
	m_NextAttackStateFrag = next;
}

void Player::DecentHitStop(){
	if (!m_IsHitStop)return;
	HitStop(PlayerHitStopTime, 0.f);
}

void Player::HitStop(float time, float scale){
	if (m_HitStopTimer <= 0.f)m_HitStopTimer = time;

	m_World->PlaySlowTimer(scale);
	m_HitStopTimer--;

	if (m_HitStopTimer <= 0.f) {
		m_World->DefaultTime();
		m_IsHitStop = false;
	}
}

void Player::SlowMagnification(float scale){
	//スロー倍率を掛ける
	m_SlowMagnification = scale;
	//アタックコマンドを押したら
	if (InputManager::Instance().IsBottonDown(InputManager::InputType::Attack) && m_IsJustAvoid) {
		ChangeState(PlayerOwnedState::JustAvoidanceAttack);
		return;
	}
}

void Player::JustAvoid(){
	//ジャスト回避したら
	if (m_IsJustAvoid) {
		SlowMagnification(SlowMagnificationValue);
		return;
	}
	m_SlowMagnification = 1.f;
	return;
}

void Player::CurrentState(int s){
	m_StateMachine.SetCurrentState(s);
}

bool Player::BossFight()const{
	return m_Transform.position().z >= BossFightPos;
}

void Player::RimColor(GScolor color, float exponent)const {
	GScolor black{ 0.f, 0.f, 0.f, 1.0f };
	//補間値を求める
	float t = GSmathf::inverseLerp(0.f, m_FlashTime, m_FlashTimer);
	color = GScolor::lerp(black, color, t);

	gsSetRimLightColor(&color);
	gsSetRimLightExponent(exponent);
}

void Player::WeponDraw() const{
	glPushMatrix();
	//手のボーン(74番目)の位置にメッシュを描画
	glMultMatrixf(m_Mesh.BoneMatrices(PlayerBoneNum));
	// 平行移動させる
	glRotatef(RotateWeaponX, 1.f, 0.f, 0.f);
	glRotatef(RotateWeaponY, 0.f, 1.f, 0.f);
	glRotatef(RotateWeaponZ, 0.f, 0.f, 1.f);

	gsDrawMesh(Mesh_Weapon);
	glPopMatrix();
}


void Player::GenerateAttackCollider(std::string name, PlayerStatus::AttackType damage, float distance,float rightdis, float radius, float height, float life_span){
	// 衝突判定を出現させる座標を求める（前方の位置）
	GSvector3 position = m_Transform.position() + (m_Transform.forward() * distance) + (m_Transform.right() * rightdis);
	// 高さの補正（足元からの高さ）
	position.y += height;
	// 衝突判定用の球を作成
	BoundingSphere collider{ radius, position };
	// 衝突判定を出現させる
	m_World->AddActor(new BasicPlayerAttackCollider{ m_World, collider,
		name, st_PlayerAttackTag, m_Tag, m_Status.AttackPower(damage), m_Status.StanPower(damage),life_span, 0.f});
}

void Player::GenerateWaveCollider(std::string name, PlayerStatus::AttackType damage,float distance,float radius,float height){
	// 衝突判定を出現させる座標を求める（前方の位置）
	GSvector3 position = m_Transform.position() + m_Transform.forward() * distance;
	// 高さの補正（足元からの高さ）
	position.y += height;
	// 衝突判定用の球を作成
	BoundingSphere collider{ radius, position };
	GSvector3 velocity = m_Transform.forward() * WaveVelocityOffset;
	m_World->AddActor(new WaveAttackCollider{ m_World,collider,velocity,m_Status.AttackPower(damage),m_Status.StanPower(damage),name });
}

void Player::ChangeVelocity(const GSvector3& vec){
	if (IsSelect())return;
	m_Velocity = vec;
}

void Player::ResetVelocity(){
	m_Velocity = GSvector3::zero();
}

void Player::AddState() {

	m_StateMachine.AddState(PlayerOwnedState::Idle, new PlayerIdleState{ this,m_World });
	m_StateMachine.AddState(PlayerOwnedState::FirstAttack, new PlayerAttackFirstState{ this,m_World });
	m_StateMachine.AddState(PlayerOwnedState::SecondAttack, new PlayerAttackSecondState{ this,m_World });
	m_StateMachine.AddState(PlayerOwnedState::ThirdAttack, new PlayerAttackThirdState{ this,m_World });
	m_StateMachine.AddState(PlayerOwnedState::HitForward, new PlayerHitForwardState{ this,m_World });
	m_StateMachine.AddState(PlayerOwnedState::HitBackward, new PlayerHitBackwardState{ this,m_World });
	m_StateMachine.AddState(PlayerOwnedState::HitLeft, new PlayerHitLeftState{ this,m_World });
	m_StateMachine.AddState(PlayerOwnedState::HitRight, new PlayerHitRightState{ this,m_World });
	m_StateMachine.AddState(PlayerOwnedState::StrongHit, new PlayerHitStrongState{ this,m_World });
	m_StateMachine.AddState(PlayerOwnedState::GetUp, new PlayerGetUpState{ this,m_World });
	m_StateMachine.AddState(PlayerOwnedState::Die, new PlayerDieState{ this,m_World });
	m_StateMachine.AddState(PlayerOwnedState::DieLoop, new PlayerDieLoopState{ this,m_World });
	m_StateMachine.AddState(PlayerOwnedState::AvoidanceForward, new PlayerAvoidForwardState{ this,m_World });
	m_StateMachine.AddState(PlayerOwnedState::AvoidanceBack, new PlayerAvoidBackState{ this,m_World });
	m_StateMachine.AddState(PlayerOwnedState::AvoidanceRight, new PlayerAvoidRightState{ this,m_World });
	m_StateMachine.AddState(PlayerOwnedState::AvoidanceLeft, new PlayerAvoidLeftState{ this,m_World });
	m_StateMachine.AddState(PlayerOwnedState::JustAvoidanceAttack, new PlayerJustAvoidanceAttackState{ this,m_World });
	//アビリティ
	m_StateMachine.AddState(PlayerOwnedState::AbilityBlowAttack, new PlayerAbilityBlowAttackState{ this,m_World });
	m_StateMachine.AddState(PlayerOwnedState::AbilityBlowAttackReinforcement, new PlayerAbilityBlowAttackReinforcementState{ this,m_World });
	m_StateMachine.AddState(PlayerOwnedState::AbilityPushAttack, new PlayerAbilityPushAttackState{ this,m_World });
	m_StateMachine.AddState(PlayerOwnedState::AbilityAvoidAttack, new PlayerAbilityAvoidAttackState{ this,m_World });
	m_StateMachine.AddState(PlayerOwnedState::AbilityRotatingAttack, new PlayerAbilityRotatingAttackState{ this,m_World });
	m_StateMachine.AddState(PlayerOwnedState::AbilityInpactAttack, new PlayerAbilityInpactGroundAttackState{ this,m_World });
	m_StateMachine.AddState(PlayerOwnedState::AbilitySlamAttack, new PlayerAbilitySlamAttackState{ this,m_World });
	m_StateMachine.AddState(PlayerOwnedState::AbilityWildDance, new PlayerAbilityWildDanceAttackState{ this,m_World });
	m_StateMachine.AddState(PlayerOwnedState::AbilityWaveWildDance_1, new PlayerAbilityWaveWildDanceAttackState_1{ this,m_World });
	m_StateMachine.AddState(PlayerOwnedState::AbilityWaveWildDance_2, new PlayerAbilityWaveWildDanceAttackState_2{ this,m_World });
	m_StateMachine.AddState(PlayerOwnedState::AbilityWaveWildDance_3, new PlayerAbilityWaveWildDanceAttackState_3{ this,m_World });
	m_StateMachine.AddState(PlayerOwnedState::AbilitySlashAttack, new PlayerAbilitySlashAttackState{ this,m_World });

}

void Player::SetAnimationEvent() {
	//歩き
	m_Mesh.AddEvent(PlayerAnim::MotionFwdWalk, 2.0f, [=] {gsPlaySE(PlayerFoot_SE); });
	m_Mesh.AddEvent(PlayerAnim::MotionFwdWalk, 29.0f, [=] {gsPlaySE(PlayerFoot_SE); });
	//一撃目
	m_Mesh.AddEvent(PlayerAnim::MotionFirstAttack, 10.0f, [=] {ChangeVelocity(Transform().forward().normalized() * 0.08f); });
	m_Mesh.AddEvent(PlayerAnim::MotionFirstAttack, 15.0f, [=] { ResetVelocity(); });
	m_Mesh.AddEvent(PlayerAnim::MotionFirstAttack, 18.0f, [=] { PlayEffect(Effect_Attack, {0.28f,0.9f,0.0f}, {0.0f,0.0f,249.18f}, {1.0f,1.0f,1.0f}); });
	m_Mesh.AddEvent(PlayerAnim::MotionFirstAttack, 18.0f, [=] { gsPlaySE(Player_Attack_SE); });
	m_Mesh.AddEvent(PlayerAnim::MotionFirstAttack, 30.0f, [=] {GenerateAttackCollider(st_PlayerNormalAttack, PlayerStatus::AttackType::NormalAttack_1); });
	m_Mesh.AddEvent(PlayerAnim::MotionFirstAttack, 43.0f, [=] { SetNextAttackStateFrag(true); });
	//二撃目
	m_Mesh.AddEvent(PlayerAnim::MotionSecondAttack, 10.0f, [=] {ChangeVelocity(Transform().forward().normalized() * 0.08f); });
	m_Mesh.AddEvent(PlayerAnim::MotionSecondAttack, 13.0f, [=] { ResetVelocity(); });
	m_Mesh.AddEvent(PlayerAnim::MotionSecondAttack, 15.0f, [=] { PlayEffect(Effect_Attack, {0.0f,0.9f,0.0f},{0.0f,0.0f,-65.0f}, {1.0f,1.0f,1.0f}); });
	m_Mesh.AddEvent(PlayerAnim::MotionSecondAttack, 15.0f, [=] { gsPlaySE(Player_Attack2_SE); });
	m_Mesh.AddEvent(PlayerAnim::MotionSecondAttack, 15.0f, [=] {GenerateAttackCollider(st_PlayerNormalAttack, PlayerStatus::AttackType::NormalAttack_2); });
	m_Mesh.AddEvent(PlayerAnim::MotionSecondAttack, 20.0f, [=] { SetNextAttackStateFrag(true); });
	//三撃目
	m_Mesh.AddEvent(PlayerAnim::MotionThirdAttack, 10.0f, [=] {ChangeVelocity(Transform().forward().normalized() * 0.08f); });
	m_Mesh.AddEvent(PlayerAnim::MotionThirdAttack, 13.0f, [=] { ResetVelocity(); });
	m_Mesh.AddEvent(PlayerAnim::MotionThirdAttack, 19.0f, [=] { PlayEffect(Effect_Attack, {0.0f,0.9f,0.0f}, {0.0f,0.0f,48.0f}, {1.0f,1.0f,1.0f}); });
	m_Mesh.AddEvent(PlayerAnim::MotionThirdAttack, 19.0f, [=] { gsPlaySE(Player_Attack2_SE); });
	m_Mesh.AddEvent(PlayerAnim::MotionThirdAttack, 19.0f, [=] {GenerateAttackCollider(st_PlayerNormalAttack, PlayerStatus::AttackType::NormalAttack_2); });
	m_Mesh.AddEvent(PlayerAnim::MotionThirdAttack, 20.0f, [=] { SetNextAttackStateFrag(true); });
	//カウンター
	m_Mesh.AddEvent(PlayerAnim::MotionJustAvoidAttack, 25.0f, [=] { PlayEffect(Effect_Counter, {0.0f,0.9f,0.0f}, {0.0f,0.0f,29.0f}, {1.0f,1.0f,1.0f}); });
	m_Mesh.AddEvent(PlayerAnim::MotionJustAvoidAttack, 25.0f, [=] { gsPlaySE(Player_Attack_SE); });
	m_Mesh.AddEvent(PlayerAnim::MotionJustAvoidAttack, 25.0f, [=] { GenerateAttackCollider(st_PlayerNormalAttack, PlayerStatus::AttackType::CounterAttack);  });
	m_Mesh.AddEvent(PlayerAnim::MotionJustAvoidAttack, 65.0f, [=] { gsPlaySE(Player_Attack_SE); });
	m_Mesh.AddEvent(PlayerAnim::MotionJustAvoidAttack, 65.0f, [=] { PlayEffect(Effect_Counter, {-0.47f,0.9f,0.0f}, {0.0f,0.0f,127.0f}, {1.0f,1.0f,1.0f}); });
	m_Mesh.AddEvent(PlayerAnim::MotionJustAvoidAttack, 65.0f, [=] { GenerateAttackCollider(st_PlayerNormalAttack, PlayerStatus::AttackType::CounterAttack);  });
	m_Mesh.AddEvent(PlayerAnim::MotionJustAvoidAttack, 80.0f, [=] { gsPlaySE(Player_Attack_SE); });
	m_Mesh.AddEvent(PlayerAnim::MotionJustAvoidAttack, 80.0f, [=] { PlayEffect(Effect_Counter, { 0.0f,0.9f,0.0f }, { 0.0f,0.0f,-65.0f }, { 1.0f,1.0f,1.0f }); });
	m_Mesh.AddEvent(PlayerAnim::MotionJustAvoidAttack, 80.0f, [=] { GenerateAttackCollider(st_PlayerNormalAttack, PlayerStatus::AttackType::CounterAttack);  });
	m_Mesh.AddEvent(PlayerAnim::MotionJustAvoidAttack, 125.0f, [=] { PlayEffect(Effect_Counter, { 0.0f,0.9f,0.0f }, { 0.0f,0.0f,90.0f }, { 1.0f,1.0f,1.0f }); });
	m_Mesh.AddEvent(PlayerAnim::MotionJustAvoidAttack, 125.0f, [=] { gsPlaySE(Player_Attack2_SE); });
	m_Mesh.AddEvent(PlayerAnim::MotionJustAvoidAttack, 125.0f, [=] { GenerateAttackCollider(st_PlayerStrongAttack, PlayerStatus::AttackType::CounterAttack);  });
	//回避(フレーム回避あり)
	m_Mesh.AddEvent(PlayerAnim::MotionForwardAvoidance, 1.0f, [=] { m_IsInvincible = true; });
	m_Mesh.AddEvent(PlayerAnim::MotionForwardAvoidance, 1.0f + m_InvincibleTime, [=] { m_IsInvincible = false; });
	m_Mesh.AddEvent(PlayerAnim::MotionBackAvoidance, 1.0f, [=] { m_IsInvincible = true; });
	m_Mesh.AddEvent(PlayerAnim::MotionBackAvoidance, 1.0f + m_InvincibleTime, [=] { m_IsInvincible = false; });
	m_Mesh.AddEvent(PlayerAnim::MotionRightAvoidance, 1.0f, [=] { m_IsInvincible = true; });
	m_Mesh.AddEvent(PlayerAnim::MotionRightAvoidance, 1.0f + m_InvincibleTime, [=] { m_IsInvincible = false; });
	m_Mesh.AddEvent(PlayerAnim::MotionLeftAvoidance, 1.0f, [=] { m_IsInvincible = true; });
	m_Mesh.AddEvent(PlayerAnim::MotionLeftAvoidance, 1.0f + m_InvincibleTime, [=] { m_IsInvincible = false; });
	//死亡
	m_Mesh.AddEvent(PlayerAnim::MotionDieLoop, 30.0f, [=] {m_World->SetGameOverFrag(); });
	//ブロー攻撃
	m_Mesh.AddEvent(PlayerAnim::MotionAbilityBlowAttack, 10.0f, [=] { ChangeVelocity(Transform().forward().normalized() * 0.08f); });
	m_Mesh.AddEvent(PlayerAnim::MotionAbilityBlowAttack, 35.0f, [=] {GenerateAttackCollider(st_PlayerStrongAttack, PlayerStatus::AttackType::AbilityBlowAttack); });
	m_Mesh.AddEvent(PlayerAnim::MotionAbilityBlowAttack, 35.0f, [=] { gsPlaySE(Player_Attack_SE); });
	m_Mesh.AddEvent(PlayerAnim::MotionAbilityBlowAttack, 40.0f, [=] { PlayEffect(Effect_BlowAttack, { 0.0f,0.9f,0.0f }, { 0.0f,0.0f,130.0f }, { 1.0f,1.0f,1.0f }); });
	m_Mesh.AddEvent(PlayerAnim::MotionAbilityBlowAttack, 50.0f, [=] { ResetVelocity(); });
	m_Mesh.AddEvent(PlayerAnim::MotionAbilityBlowAttackReinforcement, 1.0f, [=] {GenerateAttackCollider(st_PlayerBlowAttack, PlayerStatus::AttackType::AbilityBlowAttackReinforcement); });
	m_Mesh.AddEvent(PlayerAnim::MotionAbilityBlowAttackReinforcement, 10.0f, [=] {GenerateAttackCollider(st_PlayerBlowAttack, PlayerStatus::AttackType::AbilityBlowAttackReinforcement); });
	m_Mesh.AddEvent(PlayerAnim::MotionAbilityBlowAttackReinforcement, 15.0f, [=] { PlayEffect(Effect_BlowAttack, { 0.0f,0.9f,0.0f }, { 0.0f,0.0f,40.0f }, { 1.0f,1.0f,1.0f }); });
	m_Mesh.AddEvent(PlayerAnim::MotionAbilityBlowAttackReinforcement, 15.0f, [=] { gsPlaySE(Player_Attack2_SE); });
	//プッシュ攻撃
	m_Mesh.AddEvent(PlayerAnim::MotionAbilityPushAttack, 10.0f, [=] { ChangeVelocity(Transform().forward().normalized() * 0.08f); });
	m_Mesh.AddEvent(PlayerAnim::MotionAbilityPushAttack, 15.0f, [=] { ResetVelocity(); });
	m_Mesh.AddEvent(PlayerAnim::MotionAbilityPushAttack, 25.0f, [=] {GenerateAttackCollider(st_PlayerPushUpAttack, PlayerStatus::AttackType::AbilityPushUpAttack); });
	m_Mesh.AddEvent(PlayerAnim::MotionAbilityPushAttack, 30.0f, [=] { PlayEffect(Effect_PushAttack, { -0.18f,0.9f,0.0f }, { 0.0f,0.0f,243.0f }, { 1.0f,1.0f,1.0f }); });
	m_Mesh.AddEvent(PlayerAnim::MotionAbilityPushAttack, 30.0f, [=] { gsPlaySE(Player_Attack2_SE); });
	m_Mesh.AddEvent(PlayerAnim::MotionAbilityPushAttack, 50.0f, [=] {GenerateAttackCollider(st_PlayerPushUpAttack, PlayerStatus::AttackType::AbilityPushUpAttack); });
	m_Mesh.AddEvent(PlayerAnim::MotionAbilityPushAttack, 55.0f, [=] { PlayEffect(Effect_PushAttack, { 0.11f,0.9f,0.0f }, { 0.0f,0.0f,-65.0f }, { 1.0f,1.0f,1.0f }); });
	m_Mesh.AddEvent(PlayerAnim::MotionAbilityPushAttack, 55.0f, [=] { gsPlaySE(Player_Attack2_SE); });
	//回避斬り
	m_Mesh.AddEvent(PlayerAnim::MotionAbilityAvoidAttack, 10.0f, [=] { ChangeVelocity(Transform().forward().normalized() * 0.28f); });
	m_Mesh.AddEvent(PlayerAnim::MotionAbilityAvoidAttack, 20.0f, [=] { ResetVelocity(); });
	m_Mesh.AddEvent(PlayerAnim::MotionAbilityAvoidAttack, 23.0f, [=] { ChangeVelocity(-Transform().forward().normalized() * 0.28f); m_Velocity.y = 0.0f; });
	m_Mesh.AddEvent(PlayerAnim::MotionAbilityAvoidAttack, 30.0f, [=] { ResetVelocity(); });
	m_Mesh.AddEvent(PlayerAnim::MotionAbilityAvoidAttack, 21.0f, [=] {GenerateAttackCollider(st_PlayerStrongAttack, PlayerStatus::AttackType::AbilityAvoidAttack); });
	m_Mesh.AddEvent(PlayerAnim::MotionAbilityAvoidAttack, 21.0f, [=] { gsPlaySE(Player_Attack2_SE); });
	m_Mesh.AddEvent(PlayerAnim::MotionAbilityAvoidAttack, 26.0f, [=] { PlayEffect(Effect_AvoidAttack, { 0.0f,0.9f,0.0f }, { 0.0f,0.0f,-106.0f }, { 1.0f,1.0f,1.0f }); });
	//回転斬り
	m_Mesh.AddEvent(PlayerAnim::MotionAbilityRotatingAttack, 1.0f, [=] { gsPlaySE(Player_Attack_SE); });
	m_Mesh.AddEvent(PlayerAnim::MotionAbilityRotatingAttack, 1.0f, [=]  {GenerateAttackCollider(st_PlayerStrongAttack, PlayerStatus::AttackType::AbilityRotatingAttack, 1.0f,0.0f); });
	m_Mesh.AddEvent(PlayerAnim::MotionAbilityRotatingAttack, 4.0f, [=]  {GenerateAttackCollider(st_PlayerStrongAttack, PlayerStatus::AttackType::AbilityRotatingAttack, 0.0f, -1.0f); });
	m_Mesh.AddEvent(PlayerAnim::MotionAbilityRotatingAttack, 7.0f, [=]  {GenerateAttackCollider(st_PlayerStrongAttack, PlayerStatus::AttackType::AbilityRotatingAttack, -1.0f, 0.0f); });
	m_Mesh.AddEvent(PlayerAnim::MotionAbilityRotatingAttack, 10.0f, [=] {GenerateAttackCollider(st_PlayerStrongAttack, PlayerStatus::AttackType::AbilityRotatingAttack, 0.0f, 1.0f); });
	m_Mesh.AddEvent(PlayerAnim::MotionAbilityRotatingAttack, 10.0f, [=] { gsPlaySE(Player_Attack_SE); });
	m_Mesh.AddEvent(PlayerAnim::MotionAbilityRotatingAttack, 13.0f, [=] {GenerateAttackCollider(st_PlayerStrongAttack, PlayerStatus::AttackType::AbilityRotatingAttack, 1.0f, 0.0f); });
	m_Mesh.AddEvent(PlayerAnim::MotionAbilityRotatingAttack, 16.0f, [=] {GenerateAttackCollider(st_PlayerStrongAttack, PlayerStatus::AttackType::AbilityRotatingAttack, 0.0f, -1.0f); });
	m_Mesh.AddEvent(PlayerAnim::MotionAbilityRotatingAttack, 19.0f, [=] {GenerateAttackCollider(st_PlayerStrongAttack, PlayerStatus::AttackType::AbilityRotatingAttack, -1.0f, 0.0f); });
	m_Mesh.AddEvent(PlayerAnim::MotionAbilityRotatingAttack, 20.0f, [=] { gsPlaySE(Player_Attack_SE); });
	m_Mesh.AddEvent(PlayerAnim::MotionAbilityRotatingAttack, 22.0f, [=] {GenerateAttackCollider(st_PlayerStrongAttack, PlayerStatus::AttackType::AbilityRotatingAttack, 0.0f, 1.0f); });
	m_Mesh.AddEvent(PlayerAnim::MotionAbilityRotatingAttack, 25.0f, [=] {GenerateAttackCollider(st_PlayerStrongAttack, PlayerStatus::AttackType::AbilityRotatingAttack, 1.0f, 0.0f); });
	m_Mesh.AddEvent(PlayerAnim::MotionAbilityRotatingAttack, 28.0f, [=] {GenerateAttackCollider(st_PlayerStrongAttack, PlayerStatus::AttackType::AbilityRotatingAttack, 0.0f, -1.0f); });
	m_Mesh.AddEvent(PlayerAnim::MotionAbilityRotatingAttack, 30.0f, [=] { gsPlaySE(Player_Attack_SE); });
	m_Mesh.AddEvent(PlayerAnim::MotionAbilityRotatingAttack, 31.0f, [=] {GenerateAttackCollider(st_PlayerStrongAttack, PlayerStatus::AttackType::AbilityRotatingAttack, -1.0f, 0.0f); });
	m_Mesh.AddEvent(PlayerAnim::MotionAbilityRotatingAttack, 34.0f, [=] {GenerateAttackCollider(st_PlayerStrongAttack, PlayerStatus::AttackType::AbilityRotatingAttack, 0.0f, 1.0f); });
	m_Mesh.AddEvent(PlayerAnim::MotionAbilityRotatingAttack, 37.0f, [=] {GenerateAttackCollider(st_PlayerStrongAttack, PlayerStatus::AttackType::AbilityRotatingAttack, 1.0f, 0.0f); });
	m_Mesh.AddEvent(PlayerAnim::MotionAbilityRotatingAttack, 40.0f, [=] {GenerateAttackCollider(st_PlayerStrongAttack, PlayerStatus::AttackType::AbilityRotatingAttack, 0.0f, -1.0f); });
	m_Mesh.AddEvent(PlayerAnim::MotionAbilityRotatingAttack, 40.0f, [=] { gsPlaySE(Player_Attack_SE); });
	m_Mesh.AddEvent(PlayerAnim::MotionAbilityRotatingAttack, 43.0f, [=] {GenerateAttackCollider(st_PlayerStrongAttack, PlayerStatus::AttackType::AbilityRotatingAttack, -1.0f, 0.0f); });
	m_Mesh.AddEvent(PlayerAnim::MotionAbilityRotatingAttack, 47.0f, [=] {GenerateAttackCollider(st_PlayerStrongAttack, PlayerStatus::AttackType::AbilityRotatingAttack, 0.0f, 1.0f); });
	m_Mesh.AddEvent(PlayerAnim::MotionAbilityRotatingAttack, 50.0f, [=] {GenerateAttackCollider(st_PlayerStrongAttack, PlayerStatus::AttackType::AbilityRotatingAttack, 1.0f, 0.0f); });
	m_Mesh.AddEvent(PlayerAnim::MotionAbilityRotatingAttack, 50.0f, [=] { gsPlaySE(Player_Attack_SE); });
	m_Mesh.AddEvent(PlayerAnim::MotionAbilityRotatingAttack, 53.0f, [=] {GenerateAttackCollider(st_PlayerStrongAttack, PlayerStatus::AttackType::AbilityRotatingAttack, 0.0f, -1.0f); });
	m_Mesh.AddEvent(PlayerAnim::MotionAbilityRotatingAttack, 56.0f, [=] {GenerateAttackCollider(st_PlayerStrongAttack, PlayerStatus::AttackType::AbilityRotatingAttack, -1.0f, 0.0f); });
	m_Mesh.AddEvent(PlayerAnim::MotionAbilityRotatingAttack, 59.0f, [=] {GenerateAttackCollider(st_PlayerStrongAttack, PlayerStatus::AttackType::AbilityRotatingAttack, 0.0f, 1.0f); });
	m_Mesh.AddEvent(PlayerAnim::MotionAbilityRotatingAttack, 60.0f, [=] { gsPlaySE(Player_Attack_SE); });
	m_Mesh.AddEvent(PlayerAnim::MotionAbilityRotatingAttack, 62.0f, [=] {GenerateAttackCollider(st_PlayerStrongAttack, PlayerStatus::AttackType::AbilityRotatingAttack, 0.0f, 1.0f); });
	//インパクト攻撃
	m_Mesh.AddEvent(PlayerAnim::MotionAbilityInpactAttack, 10.0f, [=] {ChangeVelocity(Transform().forward().normalized() * 0.28f); });
	m_Mesh.AddEvent(PlayerAnim::MotionAbilityInpactAttack, 20.0f, [=] { ResetVelocity(); });
	m_Mesh.AddEvent(PlayerAnim::MotionAbilityInpactAttack, 60.0f, [=] { PlayEffect(Effect_InpactAttack, { 0.0f,0.0f,1.0f }, { 0.0f,0.0f,0.0f }, { 1.5f,1.5f,1.5f }); });
	m_Mesh.AddEvent(PlayerAnim::MotionAbilityInpactAttack, 65.0f, [=] { gsPlaySE(Player_Attack2_SE); });
	m_Mesh.AddEvent(PlayerAnim::MotionAbilityInpactAttack, 65.0f, [=] {GenerateAttackCollider(st_PlayerBlowAttack, PlayerStatus::AttackType::AbilityInpactAttack2, 1.0f,0.0f,1.5f); });
	//叩きつけ
	m_Mesh.AddEvent(PlayerAnim::MotionAbilitySlamAttack, 20.0f, [=] { ChangeVelocity(Transform().forward().normalized() * 0.28f); });
	m_Mesh.AddEvent(PlayerAnim::MotionAbilitySlamAttack, 25.0f, [=] { ResetVelocity(); });
	m_Mesh.AddEvent(PlayerAnim::MotionAbilitySlamAttack, 26.0f, [=] {GenerateAttackCollider(st_PlayerStrongAttack, PlayerStatus::AttackType::AbilitySlamAttack, 1.0f); });
	m_Mesh.AddEvent(PlayerAnim::MotionAbilitySlamAttack, 31.0f, [=] { gsPlaySE(Player_Attack2_SE); });
	m_Mesh.AddEvent(PlayerAnim::MotionAbilitySlamAttack, 31.0f, [=] { PlayEffect(Effect_SlamAttack, { 0.21f,1.0f,0.0f }, { 0.0f,0.0f,-67.0f }, { 1.0f,1.0f,1.0f }); });
	m_Mesh.AddEvent(PlayerAnim::MotionAbilitySlamAttack, 75.0f, [=] {GenerateAttackCollider(st_PlayerStrongAttack, PlayerStatus::AttackType::AbilitySlamAttack2, 1.0f); });
	m_Mesh.AddEvent(PlayerAnim::MotionAbilitySlamAttack, 80.0f, [=] { PlayEffect(Effect_InpactAttack, { 0.0f,0.0f,1.0f }, { 0.0f,0.0f,0.0f }, { 1.0f,1.0f,1.0f }); });
	m_Mesh.AddEvent(PlayerAnim::MotionAbilitySlamAttack, 80.0f, [=] { gsPlaySE(Player_Attack2_SE); });
	//乱舞
	m_Mesh.AddEvent(PlayerAnim::MotionAbilityWildDanceAttack, 1.0f, [=] { ChangeVelocity(Transform().forward().normalized() * 0.018f); });
	m_Mesh.AddEvent(PlayerAnim::MotionAbilityWildDanceAttack, 15.0f, [=] { ResetVelocity(); });
	m_Mesh.AddEvent(PlayerAnim::MotionAbilityWildDanceAttack, 20.0f, [=] {GenerateAttackCollider(st_PlayerNormalAttack, PlayerStatus::AttackType::AbilityWildDance_1, 1.0f); });
	m_Mesh.AddEvent(PlayerAnim::MotionAbilityWildDanceAttack, 25.0f, [=] { gsPlaySE(Player_Attack_SE); });
	m_Mesh.AddEvent(PlayerAnim::MotionAbilityWildDanceAttack, 25.0f, [=] { PlayEffect(Effect_WildDance, { -0.11f,0.7f,0.0f }, { 0.0f,0.0f,130.0f }, { 1.0f,1.0f,1.0f }); });
	m_Mesh.AddEvent(PlayerAnim::MotionAbilityWildDanceAttack, 45.0f, [=] { ChangeVelocity(Transform().forward().normalized() * 0.018f); });
	m_Mesh.AddEvent(PlayerAnim::MotionAbilityWildDanceAttack, 48.0f, [=] { ResetVelocity(); });
	m_Mesh.AddEvent(PlayerAnim::MotionAbilityWildDanceAttack, 50.0f, [=] {GenerateAttackCollider(st_PlayerNormalAttack, PlayerStatus::AttackType::AbilityWildDance_2, 2.0f); });
	m_Mesh.AddEvent(PlayerAnim::MotionAbilityWildDanceAttack, 55.0f, [=] { gsPlaySE(Player_Attack_SE); });
	m_Mesh.AddEvent(PlayerAnim::MotionAbilityWildDanceAttack, 55.0f, [=] { PlayEffect(Effect_WildDance, {0.0f,0.9f,0.0f}, {0.0f,0.0f,-58.0f}, {1.0f,1.0f,1.0f}); });
	m_Mesh.AddEvent(PlayerAnim::MotionAbilityWildDanceAttack, 55.0f, [=] { ChangeVelocity(Transform().forward().normalized() * 0.08f); });
	m_Mesh.AddEvent(PlayerAnim::MotionAbilityWildDanceAttack, 70.0f, [=] { ResetVelocity(); });
	m_Mesh.AddEvent(PlayerAnim::MotionAbilityWildDanceAttack, 100.0f, [=] {GenerateAttackCollider(st_PlayerNormalAttack, PlayerStatus::AttackType::AbilityWildDance_3, 1.0f); });
	m_Mesh.AddEvent(PlayerAnim::MotionAbilityWildDanceAttack, 105.0f, [=] { gsPlaySE(Player_Attack_SE); });
	m_Mesh.AddEvent(PlayerAnim::MotionAbilityWildDanceAttack, 105.0f, [=] { PlayEffect(Effect_WildDance, {0.0f,0.9f,0.0f}, {0.0f,0.0f,55.0f}, {1.0f,1.0f,1.0f}); });
	m_Mesh.AddEvent(PlayerAnim::MotionAbilityWildDanceAttack, 105.0f, [=] { ChangeVelocity(Transform().forward().normalized() * 0.018f); });
	m_Mesh.AddEvent(PlayerAnim::MotionAbilityWildDanceAttack, 115.0f, [=] { ResetVelocity(); });
	m_Mesh.AddEvent(PlayerAnim::MotionAbilityWildDanceAttack, 125.0f, [=] {GenerateAttackCollider(st_PlayerNormalAttack, PlayerStatus::AttackType::AbilityWildDance_4, 1.0f); });
	m_Mesh.AddEvent(PlayerAnim::MotionAbilityWildDanceAttack, 130.0f, [=] { gsPlaySE(Player_Attack_SE); });
	m_Mesh.AddEvent(PlayerAnim::MotionAbilityWildDanceAttack, 130.0f, [=] { PlayEffect(Effect_WildDance, { 0.0f,0.9f,0.0f }, {0.0f,0.0f,-130.0f}, {1.0f,1.0f,1.0f}); });
	m_Mesh.AddEvent(PlayerAnim::MotionAbilityWildDanceAttack, 130.0f, [=] { ChangeVelocity(Transform().forward().normalized() * 0.018f); });
	m_Mesh.AddEvent(PlayerAnim::MotionAbilityWildDanceAttack, 140.0f, [=] { ResetVelocity(); });
	m_Mesh.AddEvent(PlayerAnim::MotionAbilityWildDanceAttack, 160.0f, [=] {GenerateAttackCollider(st_PlayerNormalAttack, PlayerStatus::AttackType::AbilityWildDance_5, 1.0f); });
	m_Mesh.AddEvent(PlayerAnim::MotionAbilityWildDanceAttack, 165.0f, [=] { gsPlaySE(Player_Attack_SE); });
	m_Mesh.AddEvent(PlayerAnim::MotionAbilityWildDanceAttack, 165.0f, [=] { PlayEffect(Effect_WildDance, { 0.0f,0.9f,0.0f }, {0.0f,0.0f,-58.0f}, {1.0f,1.0f,1.0f}); });
	m_Mesh.AddEvent(PlayerAnim::MotionAbilityWildDanceAttack, 165.0f, [=] { ChangeVelocity(Transform().forward().normalized() * 0.018f); });
	m_Mesh.AddEvent(PlayerAnim::MotionAbilityWildDanceAttack, 180.0f, [=] { ResetVelocity(); });
	m_Mesh.AddEvent(PlayerAnim::MotionAbilityWildDanceAttack, 220.0f, [=] {GenerateAttackCollider(st_PlayerNormalAttack, PlayerStatus::AttackType::AbilityWildDance_6, 1.0f); });
	m_Mesh.AddEvent(PlayerAnim::MotionAbilityWildDanceAttack, 225.0f, [=] { gsPlaySE(Player_Attack_SE); });
	m_Mesh.AddEvent(PlayerAnim::MotionAbilityWildDanceAttack, 225.0f, [=] { PlayEffect(Effect_WildDance, { 0.0f,0.9f,0.0f }, {0.0f,0.0f,43.0f}, {1.0f,1.0f,1.0f}); });
	m_Mesh.AddEvent(PlayerAnim::MotionAbilityWildDanceAttack, 225.0f, [=] { ChangeVelocity(Transform().forward().normalized() * 0.018f); });
	m_Mesh.AddEvent(PlayerAnim::MotionAbilityWildDanceAttack, 240.0f, [=] { ResetVelocity(); });
	m_Mesh.AddEvent(PlayerAnim::MotionAbilityWildDanceAttack, 245.0f, [=] { ChangeVelocity(Transform().forward().normalized() * 0.018f); });
	m_Mesh.AddEvent(PlayerAnim::MotionAbilityWildDanceAttack, 280.0f, [=] { ResetVelocity(); });
	m_Mesh.AddEvent(PlayerAnim::MotionAbilityWildDanceAttack, 280.0f, [=] {GenerateAttackCollider(st_PlayerNormalAttack, PlayerStatus::AttackType::AbilityWildDance_7, 1.0f); });
	m_Mesh.AddEvent(PlayerAnim::MotionAbilityWildDanceAttack, 285.0f, [=] { gsPlaySE(Player_Attack_SE); });
	m_Mesh.AddEvent(PlayerAnim::MotionAbilityWildDanceAttack, 285.0f, [=] { PlayEffect(Effect_WildDance, { 0.0f,0.9f,0.0f }, {0.0f,0.0f,122.0f}, {1.0f,1.0f,1.0f}); });
	//波動乱舞
	m_Mesh.AddEvent(PlayerAnim::MotionAbilityWaveWildDance_1, 20.0f, [=] { ChangeVelocity(Transform().forward().normalized() * 0.018f); });
	m_Mesh.AddEvent(PlayerAnim::MotionAbilityWaveWildDance_1, 39.0f, [=] { ResetVelocity(); });
	m_Mesh.AddEvent(PlayerAnim::MotionAbilityWaveWildDance_1, 40.0f, [=] {GenerateWaveCollider(st_PlayerWaveAttack, PlayerStatus::AttackType::AbilityWaveWildDance_1); });
	m_Mesh.AddEvent(PlayerAnim::MotionAbilityWaveWildDance_1, 40.0f, [=] { gsPlaySE(Player_Attack2_SE); });
	m_Mesh.AddEvent(PlayerAnim::MotionAbilityWaveWildDance_2, 30.0f, [=] { ChangeVelocity(Transform().forward().normalized() * 0.28f); });
	m_Mesh.AddEvent(PlayerAnim::MotionAbilityWaveWildDance_2, 35.0f, [=] { ResetVelocity(); });
	m_Mesh.AddEvent(PlayerAnim::MotionAbilityWaveWildDance_2, 40.0f, [=] {GenerateWaveCollider(st_PlayerWaveAttack, PlayerStatus::AttackType::AbilityWaveWildDance_2); });
	m_Mesh.AddEvent(PlayerAnim::MotionAbilityWaveWildDance_2, 40.0f, [=] { gsPlaySE(Player_Attack2_SE); });
	m_Mesh.AddEvent(PlayerAnim::MotionAbilityWaveWildDance_2, 43.0f, [=] { ChangeVelocity(Transform().forward().normalized() * 0.018f); });
	m_Mesh.AddEvent(PlayerAnim::MotionAbilityWaveWildDance_2, 50.0f, [=] { ResetVelocity(); });
	m_Mesh.AddEvent(PlayerAnim::MotionAbilityWaveWildDance_3, 30.0f, [=] { ChangeVelocity(Transform().forward().normalized() * 0.28f); });
	m_Mesh.AddEvent(PlayerAnim::MotionAbilityWaveWildDance_3, 35.0f, [=] { ResetVelocity(); });
	m_Mesh.AddEvent(PlayerAnim::MotionAbilityWaveWildDance_3, 40.0f, [=] {GenerateAttackCollider(st_PlayerStrongAttack, PlayerStatus::AttackType::AbilityWaveWildDance_3, 2.0f); });
	m_Mesh.AddEvent(PlayerAnim::MotionAbilityWaveWildDance_3, 40.0f, [=] { gsPlaySE(Player_Attack2_SE); });
	//牙突
	m_Mesh.AddEvent(PlayerAnim::MotionAbilitySlashAttack, 30.0f, [=] { ChangeVelocity(Transform().forward().normalized() * 0.28f); });
	m_Mesh.AddEvent(PlayerAnim::MotionAbilitySlashAttack, 35.0f, [=] { ResetVelocity(); });
	m_Mesh.AddEvent(PlayerAnim::MotionAbilitySlashAttack, 42.0f, [=] {GenerateWaveCollider(st_PlayerWaveAttack, PlayerStatus::AttackType::AbilityFangs); });
	m_Mesh.AddEvent(PlayerAnim::MotionAbilitySlashAttack, 42.0f, [=] { gsPlaySE(Player_Attack_SE); });
}