#include"Score.h"
#include"Assets.h"
#include"NumberTexture.h"
#include<gslib.h>
#include<algorithm>
//コンストラクタ
Score::Score(int score) :
	score_{ score }
{

}
//スコアの初期化
void Score::initialize(int score)
{
	score_ = score;
}
//スコアの加算
void Score::add(int score)
{
	//スコアの上限値99999
	score_ = std::min(score_ + score, 99999);
}
//スコアの描画
void Score::draw()const
{
	static const NumberTexture number{ Texture_Number ,32,64 };
	number.draw(GSvector2{ 512,0 }, score_, 3);
}
//スコアの取得
int Score::get()const
{
	return score_;
}
//スコアのクリア
void Score::clear()
{
	score_ = 0;
}
