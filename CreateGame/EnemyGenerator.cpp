#include"EnemyGenerator.h"
#include"IWorld.h"
#include"Field.h"
#include"Enemy.h"



//コンストラクタ
EnemyGenerator::EnemyGenerator(IWorld* world,int stage_id)
{
	world_ = world;
	tag_ = "Generator";
	name_ = "EnemyGenerator";
	timer_ = 0.0f;
	enable_collider_ = false;
	has_spawned_ = false;
	stage_id_ = stage_id;
}
//更新
void EnemyGenerator::update(float delta_time)
{
	//まだ出ていない
	if (!has_spawned_)
	{
		//エネミータグを呼び出す
		if (world_->count_actor_with_tag("EnemyTag") < 1)
		{
			//エネミーを出す
			auto* enemy = new Enemy{ world_, GSvector3{0.0f, 0.0f, 20.0f} };
			// ステージ番号に応じてパラメータ変更
			if (stage_id_ == 1) enemy->setstats(3,1);   // HP3, 攻撃1
			if (stage_id_ == 2) enemy->setstats(5,3);   // HP5, 攻撃2
			world_->add_actor(enemy);
			has_spawned_ = true;
		}
	}
	timer_ -= delta_time;
}
// 描画 (何も表示しない)
void EnemyGenerator::draw() const { }


