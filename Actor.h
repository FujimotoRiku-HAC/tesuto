#ifndef ACTOR_H_
#define ACTOR_H_

#include "Collision/BoundingSphere.h"
#include "Tween/Tween.h"

//アクタークラス
class Actor {
public:
	//コンストラクタ
	Actor() = default;
	//仮想デストラクタ
	virtual ~Actor() = default;
	//更新
	virtual void Update(float delta_time);
	//遅延更新
	virtual void LateUpdate(float delta_time);
	//描画
	virtual void Draw() const;
	//半透明描画
	virtual void DrawTransparent() const;
	//GUIの描画
	virtual void DrawGui() const;
	//衝突判定
	virtual void React(Actor& other);
	//メッセージ処理
	virtual void HandleMessage(const std::string& message, void* param);
	//状態追加
	virtual void AddState(){}
	//衝突判定
	void Collide(Actor& other);
	//死亡する
	void Die();

public:

	//衝突しているか
	bool IsCollide(const Actor& other)const;
	//死亡しているか
	bool IsDead() const;

public:

	//名前取得
	const std::string& Name() const;
	//タグ名取得
	const std::string& Tag() const;
	//トランスフォーム取得(const)
	const GStransform& Transform() const;

public:

	//トランスフォーム取得
	GStransform& Transform();
	//移動量取得
	GSvector3 Velocity() const;
	//衝突判定データ取得
	BoundingSphere Collider() const;
	//指定された場所までTweenで移動する
	TweenUnit& MoveTo(const GSvector2&from,const GSvector2& to, float duration);
	// 指定された場所までTweenで移動する
	TweenUnit& MoveTo(const GSvector3& to, float duration);

	//コピー禁止
	Actor(const Actor& other) = delete;
	Actor& operator = (const Actor& other) = delete;

protected:

	//エフェクト処理
	void PlayEffect(GSuint id, const GSvector3& local_position, const GSvector3& local_rotation,
		const GSvector3& local_scale);

protected:

	//ワールド
	IWorld* m_World{ nullptr };
	//衝突判定
	BoundingSphere m_Collider;
	//タグ名
	std::string m_Tag;
	//名前
	std::string m_Name;
	//トランスフォーム
	GStransform m_Transform;
	//移動量
	GSvector3 m_Velocity{ 0.0f,0.0f,0.0f };
	//死亡フラグ
	bool m_Dead{ false };
	//衝突判定が有効か
	bool m_EnableCollider{ true };
};

#endif
