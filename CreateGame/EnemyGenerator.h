#pragma once
#ifndef ENEMY_GENERATOR_H_
#define ENEMY_GENERATOR_H_
#include"Actor.h"
#include"World.h"
//敵生成クラス
class EnemyGenerator :public Actor
{
public:
	//コンストラクタ
	EnemyGenerator(IWorld* world, int stage_id);
		
	//更新
	virtual void update(float delta_time)override;
	//描画
	virtual void draw()const override;
private:
	//生成時間タイマ
	float timer_{ 0.0f };
	//敵が出現（一度だけ）
	bool has_spawned_;

private:
	int stage_id_;
};
#endif // !ENEMY_GENERATOR_H_

