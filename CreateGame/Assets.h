#pragma once
#ifndef ASSETS_H_
#define ASSETS_H_

//メッシュデータ
enum
{
    Mesh_Player,                    // 自機
    Mesh_Enemy,                     // 敵
 
};

//オクトリー
enum
{
    Octree_Koutei,      //ステージ用オクトリー
    Octree_KouteiCollider    //衝突判定オクトリー
};

// エフェクト
enum {
    Texture_Skybox,  //スカイボックス
    Texture_Kendo,   //剣道部長
    Texture_Karate,  //空手部長
    Texture_Start,//開始メッセージ
    Texture_Number,  //数字フォント
    Texture_Text,   //テキスト画像
    Texture_BlueBack,//リザルト用背景
    Texture_Result1,//リザルト用テキスト1
    Texture_Result2, //リザルト用テキスト2
    Texture_Radar,   //レーダー画像
    Texture_RadarPoint, //レーダーの点
    Texture_ExplosionFlash,  //中心の大きな花火
    Texture_ExplosionFire  //大量に飛び散る小さな花火
};

enum
{
    Texture_Title=100,             //タイトル
    Texture_Title_result_text, //リザルト画面に戻る文字
    Texture_next_game,         //次のステージへの文字
    Texture_game_over,
    Texture_next_stage,
    Texture_clear,
    Texture_start,
};
enum 
{
    Sound_TitleBGM,         // タイトルシーン用ＢＧＭ 
    Sound_PlayingBGM,       // ゲームシーン用のＢＧＭ 
    Sound_ResultBGM         // リザルト用ＢＧＭ 
};

enum
{
    Se_GameStart,     //ゲーム開始時の効果音
    Se_PlayerAttack,    //剣道部長　攻撃時効果音
    Se_PlayerDamage,    //剣道部長　やられ効果音
    Se_EnemyDamage,     //空手部長　やられ効果音
    Se_Timeout,          //タイムアウトの効果音
    Se_AttackDamage,     //剣道部長の竹刀で当てる音
    Se_Clear
};
#endif
