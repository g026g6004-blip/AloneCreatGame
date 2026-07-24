#pragma once
#include "IScene.h"
#include "World.h"
#include "Result.h"
#include "GameOver.h"
#include"ClearScene.h"
#include "Assets.h"

class StageBase : public IScene {
public:
    StageBase(int id);
    virtual ~StageBase() = default;

    // start() は各派生クラスで実装（ステージ固有のロード）
    virtual void start() override = 0;
    virtual void update(float delta_time) override;
    virtual void draw() const override;
    virtual bool is_end() const override { return is_end_; }
    virtual std::string next() const override = 0;
    virtual void end() override;

protected:
    // 派生クラスから呼ぶ共通ユーティリティ
    void loadCommonAssets();    // すべてのステージで共通のテクスチャ／SE／BGMなど
    void unloadCommonAssets();

    void enterResult();
    void enterGameover();
    void enterClear();
    // メンバ
    World world_;
    Result result_;
    Gameover gameover_;
    ClearScene clear_;

    enum class State { Playing, Result, Gameover,Clear };
    State state_;
    bool is_end_;
    float result_timer_;
    float clear_timer_;
    float gameover_timer_;
    float enemy_kill_time_;

    int stage_id_; // ステージ番号の保持（デバッグなどで使える）
};
