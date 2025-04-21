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
	//�^�[�Q�b�g�����Ȃ����0��Ԃ�
	if (m_Player == nullptr)return 0.f;
	//�^�[�Q�b�g�����̃x�N�g�������߂�
	GSvector3 to_target = m_Player->Transform().position() - m_Transform.position();
	//�O���������̃x�N�g�����擾
	GSvector3 forward = m_Transform.forward();
	//�x�N�g����y�����𖳌��ɂ���
	forward.y = 0.f;
	to_target.y = 0.f;
	//�O���������̃x�N�g���ƃ^�[�Q�b�g�����̃x�N�g���̊p�x�������߂�
	return GSvector3::signedAngle(forward, to_target);
}

float Enemy::TargetSignedAngle(GSvector3 target) const{
	//�^�[�Q�b�g�����̃x�N�g�������߂�
	GSvector3 to_target = target - m_Transform.position();
	//�O���������̃x�N�g�����擾
	GSvector3 forward = m_Transform.forward();
	//�x�N�g����y�����𖳌��ɒm��
	forward.y = 0.f;
	to_target.y = 0.f;
	//�O���������Ƀx�N�g���ƃ^�[�Q�b�g�����̃x�N�g���̊p�x�������߂�
	return GSvector3::signedAngle(forward, to_target);
}

float Enemy::TargetAngle() const{
	return std::abs(TargetSignedAngle());
}

float Enemy::TargetDistance() const{
	//�^�[�Q�b�g�����Ȃ���΍ő勗����Ԃ�
	if (m_Player == nullptr)return FLT_MAX;
	//�^�[�Q�b�g�Ƃ̋������v�Z����
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
	//������̌v�Z�E�쐬
	gsMatrix4GetModelViewProjectionFromOpenGL(&model, &view, &proj, &model_view);
	gsFrustumFromMatrices(&camera_area, &view, &proj);
	//�J��������
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

	//���ł��Ă�������
	m_ExtinctionTimer = std::max(m_ExtinctionTimer - ExtictionTime * delta_time, 0.0f);

	if (m_ExtinctionTimer <= 0.f) Die();
}

void Enemy::GenerateTimer(float delta_time) {

	if (m_IsGenerate) {
		//�������畁�ʂɂȂ�܂�
		m_ColorTimer = MIN(m_ColorTimer + GenerateTime * delta_time, 1.0f);
	}
}

void Enemy::DrawHitColorTime(float delta_time) {
	if (m_IsDamage) {
		//�t���b�V���J���[�̌��Z
		m_FlashTimer = std::max(m_FlashTimer - delta_time, 0.f);
		
		if (m_FlashTimer <= 0.f) {
			m_IsDamage = false;
			m_FlashTimer = m_FlashTime;
		}
	}
}

void Enemy::HitStop(float time, float scale) {
	//�^�C�}�[�̌���
	if (m_HitStopTimer <= 0.f)m_HitStopTimer = time;
	//�{���̌���
	m_World->PlaySlowTimer(scale);
	m_HitStopTimer--;

	if (m_HitStopTimer <= 0.f) {
		m_World->DefaultTime();
		m_IsHitStop = false;
	}
}

void Enemy::DrawRimColor() const{
	//�q�b�g�����点��
	DrawHitRimColor();
	//�U������Ώۂ����点��
	AttackRimColor();
	//���S���������
	DeleteTransparency();
	//�o�����̃J���[
	GenerateTransparency();
}

void Enemy::DrawHitRimColor() const {
	if (!m_IsDamage)return;
	// �t���b�V���J���[�̕�Ԃ��s��(gsEasingInBounce�œ_�ł�����j
	GScolor black{ 0.f, 0.f, 0.f, 1.f };

	// ��Ԓl(0.0�`1.0)�����߂�
	float t = GSmathf::inverseLerp(0.f, m_FlashTime, m_FlashTimer);
	GScolor bounce_color = GScolor::lerp(black, m_FlashColor, gsEasingInBounce(t));
	//�O��ɗh�炷
	float elastic = GSmathf::lerpUnclamped(0.0f, ElasticValue, gsEasingInBounce(t));

	//�O��ɗh�炷
	glTranslatef(0.f, 0.f, elastic);
	// ���Z�J���[�̐ݒ�
	glSecondaryColor3fv(bounce_color);
	// ��Z�J���[�̐ݒ�
	glColor4fv(MultiplicationColor);
}

void Enemy::AttackRimColor()const {
	if (!IsAttack())return;

	//�U�����Ƀ����J���[��ς���
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
	//�W���X�g������W���X�g�U���o������X���[�ɂ���
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
	//�Փ˔�����o����������W�����߂�(�O���̈ʒu)
	GSvector3 position = m_Transform.position() + m_Transform.forward() * distance;
	//�����␳
	position.y += height;
	//�e�쐬
	BoundingSphere collider{ radius,position };
	//�U�����̏Փ˔�����o��������
	m_World->AddActor(new BasicEnemyAttackCollider{ m_World,collider,
		name,st_EnemyAttackTag,m_Tag,damage,life_span , AttackDelay });
}

void Enemy::ColliderField() {
	//�ǂƂ̏Փ˔���(���̂Ƃ̔���)
	GSvector3 center;
	if (m_World->GetField()->Collide(Collider(), &center)) {
		//y���W�͕ύX���Ȃ�
		center.y = m_Transform.position().y;
		//�␳��̍��W�ɕύX����
		m_Transform.position(center);
	}

	GSvector3 position = m_Transform.position();
	GSvector3 intersect;
	Line line;
	line.start = position + m_Collider.m_Center;
	line.end = position + GSvector3{ 0.f,FootOffset,0.f };

	if (m_World->GetField()->Collide(line, &intersect)) {
		//���������_����y���W�̂ݕ␳����
		position.y = intersect.y;
		//���W��ύX����
		m_Transform.position(position);
		//�d�͂�����������
		m_Velocity.y = 0.f;
	}
}

void Enemy::ColliderActor(Actor& other) {
	//y���W���������W�����߂�
	GSvector3 position = m_Transform.position();
	position.y = 0.0f;
	GSvector3 target = other.Transform().position();
	target.y = 0.0f;
	//����Ƃ̋���
	float distance = GSvector3::distance(position, target);
	//�Փ˔��苉�̔��a���m�����������������߂�
	float length = m_Collider.m_Radius + other.Collider().m_Radius;
	//�Փ˔��苅�̏d�Ȃ��Ă��钷�������߂�
	float overlap = length - distance;
	//�d�Ȃ��Ă��镔���̔����̋������������ړ��ʂ����߂�
	GSvector3 v = (position - target).getNormalized() * overlap * OverlapValue;
	m_Transform.translate(v, GStransform::Space::World);
	//�t�B�[���h�Ƃ̏Փ˔���
	ColliderField();
}

void Enemy::EnemyUpdate(float delta_time) {
	//�v���C���[����
	m_Player = m_World->FindActor(st_Player);
	//�d�͍X�V
	if (m_UseGravity)m_Velocity.y += Gravity * delta_time;
	//�d�͂�������
	m_Transform.translate(m_Velocity * delta_time, GStransform::Space::World);

	//�t�B�[���h�Ƃ̏Փ˔���
	ColliderField();
	//�q�b�g���̃J���[�ύX
	DrawHitColorTime(delta_time);
	//�W���X�g������̃X���[
	DecentSlowDown();
	//�q�b�g�X�g�b�v
	DecentHitStop();
}

void Enemy::MeshDraw() const{
	//�f�B�U�������̐ݒ�擾
	float transparency = gsGetDitheredTransparency();
	// ���݂̏�Z�J���[���擾�i�ޔ����Ă����j
	GScolor current_color;
	glGetFloatv(GL_CURRENT_COLOR, current_color);
	// ���݂̉��Z�J���[�̎擾�i�ޔ����Ă����j
	GScolor current_secondary_color;
	glGetFloatv(GL_CURRENT_SECONDARY_COLOR, current_secondary_color);
	// �������C�g�̐ݒ�
	float rim = gsGetRimLightExponent();
	//�����J���[�`��
	DrawRimColor();
	//���b�V���`��
	m_Mesh.Draw();
	//�f�B�U�𕜋A
	gsSetDitheredTransparency(transparency);
	//�����J���[���A
	gsSetRimLightExponent(rim);
	gsSetRimLightColor(&current_secondary_color);
	// ��Z�J���[�𕜋A����
	glColor4fv(current_color);
	// ���Z�J���[�𕜋A����
	glSecondaryColor3fv(current_secondary_color);
}
