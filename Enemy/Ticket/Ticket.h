#ifndef TICKET_H_
#define TICKET_H_

#include "Actor/Actor.h"

class Ticket : public Actor{
public:

	//�R���X�g���N�^
	Ticket(IWorld* world);
	//�X�V
	virtual void Update(float delta_time)override;

private:
	int m_EnemyNum = 0;				//���݂̓G
	bool m_IsGenerate{ false };		//�ŏ��ɕt�^����
	bool m_IsDead{ false };			//���S������
};

#endif