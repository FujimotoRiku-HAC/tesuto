#ifndef ACTOR_MANAGER_H_
#define ACTOR_MANAGER_H_

class Actor;

//�A�N�^�[�}�l�[�W���[
class ActorManager {
public:

	//�f�t�H���g�R���X�g���N�^
	ActorManager() = default;
	//���z�f�X�g���N�^
	virtual ~ActorManager();

public:

	//�A�N�^�[�̒ǉ�
	void Add(Actor* actor);
	//�A�N�^�[�̍X�V
	void Update(float delta_time);
	//�A�N�^�[�̒x���X�V
	void LateUpdate(float delta_time);
	//�A�N�^�[�̕`��
	void Draw() const;
	//�������A�N�^�[�̕`��
	void DrawTransparent() const;
	//�A�N�^�[��GUI�`��
	void DrawGui() const;
	//�A�N�^�[�̏Փ˔���
	void Collide();
	//���S���Ă���A�N�^�[�̍폜
	void Remove();
	//���b�Z�[�W�̑��M
	void OnMessage(const std::string& message, void* param);
	//����
	void Clear();

public:

	//�A�N�^�[�̌���
	Actor* Find(const std::string& name) const;
	//�w�肵���^�O�������A�N�^�[�̌���
	std::vector<Actor*>FindWithTag(const std::string& tag) const;

public:

	//�A�N�^�[����Ԃ�
	int Count() const;
	//�w�肵���^�O�������A�N�^�[����Ԃ�
	int CountWithTag(const std::string& tag) const;

	//�R�s�[�֎~
	ActorManager(const ActorManager& other) = default;
	ActorManager& operator = (const ActorManager& other) = default;

protected:
	//�A�N�^�[���X�g
	std::list<Actor*>m_Actors;
};

#endif
