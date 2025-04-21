#ifndef ENEMY_H_
#define ENEMY_H_

#include "Actor/Actor.h"
#include "Animation/AnimatedMesh.h"
#include "State/StateMachine.h"
#include "UI/Slider.h"

class Enemy : public Actor {
public:

	//�R���X�g���N�^
	Enemy() = default;
	//�f�X�g���N�^
	~Enemy() = default;
	//�X�V
	void Update(float delta_time)override;
	//�`��
	void Draw()const override;
	//GUI�`��
	void DrawGui()const;
	//���A�N�V����
	void React(Actor& other)override;
	//���b�Z�[�W
	void HandleMessage(const std::string& message, void* param)override;

public:

	// �v���C���[�̎擾	
	Actor* GetPlayer()const;

public:

	// �^�[�Q�b�g�����̊p�x�����߂�
	float TargetSignedAngle()const;
	//�^�[�Q�b�g�����̊p�x�����߂�(��������)
	float TargetSignedAngle(GSvector3 target)const;
	//�^�[�Q�b�g�����̊p�x�����߂�(�����Ȃ�)
	float TargetAngle() const;
	// �^�[�Q�b�g�̋��������߂�
	float TargetDistance()const;
	//�^�[�Q�b�g�̋��������߂�
	float TargetDistance(GSvector3 target) const;
	//�ړ����x�擾
	float Speed()const;

public:

	//�̗͎擾
	int GetHP()const;
	//�ő�HP�擾
	int GetMaxHP()const;
	//�X�^���|�C���g�擾
	int GetSP()const;
	//�ő�X�^���|�C���g�擾
	int GetMaxSP()const;

public:

	//�A�j���[�V�����^�C�}�\�擾
	bool GetAnimationTimer(float num, float num2 = 0.0f)const;
	//���[�V�����I����
	bool IsMotionEnd(float subtract = 1.0f)const;
	//�v���C���[�������
	bool IsPlayer()const;
	//�`�P�b�g�������Ă��邩
	bool IsTicket()const;
	//�A�^�b�N���邩
	bool IsAttack()const;
	//�A�^�b�N������
	bool IsAttacked()const;
	//���S������
	bool IsDead()const;
	//�X�^��������
	bool IsStan()const;
	//�U���C���^�[�o������
	bool IsInterval()const;
	//�_���[�W���󂯂���
	bool IsDamage()const;
	//�U�����邩(�{�X)
	bool CanAttack(const float can_attack_distance)const;
	//�U�����邩(�U�R�G)
	bool CanSoldierAttack(const float can_attack_distance, const float can_attack_distance2 = 0.0f)const;
	//�J��������
	bool IsInErea()const;

public:

	//���[�V�����ύX
	void ChangeMotion(GSuint motion, bool loop, float speed = 1.0f);
	//�ړ��ʉ��Z
	void ChangeVelocity(GSvector3 vec);
	//�󒆂ɍs��
	void Velocity_Y(float fly_velocity);
	//�ړ��ʏ�����
	void ResetVelocity();
	//�U���C���^�[�o���̐ݒ�
	void ResetInterval(float interval_time = 0.0f);
	//�`�P�b�g������
	void HaveTicket(bool have);
	//�U�����n�߂邩
	void AttackStart(bool finish);
	//�U�������Ă��邩
	void AttackingCheck(bool attacking);
	//���S������
	void DeadCheck(bool died);
	//�X�^��������
	void StanCheck(bool stan);
	//�d�͎g�����g��Ȃ���
	void ToUseOrNotUseGravity(bool gravity);
	//�_���[�W��Ԃ̃��Z�b�g
	void ResetDamageCondition();
	//�U���C���^�[�o���̌��Z
	void DecrementInterval(float delta_time);
	//���S�����ۂɏ����Ă���
	void ExtinctionDie(float delta_time);
	//�o������
	void GenerateTimer(float delta_time);
	//�����J���[�̐ݒ�
	void DrawHitColorTime(float delta_time);
	//�q�b�g�X�g�b�v�̏ڍ�
	void HitStop(float time, float scale);
	//�����J���[
	void DrawRimColor()const;
	//�q�b�g���̕`��
	void DrawHitRimColor()const;
	//�U�����ł���Ȃ�J���[��ς���
	void AttackRimColor()const;
	//�q�b�g�X�g�b�v����
	void DecentHitStop();
	//�����Ă���
	void DeleteTransparency()const;
	//�o�����̔������f�B�U
	void GenerateTransparency()const;
	//�W���X�g������̃X���[
	void DecentSlowDown();
	//�_���[�W�`��
	void DrawDamage(const GSvector3& pos, const int damage, const float timer = 30.0f);
	//�^�[�Q�b�g�摜�`��
	void DrawTarget();
	//�t�B�[���h�Ƃ̏Փ˔���
	void ColliderField();
	//�A�N�^�[�Ƃ̏Փ˔���
	void ColliderActor(Actor& other);
	//�G���ʂ̍X�V����
	void EnemyUpdate(float delta_time);
	//���b�V���`��
	void MeshDraw()const;

public:

	//�U������̐���
	void GenerateAttackCollider(
		std::string name,			//�U���^�O
		int damage,					//�_���[�W��
		float distance = 1.5f,		//�ʒu
		float life_span = 10.0f,	//����
		float radius = 1.0f,		//���a
		float height = 1.0f			//����
		);	

protected:
	//�A�j���[�V�������b�V��
	AnimatedMesh m_Mesh{ NULL,NULL,NULL };
	//�v���C���[
	Actor* m_Player{ nullptr };
	//HP�Q�[�W
	Slider m_HPSlider{ GSvector2::zero(),GSvector2::one(),0,1 };
	//�X�^���Q�[�W
	Slider m_SPSlider{ GSvector2::zero(),GSvector2::one(),0,1 };
	//�U�����󂯂���
	bool m_IsDamage{ false };
	//�d�͂������邩
	bool m_UseGravity{ true };
	//�o������
	bool m_IsGenerate{ false };
	//�q�b�g�X�g�b�v
	bool m_IsHitStop{ false };
	//�`�P�b�g����
	bool m_IsTicket{ false };
	//�U�����邩
	bool m_IsAttacking{ false };
	//�U��������
	bool m_IsAttacked{ false };
	//���S���邩
	bool m_IsDead{ false };
	//�X�^��������
	bool m_IsStan{ false };
	//�U���̃C���^�[�o��
	float m_AttackInterval{ 0.0f };
	//�x������{��
	float m_SlowMagnification{ 1.0f };
	//�ړ����x
	float m_Speed{ 0.01f };
	//���Ŏ���
	float m_ExtinctionTimer{ 5.0f };
	//�q�b�g�X�g�b�v�^�C�}�[
	float m_HitStopTimer{ 0.0f };
	// �t���b�V������
	float m_FlashTime{ 20.0f };
	// �t���b�V���p�̃^�C�}
	float m_FlashTimer{ m_FlashTime };
	//���ɖ߂�܂�
	float m_ColorTimer{ 0.0f };
	//HP
	int m_Health{ 10 };
	//�ő�HP
	int m_MaxHealth{ 10 };
	//SP
	int m_StanPoint{ 0 };
	//�ő�SP
	int m_MaxStanPoint{ 10 };
};

#endif