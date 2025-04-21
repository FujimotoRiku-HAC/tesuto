#ifndef PLAYER_H_
#define PLAYER_H_

#include "Actor/Actor.h"
#include "Animation/AnimatedMesh.h"
#include "State/StateMachine.h"
#include "Actor/Player/PlayerAnim.h" 
#include "Actor/Player/Ability/PlayerAbility.h"
#include "Actor/Player/Item/ItemData.h"
#include "Actor/Player/PlayerStatus.h"
#include "UI/Slider.h"
#include "Skill/SkillData.h"

//�v���[���N���X
class Player : public Actor {
public:

	//�R���X�g���N�^
	Player(IWorld* world, const GSvector3& position);
	//�f�X�g���N�^
	~Player();
	//�X�V
	virtual void Update(float delta_time)override;
	//�`��
	virtual void Draw() const override;
	//GUI�`��
	virtual void DrawGui() const override;
	//�Փ˃��A�N�V����
	virtual void React(Actor& other)override;
	//���b�Z�[�W
	virtual void HandleMessage(const std::string& message, void* param)override;

public:

	//�X�e�[�^�X�N���X�̎擾
	PlayerStatus& Status()const;
	//���͂ɉ����Ĉړ��ʂ�ς���
	GSvector3 InputVelocity(GSvector2 input);
	//�G�̐������߂�
	Actor* FindTargetEnemy()const;
	//���͂ɉ����ĉ�]������
	void Interpolation(GSvector3 velocity, float delta_time);
	//�������̌���
	void DirectionOfAvoidance(GSvector2 input);
	//�A�j���[�V�����C�x���g�ݒ�
	void SetAnimationEvent();
	// �ړ��ʂ̕ύX
	void ChangeVelocity(const GSvector3& vec);
	// �ړ��ʂ̏�����
	void ResetVelocity();
	//���[�V�����ύX
	void ChangeMotion(GSuint motion, bool loop, float speed = 1.0f);
	//�X�e�[�g�ύX
	void ChangeState(int state, bool restate = false);
	//�U���ɑJ�ڂ���
	void SetNextAttackStateFrag(bool next);
	//�W���X�g�U��
	void AvoidanceFrag(bool state);
	//�W���X�g�U��
	void JustAttackFrag(bool state);
	// �A�C�e���Ŏg���G�t�F�N�g�̃Z�b�g
	void SetEffect(GSuint effect, bool continuation = false);
	// �G�t�F�N�g�̒�~
	void StopEffect();

public:

	//�X�s�[�h�ݒ�
	float PlayerSpeed()const;
	//�^�[�Q�b�g�̋���
	float TargetDistance(Actor* other)const;

public:

	//���[�V�������I�����Ă��邩
	bool IsMotionEnd(float time = 1.f)const;
	//���̃X�e�[�g�ɑJ�ڂ��邩
	bool CanAttackNextState()const;
	//�Z���N�g�V�[����
	bool IsSelect()const;
	//�{�X��
	bool BossFight()const;
	//�A�r���e�B���ɍU��������������
	bool IsAbilityReact()const;
	//�W���X�g�U��������
	bool IsJustAttack()const;
	//�W���X�g�U��������
	bool IsAvoidance()const;
	//���[�V��������
	bool IsMotionTime(float num, float num2 = 0.f)const;

private:

	//���݂̃X�e�[�g
	void CurrentState(int s);
	//�q�b�g�X�g�b�v���邩
	void DecentHitStop();
	//�G�S�̂̓�����x������
	void HitStop(float time, float scale);
	//�t�B�[���h�Ƃ̏Փˏ���
	void CollideField();
	//�A�N�^�[�Ƃ̏Փˏ���
	void CollideActor(Actor& other);
	//�_���[�W���[�V�����̌���
	void DecentHitMotion(Actor& other);
	//�W���X�g������̍X�V�{��
	void SlowMagnification(float scale);
	//�W���X�g������s
	void JustAvoid();
	//�X�e�[�g�ǉ�
	void AddState()override;
	//�����J���[
	void RimColor(GScolor color,float exponent)const;
	//����`��
	void WeponDraw()const;

private:

	//�^�[�Q�b�g�̊p�x�������߂�
	float TargetSignedAngle()const;
	//�����Ȃ�
	float TargetAngle()const;

private:

	//������Ԃ�
	bool IsInState(int state)const;
	//�m�b�N�o�b�N��
	bool IsKnockBack(Actor& other)const;

private:
		//�U������̐���
		void GenerateAttackCollider(
			std::string name,					//�U���^�O
			PlayerStatus::AttackType damage,	//�_���[�W��
			float distance = 1.0f,				//�ʒu
			float rightdis = 0.0f,				//���ɏo���ʒu
			float radius = 1.0f,				//���a
			float height = 1.0f,				//����
			float life_span = 10.0f);			//����
		//�Ռ��g�U���̐���
		void GenerateWaveCollider(
			std::string name,
			PlayerStatus::AttackType damage,
			float distance = 1.0f,
			float radius= 0.2f,
			float height = 1.0f);

private:

	//�A�j���[�V�������b�V��
	AnimatedMesh m_Mesh{ NULL,NULL,NULL };
	//�X�e�[�g�}�V���C���X�^���X
	StateMachine<Player> m_StateMachine;
	//�K�E�Z
	PlayerAbility m_Ability;
	//�X�L��
	SkillData m_Skill;
	//�A�C�e��
	ItemData m_Item;
	//�X�e�[�^�X
	PlayerStatus m_Status;
	//�A�j���[�V�����擾
	PlayerAnim m_Anim;
	//HP�o�[�擾
	Slider* m_HPSlider;
	//�G�l�~�[�擾
	Actor* m_Enemy{ nullptr };
	// ���[�V����
	GSuint m_Motion;
	// �G�t�F�N�g�n���h��
	GSint m_EffectHandle;
	//���G����
	float m_InvincibleTime{ 0.0f };
	//�������
	float m_AvoidDistance;
	//�q�b�g�X�g�b�v�^�C�}�[
	float m_HitStopTimer{ 0.0f };
	//�X���[�ɂ���{��
	float m_SlowMagnification{ 1.0f };
	// �t���b�V������
	float m_FlashTime{ 60.0f };
	// �t���b�V���p�̃^�C�}
	float m_FlashTimer{ m_FlashTime };
	//�󂯂��_���[�W
	int m_DamageValue{ 0 };
	//�G�̔ԍ�
	int m_TargetEnemyNum{ 0 };
	//���̍U��
	bool m_NextAttackStateFrag{ false };
	//�q�b�g�X�g�b�v���邩
	bool m_IsHitStop{ false };
	//���G��
	bool m_IsInvincible{ false };
	//���������
	bool m_IsAvoidance{ false };
	//����X�L�����g���Ă��邩
	bool m_IsHitAvoidCollider{ false };
	//����X���[��Ԃɂ���
	bool m_IsJustAvoid{ false };
	//�p���B�X���[��Ԃɂ���
	bool m_IsJustAttack{ false };
	//�Z���N�g�V�[����
	bool m_IsSelect{ false };
	//�U�����󂯂���
	bool m_IsReact{ false };


};

#endif
