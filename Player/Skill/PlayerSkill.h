#ifndef PLAYER_SKILL_H_
#define PLAYER_SKILL_H_

class Player;

class PlayerSkill {
public:
	//スキル使用
	void UseSkill(Player* player, int skill);
};

#endif