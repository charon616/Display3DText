#ifndef dMoji_h
#define dMoji_h

#include <stdio.h>
#include <iostream>
#include <vector>
#include <FTGL/ftgl.h>
#include <btBulletDynamicsCommon.h>
#include "dMaterial.hpp"

#define D_FONT_FILE "Fonts/FOT-CezannePro-EB.otf"
#define GUI_FONT_FILE "Fonts/A-OTF-UDShinMGoPr6-Bold.otf"
#define DEPTH 3

static FTExtrudeFont* dFont = new FTExtrudeFont(D_FONT_FILE);
static FTPixmapFont* guiFont = new FTPixmapFont(GUI_FONT_FILE);

class dMoji{

public:
    dMoji();
    ~dMoji();
    
    void DrawSolidStrings(std::vector<std::string> &strs, int w, int h, int x0, int y0, int z0);
    void DrawSolidStrings(std::string &strs, int w, int h, int x0, int y0, int z0);
    void DrawSolidStrings(const char* str, int w, int h, int x0, int y0, int z0);

    
    void DrawGuiStrings(std::string &strs, int x0, int y0, int z0);
    
    void DrawBBox(const char* str, int x0, int y0, int z0);
    
    void PrintTextInfo(std::string str);
    btVector3 GetTextSize(std::string str);
    
private:
    int dFontSize = 6;  // フォントサイズ
    int guiFontSize = 24;  // フォントサイズ
    
};

#endif
