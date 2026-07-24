#include"NumberTexture.h"
#include<sstream>
#include<iomanip>

//コンストラクタ
NumberTexture::NumberTexture(GSuint texture, int width, int height):
texture_{ texture }, width_{ width }, height_{ height }{
}
void NumberTexture::draw(const GSvector2& position, int num, int dight, char fill,const 
	GScolor&color)const
{
	std::stringstream ss;
	ss << std::setw(dight) << std::setfill(fill) << num;
	draw(position, ss.str(),color);
}

void NumberTexture::draw(const GSvector2& position, int num,const GScolor&color)const
{
	draw(position, std::to_string(num),color);
}

void NumberTexture::draw(const GSvector2& position, const std::string& num,const GScolor&color)const
{
	//数字を1文字ずつ描画する
	for (int i = 0; i < (int)num.size(); ++i)
	{
		if (num[i] == ' ')continue;
		//char型をint型に変換
		int n = (num[i] != '.') ? num[i] - '0' : 10;//小数点は9の次にあることを前提
		//数字に対応する画像を切り出すための短形を計算する（数値フォント画像内の位置）
		GSrect rect(n * width_, 0.0f, (n * width_) + width_, height_);
		//数字を描画する座標を計算
		GSvector2 pos{ position.x + i * width_,position.y };
		//数字を1描画
		gsDrawSprite2D(texture_, &pos, &rect, NULL, &color, NULL, 0);
	}
}
