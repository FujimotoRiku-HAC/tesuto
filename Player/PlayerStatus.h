#ifndef PLAYER_STATUS_H_
#define PLAYER_STATUS_H_

class PlayerStatus {
public:
	enum AttackType {
		NormalAttack_1,					//��U��
		NormalAttack_2,					//�񌂖�
		NormalAttack_3,					//�O����
		CounterAttack,					//�J�E���^�[
		AbilityBlowAttack,				//�u���[�U��
		AbilityBlowAttackReinforcement,	//�u���[�U��2����
		AbilityPushUpAttack,			//�v�b�V���U��
		AbilityAvoidAttack,				//����U��
		AbilityRotatingAttack,			//��]�a��
		AbilityInpactAttack,			//�C���p�N�g�U��
		AbilityInpactAttack2,			//�C���p�N�g�񌂖�
		AbilitySlamAttack,				//�X�����U��
		AbilitySlamAttack2,				//�X�����U���񌂖�
		AbilityWildDance_1,				//�����P
		AbilityWildDance_2,				//�����Q
		AbilityWildDance_3,				//�����R
		AbilityWildDance_4,				//�����S
		AbilityWildDance_5,				//�����T
		AbilityWildDance_6,				//�����U
		AbilityWildDance_7,				//�����V
		AbilityWaveWildDance_1,			//�g������
		AbilityWaveWildDance_2,			//�g�������Q
		AbilityWaveWildDance_3,			//�g�������R
		AbilityFangs					//���
	};
public:
	PlayerStatus();
	//�X�V
	void Update(float delta_time);
	//�_���[�W�ݒ�
	void HitDamage(int damage);
	//�̗͂̉�
	void HealHealth(float heal_health);
	// �ő�̗͂̒ǉ�(��Z)
	void AddMaxHealth(float magnification);
	//�̗͉񕜗ʂ̒ǉ�
	void AddHealHealthPower(float magnification);
	//�U���͂̒ǉ�(���Z������)
	void AddAttackPower(float adition);
	//�U���͂ɂ�����{���̒ǉ�(��Z������)
	void AddDamageIncreaseMultiplier(float magnification);
	//�K�[�h���
	void AddDamageGuardIncreaseMultiplier(float magnification);
	//���G���Ԃ̒ǉ�(���Z)
	void AddInvicincibleTime(float adition);
	//��������̒ǉ�(��Z)
	void AddAvoidDistance(float magnification);
	//�ړ����x�̒ǉ�(��Z)
	void SetMoveSpeed(float magnification);
	//�t���`���[�W��
	void SetFullCharge();
	//�Ύ��ꂩ
	void SetCrisisCharge();
	//�X�[�p�[�A�[�}�[��
	void SetSuperArmor();
	//����X�L�����g����
	void SetDivineProtection();
	//���⏕���邩
	void SetAvoidAuxiliary();
	
	//�U���̓|�[�V�����̍U���͔{���̐ݒ�
	void RateAttackPortion(float magnification);
	//�U���̓|�[�V�����̌��ʂ��؂ꂽ
	void EffectWearsAttackPortion();
	//�U���̓A�b�v���g����
	void UseAttackPowerUp(bool use);

	//
	void UseHealPortion(bool use);
	//�e�U���̍U���{��
	void SetAttackRate();

public:
	//���񂾂�
	bool IsDead()const;
	//�w�肵���̗͂��c���Ă��邩
	bool IsRemaingValueHealth(float ratio)const;
	//�X�[�p�[�A�[�}�[�g���Ă��邩
	bool IsSuperArmar()const;
	//���⏕���g���Ă��邩
	bool IsAvoidAuxiliary()const;
	//�񕜃|�[�V�������g������
	bool IsHealPortion()const;
	//�U���̓A�b�v���g������
	bool IsIsAttackPowerupPortion()const;
public:
	//���݂̗͎̑擾
	int Health()const;
	//�ő�̗͂̎擾
	int MaxHealth()const;
	//�U���͂̎擾
	int AttackPower(AttackType type)const;
	//�X�^���ʂ̎擾
	int StanPower(AttackType type)const;
	//�ړ����x
	float MoveSpeed()const;
	//��������擾
	float AvoidDistance()const;
	//���G���Ԏ擾
	float InvincibleTime()const;

private:

	int   m_Health;
	int   m_MaxHealth;
	int   m_HealthHealPower;
	float m_DamageIncreaseMultiplier = 1.0f;
	float m_DamageGuardIncreaseMultiplier = 1.0f;

	float m_InvincibleTime;
	float m_AvoidDistance;
	float m_BasicAttackPower;
	float m_BasicStanPower;
	float m_MoveSpeed;
	float m_FullChargePower;
	float m_CrisisChargePower;
	float m_ItemPowerUp;
	bool  m_AvoidAuxiliary = false;
	bool  m_Burning = false;
	bool  m_IsFullCharge = false;
	bool  m_IsCrisisCharge = false;
	bool  m_IsSuperArmor = false;
	bool  m_IsDivineProtection = false;
	bool m_HealPortion = false;
	bool m_PowerUpPortion = false;
	std::unordered_map<AttackType, float> m_AttackRate;
};

#endif