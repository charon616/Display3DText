#include "TwoDimensionalShape.hpp"

using namespace std;

TwoDimensionalShape::TwoDimensionalShape(){
};
TwoDimensionalShape::~TwoDimensionalShape(){
};

void TwoDimensionalShape::projection_transformation(){
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();// 現在の射影変換行列を保存
    glLoadIdentity();
    //glOrtho(-1.0, 1.0, -1.0, 1.0, 1.0, -1.0); //各軸-1.0～1.0で囲まれる立方体の範囲を平行投影
    glOrtho(-glutGet(GLUT_WINDOW_WIDTH)/2, glutGet(GLUT_WINDOW_WIDTH)/2, -glutGet(GLUT_WINDOW_HEIGHT)/2, glutGet(GLUT_WINDOW_HEIGHT)/2, -1, 1);// 正射影変換設定
    glMatrixMode(GL_MODELVIEW);// モデルビュー変換行列設定
    glPushMatrix();// 現在のモデルビュー行列を保存
    glLoadIdentity();// 単位行列を設定
}

void TwoDimensionalShape::draw_square1()
{
    glBegin(GL_LINE_LOOP);
    
    glColor3d(1.0, 0.0, 0.0);
    glVertex2d(-0.9, -0.9);
    glVertex2d(0.9, -0.9);
    glVertex2d(0.9, 0.9);
    glVertex2d(-0.9, 0.9);
    
    glEnd();
}

void TwoDimensionalShape::draw_square2()
{
    glBegin(GL_POLYGON);
    
    glColor3d(1.0, 0.0, 0.0);
    glVertex2d(-0.9, -0.9);
    glVertex2d(0.9, -0.9);
    glVertex2d(0.9, 0.9);
    glVertex2d(-0.9, 0.9);
    
    glEnd();
}

void TwoDimensionalShape::draw_square3()
{
    glBegin(GL_POLYGON);
    
    glColor3d(1.0, 0.0, 0.0);
    glVertex2d(-0.9, -0.9);
    glColor3d(1.0, 1.0, 0.0);
    glVertex2d(0.9, -0.9);
    glColor3d(0.0, 1.0, 1.0);
    glVertex2d(0.9, 0.9);
    glColor3d(0.0, 0.0, 0.0);
    glVertex2d(-0.9, 0.9);
    
    glEnd();
}

void TwoDimensionalShape::draw_n_square(int n)
{
    glBegin(GL_POLYGON);
    
    random_device rd;
    mt19937 mt(rd()); // シードをランダムに決定した擬似乱数生成器
    uniform_real_distribution<double> c(0.0, 1.0); //一様分布
    
    for (int i = 0; i < n; i++) {
        glColor3d(c(mt), c(mt), c(mt));
        glVertex2d(radius * cos(2 * M_PI * i / n), radius * sin(2 * M_PI * i / n));
    }
    
    glEnd();
}

void TwoDimensionalShape::draw_cursor_circle(int x, int y)
{
    // ブレンドの有効化
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glPushMatrix();
    glBegin(GL_POLYGON);
    int r = 10;
    int n = 100;
    
    glColor4d(255/255.0, 241/255.0, 0.0, 0.5);
    //glTranslatef(-glutGet(GLUT_WINDOW_WIDTH)/2, 0.0, 0.0);
    for (int i = 0; i < n; i++) {
        glVertex2d(r * cos(2 * M_PI * i / n) + x - glutGet(GLUT_WINDOW_WIDTH)/2, r * sin(2 * M_PI * i / n) - y + glutGet(GLUT_WINDOW_HEIGHT)/2);
    }
    glEnd();
    glPopMatrix();
    
    glDisable(GL_BLEND);
}


