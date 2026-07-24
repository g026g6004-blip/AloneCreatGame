#pragma once
#ifndef STAGE2_SCENE_H_
#define STAGE2_SCENE_H_
#include"IScene.h"
#include"StageBase.h"

class Stage2Scene :public StageBase
{
public:

    Stage2Scene() : StageBase(1) {}
    void start() override;
    std::string next() const override;
    // もしステージ固有の終了処理があれば override end() も可能
};
#endif 
