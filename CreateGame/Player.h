#pragma once
#ifndef PLAYER_H_
#define PLAYER_H_

#include "Actor.h"
#include"AnimatedMesh.h"


//プレーヤークラス
class Player :public Actor
{
public:
	
	//プレーヤーの状態
	enum class State {
		Move,
		Attack_1,
		Attack_2,
		Attack_3,
		Attack_end,
		Damage,
		Up,
		Jump_up,
		Jump_down,
		Jump_up_2,
		Jump_down_2,
		Jump_landing,
		Jump_attack,
		

	};

	//コンストラクタ
	Player(IWorld* world, const GSvector3& position);
	//更新
	virtual void update(float delta_time)override;
	//描画
	virtual void draw()const override;
	//衝突リアクション
	virtual void react(Actor& other)override;
private:
	//状態の更新
	void update_state(float delta_time);
	//状態の変更
	void change_state(State state, GSuint motion, bool loop = true);
	//移動処理
	void move(float delta_time);
	//ダメージ中
	void damage(float delta_time);
	//攻撃判定
	void generate_attack_collider();
	//起き上がり中
	void up(float delta_time);
	////ジャンプアップ
	//void jump_up(float delta_time);
	////ジャンプダウン
	//void jump_down(float delta_time);
	////着地
	//void jump_landing(float delta_time);
	////ジャンプアップ
	//void jump_up_2(float delta_time);
	////ジャンプダウン
	//void jump_down_2(float delta_time);
	//ジャンプ攻撃
	//void jump_attack(float delta_time);
	//1コンボ処理
	void attack_1_flag(float delta_time);
	//２コンボ攻撃
	void attack_2_flag(float delta_time);
	//3コンボ攻撃
	void attack_3_flag(float delta_time);
	//最後のコンボ攻撃終了
	void attack_end(float delta_time);
	//フィールドとの衝突判定
	void collide_field();
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
	//ジャンプしているか
	bool is_jump{false};
	//攻撃しているか
	bool is_attack{ false };
	//２コンボ目の攻撃しているか
	bool is_attack2_flag = false;
	//3コンボ目の攻撃をしているか
	bool is_attack3_flag = false;
	//モーションの描画が最後まで行われるか？
	bool MotionTimer = false;
	//移動
	GSvector2 axis = { 0.0f,0.0f };
	int life_;
private:
	
};
#endif // !PLAYER_H_

