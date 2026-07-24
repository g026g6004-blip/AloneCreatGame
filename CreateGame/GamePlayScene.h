#pragma once
#ifndef GAME_PLAY_SCENE_H_
#define GAME_PLAY_SCENE_H_

#include"IScene.h"
#include"World.h"
#include"Result.h"
#include"Gameover.h"

//ゲームプレイシーン
class GamePlayScene :public IScene
{
public:

	//開始
	virtual void start()override;
	//更新
	virtual void update(float delta_time)override;
	//描画
	virtual void draw()const override;
	//終了しているか？
	virtual bool is_end()const override;
	//次のシーンを返す
	virtual std::string next()const override;
	//終了
	virtual void end()override;
private:
	//ゲーム中状態の更新
	void update_playing(float delta_time);
	//リザルト状態の更新
	void update_result(float delta_time);
	//ゲームオーバー状態の更新
	void update_gameover(float delta_time);
	void update_stage2(float delta_time); 

private:
	//ゲームプレイシーンの状態
	enum class State
	{
		Playing,//ゲームプレイ中
		Result,	//リザルト中
		Gameover,//ゲームオーバー中
	};
	//状態
	State state_{ State::Playing };
	//ワールドクラス
	World world_;
	//リザルト
	Result result_;
	//ゲームオーバー
	Gameover gameover_;

	//リザルト用タイマ
	float result_timer_{ 0.0f };
	//ゲームオーバー用タイマ
	float gameover_timer_{ 0.0f };
	//終了フラグ
	bool is_end_{ false };
	//敵を倒した時間
	float enemy_kill_time_ = 0.0f;
	
	//ステージ番号の保持
	int stage_id_;
};
#endif // !GAME_PLAY_SCEN_H_

