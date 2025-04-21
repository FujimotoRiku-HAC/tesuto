#include "Enemy.h"
#include "Field/Field.h"
#include "Math/Line.h"
#include "Collision/BasicEnemyAttackCollider.h"
#include "Number/DamageNumber.h"
#include "Actor/Player/Player.h"
#include "Camera/Target.h"

#include <GSmathf.h>

void Enemy::Update(float delta_time){}

void Enemy::Draw() const{}

void Enemy::DrawGui() const{}

void Enemy::React(Actor& other){}

void Enemy::HandleMessage(const std::string& message, void* param){}

Actor* Enemy::GetPlayer() const{
	return m_Player;
}

float Enemy::TargetSignedAngle() const{
	//ターゲットがいなければ0を返す
	if (m_Player == nullptr)return 0.f;
	//ターゲット方向のベクトルを求める
	GSvector3 to_target = m_Player->Transform().position() - m_Transform.position();
	//前向き方向のベクトルを取得
	GSvector3 forward = m_Transform.forward();
	//ベクトルのy成分を無効にする
	forward.y = 0.f;
	to_target.y = 0.f;
	//前向き方向のベクトルとターゲット方向のベクトルの角度差を求める
	return GSvector3::signedAngle(forward, to_target);
}

float Enemy::TargetSignedAngle(GSvector3 target) const{
	//ターゲット方向のベクトルを求める
	GSvector3 to_target = target - m_Transform.position();
	//前向き方向のベクトルを取得
	GSvector3 forward = m_Transform.forward();
	//ベクトルのy成分を無効に知る
	forward.y = 0.f;
	to_target.y = 0.f;
	//前向き方向にベクトルとターゲット方向のベクトルの角度差を求める
	return GSvector3::signedAngle(forward, to_target);
}

float Enemy::TargetAngle() const{
	return std::abs(TargetSignedAngle());
}

float Enemy::TargetDistance() const{
	//ターゲットがいなければ最大距離を返す
	if (m_Player == nullptr)return FLT_MAX;
	//ターゲットとの距離を計算する
	return GSvector3::distance(m_Player->Transform().position(), m_Transform.position());
}

float Enemy::TargetDistance(GSvector3 target) const{
	return GSvector3::distance(target, m_Transform.position());
}

bool Enemy::GetAnimationTimer(float num, float num2) const{
	if (m_Mesh.MotionTime() > num)return false;
	if (m_Mesh.MotionTime() < num2)return false;
	return true;
}

float Enemy::Speed() const {
	return m_Speed;
}

int Enemy::GetHP() const{
	return m_Health;
}

int Enemy::GetMaxHP() const{
	return m_MaxHealth;
}

int Enemy::GetSP() const{
	return m_StanPoint;
}

int Enemy::GetMaxSP() const{
	return m_MaxStanPoint;
}

bool Enemy::IsMotionEnd(float subtract) const {
	return m_Mesh.MotionTime() >= m_Mesh.MotionEndTime() - subtract;
}

bool Enemy::IsTicket()const {
	return m_IsTicket;
}

bool Enemy::IsAttack() const {
	return m_IsAttacking;
}

bool Enemy::IsAttacked() const {
	return m_IsAttacked;
}

bool Enemy::IsDead() const {
	return m_IsDead;
}

bool Enemy::IsStan() const {
	return m_IsStan;
}

bool Enemy::IsInterval() const {
	return m_AttackInterval <= 0.f;
}

bool Enemy::IsDamage() const {
	return m_IsDamage;
}

bool Enemy::IsPlayer() const {
	return (m_World->FindActor(st_Player) != nullptr);
}

bool Enemy::CanAttack(const float can_attack_distance)const {
	if (!IsInErea())return false;
	if (!IsInterval())return false;
	if (TargetDistance() > can_attack_distance)return false;
	return true;
}

bool Enemy::CanSoldierAttack(const float can_attack_distance, const float can_attack_distance2)const {
	if (!IsInErea())return false;
	if (!IsTicket())return false;
	if (!IsInterval())return false;
	if (TargetDistance() > can_attack_distance)return false;
	if (TargetAngle() > TargerAngleValue)return false;
	return true;
}

bool Enemy::IsInErea()const {
	GSfrustum camera_area;
	GSmatrix4 model, view, proj, model_view;
	GSvector3 center = Collider().m_Center;
	//視錐台の計算・作成
	gsMatrix4GetModelViewProjectionFromOpenGL(&model, &view, &proj, &model_view);
	gsFrustumFromMatrices(&camera_area, &view, &proj);
	//カメラ内か
	return gsFrustumIsSphereInside(&camera_area, &center, Collider().m_Radius);
}

void Enemy::ChangeMotion(GSuint motion, bool loop, float speed) {
	m_Mesh.ChangeMotion(motion, loop, speed);
}

void Enemy::ChangeVelocity(GSvector3 vec){
	m_Velocity = vec;
}

void Enemy::Velocity_Y(float fly_velocity){
	m_Velocity.y = fly_velocity;
}

void Enemy::ResetVelocity(){
	m_Velocity = m_Velocity.zero();
}

void Enemy::ResetInterval(float interval_time){
	m_AttackInterval = interval_time;
}

void Enemy::HaveTicket(bool have) {
	m_IsTicket = have;
}

void Enemy::AttackStart(bool finish) {
	m_IsAttacking = finish;
}

void Enemy::AttackingCheck(bool attacking) {
	m_IsAttacked = attacking;
}

void Enemy::DeadCheck(bool died) {
	m_IsDead = died;
}

void Enemy::StanCheck(bool stan) {
	m_IsStan = stan;
}

void Enemy::ToUseOrNotUseGravity(bool gravity) {
	m_UseGravity = gravity;
}

void Enemy::ResetDamageCondition() {
	m_IsDamage = false;
}

void Enemy::DecrementInterval(float delta_time) {
	m_AttackInterval -= delta_time;
}

void Enemy::ExtinctionDie(float delta_time) {

	//消滅していく時間
	m_ExtinctionTimer = std::max(m_ExtinctionTimer - ExtictionTime * delta_time, 0.0f);

	if (m_ExtinctionTimer <= 0.f) Die();
}

void Enemy::GenerateTimer(float delta_time) {

	if (m_IsGenerate) {
		//透明から普通になるまで
		m_ColorTimer = MIN(m_ColorTimer + GenerateTime * delta_time, 1.0f);
	}
}

void Enemy::DrawHitColorTime(float delta_time) {
	if (m_IsDamage) {
		//フラッシュカラーの減算
		m_FlashTimer = std::max(m_FlashTimer - delta_time, 0.f);
		
		if (m_FlashTimer <= 0.f) {
			m_IsDamage = false;
			m_FlashTimer = m_FlashTime;
		}
	}
}

void Enemy::HitStop(float time, float scale) {
	//タイマーの決定
	if (m_HitStopTimer <= 0.f)m_HitStopTimer = time;
	//倍率の決定
	m_World->PlaySlowTimer(scale);
	m_HitStopTimer--;

	if (m_HitStopTimer <= 0.f) {
		m_World->DefaultTime();
		m_IsHitStop = false;
	}
}

void Enemy::DrawRimColor() const{
	//ヒット時光らせる
	DrawHitRimColor();
	//攻撃する対象を光らせる
	AttackRimColor();
	//死亡したら消滅
	DeleteTransparency();
	//出現時のカラー
	GenerateTransparency();
}

void Enemy::DrawHitRimColor() const {
	if (!m_IsDamage)return;
	// フラッシュカラーの補間を行う(gsEasingInBounceで点滅させる）
	GScolor black{ 0.f, 0.f, 0.f, 1.f };

	// 補間値(0.0～1.0)を求める
	float t = GSmathf::inverseLerp(0.f, m_FlashTime, m_FlashTimer);
	GScolor bounce_color = GScolor::lerp(black, m_FlashColor, gsEasingInBounce(t));
	//前後に揺らす
	float elastic = GSmathf::lerpUnclamped(0.0f, ElasticValue, gsEasingInBounce(t));

	//前後に揺らす
	glTranslatef(0.f, 0.f, elastic);
	// 加算カラーの設定
	glSecondaryColor3fv(bounce_color);
	// 乗算カラーの設定
	glColor4fv(MultiplicationColor);
}

void Enemy::AttackRimColor()const {
	if (!IsAttack())return;

	//攻撃時にリムカラーを変える
	gsSetRimLightColor(&AttackColor);
	gsSetRimLightExponent(AttackExponent);
}

void Enemy::DecentHitStop() {
	if (!m_IsHitStop)return;
	HitStop(HitStopTime, 0.f);
}

void Enemy::DeleteTransparency() const {
	if (!m_IsDead)return;
	gsSetDitheredTransparency(m_ExtinctionTimer);
}

void Enemy::GenerateTransparency() const {
	if (!m_IsGenerate)return;
	gsSetDitheredTransparency(m_ColorTimer);
}

void Enemy::DecentSlowDown() {
	if (m_Player == nullptr)return;
	//ジャスト回避かジャスト攻撃出来たらスローにする
	if (static_cast<Player*>(m_World->FindActor(st_Player))->IsJustAttack() ||
		(static_cast<Player*>(m_World->FindActor(st_Player))->IsAvoidance())) {
		m_SlowMagnification = SlowMagnification;
	}
	else if (!static_cast<Player*>(m_World->FindActor(st_Player))->IsAvoidance()) {
		m_SlowMagnification = 1.f;
	}
}

void Enemy::DrawDamage(const GSvector3& pos, const int damage, const float timer) {
	m_World->AddNumber(new DamageNumber{ pos,damage,timer });
}

void Enemy::DrawTarget() {
	auto target = static_cast<Target*>(m_World->FindActor(st_Target));
	target->TargetDraw(this);
}

void Enemy::GenerateAttackCollider(std::string name, int damage, float distance, float life_span, float radius, float height) {
	//衝突判定を出現させる座標を求める(前方の位置)
	GSvector3 position = m_Transform.position() + m_Transform.forward() * distance;
	//高さ補正
	position.y += height;
	//弾作成
	BoundingSphere collider{ radius,position };
	//攻撃時の衝突判定を出現させる
	m_World->AddActor(new BasicEnemyAttackCollider{ m_World,collider,
		name,st_EnemyAttackTag,m_Tag,damage,life_span , AttackDelay });
}

void Enemy::ColliderField() {
	//壁との衝突判定(球体との判定)
	GSvector3 center;
	if (m_World->GetField()->Collide(Collider(), &center)) {
		//y座標は変更しない
		center.y = m_Transform.position().y;
		//補正後の座標に変更する
		m_Transform.position(center);
	}

	GSvector3 position = m_Transform.position();
	GSvector3 intersect;
	Line line;
	line.start = position + m_Collider.m_Center;
	line.end = position + GSvector3{ 0.f,FootOffset,0.f };

	if (m_World->GetField()->Collide(line, &intersect)) {
		//交差した点からy座標のみ補正する
		position.y = intersect.y;
		//座標を変更する
		m_Transform.position(position);
		//重力を初期化する
		m_Velocity.y = 0.f;
	}
}

void Enemy::ColliderActor(Actor& other) {
	//y座標を除く座標を求める
	GSvector3 position = m_Transform.position();
	position.y = 0.0f;
	GSvector3 target = other.Transform().position();
	target.y = 0.0f;
	//相手との距離
	float distance = GSvector3::distance(position, target);
	//衝突判定級の半径同士を加えた長さを求める
	float length = m_Collider.m_Radius + other.Collider().m_Radius;
	//衝突判定球の重なっている長さを求める
	float overlap = length - distance;
	//重なっている部分の半分の距離だけ離れる移動量を求める
	GSvector3 v = (position - target).getNormalized() * overlap * OverlapValue;
	m_Transform.translate(v, GStransform::Space::World);
	//フィールドとの衝突判定
	ColliderField();
}

void Enemy::EnemyUpdate(float delta_time) {
	//プレイヤー検索
	m_Player = m_World->FindActor(st_Player);
	//重力更新
	if (m_UseGravity)m_Velocity.y += Gravity * delta_time;
	//重力を加える
	m_Transform.translate(m_Velocity * delta_time, GStransform::Space::World);

	//フィールドとの衝突判定
	ColliderField();
	//ヒット時のカラー変更
	DrawHitColorTime(delta_time);
	//ジャスト回避時のスロー
	DecentSlowDown();
	//ヒットストップ
	DecentHitStop();
}

void Enemy::MeshDraw() const{
	//ディザ半透明の設定取得
	float transparency = gsGetDitheredTransparency();
	// 現在の乗算カラーを取得（退避しておく）
	GScolor current_color;
	glGetFloatv(GL_CURRENT_COLOR, current_color);
	// 現在の加算カラーの取得（退避しておく）
	GScolor current_secondary_color;
	glGetFloatv(GL_CURRENT_SECONDARY_COLOR, current_secondary_color);
	// リムライトの設定
	float rim = gsGetRimLightExponent();
	//リムカラー描画
	DrawRimColor();
	//メッシュ描画
	m_Mesh.Draw();
	//ディザを復帰
	gsSetDitheredTransparency(transparency);
	//リムカラー復帰
	gsSetRimLightExponent(rim);
	gsSetRimLightColor(&current_secondary_color);
	// 乗算カラーを復帰する
	glColor4fv(current_color);
	// 加算カラーを復帰する
	glSecondaryColor3fv(current_secondary_color);
}
