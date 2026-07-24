#include"Enemy.h"
#include"AttackCollider.h"
#include"IWorld.h"
#include "Field.h"
#include "Line.h"
#include"Assets.h"
#include"Explosion.h"

#include"Player.h"
enum
{
	MotionIdle = 0,
	MotionDash = 1,
	MotionAttack_1 = 4,
	MotionAttack_2 = 5,
	MotionAttack_3 = 6,
	MotionDamage = 7,
	MotionDown = 8,
	MotionDefence = 10,
};

//エネミーの高さ
const float PlayerHeight{ 0.8f };
//衝突判定の半径
const float PlayerRadius{ 0.4f };
// 攻撃判定の距離
const float AttackDistance{ 1.2f };
//移動の速さ
const float MoveSpeed{ 0.10f };
//移動判定の距離
const float WalkDistance{ 10.0f };
// 振り向き判定の距離
const float TurnDistance{ 1.5f };
// 振り向く角度
const float TurnAngle{ 2.5f };


//コンストラクタ
Enemy::Enemy(IWorld* world, const GSvector3& position) :
	mesh_{ Mesh_Enemy,MotionIdle,true },
	motion_{ MotionIdle },
	motion_loop_{ true },
	state_{ State::Idle },
	state_timer_{ 0.0f },
	life_{ 3 },
	attack_{ 1 }
{
	//ワールド設定
	world_ = world;
	//座標の初期化
	transform_.position(position);
	//メッシュの変換行列を初期化
	mesh_.transform(transform_.localToWorldMatrix());
	name_ = "Enemy";
	tag_ = "EnemyTag";
	//衝突判定用の設定
	collider_ = BoundingSphere{ PlayerRadius,GSvector3{0.0f,PlayerHeight,0.0} };
	timer = 60.0f;

}
void Enemy::setstats(int hp, int attack) 
{
	life_ = hp;
	attack_ = attack;
}
//更新
void Enemy::update(float delta_time)
{
	//プレーヤーを検索する
	player_ = world_->find_actor("Player");
	//状態の更新
	update_state(delta_time);
	//モーションを変更
	mesh_.change_motion(motion_, motion_loop_);
	//メッシュ更新
	mesh_.update(delta_time);
	//行列を設定
	mesh_.transform(transform_.localToWorldMatrix());
	transform_.translate(0.0f, velocity_.y, 0.0f);
}
//描画
void Enemy::draw()const
{

	mesh_.draw();
	////衝突判定のデバッグ表示
	//collider().draw();
}
void Enemy::react(Actor& other)
{

	//衝突判定用のコライダーに衝突した場合はスコアを加算
	if (other.tag() == "Player_attackTag")
	{
		life_--;

		GSvector3 collider_position=transform_.position();
		//エネミーの高さに攻撃判定が来る処理
		collider_position.y += PlayerHeight;
		// 爆発エフェクトを生成
		world_->add_actor(new Explosion{ world_, collider_position });

		if (life_ <= 0)
		{
			//ダメージ効果音を再生
			gsPlaySE(Se_EnemyDamage);
			/*world_->add_score(20);*/
		}
		else
		{
			gsPlaySE(Se_AttackDamage);
		}

		//ダメージ中に遷移する
		change_state(State::Damage, MotionDamage, false);
	}


}
void Enemy::update_state(float delta_time)
{
	//状態遷移
	switch (state_)
	{
	case State::Idle:idle(delta_time); break;
	case State::Move:move(delta_time); break;
	case State::Attack:attack(delta_time); break;
	case State::Damage:damage(delta_time); break;
	case State::Down:down(delta_time); break;
	case State::Dead:dead(delta_time); break;
	}
	//状態タイマの更新
	state_timer_ += delta_time;
}
//状態の変更
void Enemy::change_state(State state, GSuint motion, bool loop)
{
	motion_ = motion;
	motion_loop_ = loop;
	state_ = state;
	state_timer_ = 0.0f;
}
//アイドル状態
void Enemy::idle(float delta_time)
{

	timer -= delta_time;
	//ダメージ中の終わりでプレイヤーの方向を制限
	if (stop_angle_!=true)
	{
		//ターゲット方向の角度を求める
		float angle = target_signed_angle();
		//向きを変える
		transform_.rotate(0.0f, angle, 0.0f);
	}

	//動くか？
	if (is_walk())
	{
		change_state(State::Move, MotionDash);
		timer = 60.0f;
		return;
	}

	//攻撃するか？
	if (is_attack())
	{
		combo_step_ = 0;
		//攻撃判定を生成
		generate_attack_collider();
		timer = 60.0f;
		//攻撃状態に遷移
		change_state(State::Attack,MotionAttack_1);
		return;
	}
	change_state(State::Idle, MotionIdle);
}
//移動状態
void Enemy::move(float delta_time)
{

	// プレイヤーの方向ベクトルを求める
	GSvector3 to_target = player_->transform().position() - transform_.position();
	to_target.y = 0.0f; // 水平方向だけにする
	to_target.normalize();

	// 前方向
	GSvector3 forward = transform_.forward();
	forward.y = 0.0f;

	// プレイヤーへの角度差を求める
	float angle = GSvector3::signedAngle(forward, to_target);

	// 旋回制御（急に振り向かないように補正）
	angle = CLAMP(angle, -TurnAngle, TurnAngle) * delta_time;

	// 向きを変える
	transform_.rotate(0.0f, angle, 0.0f);

	//前進する
	transform_.translate(0.0f, 0.0f, MoveSpeed * delta_time);

	// 攻撃するか？
	if (is_attack()) {
		combo_step_ = 0;
		// 攻撃判定を生成
		generate_attack_collider();
		// 攻撃状態に遷移
		change_state(State::Attack, MotionAttack_1);
	}

	GSvector3 position = transform_.position();

	if (position.x > 21 || position.x < -21 || position.z>21 || position.z < -21)
	{
		change_state(State::Dead, MotionDefence);
		timer = 60.0f;
	}

}

// 攻撃中
void Enemy::attack(float delta_time) {
	if (state_timer_ >= mesh_.motion_end_time())
	{
		combo_timer_ += delta_time;

		if (combo_timer_ >= 0.5f)
		{ // 次に繋げるタイミング
			combo_timer_ = 0.0f;
			if (is_attack())
			{
				combo_step_++;
				if (combo_step_ < max_combo_)
				{
					//次のコンボモーションへ
					switch (combo_step_)
					{
					case 1:
						generate_attack_collider();
						change_state(State::Attack, MotionAttack_2, false);
						break;
					case 2:
						generate_attack_collider();
						change_state(State::Attack, MotionAttack_3, false);
						break;
					}
				}
				else
				{
					//全部終わったらIdle移行
					combo_step_ = 0;
					change_state(State::Idle, MotionIdle, true);
				}
			}
			else
			{
				//攻撃範囲外ならコンボ中断Idle状態に戻す
				combo_step_ = 0;
				change_state(State::Idle, MotionIdle, true);
			}
		}
	}
}


void Enemy::dead(float delta_time)
{
	timer -= delta_time;
	if (timer < 0)
	{
		die();
	}
}

void Enemy::damage(float delta_time)
{
	
	if (state_timer_ < mesh_.motion_end_time())
	{
		//ダメージモーション中は何もしない
		transform_.translate(velocity_ * delta_time, GStransform::Space::World);
		velocity_ -= GSvector3{ velocity_.x,0.0f,velocity_.z }*0.5f * delta_time;
		return;
	}
	else
	{

		if (life_ > 0)
		{
			//0になるまで方向制限
			stop_angle_ = true;
			idle(delta_time);
			return;
		}
		else
		{
			//ダウン中に遷移
			change_state(State::Down, MotionDown, false);
			//ダウン中にいじめ殴りが起きないように衝突判定を無効にする
			enable_collider_ = false;	
		}
	}
}
//ダウン状態
void Enemy::down(float delta_time)
{
	timer -= delta_time;
	if (timer < 0)
	{
		die();
	}
}
float Enemy::target_signed_angle()const
{
	//ターゲットがいなければ0を返す
	if (player_ == nullptr)return 0.0f;

	//ターゲット方向のベクトルを求める
	GSvector3 is_target = player_->transform().position() - transform_.position();

	//前向き方向のベクトルを取得
	GSvector3 forward = transform_.forward();

	//ベクトルのy成分を無効にする
	forward.y = 0.0f;
	is_target.y = 0.0f;

	//前向き方向のベクトルとターゲット方向のベクトルの角度差を求める
	return GSvector3::signedAngle(forward, is_target);

}
//前向き方向のベクトルとターゲット方向のベクトルの角度差を求める（符号なし）
float Enemy::target_angle()const
{
	return std::abs(target_signed_angle());
}

//移動判定
bool Enemy::is_walk()const
{
	return(timer < 0.0f);
}

// 攻撃判定
bool Enemy::is_attack() const {
	// 攻撃距離内かつ前向き方向のベクトルとターゲット方向のベクトルの角度差が20.0度以下か？ 
	return (target_distance() <= AttackDistance) && (target_angle() <= 20.0f);
	
}

//ターゲットとの距離を求める
float Enemy::target_distance()const
{
	return GSvector3::distance(player_->transform().position(), transform_.position());
}
//
void Enemy::generate_react_collider()
{
	//衝突判定を出現させる場所の距離
	transform_.position();
}

void Enemy::generate_attack_collider()
{
	// 攻撃判定を出現させる場所の距離
	const float AttackColliderDistance{ 0.5f };
	// 攻撃判定の半径
	const float AttackColliderRadius{ 0.3f };
	// 攻撃判定を出す場所の高さ
	const float AttackColliderHeight{ 1.0f };

	// 攻撃判定が有効になるまでの遅延時間
	const float AttackColliderDelay{ 15.0f };
	// 攻撃判定の寿命
	const float AttackColliderLifeSpan{ 5.0f };

	// 衝突判定を出現させる座標を求める（前方の位置）
	GSvector3 position = transform_.position() + transform_.forward() * AttackColliderDistance;
	// 高さの補正（足元からの高さ）
	position.y += AttackColliderHeight;
	// 衝突判定用の球を作成
	BoundingSphere collider{ AttackColliderRadius, position };

	// 衝突判定を出現させる
	world_->add_actor(new AttackCollider{ world_, collider,
		"EnemyAttackTag", "EnemyAttack", tag_, AttackColliderLifeSpan, AttackColliderDelay });

}