#include "StoneSoldier.h"
#include "Collision/AttackCollider.h"
#include "StoneSoldierState/StoneSoldierOwnedState.h"

StoneSoldier::StoneSoldier(IWorld* world, const GSvector3& position,const GSvector3& rotation) {
	//���[���h�ݒ�
	m_World = world;
	//�^�O���ݒ�
	m_Tag = st_Enemy;
	//���O�ݒ�
	m_Name = st_Soldier;
	//�Փ˔��苅�̐ݒ�
	m_Collider = BoundingSphere{ SoldierRadius,GSvector3{0.f,SoldierHeight,0.f} };
	//���b�V���ݒ�
	m_Mesh = AnimatedMesh{ Mesh_StoneSoldier,StoneSoldierSpawn,false };
	//���W�̏�����
	m_Transform.position(position);
	//��]������
	m_Transform.rotate(rotation);
	//���[���h�ϊ��s��̏�����
	m_Mesh.Transform(m_Transform.localToWorldMatrix());
	//�̗͐ݒ�
	m_Health = SoldierHealth;
	//�X�s�[�h�ݒ�
	m_Speed = SoldierRunSpeed;
	//�̗̓o�[�ݒ�
	m_HPSlider = Slider(m_Transform.position(), GSvector2{ m_Health * SoldierHPValue.x,SoldierHPValue.y }, 0, m_Health, EnemyHPColor);
	//�v���C���[����
	m_Player = m_World->FindActor(st_Player);
	//�A�j���[�V�����C�x���g�ǉ�
	SetAnimationEvent();
	//�X�e�[�g�ǉ�
	AddState();
	//�X�e�[�g�ݒ�
	m_StateMachine.SetCurrentState(StoneSoldierOwnedState::Spawn);
}

void StoneSoldier::Update(float delta_time) {
	//���ʍX�V
	EnemyUpdate(delta_time);
	//���b�V�����X�V
	m_Mesh.Update(delta_time * m_SlowMagnification);
	//�s���ݒ�
	m_Mesh.Transform(m_Transform.localToWorldMatrix());
	m_StateMachine.Update(delta_time * m_SlowMagnification);
	//�̗͂̐ݒ�E�l
	GSvector3 pos = m_Transform.position() + SoldierOffset;
	m_HPSlider.SetPos(pos);
	m_HPSlider.SetValue(m_Health);
}

void StoneSoldier::Draw() const {
	//�`��
	MeshDraw();
}

void StoneSoldier::DrawGui() const{
	//�J���������̗͂������
	if (m_Health > 0.f && IsInErea()) m_HPSlider.Draw();
}

//�Փˏ���
void StoneSoldier::React(Actor& other) {
	//�U��������������
	if (other.Tag() == st_PlayerAttackTag) {
		gsPlaySE(React_SE);

		//�ړ���~
		ResetVelocity();
		//�G�t�F�N�g����
		m_IsDamage = true;
		//���b�Z�[�W����
		m_Player->HandleMessage("Hit", nullptr);

		//�_���[�W�ʂ̎擾
		int damage = static_cast<AttackCollider*>(&other)->DamageValue();
		//�_���[�W�`��̈ʒu
		GSvector3 pos = m_Transform.position();
		pos.y = 1.0f;
		GSvector3 intersect = other.Collider().m_Center * m_Transform.worldToLocalMatrix();
		//�_���[�W�`��
		DrawDamage(m_Transform.localToWorldMatrix().transform(intersect), damage);

		//�̗͌���
		m_Health -= damage;
		//�U�����󂯂���
		m_IsAttacking = false;

	    if (m_Health <= 0) {
			//���S���[�V�����ǉ�����
			gsPlaySE(Enemy_Finish_SE);
			m_EnableCollider = false;
			ChangeState(StoneSoldierOwnedState::BeforeDie);
			return;
		}
		//���U����ł��ꂽ��
		if (other.Name() == st_PlayerStrongAttack) {
			m_IsStrongHit = true;
		}
		//������΂��U��������������
		if (other.Name() == st_PlayerBlowAttack) {
			m_IsBlowHit = true;
		}
		//�󒆍U��������������
		if (other.Name() == st_PlayerPushUpAttack) {
			m_IsPush = true;
			ToUseOrNotUseGravity(false);
		}
		//�U���ɂ����
		DecentHitMotion(other);
	}
}

void StoneSoldier::HandleMessage(const std::string& message, void* param) {
	//�q�b�g�X�g�b�v
	if (message == st_Message_Hit)m_IsHitStop = true;
}

void StoneSoldier::ChangeState(int state, bool restate) {
	m_StateMachine.Change_State(state, restate);
}

void StoneSoldier::DecentHitMotion(Actor& other){
	//�R���C�_�\�̌v�Z
	GSvector3 intersect = (Collider().m_Center + other.Collider().m_Center) * (Collider().m_Radius / (Collider().m_Radius + other.Collider().m_Radius));
	GSvector3 vec = (m_Transform.position() - intersect).getNormalized();
	vec.y = 0.f;
	GSvector3 f = m_Transform.forward();
	f.y = 0.f;
	float angle = GSvector3::signedAngle(vec, f);

	//�m�b�N�o�b�N�ł͂Ȃ�������
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
	//���U���ɓ���������
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
	//�u���E�U���ɓ���������
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
	//�v�b�V���U���ɓ���������
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
	//0�x�ȏゾ������E����
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
	//�����_���̍U��
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

//�X�e�[�g�ǉ�
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
	//�ߋ����U��
	m_Mesh.AddEvent(StoneSoldierMotion::StoneSoldierPeripheralAttack, 25.0f, [=] { gsPlaySE(Soldier_Attack_SE); });
	m_Mesh.AddEvent(StoneSoldierMotion::StoneSoldierPeripheralAttack, 20.0f, [=] { GenerateAttackCollider(st_EnemyAttack, SoldierNormalDamage); });
	m_Mesh.AddEvent(StoneSoldierMotion::StoneSoldierPeripheralAttack, 60.0f, [=] { gsPlaySE(Soldier_Attack_SE); });
	m_Mesh.AddEvent(StoneSoldierMotion::StoneSoldierPeripheralAttack, 65.0f, [=] { GenerateAttackCollider(st_EnemyAttack, SoldierNormalDamage); });
	//�Ƃт�����U�����[�V����
	m_Mesh.AddEvent(StoneSoldierMotion::StoneSoldierPounceAttack, 30.0f, [=] {GenerateAttackCollider(st_EnemyStrongAttack, SoldierStrongDamage,3.2f); });
	m_Mesh.AddEvent(StoneSoldierMotion::StoneSoldierPounceAttack, 30.0f, [=] {gsPlaySE(Soldier_Attack2_SE); });

}
