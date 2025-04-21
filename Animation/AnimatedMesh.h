#ifndef ANIMATED_MESH_H_
#define ANIMATED_MESH_H_

//�A�j���[�V�����C�x���g�\����
//�A�j���[�V�����C�x���g(�A�j���[�V�����̎w�肳�ꂽ�^�C�~���O�ŏ��������s)�Ɋւ������ێ�����
struct AnimationEvent {
	//�R���X�g���N�^
	AnimationEvent(GSuint motion, GSfloat time, std::function<void()>callback) :
		m_Motion{ motion },
		m_Time{ time },
		m_Callback{ callback } {
	}
	//���������郂�[�V�����ԍ�
	GSuint m_Motion;
	//�C�x���g����������^�C�~���O
	GSfloat m_Time;
	//�C�x���g�������̃R�[���o�b�N
	std::function<void()>m_Callback;
};

class GStransform;

//�A�j���[�V�����t�����b�V���N���X
class AnimatedMesh {
public:
	//�R���X�g���N�^
	AnimatedMesh(GSuint id, GSuint motion = 0, bool loop = true, GSuint num_bones = 256);
	//�X�V
	void Update(float delta_time);
	//�`��
	void Draw() const;
	//���[�V�����ύX
	void ChangeMotion(GSuint motion, bool loop = true, GSfloat speed = 1.0f);;
	//�ϊ��s���ݒ�
	void Transform(const GSmatrix4& matrix);
	//���݂̃��[�V�����^�C����ݒ�
	void MotionTime(float time);
	//���[�V�����X�g�b�v
	void MotionStop(int value);
	//���[�g���[�V�����K�p
	void ApplyRootMotion(GStransform& transform);
	//�A�j���[�V�����C�x���g��o�^
	void AddEvent(GSuint motion, GSfloat time, std::function<void()>callback);

public:

	//���݂̃��[�V�����̍Đ����Ԃ��擾
	float MotionTime() const;
	//�X�s�[�h�擾
	float MotionSpeed()const;
	//���[�V�����̏I�����Ԃ��擾
	float MotionEndTime() const;

public:

	//�{�[���̃��[���h�ϊ����擾
	GSmatrix4 BoneMatrices(int bone_no)const;

private:
	//���[���h�ϊ��s��
	GSmatrix4 m_Transform;
	//�O��Đ��������[�V�����ԍ�
	GSuint m_PrevMotion;
	//�A�Z�b�gID
	GSuint m_Id;
	//���[�V�����ԍ�
	GSuint m_Motion;
	//���[�V�����^�C�}
	GSfloat m_MotionTimer;
	//�O��̃t���[���̃��[�V�����^�C�}�[
	GSfloat m_PrevTimer;
	//�O��Đ������ŏI�A�j���[�V�����^�C�}
	GSfloat m_PrevMotionTimer;
	//��ԃ^�C�}
	GSfloat m_LerpTimer;
	//�X�s�[�h
	GSfloat m_Speed;

	//�{�[���̃��[�J���p�ϊ��s��
	std::vector<GSmatrix4> m_LocalBoneMatrices;
	//�{�[���̕ϊ��s��
	std::vector<GSmatrix4> m_BoneMatrices;
	//�����̃A�j���[�V�����C�x���g���i�[���邽�߂�vector
	std::vector<std::unique_ptr<AnimationEvent>>m_Events;

	//���[�V�������[�v�t���O
	bool m_MotionLoop;
};

#endif
