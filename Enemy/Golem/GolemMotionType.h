#ifndef GOLEM_MOTION_TYPE_H_
#define GOLEM_MOTION_TYPE_H_

enum GolemMotion {
	MotionGolemIdle = 7,						//�A�C�h��
	MotionGolemForwardWalk = 4,					//�O����
	MotionGolemLeftWalk = 9,					//������
	MotionGolemRightWalk = 11,					//�E����
	MotionGolemBackwardWalk = 0,				//������
	MotionGolemJump = 8,						//�W�����v
	MotionGolemTripleAttack = 13,				//3�A�U��
	MotionGolemForAttack = 18,					//���ߍU��
	MotionGolemJumpingAttack = 14,				//�W�����v�U��
	MotionGolemWildDanceRotatingAttack = 15,	//������]�a��
	MotionGolemChargeSpinAttack = 17,			//���߉�]�a��
	MotionGolemChargeDanceAttack = 19,			//���ߗ���
	MotionGolemPushAttack = 12,					//������΂��U��
	MotionGolemGenerate = 10,					//�o�����
	MotionGolemStanStart = 3,					//�X�^���X�^�[�g
	MotionGolemStanLoop = 2,					//�X�^�����[�v
	MotionGolemStanEnd = 1,						//�X�^���G���h
	MotionGolemDie = 5,							//���S
};

#endif