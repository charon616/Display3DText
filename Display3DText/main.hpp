#ifndef main_h
#define main_h

#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>

#include <GLUT/glut.h>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <btBulletDynamicsCommon.h>

#include "TwoDimensionalShape.hpp"
#include "ThreeDimensionalShape.hpp"
#include "dMoji.hpp"
#include "dMaterial.hpp"
#include "dPhysics.hpp"
#include "dOBJ.hpp"

#define GL_SILENCE_DEPRECATION
#define WINDOW_NAME1 "Window1"
#define WINDOW_NAME2 "Window2"

#define TEXTURE_HEIGHT (512)
#define TEXTURE_WIDTH (512)
#define TEXTURE_NUM 3

static int WINDOW_WIDTH = 800;
static int WINDOW_HEIGHT = 800;

std::string TITLE = "言葉のドッジボール（仮）";

//初期化
void init_GL();
void init();
void set_callback_functions();

//コールバック関数
void glut_display1();
void glut_display2();
void glut_keyboard(unsigned char key, int x, int y);
void glut_keyboard(int key, int x, int y);
void glut_mouse(int button, int state, int x, int y);
void glut_motion(int x, int y);
void glut_reshape(int w, int h);
void glut_idle();

//カメラの設定
double g_angle1 = 0.0;
double g_angle2 = 0.1;
double g_distance = 100.0;
bool g_isLeftButtonOn = false;
bool g_isRightButtonOn = false;

//ライトの設定
void set_lighting();
void set_fog();

//テクスチャとマテリアルの設定
GLuint g_TextureHandles[TEXTURE_NUM] = {0,0,0};
void set_texture();
void set_material();

//描画の切り分け
void draw_ground();
void draw_distant();
void draw_moji();
void draw_menu();

OBJMESH mesh;

//Global variable
TwoDimensionalShape two_d_shape;
ThreeDimensionalShape three_d_shape;
dMoji moji;
dPhysics bullet;

int window1, window2;

cv::Point3f mPos(-12, 4, 0); //文字物体の表示位置
cv::Point2i mouse;
std::string out_moji;

int limit_time = 60;

#endif
