#include"World.h"
#include"Field.h"
#include"Actor.h"


//デストラクタ
World::~World() 
{
	clear();
}

//更新
void World::update(float delta_time)
{
	//フィールドの更新
	field_->update(delta_time);
	//アクターの更新
	actors_.update(delta_time);
	//アクターの衝突
	actors_.collide();
	//アクターの遅延更新
	actors_.late_update(delta_time);
	//アクターの消去
	actors_.remove();
	//カメラの更新
	camera_->update(delta_time);
	//ライトの更新
	light_->update(delta_time);
	//タイマの更新
	timer_.update(delta_time);

}

//描画
void World::draw()const
{
	//カメラの設定
	camera_->draw();
	//ライトの設定
	light_->draw();
	//シャドウマップの描画
	gsDrawShadowMap(World::Shadow_Map_callback, (void*)this);
	//フィールドの描画
	field_->draw();
	//アクターの描画
	actors_.draw();
	//半透明アクターの描画
	actors_.draw_transparent();
	////スコアの描画
	//score_.draw();
	////得点の描画
	//text_.draw();
	//GUIの描画
	actors_.draw_gui();
	//タイマの描画
	timer_.draw();
	
}

//消去
void World::clear()
{
	//アクター削除
	actors_.clear();
	//カメラを消去
	delete camera();
	//スコアの消去
	score_.clear();
	camera_ = nullptr;
	//ライトを消去
	delete field_;
	field_ = nullptr;
	// フラグをリセット
	game_over_ = false;
}

//カメラの追加
void World::add_camera(Actor* camera)
{
	delete camera_;//現在のカメラを削除
	camera_ = camera;
}

//ライトの追加
void World::add_light(Actor*light)
{
	delete light_; //現在のフィールドを削除
	light_ = light;
}

//フィールドの追加
void World::add_field(Field* field)
{
	delete field_;
	field_ = field;

}

//アクターの追加
void World::add_actor(Actor* actor)
{
	actors_.add(actor);
}

//アクターの検索
Actor* World::find_actor(const std::string& name)const
{
	return actors_.find(name);
	
}

//指定したタグ名を持つアクターの検索
std::vector<Actor*>World::find_actor_with_tag(const std::string& tag)const 
{
	return actors_.find_with_tag(tag);
}

//アクター数を返す
int World::count_actor()const
{
	return actors_.count();
}

//指定したタグ名を持つアクター数を返す
int World::count_actor_with_tag(const std::string& tag)const
{
	return actors_.count_with_tag(tag);
}

//メッセージ送信
void World::send_message(const std::string& message, void* param)
{
	actors_.send_message(message, param);
}

//カメラの取得
Actor* World::camera()
{
	return camera_;
}

//ライトの取得
Actor* World::light()
{
	return light_;
}

//フィールドの取得
Field* World::field()
{
	return field_;

}
//スコアの加算
void World::add_score(int score)
{
	score_.add(score);
}
//シャドウマップの描画用の関数
void World::Shadow_Map_callback(void* param, const GSmatrix4*, const GSmatrix4*)
{
	World* self = (World*)param;

	//シャドウマップにはアクターのみ描画
	self->actors_.draw();
}
//スコアの取得
//Score& World::score()
//{
//	return score_;
//}
////タイマの取得
Timer& World::timer()
{
	return timer_;
}
//ゲームオーバーか？
bool World::is_game_over()const
{
	return timer_.is_timeout()||game_over_;//時間切れか？
}
//敵が０か？
bool World::is_enemy_dead() const
{
	return count_actor_with_tag("EnemyTag") == 0;
}

void World::set_game_over(bool flag)
{
	game_over_ = flag;
}

