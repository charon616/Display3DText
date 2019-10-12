#include "dMoji.hpp"

dMoji::dMoji(){
    dFont->FaceSize(dFontSize);
    dFont->Depth(DEPTH);
    guiFont->FaceSize(guiFontSize);
}

dMoji::~dMoji(){
}

void dMoji::DrawSolidStrings(std::vector<std::string> &strs, int w, int h, int x0, int y0, int z0){
    glPushMatrix();
    // FTGLで文字列を描画
    if(dFont){
        for(int j = 0; j < (int)strs.size(); ++j){
            glRasterPos2f(x0, y0);
            strs[j].push_back(L'\0');
            dFont->Render(strs[j].c_str(), -1, FTPoint(x0, y0, z0), FTPoint(0, 0, 0), FTGL::RENDER_ALL);
        }
    }
    glPopMatrix();
}

void dMoji::DrawSolidStrings(std::string &strs, int w, int h, int x0, int y0, int z0){
    glPushMatrix();
    // FTGLで文字列を描画
    if(dFont){
        glRasterPos2f(x0, y0);
        dFont->Render(strs.c_str(), -1, FTPoint(x0, y0, z0), FTPoint(0, 0, 0), FTGL::RENDER_ALL);
    }
    glPopMatrix();
}

void dMoji::DrawSolidStrings(const char* str, int w, int h, int x0, int y0, int z0){
    glPushMatrix();
    // FTGLで文字列を描画
    if(dFont){
        glRasterPos2f(x0, y0);
        dFont->Render(str, -1, FTPoint(x0, y0, z0), FTPoint(0, 0, 0), FTGL::RENDER_ALL);
    }
    glPopMatrix();
}

void dMoji::DrawGuiStrings(std::string &strs, int x0, int y0, int z0){
    
    glPushMatrix();
    glColor4dv(gui_color);
    
    // FTGLで文字列を描画
    if(guiFont){
        glRasterPos2f(x0, y0);
        guiFont->Render(strs.c_str(), -1, FTPoint(x0, y0, z0), FTPoint(0, 0, 0), FTGL::RENDER_ALL);
    }
    glPopMatrix();
}

void dMoji::DrawBBox(const char* str, int x0, int y0, int z0){
    glPushMatrix();
    if(dFont){
        glRasterPos2f(x0, y0);
        //dFont->BBox(str, -1, FTPoint(x0, y0, z0), FTPoint(0, 0, 0));
        
        FTBBox test = dFont->BBox(str);
        FTPoint p = test.Lower();
        std::cout << p.X() << " " << p.Y() << " " << p.Z() << std::endl;
        
    }
    glPopMatrix();
};

void dMoji::PrintTextInfo(std::string str){
    std::cout << "Ascender: " << dFont->Ascender() << std::endl;
    std::cout << "Descender: " << dFont->Descender() << std::endl;
    std::cout << "LineHeight: " << dFont->LineHeight() << std::endl;
    FTBBox tmp = dFont->BBox(str.c_str(), -1, FTPoint(0, 0, 0), FTPoint(0, 0, 0));
    std::cout << "Lower.x: " << tmp.Lower().X() << "Lower.y: " << tmp.Lower().Y() << "Lower.z: " << tmp.Lower().Z() << std::endl;
    std::cout << "Upper.x: " << tmp.Upper().X() << "Upper.y: " << tmp.Upper().Y() << "Upper.z: " << tmp.Upper().Z() << std::endl;
    std::cout << "Advance: " << dFont->Advance("test", -1, FTPoint(0, 0, 0)) << std::endl;
}

btVector3 dMoji::GetTextSize(std::string str){
    FTBBox tmp = dFont->BBox(str.c_str(), -1, FTPoint(0, 0, 0), FTPoint(0, 0, 0));
    float width = tmp.Upper().X() - tmp.Lower().X();
    float height = tmp.Upper().Y() - tmp.Lower().Y();
    float depth = tmp.Upper().Z() - tmp.Lower().Z();
    return btVector3(width, height, depth);
}
