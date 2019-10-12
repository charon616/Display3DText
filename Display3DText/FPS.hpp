#pragma once

#include <string>
#include <GLUT/glut.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#ifndef FPS_h
#define FPS_h

using namespace std;

#define CPUMax 30000
#define CPUMin 10000

//FPS管理クラス
class FPS{
    public:
   
        FPS();
        FPS(int);
        ~FPS();
    
        void SetFPS(int);
        string GetFPS();
    //ループカウンタ,現在のフレーム,スリープ値,現在のフレームスキップ数,一秒おきのフレームスキップ数,一秒おきのループカウンタ,現在のFPS値
        int LoopCount,Frame,sleep_ms,Skip,FrameSkip,Fps,LoopMax,CurrentFps;
        bool draw;
        int Time,Oldtime;//現在の時間,前回の時間
        float FrameTime,FrameTimeO;
    
    private:
        string str;
        char buf[255];
};

#endif
