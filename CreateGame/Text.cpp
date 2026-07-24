#include"Text.h"
#include"Assets.h"
#include"NumberTexture.h"
#include<gslib.h>
#include<algorithm>
//コンストラクタ
Text::Text(int text) :
	text_{ text }
{

}
//テキストの初期化
void Text::initialize(int text)
{
	text_ = text;
}

//テキストの描画
void Text::draw()const
{
	GSvector2 position = { 360,0 };
	float srcX, srcY;
	srcX = 0;
	srcY = 64;
	GSrect rect = { srcX,srcY,srcX + 128,srcY + 64 };
	gsDrawSprite2D(Texture_Text, &position, &rect, NULL, NULL, NULL, 0.0f);
	
}

