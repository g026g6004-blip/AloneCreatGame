#pragma once
#ifndef ENEMY_H_
#define ENEMY_H_
#include "Actor.h"
#include"AnimatedMesh.h"
class Enemy:public Actor
{
public:
	//エネミーーの状態
	enum class State {
		Move,
		Damage,
		Damage_Recovary,
		Idle,
		Attack,
		Down,
		Dead,
	};
	//コンストラクタ
	Enemy(IWorld* world, const GSvector3& position);
	//更新
	virtual void update(float delta_time)override;
	//描画
	virtual void draw()const override;
	//衝突リアクション
	virtual void react(Actor& other)override;
	//衝突判定を生成
	virtual void generate_react_collider();
	// 攻撃判定を生成
	virtual void generate_attack_collider();
	//敵のステータス
	void setstats(int hp, int attack);
private:
	
	//状態の更新
	void update_state(float delta_time);
	//状態の変更
	void change_state(State state, GSuint motion, bool loop = true);
	//移動処理
	void move(float delta_time);
	//ダメージ中
	void damage(float delta_time);
	//アイドル状態
	void idle(float delta_time);
	// 攻撃中
	void attack(float delta_time);
	//ダウン状態
	void down(float delta_time);
	//死亡状態
	void dead(float delta_time);
	//移動判定
	bool is_walk()const;
	//攻撃判定
	bool is_attack()const;
	//ターゲットの角度を求める(符号なし）
	float target_angle()const;
	//ターゲットの角度を求める(符号あり)
	float target_signed_angle()const;
	//ターゲットの距離を求める
	float target_distance()const;
	
private:
	//アニメーションメッシュ
	AnimatedMesh mesh_;
	//モーション番号
	GSuint motion_;
	//モーションのループ指定
	bool motion_loop_;
	//状態
	State state_;
	//状態タイマ
	float state_timer_;
	//プレーヤー
	Actor* player_;
	//1秒後移動中に遷移するタイマー
	float timer;
	//体力
	int life_;
	// 攻撃力
	int attack_;    
	//ダメージ中終わった後の敵のアングル阻止
	bool stop_angle_ = false;
    //現在のコンボ段階（1,2,3)
	int combo_step_=0;
	//最大コンボ数
	const int max_combo_ = 3;   
	//コンボ待機時間
	float combo_timer_ = 0.0f;
private:
};
#endif