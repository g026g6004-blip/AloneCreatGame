#pragma once
#ifndef RESULT_H_
#define RESULT_H_

#include<vector>
#include<string>

//リザルトクラス
class Result
{
public:
	//コンストラクタ
	Result();
	//初期化
	void initialize();
	//更新
	void update(float delta_time);
	//描画
	void draw()const;

private:
	//背景の描画
	void draw_background()const;
	//評価メッセージの描画
	void draw_grade_message()const;
	//リザルト画面1秒後に開始ボタン
	void draw_start()const;
	//スコアから評価を計算する
	int calculate_grades()const;
private:
	//ランキングのデータ
	std::vector<int>ranking_scores_;
	//最後に追加したスコア
	int final_score_{ 0 };
	//最後に追加したスコアのランク
	int final_rank_{ 0 };
	//プレーヤー移動用タイマ
	float moving_timer_{ 0.0f };
	//リザルトタイマ
	float result_timer_{ 0.0f };
};
#endif // !RESULT_H_

