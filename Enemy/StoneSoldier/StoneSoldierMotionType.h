#ifndef STONE_SOLDIER_MOTION_TYPE_H_
#define STONE_SOLDIER_MOTION_TYPE_H_

enum StoneSoldierMotion {
	StoneSoldierSpawn = 27,					//�X�|�[��
	StoneSoldierIdle = 29,					//�A�C�h��
	StoneSoldierForwardWalk = 32,			//�O����
	StoneSoldierBackwardWalk = 31,			//������
	StoneSoldierBeforePounceAttack = 6,		//�Ƃт�����U���O
	StoneSoldierBeforePeripheralAttack = 5,	//���ӍU���O
	StoneSoldierPounceAttack = 0,			//�Ƃт�����U��
	StoneSoldierPeripheralAttack = 2,		//���ӍU��
	StoneSoldierFrontHit = 20,				//�ʏ�q�b�g
	StoneSoldierFrontStrongHit = 28,		//���U���q�b�g
	StoneSoldierFrontBlowHit = 16,			//�u���[�q�b�g
	StoneSoldierFrontPushUpHit = 12,		//�v�b�V���q�b�g
	StoneSoldierBackHit = 19,				//���ʏ�q�b�g
	StoneSoldierBackStrongHit = 26,			//��닭�U���q�b�g
	StoneSoldierBackBlowHit = 18,			//���u���[�q�b�g
	StoneSoldierBackPushUpHit = 10,			//���v�b�V���U��
	StoneSoldierGetUp = 21,					//�N���オ��
	StoneSoldierGetUpAfterPushUp = 35,		//�v�b�V���U����̋N���オ��
	StoneSoldierLeftThink = 34,				//������
	StoneSoldierRightThink = 33,			//�E����
	StoneSoldierBeforeDie = 36,				//���S�O
	StoneSoldierDie = 1,					//���S
};

#endif