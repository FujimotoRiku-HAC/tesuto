#ifndef PLAYER_ANIM_H_
#define PLAYER_ANIM_H_

//���[�V�����ԍ�
enum TitlePlayerAnim {
	MotionTitle = 0
};

enum PlayerAnim {
	//�R���{3�U�� 0.1.2  ALL 3
	//�R���{4�U�� 4.5.6.7  ALL 8
	//�R���{7�U�� 9.10.11.12.13.14.15 ALL 16
	// �_�b�V���U��40 41
	//���S 17 21 32 84 �N���オ�� 18,29
	//�U���q�b�g 19 24 25 34 36 37 38 39 82 87 
	// �|���22 32 ���[�����O 42 43 44 45 �A�C�h�� 131 46 59
	//�^�[���@47 50 48 49 51 52 53 54 55 56 70 71 116 117 118 119 120 121 122 123
	// 124 127 128 129 130
	//  �ʏ�57 58
	//���� 60 61 62.63 64 65 66 67 68 69 133 134 135 136 137 138 139 140 141
	// 142 143
	// ������ 132
	//������� 80 85 89 �|��Ă��烍�[�����O 81 83 86 88
	//�K�[�h 90 92 93 94 95 ALL91
	//Dash  96 99 ��e���̃_�b�V��97 98 101 �~�܂� 100
	// �|��Ă���U�� 115
	//Skill 102 104 105 106 107 110 111 112 114 125 126
	MotionPlayerIdle = 46,						//�A�C�h��
	MotionFwdWalk = 65,							//�O����
	MotionFirstAttack = 0,						//�ꌂ��
	MotionSecondAttack = 1,						//�񌂖�
	MotionThirdAttack = 2,						//�O����
	MotionHitForward = 34,						//�O�q�b�g
	MotionHitBackward = 19,						//���q�b�g
	MotionHitLeft = 36,							//���q�b�g
	MotionHitRight = 38,						//�E�q�b�g
	MotionStrongHit = 85,						//���U���q�b�g
	MotionGetUp = 29,							//�N���オ��
	MotionDie = 21,								//���S
	MotionDieLoop = 22,							//���S���[�v
	MotionRightAvoidance = 45,					//�E���
	MotionLeftAvoidance = 44,					//�����
	MotionForwardAvoidance = 43,				//�O���
	MotionBackAvoidance = 42,					//�����
	MotionJustAvoidAttack = 8,					//�J�E���^�[
	MotionAbilityBlowAttack = 102,				//�u���[�U��
	MotionAbilityBlowAttackReinforcement = 41,	//�u���[�U��2����
	MotionAbilityPushAttack = 104,				//�v�b�V���U��
	MotionAbilityAvoidAttack = 105,				//����U��
	MotionAbilityRotatingAttack = 106,			//��]�a��
	MotionAbilityInpactAttack = 107,			//�C���p�N�g�U��
	MotionAbilitySlamAttack = 110,				//�@�����U��
	MotionAbilityWaveWildDance_1 = 111,			//�g���U��
	MotionAbilityWaveWildDance_2 = 112,			//�g���U��2����
	MotionAbilityWaveWildDance_3 = 114,			//�g���U��3����
	MotionAbilityWildDanceAttack = 16,			//����
	MotionAbilitySlashAttack = 40,				//���
};

#endif