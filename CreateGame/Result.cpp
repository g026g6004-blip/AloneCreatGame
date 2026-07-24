#include"Result.h"
#include"NumberTexture.h"
#include"Assets.h"
#include<gslib.h>
#include<fstream>

//ランキング数
const int RankingCount{ 5 };
//プレーヤーの移動時間
const float MovingTime{ 30.0f };

//コンストラクタ
Result::Result()
{
	//初期化する
	initialize();
}
//初期化
void Result::initialize()
{
	//プレーヤーが移動用タイマの初期化
	moving_timer_ = 0.0f;
	
}
//更新
void Result::update(float delta_time)
{
	//移動時間の更新
	moving_timer_ = MIN(moving_timer_ += delta_time, MovingTime);
}
//描画
void Result::draw()const
{
	//背景を描画
	draw_background();
	//評価メッセージの表示
	draw_grade_message();
	//リザルト開始１秒後に開始ボタンを押す
	draw_start();
	static const GSvector2 position{ 0.0f,0.0f };
		gsDrawSprite2D(Texture_next_stage, &position, NULL, NULL, NULL, NULL, 0.0f);
}

//背景の描画
void Result::draw_background()const
{
	//半透明の青い背景を表示
	static const GScolor bg_color{ 1.0f,1.0f,1.0f,0.5f };
	static const GSvector2 bg_position{ 20.0f,100.0f };
	gsDrawSprite2D(Texture_BlueBack, &bg_position, NULL, NULL, &bg_color, NULL, 0.0f);
}


void Result::draw_start()const
{
	//リザルト開始１秒後に「開始ボタンを押忍！」を点滅表示する
	if (result_timer_ >= 60.0f && fmod(result_timer_, 40.0f) < 20.0f)
	{
		static const GSvector2 position{ 250,700 };
		gsDrawSprite2D(Texture_Title_result_text, &position, NULL, NULL, NULL, NULL, 0.0f);
	}
}
//評価メッセージの描画
void Result::draw_grade_message()const
{
	//スコアから評価を計算する
	int grades = calculate_grades();
	//評価のタイトルを表示
	GSrect grades1_rect{ 0.0f,64.0f * grades,1024,64.0f * grades + 64.0f };
	static const GSvector2 grades1_position{ 40.0f,120.0f };
	static const GScolor grades1_color{ 1.0f,0.0f,0.0f,1.0f };
	gsDrawSprite2D(Texture_Result1, &grades1_position, &grades1_rect, NULL, &grades1_color, NULL, 0.0f);
	//評価の文面を表示
	GSrect grades2_rect{ 0.0f,200.0f * grades,1024,200.0f * grades + 180.0f };
	static const GSvector2 grades2_position{ 40.0f,190.0f };
	gsDrawSprite2D(Texture_Result2, &grades2_position, &grades2_rect, NULL, NULL, NULL, 0.0f);
}
//スコアから評価を計算する
int Result::calculate_grades()const
{
	if (final_score_ >= 60)return 0;//熱血硬派、剣道部長
	if (final_score_ >= 50)return 1;//素晴らしい剣道部長
	if (final_score_ >= 40)return 2;//普通の剣道部長
	if (final_score_ >= 30)return 3;//勘違いの剣道部長
	return 4;						//軟弱な剣道部長
}