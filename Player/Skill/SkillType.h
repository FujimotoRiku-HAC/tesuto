#ifndef SKILL_TYPE_H_
#define SKILL_TYPE_H_

enum SkillType {
	NoneSkill,
	HealthUpAndHealSkill,							//�������{��
	AttackPowerUpSkill,								//�U����1.5�{�㏸
	AttackPowerLergeUpSkill,						//�U����2.0�{�㏸
	HealthHealSeedUpSkill,							//�̗͉񕜗ʏ㏸
	AvoidPerformanceUpSkill,						//��𐫔\�㏸
	AvoidDistanceUpSkill,							//��������㏸
	JustAvoidSlowSkill,								//�W���X�g���⏕
	DivineProtectionSkill,							//����X�L��
	FullChargeSkill,								//�t���`���[�W
	CrisisChargeSkill,								//�Ύ���
	SuperArmorSkill,								//�̊�����
	TotalSkillNum
};

#endif