#ifndef ANIMATED_MESH_H_
#define ANIMATED_MESH_H_

//アニメーションイベント構造体
//アニメーションイベント(アニメーションの指定されたタイミングで処理を実行)に関する情報を保持する
struct AnimationEvent {
	//コンストラクタ
	AnimationEvent(GSuint motion, GSfloat time, std::function<void()>callback) :
		m_Motion{ motion },
		m_Time{ time },
		m_Callback{ callback } {
	}
	//発生させるモーション番号
	GSuint m_Motion;
	//イベント発生させるタイミング
	GSfloat m_Time;
	//イベント発生時のコールバック
	std::function<void()>m_Callback;
};

class GStransform;

//アニメーション付きメッシュクラス
class AnimatedMesh {
public:
	//コンストラクタ
	AnimatedMesh(GSuint id, GSuint motion = 0, bool loop = true, GSuint num_bones = 256);
	//更新
	void Update(float delta_time);
	//描画
	void Draw() const;
	//モーション変更
	void ChangeMotion(GSuint motion, bool loop = true, GSfloat speed = 1.0f);;
	//変換行列を設定
	void Transform(const GSmatrix4& matrix);
	//現在のモーションタイムを設定
	void MotionTime(float time);
	//モーションストップ
	void MotionStop(int value);
	//ルートモーション適用
	void ApplyRootMotion(GStransform& transform);
	//アニメーションイベントを登録
	void AddEvent(GSuint motion, GSfloat time, std::function<void()>callback);

public:

	//現在のモーションの再生時間を取得
	float MotionTime() const;
	//スピード取得
	float MotionSpeed()const;
	//モーションの終了時間を取得
	float MotionEndTime() const;

public:

	//ボーンのワールド変換を取得
	GSmatrix4 BoneMatrices(int bone_no)const;

private:
	//ワールド変換行列
	GSmatrix4 m_Transform;
	//前回再生したモーション番号
	GSuint m_PrevMotion;
	//アセットID
	GSuint m_Id;
	//モーション番号
	GSuint m_Motion;
	//モーションタイマ
	GSfloat m_MotionTimer;
	//前回のフレームのモーションタイマー
	GSfloat m_PrevTimer;
	//前回再生した最終アニメーションタイマ
	GSfloat m_PrevMotionTimer;
	//補間タイマ
	GSfloat m_LerpTimer;
	//スピード
	GSfloat m_Speed;

	//ボーンのローカル用変換行列
	std::vector<GSmatrix4> m_LocalBoneMatrices;
	//ボーンの変換行列
	std::vector<GSmatrix4> m_BoneMatrices;
	//複数のアニメーションイベントを格納するためのvector
	std::vector<std::unique_ptr<AnimationEvent>>m_Events;

	//モーションループフラグ
	bool m_MotionLoop;
};

#endif
