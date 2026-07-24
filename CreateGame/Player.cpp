#include"Player.h"
#include"AttackCollider.h"
#include"IWorld.h"
#include "Field.h"
#include "Line.h"
#include"Enemy.h"
#include"Assets.h"
enum
{
	MotionIdle		=0,
	MotionAttack	=2,
	MotionAttack2	=3,
	MotionAttack3	=4,
	MotionDash		=5,
	MotionDamage	=6,
	MotionUp 		=16,

	MotionWalk = 20,
	MotionWalk_Left = 21,
	MotionWalk_LeftR = 22,
	MotionWalk_Right = 23,
	MotionWalk_RightR = 24,

	MotionWalk_Back=10,
	MotionWalk_Back_Left = 11,
	MotionWalk_Back_LeftR = 12,
	MotionWalk_Back_Right = 13,
	MotionWalk_Back_RightR = 14,
};
//攻撃判定の距離
const float AttackDistance{ 2.2f };
//プレイヤーの高さ
const float PlayerHeight{ 0.8f };
//衝突判定の半径
const float PlayerRadius{ 0.4f };
//重力値
const float Gravity{ -0.016f };
// 足元のオフセット
const float FootOffset{ 0.1f };
//コンストラクタ
Player::Player(IWorld* world, const GSvector3& position) :
	mesh_{ Mesh_Player,MotionIdle,true },
	motion_{ MotionIdle },
	motion_loop_{ true },
	state_{ State::Move },
	state_timer_{ 0.0f },
	life_{ 3 }
{
	//ワールド設定
	world_ = world;
	//座標の初期化
	transform_.position(position);
	//メッシュの変換行列を初期化
	mesh_.transform(transform_.localToWorldMatrix());
	name_ = "Player";
	tag_ = "PlayerTag";
	//衝突判定用の設定
	collider_ = BoundingSphere{ PlayerRadius,GSvector3{0.0f,PlayerHeight,0.0} };
	Weight_ = 1.0f;
}

//更新
void Player::update(float delta_time)
{
	//状態の更新
	update_state(delta_time);
	//モーションを変更
	mesh_.change_motion(motion_, motion_loop_);
	//メッシュ更新
	mesh_.update(delta_time);
	//行列を設定
	mesh_.transform(transform_.localToWorldMatrix());
	if (state_ != State::Jump_attack)
	{

		//重力で下向きに加速する
		velocity_.y += Gravity * delta_time;
	}
	//フィールドとの衝突判定
	collide_field();
}

//描画
void Player::draw()const
{
	mesh_.draw();
	//衝突判定のデバッグ表示
	/*collider().draw();*/
}

//衝突リアクション
void Player::react(Actor& other)
{

	if (other.tag() == "EnemyAttackTag")
	{
		life_--;

		if (life_ <= 0)
		{
			world_->set_game_over(true);
			return;
		}
		else
		{
			//ダメージ効果音を再生
			gsPlaySE(Se_PlayerDamage);
		}
		
		//ダメージ中は衝突判定を無効にする
		enable_collider_ = false;
		//ダメージ中に遷移する
		change_state(State::Damage, MotionDamage, false);

	}

}
void Player::update_state(float delta_time)
{
	//状態遷移
	switch (state_)
	{
	case State::Move:move(delta_time); break;
	case State::Attack_1:attack_1_flag(delta_time); break;
	case State::Attack_2:attack_2_flag(delta_time); break;
	case State::Attack_3:attack_3_flag(delta_time); break;
	case State::Attack_end:attack_end(delta_time); break;
	case State::Damage:damage(delta_time); break;
	case State::Up:up(delta_time); break;
	/*case State::Jump_up:jump_up(delta_time); break;
	case State::Jump_down:jump_down(delta_time); break;
	case State::Jump_up_2:jump_up_2(delta_time); break;
	case State::Jump_down_2:jump_down_2(delta_time); break;
	case State::Jump_landing:jump_landing(delta_time); break;*/
	//case State::Jump_attack:jump_attack(delta_time); break;

	}
	//状態タイマの更新
	state_timer_ += delta_time;
}

//状態の変更
void Player::change_state(State state, GSuint motion, bool loop)
{
	motion_ = motion;
	motion_loop_ = loop;
	state_ = state;
	state_timer_ = 0.0f;
}
//移動処理
void Player::move(float delta_time)
{

	gsXBoxPadGetLeftAxis(0, &axis);

	//コントローラースティック検知（0.2以上だったら0に）
	if (fabs(axis.x) < 0.2f) axis.x = 0.0f;
	if (fabs(axis.y) < 0.2f) axis.y = 0.0f;

	//アイドル状態
	GSuint motion{ MotionIdle };
	//前後移動の速さ
	float around_speed = 0.0f;
	//左右移動の速さ
	float side_speed = 0.0f;

	//パッド入力操作
	GSvector3 move_for;
	move_for.x = -axis.x;
	move_for.z = axis.y; //前後移動

	//カメラ移動変数
	float target_yaw = atan2(move_for.x, move_for.z);		//ターゲットのカメラ
	float now_yaw = transform_.rotation().y;				//現在のカメラ
	float delta_time_yaw = target_yaw - now_yaw;			//現在の時間のカメラ

	//角度のずれを収める
	while (delta_time_yaw > GS_PI)  delta_time_yaw -= GS_PI * 2;
	while (delta_time_yaw < -GS_PI) delta_time_yaw += GS_PI * 2;

	GSvector3 forward = transform_.forward();
	GSvector3 right = transform_.right();
	GSuint next_motion = motion_; // ← 今のモーションを保持

	//パッドのコントローラー入力
	if (move_for.length() > 0.0f)
	{
		move_for.normalize();
		around_speed = 0.1f;

		//モーション切り替え
		if (axis.y > 0.2f) // 前方向入力あり
		{
			if (axis.x < -0.2f)
			{
				next_motion = MotionWalk_LeftR;   // 前＋左
			}
			else if (axis.x > 0.2f)
			{
				next_motion = MotionWalk_RightR;  // 前＋右
			}
			else
			{
				next_motion = MotionWalk;         // 前
			}
		}
		//横移動
		else
		{
			if (axis.x < -0.2f)
			{
				next_motion = MotionWalk_Left;
			}
			else if (axis.x > 0.2f)
			{
				next_motion = MotionWalk_Right;
			}
		}

		//移動の中身
		transform_.translate(
			move_for.x * around_speed * delta_time,
			velocity_.y,
			move_for.z * around_speed * delta_time
		);
	}
	else
	{
		next_motion = MotionIdle;
	}
	//走りモードパッド入力
	if (gsXBoxPadButtonState(0,GS_XBOX_PAD_LEFT_THUMB))
	{
		around_speed = around_speed * 1.5f;
		next_motion = MotionDash;
	}

	if (motion_ != next_motion)
	{
		motion_ = next_motion;
		motion_loop_ = true;
	}

	GSvector3 position = transform_.position();

	position.x = CLAMP(position.x, -20.0f, 20.0f);
	position.z = CLAMP(position.z, -20.0f, 20.0f);
	transform_.position(position);
	//攻撃中（1コンボ攻撃）
	if (gsGetKeyTrigger(GKEY_SPACE) || gsXBoxPadButtonTrigger(0, GS_XBOX_PAD_A))
	{
		//攻撃効果音を再生
		gsPlaySE(Se_PlayerAttack);
		//自分の前方に衝突判定を出現させる
		generate_attack_collider();
		//攻撃中に遷移
		change_state(State::Attack_1, MotionAttack, false);
		MotionTimer = false;
		return;
	}

}
void Player::attack_1_flag(float delta_time)
{
	//20フレームの間にスペースキー押されたら2コンボ目の受付
	if (state_timer_ <= 20)
	{
		if (gsGetKeyTrigger(GKEY_SPACE)|| gsXBoxPadButtonTrigger(0, GS_XBOX_PAD_A))
		{
			MotionTimer = true;
		}
	}
	if (state_timer_ >= mesh_.motion_end_time())
	{
		change_state(State::Move, MotionIdle, true);
		return;
	}
	///20フレーム以上
	if (state_timer_ >= 20&&MotionTimer)
	{
		// 2撃目入力フラグがtrueの場合、2撃目を発動
			//攻撃効果音を再生
		gsPlaySE(Se_PlayerAttack);
		//自分の前方に衝突判定を出現させる
		generate_attack_collider();
		//2コンボに遷移
		change_state(State::Attack_2, MotionAttack2, false);
		MotionTimer = false;
		return;
	}
	
}
//攻撃処理(2コンボ追加）
void Player::attack_2_flag(float delta_time)
{
	//20フレームの間にスペースキー押されたら2コンボ目の受付
	if (state_timer_ <= 20)
	{
		if (gsGetKeyTrigger(GKEY_SPACE)||gsXBoxPadButtonTrigger(0, GS_XBOX_PAD_A))
		{
			MotionTimer = true;
		}
	}
	if (state_timer_ >= mesh_.motion_end_time())
	{
		change_state(State::Move, MotionIdle, true);
		return;
	}
	///20フレーム以上
	if (state_timer_ >= 20&&MotionTimer)
	{
		// 2撃目入力フラグがtrueの場合、2撃目を発動

			//攻撃効果音を再生
		gsPlaySE(Se_PlayerAttack);
		//自分の前方に衝突判定を出現させる
		generate_attack_collider();
		//3コンボに遷移
		change_state(State::Attack_3, MotionAttack3, false);
		MotionTimer = false;
		return;
	}
	
}

//3コンボ攻撃中
void Player::attack_3_flag(float delta_time)
{
	
	if (state_timer_ >= mesh_.motion_end_time())
	{
		change_state(State::Move, MotionIdle, true);
		return;
	}
}
//最後のコンボ攻撃終了中
void Player::attack_end(float delta_time)
{
	if (state_timer_ >= mesh_.motion_end_time())
	{
		move(delta_time);
		return;
	}
}

////ジャンプアップ中(垂直で動かない）
//void Player::jump_up(float delta_time)
//{
//	velocity_.y = PlayerHeight / 3.5;
//	is_jump = false;
//	//前後移動の速さ
//	float around_speed = 0.0f;
//	//左右移動の速さ
//	float side_speed = 0.0f;
//	//平行移動する
//	transform_.translate(side_speed * delta_time, velocity_.y, around_speed * delta_time);
//}
//
////ジャンプダウン中(垂直で動かない）
//void Player::jump_down(float delta_time)
//{
//	//前後移動の速さ
//	float around_speed = 0.0f;
//	//左右移動の速さ
//	float side_speed = 0.0f;
//	//平行移動する
//	transform_.translate(side_speed * delta_time, velocity_.y, around_speed * delta_time);
//
//	if (gsGetKeyTrigger(GKEY_SPACE) && is_attack)
//	{
//		//攻撃効果音を再生
//		gsPlaySE(Se_PlayerAttack);
//		//自分の前方に衝突判定を出現させる
//		generate_attack_collider();
//		//攻撃中に遷移
//		velocity_.y = 0.0f;
//		
//		return;
//	}
//}
//
////ジャンプ中（動きながらジャンプ）
//void Player::jump_up_2(float delta_time)
//{
//	velocity_.y = PlayerHeight / 3.5;
//	is_jump = false;
//	//前後移動の速さ
//	float around_speed = 0.10f;
//	//左右移動の速さ
//	float side_speed = 0.0f;
//	//平行移動する
//	transform_.translate(side_speed * delta_time, velocity_.y, around_speed * delta_time);
//
//	
//	if (gsGetKeyTrigger(GKEY_SPACE) && is_attack)
//	{
//		//攻撃効果音を再生
//		gsPlaySE(Se_PlayerAttack);
//		//自分の前方に衝突判定を出現させる
//		generate_attack_collider();
//		//攻撃中に遷移
//		
//		return;
//	}
//}
////ジャンプダウン中（動きながらジャンプ）
//void Player::jump_down_2(float delta_time)
//{
//	//前後移動の速さ
//	float around_speed = 0.10f;
//	//左右移動の速さ
//	float side_speed = 0.0f;
//	//平行移動する
//	transform_.translate(side_speed * delta_time, velocity_.y, around_speed * delta_time);
//	
//	if (gsGetKeyTrigger(GKEY_SPACE) && is_attack)
//	{
//		//攻撃効果音を再生
//		gsPlaySE(Se_PlayerAttack);
//		//自分の前方に衝突判定を出現させる
//		generate_attack_collider();
//		
//		return;
//	}
//}
//
////着地（ジャンプしたあと）
//void Player::jump_landing(float delta_time)
//{
//	if (state_timer_ >= mesh_.motion_end_time())
//	{
//		move(delta_time);
//		return;
//	}
//}


//ダメージ中
void Player::damage(float delta_time)
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
		//起き上がり中に遷移
		change_state(State::Up, MotionUp, false);
	}
}
////ジャンプ攻撃中
//void Player::jump_attack(float delta_time)
//{
//	
//}


void Player::generate_attack_collider()
{
	//攻撃判定を出現させる場所の距離
	const float AttackColliderDistance{ 1.0f };
	//攻撃判定の半径
	const float AttackColiderRadius{ 0.8f };
	//攻撃判定を出す場所の高さ
	const float AttackColliderHeight{ 1.0f };
	//攻撃判定が有効になるまでの遅延時間
	const float AttackColliderDelay{ 10.0f };
	//攻撃判定の寿命
	const float AttackColliderLifeSpan{ 3.0f };
	//衝突判定を出現させるのを求める（前方の位置）
	GSvector3 position = transform_.position() + transform_.forward() * AttackColliderDistance;
	//高さの補正
	position.y += AttackColliderHeight;
	//衝突判定用の球を生成
	BoundingSphere collider{ AttackColiderRadius,position };
	//衝突判定を出現させる
	world_->add_actor(new AttackCollider{ world_,collider,
		"Player_attackTag","PlayerTag",tag_,AttackColliderLifeSpan,AttackColliderDelay });

}
void Player::up(float delta_time)
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
		enable_collider_ = true;
		//アイドル中に遷移
		change_state(State::Move, MotionDash, false);
	}
}
//フィールドとの衝突判定
void Player::collide_field()
{
	//壁との衝突判定（球体との判定）
	GSvector3 center; //押し戻し後の球体の中心座標

	if (world_->field()->collide(collider(), &center))
	{
		//y座標は変更しない
		center.y = transform_.position().y;

		//補正後の座標に変更する
		transform_.position(center);
	}
	//地面との衝突判定（線分との交差判定）
	GSvector3 position = transform_.position();
	Line line;
	line.start = position + collider_.center;
	line.end = position + GSvector3{ 0.0f,-FootOffset,0.0f };
	GSvector3 intersect;//地面との交点

	if (world_->field()->collide(line, &intersect))
	{
		//交点の位置方y座標のみ補正する
		position.y = intersect.y;

		//座標を変更する
		transform_.position(position);

		//重力を初期化する
		velocity_.y = 0.0f;
		is_jump = true;
	}

}