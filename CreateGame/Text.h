#pragma once
#pragma once
#ifndef TEXT_H_
#define TEXT_H_
//スコアクラス
class Text
{
public:
	///コンストラクタ
	Text(int text=0);
	//テキストの初期化
	void initialize(int text = 0);
	///スコアの描画
	void draw()const;
	

private:
	//テキスト
	int text_;
};
#endif
