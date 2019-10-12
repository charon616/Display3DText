#include "FPS.hpp"

FPS::FPS(){
    draw=false;
    LoopCount=0;
    Frame=0;
    sleep_ms=15;
    Skip=0;
    Fps=60;
    FrameSkip=0;
    LoopMax=0;
    CurrentFps=0;
    FrameTime=1000.0f/60-0.000001f;//1フレームの時間
    FrameTimeO=1000.0f/60+0.000001f;
}

//引数付きコンストラクタ
FPS::FPS(int fps){
    draw=false;
    LoopCount=0;
    Frame=0;
    sleep_ms=15;
    Skip=0;
    Fps=fps;
    FrameSkip=0;
    LoopMax=0;
    CurrentFps=0;
    FrameTime=1000.0f/fps-0.000001f;//1フレームの時間
    FrameTimeO=1000.0f/fps+0.000001f;
}

FPS::~FPS(){
}

void FPS::SetFPS(int fps){
    Fps=fps;
    FrameTime=1000.0f/fps-0.000001f;//1フレームの時間
    FrameTimeO=1000.0f/fps+0.000001f;
}

string FPS::GetFPS()
{
    draw=false;
    LoopCount++;
    Time=glutGet(GLUT_ELAPSED_TIME);
    if(Time-Oldtime>Frame*FrameTime){
        Frame++;
        if(Time-Oldtime<Frame*FrameTimeO){//描画タイミングに間に合った場合
            str = "LOOP_MAX:[" + std::to_string(LoopMax) + "] FPS:[" + std::to_string(CurrentFps-FrameSkip) + "] FRAME:[" + std::to_string(Frame) + "] FRAMESKIP:[" + std::to_string(FrameSkip) + "] SLEEP:[" + std::to_string(sleep_ms) + "]";
            draw=true;
            sleep(sleep_ms);
        }else{//描画タイミングに間に合わなかった場合：フレームスキップ
            if(sleep_ms>1)sleep_ms--;//ウェイトを減らす
            Skip++;
        }
    }
    if(Time-Oldtime>1000-FrameTime){//一秒おきの処理
        LoopMax=LoopCount;
        if(LoopMax>CPUMax){if(sleep_ms<15)sleep_ms++;}//十分に余力がある場合ウェイトを増やす
        if(LoopMax<CPUMin){if(sleep_ms>1)sleep_ms--;}//余裕が無い場合ウェイトを減らす
        CurrentFps=Frame;
        FrameSkip=Skip;
        Skip=0;
        LoopCount=0;
        Frame=0;
        Oldtime=Time;
    }
    
    return str;
}
