#ifndef DOOR_H_
#define DOOR_H_

#include "Field/FieldActor.h"

class Door : public FieldActor {
public:

	//�R���X�g���N�^
	Door(IWorld* world,const GSvector3& position,GSuint mesh,GSuint collider);
	//�X�V
	virtual void Update(float delta_time)override;
	//�`��
	virtual void Draw() const override;
	//���b�Z�[�W���M
	virtual void HandleMessage(const std::string& message, void* param)override;

private:

	//�������f�B�U
	void Transparency() const;

private:
	bool m_IsOpen = false;		//�J������
	float m_ColorTime{ 0.01f };	//�J���[�^�C��
	float m_ColorTimer{ 1.0f };	//�J���[�^�C�}�[
};

#endif