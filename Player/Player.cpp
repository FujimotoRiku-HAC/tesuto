#include "Player.h"
#include "Field/Field.h"
#include "Math/Line.h"
#include "Collision/PlayerAttackCollider/BasicPlayerAttackCollider.h"
#include "Collision/PlayerAttackCollider/WaveAttackCollider.h"
#include "Camera/DefalutCamera.h"
#include "Actor/Player/PlayerState/PlayerOwnedState.h"

//�R���X�g���N�^
Player::Player(IWorld* world, const GSvector3& position){
	//���[���h�ݒ�
	m_World = world;
	//�^�O��
	m_Tag = st_PlayerTag;
	//���O
	m_Name = st_Player;
	//���b�V���쐬
	m_Mesh = { Mesh_Player,MotionPlayerIdle,true };
	//�Փ˔��苉�̐ݒ�
	m_Collider = BoundingSphere{ PlayerRadius,GSvector3{0.f,PlayerHeight,0.f} };
	//���W�̏�����
	m_Transform.position(position);
	//���b�V���̕ϊ��s���������
	m_Mesh.Transform(m_Transform.localToWorldMatrix());
	//�K�E�Z�f�[�^�̃��[�h
	m_Ability.AbilityLoad("Data/ability_data.txt");
	//�X�L�����[�h
	m_Skill.SkillLoad("Data/skill_data.txt");
	//�X�L���Z�b�g
	m_Skill.Excute(this);
	//�X�e�[�g�ǉ�
	AddState();
	//�A�j���[�V�����C�x���g
	SetAnimationEvent();
	//�����X�e�[�g�ݒ�
	CurrentState(PlayerOwnedState::Idle);
	//���G���Ԏ擾
	m_InvincibleTime = m_Status.InvincibleTime();
	//��������擾
	m_AvoidDistance = m_Status.AvoidDistance();
	//�U���͏�����
	m_Status.SetAttackRate();
	//HP�X���C�_�[
	m_HPSlider = new Slider(PlayerHPSliderPos, GSvector2{ m_Status.MaxHealth() * PlayerHPSliderScale.x,PlayerHPSliderScale.y }, 0, m_Status.Health(), PlayerHPSliderColor);
}

Player::~Player() {
	delete m_HPSlider;
}

//�X�V
void Player::Update(float delta_time){
	//�G�l�~�[�擾
	m_Enemy = m_World->FindActor(st_Enemy);
	//�d�͂ŉ������ɉ���
	m_Velocity.y += Gravity * delta_time;
	// velocity�̕������ړ�����
	m_Transform.translate(m_Velocity * delta_time, GStransform::Space::World);
	//�t�B�[���h�Ƃ̏Փ˔���
	CollideField();
	//�X�e�[�g�}�V���̍X�V
	m_StateMachine.Update(delta_time* m_SlowMagnification);
	//�Z���N�g�V�[���ł͓����Ȃ�
	if (m_IsSelect) m_Velocity = GSvector3::zero();
	//�X�e�[�^�X�X�V
	m_Status.Update(delta_time);
	m_HPSlider->SetValue(m_Status.Health());
	//�A�r���e�B�X�V
	m_Ability.Update(m_World, delta_time);
	//�A�C�e���X�V
	m_Item.Update(m_World, delta_time);
	//���b�V���X�V
	m_Mesh.Update(delta_time * m_SlowMagnification);
	//�s��`��
	m_Mesh.Transform(m_Transform.localToWorldMatrix());
	//�W���X�g���
	JustAvoid();
	//�q�b�g�X�g�b�v
	DecentHitStop();

	// �t���b�V�����Ԃ����Z�b�g����
	if (m_IsHitAvoidCollider|| Status().IsHealPortion() || Status().IsIsAttackPowerupPortion()) {

		if (m_FlashTimer <= 0.f) {
			m_FlashTimer = m_FlashTime;
			Status().UseHealPortion(false);
			return;
		}

		m_FlashTimer = std::max(m_FlashTimer - delta_time, 0.f);
	}
}

//�`��
void Player::Draw() const{

	// �������C�g�̐ݒ�
	float transparency = gsGetRimLightExponent();
	GScolor current_secondary_color;
	//�ʏ�J���[
	gsSetRimLightExponent(usually_exponent);
	gsSetRimLightColor(&usually_color);
	if (!m_IsSelect) {
		//�I�[�g�X���[��Ԃ�������
		if (m_IsHitAvoidCollider)RimColor(just_avoidance_color, avoid_exponent);
		//�q�[���|�[�V���������񂾂�
		if (Status().IsHealPortion())RimColor(heal_color, heal_exponent);
		//�U���㏸�|�[�V���������񂾂�
		if (Status().IsIsAttackPowerupPortion())RimColor(power_color, power_exponent);
	}
	//���b�V���`��
	m_Mesh.Draw();
	//�ʏ�J���[
	gsSetRimLightExponent(transparency);
	gsSetRimLightColor(&current_secondary_color);
	//����`��
	WeponDraw();
}

void Player::DrawGui() const{
	if (m_IsSelect)return;
	m_Skill.Draw();
	m_Item.Draw();
	m_Ability.Draw();
	m_HPSlider->Draw();
	//������̃{�^���`��
	if (m_IsJustAvoid)gsDrawSprite2D(X_Button_Texture, &XButtonPos, NULL, NULL, NULL, &XButtonSc, NULL);
}

//�Փ˃��A�N�V����
void Player::React(Actor& other){

	//��𔻒�ɓ���������
	if (other.Tag() == st_AvoidTag) {
		//���⏕�X�L�����g���Ă�����
		if (m_Status.IsAvoidAuxiliary()) {
			static_cast<DefalutCamera*>(m_World->GetCamera())->ShakeOff();
			m_IsHitAvoidCollider = true;
			m_IsJustAvoid = true;
			return;
		}
		//��𒆂�������
		if (IsInState(PlayerOwnedState::AvoidanceBack) || IsInState(PlayerOwnedState::AvoidanceForward)
		|| IsInState(PlayerOwnedState::AvoidanceLeft) || IsInState(PlayerOwnedState::AvoidanceRight)) {
			m_IsJustAvoid = true;
			return;
		}
	}
	//�G�ƏՓ˂�����
	if (other.Tag() == st_Enemy) {
		//�A�N�^�[���m���d�Ȃ�Ȃ��悤�ɕ␳
		CollideActor(other);
	}

	//�G�̍U������ƏՓ˂�����
	if (m_IsJustAvoid || m_IsJustAttack)return;
	if (other.Tag() == st_EnemyAttackTag) {
		//�A�r���e�B�u���E���ɍU�����ꂽ��
		if (IsInState(PlayerOwnedState::AbilityBlowAttack)) m_IsReact = true;
		else m_IsReact = false;

		//�U���͂̎擾
		m_DamageValue = static_cast<AttackCollider*>(&other)->DamageValue();
		ResetVelocity();
		//�_���[�W����
		m_Status.HitDamage(m_DamageValue);
		//SE�ǉ�
		gsPlaySE(PlayerReact_SE);
		static_cast<DefalutCamera*>(m_World->GetCamera())->Shake();
		//�G�S���Ƀ��b�Z�[�W�𑗂�
		for (auto& enemy : m_World->FindActorWithTag(st_Enemy)) {
			enemy->HandleMessage(st_Message_Hit,nullptr);
		}

		//���S��ԑJ��
		if (m_Status.IsDead()) {
			gsStopBGM();
			gsPlaySE(Enemy_Finish_SE);
			m_EnableCollider = false;
			ChangeState(PlayerOwnedState::Die, false);
			return;
		}
		//�m�b�N�o�b�N�U����H�������
		if (IsKnockBack(other) && other.Name() == st_EnemyAttack) {
			DecentHitMotion(other);
			return;
		}
		//���U���ɓ���������
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
	//�Z���N�g�V�[��
	if (message == st_Message_SelectScene) m_IsSelect = true;
	//�q�b�g�X�g�b�v�p
	if (message == st_Message_Hit)m_IsHitStop = true;
	//�^�[�Q�b�g�ύX
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
	//���͕����̈ړ��ʂ̎Z�o
	GSvector3 velocity = forward * input.y + right * input.x;
	return velocity;
}

Actor* Player::FindTargetEnemy() const{
	std::vector<Actor*>enemys = m_World->FindActorWithTag(st_Enemy);
	if (enemys.empty())return nullptr;
	//�^�[�Q�b�g�����Ȃ��Ȃ����玟��
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
	//���͂ɂ���ĉ����������߂�
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

//�t�B�[���h�Ƃ̏Փ˔���
void Player::CollideField() {
	//�ǂƂ̏Փ˔���
	GSvector3 center;
	if (m_World->GetField()->Collide(Collider(), &center)) {
		//y���W�͕ύX���Ȃ�
		center.y = m_Transform.position().y;
		//�␳��̍��W�ɕύX
		m_Transform.position(center);
	}
	//�n�ʂƂ̏Փ˔���
	GSvector3 position = m_Transform.position();
	Line line;
	line.start = position + m_Collider.m_Center;
	line.end = position + GSvector3{ 0.f,FootOffset,0.f };
	GSvector3 intersect;//�n�ʂƂ̌�_
	//
	Actor* field_actor{ nullptr };
	//�e�����Z�b�g���Ă���
	m_Transform.parent(nullptr);
	if (m_World->GetField()->Collide(line, &intersect,nullptr,&field_actor)) {
		//��_�̈ʒu����y���W�̂ݕ␳����
		position.y = intersect.y;
		//���W��ύX
		m_Transform.position(position);
		//�d�͏�����
		m_Velocity.y = 0.f;

		//�t�B�[���h�p�̃A�N�^�[�N���X�ƏՓ˂�����
		if (field_actor != nullptr) {
			//�Փ˂����t�B�[���h�p�A�N�^�[�̐e�̃g�����X�t�H�[���Ƃ��Đݒ�
			m_Transform.parent(&field_actor->Transform());
		}
	}
}

//�A�N�^�[�Ƃ̏Փˏ���
void Player::CollideActor(Actor& other) {
	//y���W���������W�����߂�
	GSvector3 position = m_Transform.position();
	position.y = 0.f;
	GSvector3 target = other.Transform().position();
	target.y = 0.f;
	//����Ƃ̋���
	float distance = GSvector3::distance(position, target);
	//�Փ˔��苉�̔��a���m�����������������߂�
	float length = m_Collider.m_Radius + other.Collider().m_Radius;
	//�Փ˔��苉�̏d�Ȃ��Ă��钷�������߂�
	float overlap = length - distance;
	//�d�Ȃ��Ă��镔���̔����̋��������ړ��ʂ����߂�
	GSvector3 v = (position - target).getNormalized() * overlap * OverlapValue;
	m_Transform.translate(v, GStransform::Space::World);

	//�t�B�[���h�Ƃ̏Փ˔���
	CollideField();
}

void Player::SetEffect(GSuint effect, bool continuation) {
	// �p�����邩
	if (continuation) {
		// ���������Ă���G�t�F�N�g���폜
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
	//�^�[�Q�b�g�����Ȃ����0��Ԃ�
	if (m_Enemy == nullptr)return 0.f;
	//�^�[�Q�b�g�����̃x�N�g�������߂�
	GSvector3 to_target = m_Enemy->Transform().position() - m_Transform.position();
	//�O���������̃x�N�g�����擾
	GSvector3 forward = m_Transform.forward();
	//�x�N�g����y�����𖳌��ɒm��
	forward.y = 0.f;
	to_target.y = 0.f;
	//�O���������Ƀx�N�g���ƃ^�[�Q�b�g�����̃x�N�g���̊p�x�������߂�
	return GSvector3::signedAngle(forward, to_target);
}

float Player::TargetAngle() const {
	return std::abs(TargetSignedAngle());
}

float Player::PlayerSpeed() const{
	return m_Status.MoveSpeed();
}

//�^�[�Q�b�g�Ƃ̋��������߂�
float Player::TargetDistance(Actor* actor) const {
	//�^�[�Q�b�g�����Ȃ���΍ő勗����Ԃ�
	if (actor == nullptr)return FLT_MAX;//float�^�̍ő�l
	//�^�[�Q�b�g�Ƃ̋������v�Z����
	return GSvector3::distance(actor->Transform().position(), m_Transform.position());
}

//���[�V�����ύX
void Player::ChangeMotion(GSuint motion, bool loop, float speed) {
	m_Mesh.ChangeMotion(motion, loop, speed);
}

//�X�e�[�g�ύX
void Player::ChangeState(int state, bool restate) {
	m_StateMachine.Change_State(state, restate);
}

//���[�V�������I�����Ă��邩
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
	//�R���C�_�\�v�Z
	GSvector3 intersect = (Collider().m_Center + other.Collider().m_Center) * (Collider().m_Radius / (Collider().m_Radius + other.Collider().m_Radius));
	GSvector3 vec = (m_Transform.position() - intersect).getNormalized();
	vec.y = 0.f;
	GSvector3 f = m_Transform.forward();
	f.y = 0.f;

	float angle = GSvector3::signedAngle(vec, f);

	//30�ȉ��Ȃ�O
	if (std::abs(angle) <= HitForwardAngle) {
		ChangeState(PlayerOwnedState::HitForward, false);
		return;
	}
	//150�ȏ�Ȃ���
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
	//�X���[�{�����|����
	m_SlowMagnification = scale;
	//�A�^�b�N�R�}���h����������
	if (InputManager::Instance().IsBottonDown(InputManager::InputType::Attack) && m_IsJustAvoid) {
		ChangeState(PlayerOwnedState::JustAvoidanceAttack);
		return;
	}
}

void Player::JustAvoid(){
	//�W���X�g���������
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
	//��Ԓl�����߂�
	float t = GSmathf::inverseLerp(0.f, m_FlashTime, m_FlashTimer);
	color = GScolor::lerp(black, color, t);

	gsSetRimLightColor(&color);
	gsSetRimLightExponent(exponent);
}

void Player::WeponDraw() const{
	glPushMatrix();
	//��̃{�[��(74�Ԗ�)�̈ʒu�Ƀ��b�V����`��
	glMultMatrixf(m_Mesh.BoneMatrices(PlayerBoneNum));
	// ���s�ړ�������
	glRotatef(RotateWeaponX, 1.f, 0.f, 0.f);
	glRotatef(RotateWeaponY, 0.f, 1.f, 0.f);
	glRotatef(RotateWeaponZ, 0.f, 0.f, 1.f);

	gsDrawMesh(Mesh_Weapon);
	glPopMatrix();
}


void Player::GenerateAttackCollider(std::string name, PlayerStatus::AttackType damage, float distance,float rightdis, float radius, float height, float life_span){
	// �Փ˔�����o����������W�����߂�i�O���̈ʒu�j
	GSvector3 position = m_Transform.position() + (m_Transform.forward() * distance) + (m_Transform.right() * rightdis);
	// �����̕␳�i��������̍����j
	position.y += height;
	// �Փ˔���p�̋����쐬
	BoundingSphere collider{ radius, position };
	// �Փ˔�����o��������
	m_World->AddActor(new BasicPlayerAttackCollider{ m_World, collider,
		name, st_PlayerAttackTag, m_Tag, m_Status.AttackPower(damage), m_Status.StanPower(damage),life_span, 0.f});
}

void Player::GenerateWaveCollider(std::string name, PlayerStatus::AttackType damage,float distance,float radius,float height){
	// �Փ˔�����o����������W�����߂�i�O���̈ʒu�j
	GSvector3 position = m_Transform.position() + m_Transform.forward() * distance;
	// �����̕␳�i��������̍����j
	position.y += height;
	// �Փ˔���p�̋����쐬
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
	//�A�r���e�B
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
	//����
	m_Mesh.AddEvent(PlayerAnim::MotionFwdWalk, 2.0f, [=] {gsPlaySE(PlayerFoot_SE); });
	m_Mesh.AddEvent(PlayerAnim::MotionFwdWalk, 29.0f, [=] {gsPlaySE(PlayerFoot_SE); });
	//�ꌂ��
	m_Mesh.AddEvent(PlayerAnim::MotionFirstAttack, 10.0f, [=] {ChangeVelocity(Transform().forward().normalized() * 0.08f); });
	m_Mesh.AddEvent(PlayerAnim::MotionFirstAttack, 15.0f, [=] { ResetVelocity(); });
	m_Mesh.AddEvent(PlayerAnim::MotionFirstAttack, 18.0f, [=] { PlayEffect(Effect_Attack, {0.28f,0.9f,0.0f}, {0.0f,0.0f,249.18f}, {1.0f,1.0f,1.0f}); });
	m_Mesh.AddEvent(PlayerAnim::MotionFirstAttack, 18.0f, [=] { gsPlaySE(Player_Attack_SE); });
	m_Mesh.AddEvent(PlayerAnim::MotionFirstAttack, 30.0f, [=] {GenerateAttackCollider(st_PlayerNormalAttack, PlayerStatus::AttackType::NormalAttack_1); });
	m_Mesh.AddEvent(PlayerAnim::MotionFirstAttack, 43.0f, [=] { SetNextAttackStateFrag(true); });
	//�񌂖�
	m_Mesh.AddEvent(PlayerAnim::MotionSecondAttack, 10.0f, [=] {ChangeVelocity(Transform().forward().normalized() * 0.08f); });
	m_Mesh.AddEvent(PlayerAnim::MotionSecondAttack, 13.0f, [=] { ResetVelocity(); });
	m_Mesh.AddEvent(PlayerAnim::MotionSecondAttack, 15.0f, [=] { PlayEffect(Effect_Attack, {0.0f,0.9f,0.0f},{0.0f,0.0f,-65.0f}, {1.0f,1.0f,1.0f}); });
	m_Mesh.AddEvent(PlayerAnim::MotionSecondAttack, 15.0f, [=] { gsPlaySE(Player_Attack2_SE); });
	m_Mesh.AddEvent(PlayerAnim::MotionSecondAttack, 15.0f, [=] {GenerateAttackCollider(st_PlayerNormalAttack, PlayerStatus::AttackType::NormalAttack_2); });
	m_Mesh.AddEvent(PlayerAnim::MotionSecondAttack, 20.0f, [=] { SetNextAttackStateFrag(true); });
	//�O����
	m_Mesh.AddEvent(PlayerAnim::MotionThirdAttack, 10.0f, [=] {ChangeVelocity(Transform().forward().normalized() * 0.08f); });
	m_Mesh.AddEvent(PlayerAnim::MotionThirdAttack, 13.0f, [=] { ResetVelocity(); });
	m_Mesh.AddEvent(PlayerAnim::MotionThirdAttack, 19.0f, [=] { PlayEffect(Effect_Attack, {0.0f,0.9f,0.0f}, {0.0f,0.0f,48.0f}, {1.0f,1.0f,1.0f}); });
	m_Mesh.AddEvent(PlayerAnim::MotionThirdAttack, 19.0f, [=] { gsPlaySE(Player_Attack2_SE); });
	m_Mesh.AddEvent(PlayerAnim::MotionThirdAttack, 19.0f, [=] {GenerateAttackCollider(st_PlayerNormalAttack, PlayerStatus::AttackType::NormalAttack_2); });
	m_Mesh.AddEvent(PlayerAnim::MotionThirdAttack, 20.0f, [=] { SetNextAttackStateFrag(true); });
	//�J�E���^�[
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
	//���(�t���[���������)
	m_Mesh.AddEvent(PlayerAnim::MotionForwardAvoidance, 1.0f, [=] { m_IsInvincible = true; });
	m_Mesh.AddEvent(PlayerAnim::MotionForwardAvoidance, 1.0f + m_InvincibleTime, [=] { m_IsInvincible = false; });
	m_Mesh.AddEvent(PlayerAnim::MotionBackAvoidance, 1.0f, [=] { m_IsInvincible = true; });
	m_Mesh.AddEvent(PlayerAnim::MotionBackAvoidance, 1.0f + m_InvincibleTime, [=] { m_IsInvincible = false; });
	m_Mesh.AddEvent(PlayerAnim::MotionRightAvoidance, 1.0f, [=] { m_IsInvincible = true; });
	m_Mesh.AddEvent(PlayerAnim::MotionRightAvoidance, 1.0f + m_InvincibleTime, [=] { m_IsInvincible = false; });
	m_Mesh.AddEvent(PlayerAnim::MotionLeftAvoidance, 1.0f, [=] { m_IsInvincible = true; });
	m_Mesh.AddEvent(PlayerAnim::MotionLeftAvoidance, 1.0f + m_InvincibleTime, [=] { m_IsInvincible = false; });
	//���S
	m_Mesh.AddEvent(PlayerAnim::MotionDieLoop, 30.0f, [=] {m_World->SetGameOverFrag(); });
	//�u���[�U��
	m_Mesh.AddEvent(PlayerAnim::MotionAbilityBlowAttack, 10.0f, [=] { ChangeVelocity(Transform().forward().normalized() * 0.08f); });
	m_Mesh.AddEvent(PlayerAnim::MotionAbilityBlowAttack, 35.0f, [=] {GenerateAttackCollider(st_PlayerStrongAttack, PlayerStatus::AttackType::AbilityBlowAttack); });
	m_Mesh.AddEvent(PlayerAnim::MotionAbilityBlowAttack, 35.0f, [=] { gsPlaySE(Player_Attack_SE); });
	m_Mesh.AddEvent(PlayerAnim::MotionAbilityBlowAttack, 40.0f, [=] { PlayEffect(Effect_BlowAttack, { 0.0f,0.9f,0.0f }, { 0.0f,0.0f,130.0f }, { 1.0f,1.0f,1.0f }); });
	m_Mesh.AddEvent(PlayerAnim::MotionAbilityBlowAttack, 50.0f, [=] { ResetVelocity(); });
	m_Mesh.AddEvent(PlayerAnim::MotionAbilityBlowAttackReinforcement, 1.0f, [=] {GenerateAttackCollider(st_PlayerBlowAttack, PlayerStatus::AttackType::AbilityBlowAttackReinforcement); });
	m_Mesh.AddEvent(PlayerAnim::MotionAbilityBlowAttackReinforcement, 10.0f, [=] {GenerateAttackCollider(st_PlayerBlowAttack, PlayerStatus::AttackType::AbilityBlowAttackReinforcement); });
	m_Mesh.AddEvent(PlayerAnim::MotionAbilityBlowAttackReinforcement, 15.0f, [=] { PlayEffect(Effect_BlowAttack, { 0.0f,0.9f,0.0f }, { 0.0f,0.0f,40.0f }, { 1.0f,1.0f,1.0f }); });
	m_Mesh.AddEvent(PlayerAnim::MotionAbilityBlowAttackReinforcement, 15.0f, [=] { gsPlaySE(Player_Attack2_SE); });
	//�v�b�V���U��
	m_Mesh.AddEvent(PlayerAnim::MotionAbilityPushAttack, 10.0f, [=] { ChangeVelocity(Transform().forward().normalized() * 0.08f); });
	m_Mesh.AddEvent(PlayerAnim::MotionAbilityPushAttack, 15.0f, [=] { ResetVelocity(); });
	m_Mesh.AddEvent(PlayerAnim::MotionAbilityPushAttack, 25.0f, [=] {GenerateAttackCollider(st_PlayerPushUpAttack, PlayerStatus::AttackType::AbilityPushUpAttack); });
	m_Mesh.AddEvent(PlayerAnim::MotionAbilityPushAttack, 30.0f, [=] { PlayEffect(Effect_PushAttack, { -0.18f,0.9f,0.0f }, { 0.0f,0.0f,243.0f }, { 1.0f,1.0f,1.0f }); });
	m_Mesh.AddEvent(PlayerAnim::MotionAbilityPushAttack, 30.0f, [=] { gsPlaySE(Player_Attack2_SE); });
	m_Mesh.AddEvent(PlayerAnim::MotionAbilityPushAttack, 50.0f, [=] {GenerateAttackCollider(st_PlayerPushUpAttack, PlayerStatus::AttackType::AbilityPushUpAttack); });
	m_Mesh.AddEvent(PlayerAnim::MotionAbilityPushAttack, 55.0f, [=] { PlayEffect(Effect_PushAttack, { 0.11f,0.9f,0.0f }, { 0.0f,0.0f,-65.0f }, { 1.0f,1.0f,1.0f }); });
	m_Mesh.AddEvent(PlayerAnim::MotionAbilityPushAttack, 55.0f, [=] { gsPlaySE(Player_Attack2_SE); });
	//����a��
	m_Mesh.AddEvent(PlayerAnim::MotionAbilityAvoidAttack, 10.0f, [=] { ChangeVelocity(Transform().forward().normalized() * 0.28f); });
	m_Mesh.AddEvent(PlayerAnim::MotionAbilityAvoidAttack, 20.0f, [=] { ResetVelocity(); });
	m_Mesh.AddEvent(PlayerAnim::MotionAbilityAvoidAttack, 23.0f, [=] { ChangeVelocity(-Transform().forward().normalized() * 0.28f); m_Velocity.y = 0.0f; });
	m_Mesh.AddEvent(PlayerAnim::MotionAbilityAvoidAttack, 30.0f, [=] { ResetVelocity(); });
	m_Mesh.AddEvent(PlayerAnim::MotionAbilityAvoidAttack, 21.0f, [=] {GenerateAttackCollider(st_PlayerStrongAttack, PlayerStatus::AttackType::AbilityAvoidAttack); });
	m_Mesh.AddEvent(PlayerAnim::MotionAbilityAvoidAttack, 21.0f, [=] { gsPlaySE(Player_Attack2_SE); });
	m_Mesh.AddEvent(PlayerAnim::MotionAbilityAvoidAttack, 26.0f, [=] { PlayEffect(Effect_AvoidAttack, { 0.0f,0.9f,0.0f }, { 0.0f,0.0f,-106.0f }, { 1.0f,1.0f,1.0f }); });
	//��]�a��
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
	//�C���p�N�g�U��
	m_Mesh.AddEvent(PlayerAnim::MotionAbilityInpactAttack, 10.0f, [=] {ChangeVelocity(Transform().forward().normalized() * 0.28f); });
	m_Mesh.AddEvent(PlayerAnim::MotionAbilityInpactAttack, 20.0f, [=] { ResetVelocity(); });
	m_Mesh.AddEvent(PlayerAnim::MotionAbilityInpactAttack, 60.0f, [=] { PlayEffect(Effect_InpactAttack, { 0.0f,0.0f,1.0f }, { 0.0f,0.0f,0.0f }, { 1.5f,1.5f,1.5f }); });
	m_Mesh.AddEvent(PlayerAnim::MotionAbilityInpactAttack, 65.0f, [=] { gsPlaySE(Player_Attack2_SE); });
	m_Mesh.AddEvent(PlayerAnim::MotionAbilityInpactAttack, 65.0f, [=] {GenerateAttackCollider(st_PlayerBlowAttack, PlayerStatus::AttackType::AbilityInpactAttack2, 1.0f,0.0f,1.5f); });
	//�@����
	m_Mesh.AddEvent(PlayerAnim::MotionAbilitySlamAttack, 20.0f, [=] { ChangeVelocity(Transform().forward().normalized() * 0.28f); });
	m_Mesh.AddEvent(PlayerAnim::MotionAbilitySlamAttack, 25.0f, [=] { ResetVelocity(); });
	m_Mesh.AddEvent(PlayerAnim::MotionAbilitySlamAttack, 26.0f, [=] {GenerateAttackCollider(st_PlayerStrongAttack, PlayerStatus::AttackType::AbilitySlamAttack, 1.0f); });
	m_Mesh.AddEvent(PlayerAnim::MotionAbilitySlamAttack, 31.0f, [=] { gsPlaySE(Player_Attack2_SE); });
	m_Mesh.AddEvent(PlayerAnim::MotionAbilitySlamAttack, 31.0f, [=] { PlayEffect(Effect_SlamAttack, { 0.21f,1.0f,0.0f }, { 0.0f,0.0f,-67.0f }, { 1.0f,1.0f,1.0f }); });
	m_Mesh.AddEvent(PlayerAnim::MotionAbilitySlamAttack, 75.0f, [=] {GenerateAttackCollider(st_PlayerStrongAttack, PlayerStatus::AttackType::AbilitySlamAttack2, 1.0f); });
	m_Mesh.AddEvent(PlayerAnim::MotionAbilitySlamAttack, 80.0f, [=] { PlayEffect(Effect_InpactAttack, { 0.0f,0.0f,1.0f }, { 0.0f,0.0f,0.0f }, { 1.0f,1.0f,1.0f }); });
	m_Mesh.AddEvent(PlayerAnim::MotionAbilitySlamAttack, 80.0f, [=] { gsPlaySE(Player_Attack2_SE); });
	//����
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
	//�g������
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
	//���
	m_Mesh.AddEvent(PlayerAnim::MotionAbilitySlashAttack, 30.0f, [=] { ChangeVelocity(Transform().forward().normalized() * 0.28f); });
	m_Mesh.AddEvent(PlayerAnim::MotionAbilitySlashAttack, 35.0f, [=] { ResetVelocity(); });
	m_Mesh.AddEvent(PlayerAnim::MotionAbilitySlashAttack, 42.0f, [=] {GenerateWaveCollider(st_PlayerWaveAttack, PlayerStatus::AttackType::AbilityFangs); });
	m_Mesh.AddEvent(PlayerAnim::MotionAbilitySlashAttack, 42.0f, [=] { gsPlaySE(Player_Attack_SE); });
}