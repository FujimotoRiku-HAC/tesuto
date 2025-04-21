#include "Golem.h"
#include "Collision/AttackCollider.h"
#include "GolemState/GolemOwnedState.h"

Golem::Golem(IWorld* world, const GSvector3& position, const GSvector3& rotation) {
	//���[���h�ݒ�
	m_World = world;
	//�^�O���ݒ�
	m_Tag = st_Enemy;
	//���O�ݒ�
	m_Name = st_Golem;
	//�Փ˔��苅�̐ݒ�
	m_Collider = BoundingSphere{ GolemRadius,GSvector3{0.f,GolemHeight,0.f} };
	//���b�V���ݒ�
	m_Mesh = AnimatedMesh{ Mesh_Golem,NULL,true };
	//���W�̏�����
	m_Transform.position(position);
	//��]������
	m_Transform.rotate(rotation);
	//���[���h�ϊ��s��̏�����
	m_Mesh.Transform(m_Transform.localToWorldMatrix());
	//�̗͐ݒ�
	m_MaxHealth = GolemHealth;
	m_Health = m_MaxHealth;
	//�X�^���Q�[�W�ݒ�
	m_MaxStanPoint = GolemStanPoint;
	m_StanPoint = m_MaxStanPoint;
	//�X�s�[�h�ݒ�
	m_Speed = GolemRunSpeed;
	//�o���t���O�𗧂Ă�
	m_IsGenerate = true;
	//�̗̓o�[�ݒ�
	m_HPSlider = Slider(HPSliderPos, HPSliderScale, 0, m_Health, HPSliderColor);
	//�X�^���|�C���g�o�[�ݒ�
	m_SPSlider = Slider(SPSliderPos, SPSliderScale, 0, m_StanPoint, SPSliderColor);
	//�v���C���[����
	m_Player = m_World->FindActor(st_Player);
	//�C�x���g�ǉ�
	SetAnimationEvent();
	//�X�e�[�g�ǉ�
	AddState();
	//�������[�V�����ݒ�
	m_StateMachine.SetCurrentState(GolemOwnedState::Generate);
}

void Golem::Update(float delta_time) {
	//���ʃA�b�v�f�[�g
	EnemyUpdate(delta_time);
	//�X�e�[�g�}�V���X�V
	m_StateMachine.Update(delta_time * m_SlowMagnification);
	//���b�V�����X�V
	m_Mesh.Update(delta_time * GolemAnimSpeed * m_SlowMagnification);
	//���[�g���[�V�����K�p
	m_Mesh.ApplyRootMotion(m_Transform);
	//�s���ݒ�
	m_Mesh.Transform(m_Transform.localToWorldMatrix());
	//�o���^�C�}�[�̍X�V
	GenerateTimer(delta_time);
	//HP�ݒ�
	m_HPSlider.SetValue(m_Health);
	//�X�^���|�C���g�ݒ�
	m_SPSlider.SetValue(m_StanPoint);

	//�X�^���ł���Ȃ�
	if (m_StanPoint <= 0.f && !m_IsStan) {
		gsPlaySE(Enemy_Finish_SE);
		ChangeState(GolemOwnedState::StanStart, false);
		return;
	}
}

void Golem::Draw() const {
	//�`��
	MeshDraw();
}

void Golem::DrawGui() const {
	m_HPSlider.Draw();	//HP�`��
	m_SPSlider.Draw();	//SP�`��
}

//�Փˏ���
void Golem::React(Actor& other) {
	//�v���C���[�U���^�O�ɓ���������
	if (other.Tag() == st_PlayerAttackTag) {
		GSvector3 intersect = other.Collider().m_Center * m_Transform.worldToLocalMatrix();
		gsPlaySE(React_SE);
		//�ړ���~
		ResetVelocity();
		//�G�t�F�N�g����
		m_IsDamage = true;
		//�v���C���[�Ƀ��b�Z�[�W���M
		m_Player->HandleMessage(st_Message_Hit, nullptr);
		//�_���[�W�ʂ̎擾
		int damage = static_cast<AttackCollider*>(&other)->DamageValue();
		int stan_damage = static_cast<AttackCollider*>(&other)->StanPointValue();
		//�_���[�W�`��
		DrawDamage(m_Transform.localToWorldMatrix().transform(intersect), damage);

		//�̗͌���
		m_Health -= damage;
		if (m_Health <= 0) {
			m_EnableCollider = false;
			gsPlaySE(Enemy_Finish_SE);
			ChangeState(GolemOwnedState::Die, false);
			return;
		}
		//Sp���Z
		m_StanPoint -= stan_damage;

		//3��U�����󂯂Ă���W�����v
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
	//0�x�ȏゾ������E�ɓ���
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
	//�����_���U��
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

	//�W�����v
	m_Mesh.AddEvent(GolemMotion::MotionGolemJump, 10.0f, [=] {ToUseOrNotUseGravity(false); });
	m_Mesh.AddEvent(GolemMotion::MotionGolemJump, 10.0f, [=] { m_Velocity.y += 0.19f; });
	m_Mesh.AddEvent(GolemMotion::MotionGolemJump, 15.0f, [=] { m_Velocity = m_Transform.forward().normalized() * 0.35f; });
	m_Mesh.AddEvent(GolemMotion::MotionGolemJump, 30.0f, [=] {ToUseOrNotUseGravity(true); });
	m_Mesh.AddEvent(GolemMotion::MotionGolemJump, 35.0f, [=] { ResetVelocity(); });
	//��]���Ĕ��ł���U��
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
	//�W�����v�U��
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
	//��]�a��
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
	//���ߗ���
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
	//�v�b�V���U��
	m_Mesh.AddEvent(GolemMotion::MotionGolemPushAttack, 20.0f, [=] { GenerateAttackCollider(st_EnemyAttack, GolemNormalDamage,2.5f); });
	m_Mesh.AddEvent(GolemMotion::MotionGolemPushAttack, 20.0f, [=] { gsPlaySE(Golem_Attack2_SE); });
	m_Mesh.AddEvent(GolemMotion::MotionGolemPushAttack, 45.0f, [=] { GenerateAttackCollider(st_EnemyAttack, GolemNormalDamage,2.5f); });
	m_Mesh.AddEvent(GolemMotion::MotionGolemPushAttack, 45.0f, [=] { gsPlaySE(Golem_Attack2_SE); });
	m_Mesh.AddEvent(GolemMotion::MotionGolemPushAttack, 70.0f, [=] { GenerateAttackCollider(st_EnemyAttack, GolemNormalDamage,2.5f); });
	m_Mesh.AddEvent(GolemMotion::MotionGolemPushAttack, 70.0f, [=] { gsPlaySE(Golem_Attack2_SE); });
	m_Mesh.AddEvent(GolemMotion::MotionGolemPushAttack, 90.0f, [=] { GenerateAttackCollider(st_EnemyStrongAttack, GolemStrongDamage,2.5f); });
	m_Mesh.AddEvent(GolemMotion::MotionGolemPushAttack, 90.0f, [=] { gsPlaySE(Golem_Attack2_SE); });
	//�R�A�U��
	m_Mesh.AddEvent(GolemMotion::MotionGolemTripleAttack, 20.0f, [=] { GenerateAttackCollider(st_EnemyAttack, GolemNormalDamage,2.5f); });
	m_Mesh.AddEvent(GolemMotion::MotionGolemTripleAttack, 20.0f, [=] { gsPlaySE(Golem_Attack3_SE); });
	m_Mesh.AddEvent(GolemMotion::MotionGolemTripleAttack, 35.0f, [=] { GenerateAttackCollider(st_EnemyAttack, GolemNormalDamage); });
	m_Mesh.AddEvent(GolemMotion::MotionGolemTripleAttack, 35.0f, [=] { gsPlaySE(Golem_Attack3_SE); });
	m_Mesh.AddEvent(GolemMotion::MotionGolemTripleAttack, 70.0f, [=] { GenerateAttackCollider(st_EnemyStrongAttack, GolemStrongDamage,2.5f); });
	m_Mesh.AddEvent(GolemMotion::MotionGolemTripleAttack, 70.0f, [=] { gsPlaySE(Golem_Attack4_SE); });
	//���ߍU��
	m_Mesh.AddEvent(GolemMotion::MotionGolemForAttack, 55.0f, [=] { GenerateAttackCollider(st_EnemyStrongAttack, GolemStrongDamage,2.5f); });
	m_Mesh.AddEvent(GolemMotion::MotionGolemForAttack, 55.0f, [=] { gsPlaySE(Golem_Attack4_SE); });
	//
	m_Mesh.AddEvent(GolemMotion::MotionGolemDie, 105.0f, [=] { m_World->SetClearFrag(); });
}
