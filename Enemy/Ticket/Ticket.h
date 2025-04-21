#ifndef TICKET_H_
#define TICKET_H_

#include "Actor/Actor.h"

class Ticket : public Actor{
public:

	//コンストラクタ
	Ticket(IWorld* world);
	//更新
	virtual void Update(float delta_time)override;

private:
	int m_EnemyNum = 0;				//現在の敵
	bool m_IsGenerate{ false };		//最初に付与する
	bool m_IsDead{ false };			//死亡したか
};

#endif