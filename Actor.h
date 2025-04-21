#ifndef ACTOR_H_
#define ACTOR_H_

#include "Collision/BoundingSphere.h"
#include "Tween/Tween.h"

//�A�N�^�[�N���X
class Actor {
public:
	//�R���X�g���N�^
	Actor() = default;
	//���z�f�X�g���N�^
	virtual ~Actor() = default;
	//�X�V
	virtual void Update(float delta_time);
	//�x���X�V
	virtual void LateUpdate(float delta_time);
	//�`��
	virtual void Draw() const;
	//�������`��
	virtual void DrawTransparent() const;
	//GUI�̕`��
	virtual void DrawGui() const;
	//�Փ˔���
	virtual void React(Actor& other);
	//���b�Z�[�W����
	virtual void HandleMessage(const std::string& message, void* param);
	//��Ԓǉ�
	virtual void AddState(){}
	//�Փ˔���
	void Collide(Actor& other);
	//���S����
	void Die();

public:

	//�Փ˂��Ă��邩
	bool IsCollide(const Actor& other)const;
	//���S���Ă��邩
	bool IsDead() const;

public:

	//���O�擾
	const std::string& Name() const;
	//�^�O���擾
	const std::string& Tag() const;
	//�g�����X�t�H�[���擾(const)
	const GStransform& Transform() const;

public:

	//�g�����X�t�H�[���擾
	GStransform& Transform();
	//�ړ��ʎ擾
	GSvector3 Velocity() const;
	//�Փ˔���f�[�^�擾
	BoundingSphere Collider() const;
	//�w�肳�ꂽ�ꏊ�܂�Tween�ňړ�����
	TweenUnit& MoveTo(const GSvector2&from,const GSvector2& to, float duration);
	// �w�肳�ꂽ�ꏊ�܂�Tween�ňړ�����
	TweenUnit& MoveTo(const GSvector3& to, float duration);

	//�R�s�[�֎~
	Actor(const Actor& other) = delete;
	Actor& operator = (const Actor& other) = delete;

protected:

	//�G�t�F�N�g����
	void PlayEffect(GSuint id, const GSvector3& local_position, const GSvector3& local_rotation,
		const GSvector3& local_scale);

protected:

	//���[���h
	IWorld* m_World{ nullptr };
	//�Փ˔���
	BoundingSphere m_Collider;
	//�^�O��
	std::string m_Tag;
	//���O
	std::string m_Name;
	//�g�����X�t�H�[��
	GStransform m_Transform;
	//�ړ���
	GSvector3 m_Velocity{ 0.0f,0.0f,0.0f };
	//���S�t���O
	bool m_Dead{ false };
	//�Փ˔��肪�L����
	bool m_EnableCollider{ true };
};

#endif
