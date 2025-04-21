#ifndef ACTOR_MANAGER_H_
#define ACTOR_MANAGER_H_

class Actor;

//アクターマネージャー
class ActorManager {
public:

	//デフォルトコンストラクタ
	ActorManager() = default;
	//仮想デストラクタ
	virtual ~ActorManager();

public:

	//アクターの追加
	void Add(Actor* actor);
	//アクターの更新
	void Update(float delta_time);
	//アクターの遅延更新
	void LateUpdate(float delta_time);
	//アクターの描画
	void Draw() const;
	//半透明アクターの描画
	void DrawTransparent() const;
	//アクターのGUI描画
	void DrawGui() const;
	//アクターの衝突判定
	void Collide();
	//死亡しているアクターの削除
	void Remove();
	//メッセージの送信
	void OnMessage(const std::string& message, void* param);
	//消去
	void Clear();

public:

	//アクターの検索
	Actor* Find(const std::string& name) const;
	//指定したタグ名を持つアクターの検索
	std::vector<Actor*>FindWithTag(const std::string& tag) const;

public:

	//アクター数を返す
	int Count() const;
	//指定したタグ名を持つアクター数を返す
	int CountWithTag(const std::string& tag) const;

	//コピー禁止
	ActorManager(const ActorManager& other) = default;
	ActorManager& operator = (const ActorManager& other) = default;

protected:
	//アクターリスト
	std::list<Actor*>m_Actors;
};

#endif
