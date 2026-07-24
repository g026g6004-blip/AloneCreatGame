#include"Gameover.h"
#include"Assets.h"
Gameover::Gameover()
{
	//‰Šú‰»‚·‚é
	initialize();
}
void Gameover::initialize()
{

}
void Gameover::update(float delta_time)
{
}
void Gameover::draw()const
{
	//ƒ^ƒCƒgƒ‹‚Ì•`‰æ
	const static GSvector2 position_title{ 0.0f,0.0f };
	gsDrawSprite2D(Texture_game_over, &position_title, NULL, NULL, NULL, NULL, 0.0f);
}