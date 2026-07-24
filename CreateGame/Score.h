#pragma once
#ifndef SCORE_H_
#define SCORE_H_
//スコアクラス
class Score
{
public:
	///コンストラクタ
	Score(int score = 0);
	//スコアの初期化
	void initialize(int score = 0);
	//スコアの加算
	void add(int score);
	///スコアの描画
	void draw()const;
	//スコアの取得
	int get()const;
	//スコアのクリア
	void clear();

private:
	//スコア
	int score_;
};
#endif
