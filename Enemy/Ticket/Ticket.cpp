#include "Ticket.h"
#include "Actor/Enemy/Enemy.h"

Ticket::Ticket(IWorld* world){
	//���[���h�ݒ�
	m_World = world;
	//�^�O�ݒ�
	m_Tag = st_TicketTag;
	//���O�ݒ�
	m_Name = st_Ticket;
}

void Ticket::Update(float delta_time){
	//�G�擾
	std::vector<Actor*>enemys = m_World->FindActorWithTag(st_Enemy);
	if (enemys.empty())return;


	for (int i = 0; i < static_cast<int>(enemys.size()); ++i) {
		//�G�̐����͈͈ȏゾ������0�ɂ���
		if (static_cast<int>(enemys.size()) <= m_EnemyNum) {
			m_EnemyNum = 0;
		}
		//�U��������܂��́A���S������
		if (static_cast<Enemy*>(enemys.at(i))->IsAttacked() ||
			static_cast<Enemy*>(enemys.at(m_EnemyNum))->IsDead()) {
			
			//�U�����������S��������false�ɂ���
			static_cast<Enemy*>(enemys.at(i))->AttackingCheck(false);
			static_cast<Enemy*>(enemys.at(i))->DeadCheck(false);

			//�󂾂�����v���X
			if (enemys.empty())return;
			m_EnemyNum++;

			if (static_cast<int>(enemys.size()) <= m_EnemyNum) {
				m_EnemyNum = 0;
			}
			//�`�P�b�g�z�z�E�U���`�F�b�N��false�ɂ���
			static_cast<Enemy*>(enemys.at(m_EnemyNum))->HaveTicket(true);
		}

	}
	//�ŏ��Ƀ`�P�b�g��t�^����G
	if (m_IsGenerate)return;
	static_cast<Enemy*>(enemys.at(0))->HaveTicket(true);
	m_IsGenerate = true;
}
