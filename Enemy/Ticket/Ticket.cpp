#include "Ticket.h"
#include "Actor/Enemy/Enemy.h"

Ticket::Ticket(IWorld* world){
	//ワールド設定
	m_World = world;
	//タグ設定
	m_Tag = st_TicketTag;
	//名前設定
	m_Name = st_Ticket;
}

void Ticket::Update(float delta_time){
	//敵取得
	std::vector<Actor*>enemys = m_World->FindActorWithTag(st_Enemy);
	if (enemys.empty())return;


	for (int i = 0; i < static_cast<int>(enemys.size()); ++i) {
		//敵の数が範囲以上だったら0にする
		if (static_cast<int>(enemys.size()) <= m_EnemyNum) {
			m_EnemyNum = 0;
		}
		//攻撃をするまたは、死亡したら
		if (static_cast<Enemy*>(enemys.at(i))->IsAttacked() ||
			static_cast<Enemy*>(enemys.at(m_EnemyNum))->IsDead()) {
			
			//攻撃したか死亡したかをfalseにする
			static_cast<Enemy*>(enemys.at(i))->AttackingCheck(false);
			static_cast<Enemy*>(enemys.at(i))->DeadCheck(false);

			//空だったらプラス
			if (enemys.empty())return;
			m_EnemyNum++;

			if (static_cast<int>(enemys.size()) <= m_EnemyNum) {
				m_EnemyNum = 0;
			}
			//チケット配布・攻撃チェックをfalseにする
			static_cast<Enemy*>(enemys.at(m_EnemyNum))->HaveTicket(true);
		}

	}
	//最初にチケットを付与する敵
	if (m_IsGenerate)return;
	static_cast<Enemy*>(enemys.at(0))->HaveTicket(true);
	m_IsGenerate = true;
}
