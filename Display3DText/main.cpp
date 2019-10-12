#include "main.hpp"
#define _DEBUG

using namespace cv;
using namespace std;

/////////////////////////////////////////////////////////////////////////
// initialize
/////////////////////////////////////////////////////////////////////////
void init_GL(int argc, char *argv[]){
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutSetCursor(GLUT_CURSOR_NONE);
}

void init(){
    /* bulletの初期設定 */
    bullet.init_bullet();
    //bullet.add_ground(g_pGroundBody);
    bullet.add_ground();
    out_moji = "おはよう世界ダネ！！";
    bullet.add_moji_box(out_moji, moji.GetTextSize(out_moji), btVector3(2.0, 0.0, 0.0));
    
    WINDOW_WIDTH = glutGet(GLUT_WINDOW_WIDTH);
    WINDOW_HEIGHT = glutGet(GLUT_WINDOW_HEIGHT);
    mouse.x = WINDOW_WIDTH/2;
    mouse.y = WINDOW_HEIGHT/2;
    
    if ( !mesh.LoadFile( "Models/plants.obj" ) )
        exit(0);
}

/////////////////////////////////////////////////////////////////////////
// setting
/////////////////////////////////////////////////////////////////////////
void set_callback_functions(){
    glutKeyboardFunc(glut_keyboard);
    glutSpecialFunc(glut_keyboard);
    glutReshapeFunc(glut_reshape);
    glutMouseFunc(glut_mouse); //マウスボタンが押されたか離されたとき。
    glutMotionFunc(glut_motion); //マウスでドラッグが行われているとき。
    glutPassiveMotionFunc(glut_motion); //マウスカーソルが移動しているとき
}

void set_texture(){
    glGenTextures(TEXTURE_NUM, g_TextureHandles);
    
    for(int i = 0; i < TEXTURE_NUM; i++){
        glBindTexture(GL_TEXTURE_2D, g_TextureHandles[i]);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, TEXTURE_WIDTH, TEXTURE_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    }
    
    const char* inputFileNames[TEXTURE_NUM] = {"Texture/dry-ground.jpg", "Texture/blue-sky.jpg", "Texture/balloon-sky.jpg" };
    for(int i = 0; i < TEXTURE_NUM; i++){
        cv::Mat input = cv::imread(inputFileNames[i], 1);
        cv::cvtColor(input, input, cv::COLOR_BGR2RGB); // BGR -> RGBの変換
        glBindTexture(GL_TEXTURE_2D, g_TextureHandles[i]);
        glTexSubImage2D(GL_TEXTURE_2D, 0,
                        (TEXTURE_WIDTH - input.cols) / 2,
                        (TEXTURE_HEIGHT - input.rows) / 2,
                        input.cols, input.rows,
                        GL_RGB, GL_UNSIGNED_BYTE, input.data);
    }
}

void set_material(){
    glMaterialfv(GL_FRONT, GL_AMBIENT, ms_jade.ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, ms_jade.diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, ms_jade.specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, &ms_jade.shininess);
}

void set_lighting(){
    GLfloat lightpos[] = {0.0, 3.0, 3.0};
    GLfloat diffuse[] = {1.0, 1.0, 1.0, 1.0};
    GLfloat ambient[] = {1.0, 1.0, 1.0, 1.0};
    
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);//光源0を利用
    glLightfv(GL_LIGHT0, GL_POSITION, lightpos);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
}

void set_fog(){
    glEnable(GL_FOG);
    glFogfv(GL_FOG_COLOR, bgcolor);
    glFogi(GL_FOG_MODE , GL_LINEAR); glFogi(GL_FOG_START , 0); glFogi(GL_FOG_END , 450);
}

/////////////////////////////////////////////////////////////////////////
// main
/////////////////////////////////////////////////////////////////////////
int main(int argc, char *argv[]){
    /* OpenGLの初期化 */
    init_GL(argc, argv);
    
    /* window1 */
    glutInitWindowPosition(0, 0);   // ウィンドウの位置を指定
    window1 = glutCreateWindow(WINDOW_NAME1);
    glutDisplayFunc(glut_display1);
    set_callback_functions();
    set_texture();
    //glutSetOption(GLUT_WINDOW_CURSOR ,GLUT_CURSOR_NONE);
    //glutSetOption(GLUT_RENDERING_CONTEXT ,GLUT_USE_CURRENT_CONTEXT);
    
    /* window2 */
//    glutInitWindowPosition(800, 0);   // ウィンドウの位置を指定
//    window2 = glutCreateWindow(WINDOW_NAME2);
//    glutDisplayFunc(glut_display2);
//    set_callback_functions();
//    set_texture();
//
    glutIdleFunc(glut_idle);
    init();
    
    glutMainLoop();
    return 0;
}

/////////////////////////////////////////////////////////////////////////
// display_function
/////////////////////////////////////////////////////////////////////////
void glut_display1(){
    glutSetWindow(window1);
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    glClearColor(bgcolor[0], bgcolor[1], bgcolor[2], bgcolor[3]); // 背景の塗りつぶし色を指定
    /* 3Dの描画 */
    three_d_shape.projection_transformation(WINDOW_WIDTH, WINDOW_HEIGHT);
    three_d_shape.modelview_transformation_win1(g_angle1, g_angle2, g_distance);
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    set_lighting();
    set_fog();
    
     #ifdef _DEBUG
        three_d_shape.show_axes();
     #endif
    
    glEnable(GL_TEXTURE_2D);
    draw_ground(); //地面
    draw_distant(); //遠景
    glDisable(GL_TEXTURE_2D);
    draw_moji(); //文字
    
//    glPushMatrix();
//    glScalef(100.0, 100.0, 100.0);
//    mesh.Draw();
//    glPopMatrix();
    
    glDisable(GL_LIGHT0);
    glDisable(GL_LIGHTING);
    glDisable(GL_FOG);
    glDisable(GL_DEPTH_TEST);
    
    /* 2Dの描画 */
    draw_menu();
    //glutGet(GLUT_ELAPSED_TIME);
    glutSwapBuffers();
}

void glut_display2(){
    glutSetWindow(window2);
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    glClearColor(bgcolor[0], bgcolor[1], bgcolor[2], bgcolor[3]); // 背景の塗りつぶし色を指定
    /* 3Dの描画 */
    three_d_shape.projection_transformation(WINDOW_WIDTH, WINDOW_HEIGHT);
    three_d_shape.modelview_transformation_win2(g_angle1, g_angle2, g_distance);
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    set_lighting();
    set_fog();
    
    #ifdef _DEBUG
        three_d_shape.show_axes();
    #endif
    
    glEnable(GL_TEXTURE_2D);
    draw_ground(); //地面
    draw_distant(); //遠景
    glDisable(GL_TEXTURE_2D);
    
    draw_moji(); //文字
    
    glDisable(GL_LIGHT0);
    glDisable(GL_LIGHTING);
    glDisable(GL_FOG);
    glDisable(GL_DEPTH_TEST);
    
    /* 2Dの描画 */
    draw_menu();
    //glutGet(GLUT_ELAPSED_TIME);
    glutSwapBuffers();
}

/////////////////////////////////////////////////////////////////////////
// draw_object
/////////////////////////////////////////////////////////////////////////
void draw_ground(){
    glPushMatrix();
    const btVector3& g_pos = bullet.ground->getCenterOfMassPosition();
    const btBoxShape* ground_shape = static_cast<const btBoxShape*>(bullet.ground->getCollisionShape());
    btVector3 g_halfExtent = ground_shape->getHalfExtentsWithMargin();
    glTranslatef(g_pos[0], g_pos[1], g_pos[2]);
    glScaled(2*g_halfExtent[0], 2*g_halfExtent[1], 2*g_halfExtent[2]);
    glBindTexture(GL_TEXTURE_2D, g_TextureHandles[0]);
    three_d_shape.draw_ground_plane();
    glPopMatrix();
}

void draw_distant(){
    glPushMatrix();
    glTranslated(0.0, 100.0, -300.0);
    glRotated(90, 1.0, 0.0, 0.0);
    glScalef(2.0, 1.0, 2.0);
    glBindTexture(GL_TEXTURE_2D, g_TextureHandles[2]);
    three_d_shape.draw_distant_plane();
    glPopMatrix();
}

void draw_moji(){
    
    for(int i=0; i<bullet.moji_boxes.size(); i++){
        glPushMatrix();
        btVector3 m_pos = bullet.moji_boxes[i]->getCenterOfMassPosition();
        const btBoxShape* moji_shape = static_cast<const btBoxShape*>(bullet.moji_boxes[i]->getCollisionShape());
        btVector3 mojiHalfExtent = moji_shape->getHalfExtentsWithMargin();
        string txt = bullet.moji_boxes[i]->getStr();
        
        set_material();
        //moji.DrawSolidStrings(txt, WINDOW_WIDTH, WINDOW_HEIGHT, m_pos[0]-2*mojiHalfExtent[0], m_pos[1]-2*mojiHalfExtent[1], m_pos[2]+2*mojiHalfExtent[2]);
        
        btScalar m[16];
        btMatrix3x3 rot;
        rot.setIdentity();
        
        btDefaultMotionState* myMotionState = (btDefaultMotionState*)bullet.moji_boxes[i]->getMotionState();
        myMotionState->m_graphicsWorldTrans.getOpenGLMatrix(m);
        rot=myMotionState->m_graphicsWorldTrans.getBasis();

        //glTranslatef(m_pos[0], m_pos[1], m_pos[2]);
        glMultMatrixf(m);
        
        
        moji.DrawSolidStrings(txt, WINDOW_WIDTH, WINDOW_HEIGHT, -mojiHalfExtent[0], -mojiHalfExtent[1], mojiHalfExtent[2]);
        
         #ifdef _DEBUG
            glScaled(2*mojiHalfExtent[0], 2*mojiHalfExtent[1], 2*mojiHalfExtent[2]);
            glutWireCube(1.0);
         #endif
        glPopMatrix();
    }
    
    //moji.PrintTextInfo(out_moji);
}

void draw_menu(){
    two_d_shape.projection_transformation();
    two_d_shape.draw_cursor_circle(mouse.x, mouse.y);
    moji.DrawGuiStrings(TITLE, -60, WINDOW_HEIGHT/2 - WINDOW_HEIGHT * 0.27, 0);
}

/////////////////////////////////////////////////////////////////////////
// event_function
/////////////////////////////////////////////////////////////////////////
void glut_idle(){
    bullet.idle();
    
    glutPostRedisplay();
    //glut_display1();
    //glut_display2();
}

void glut_keyboard(unsigned char key, int x, int y){
    switch (key)
    {
        case 'q':
        case 'Q':
        case '\033': // Escキーのこと
            mesh.Release();
            bullet.clean_Bullet();
            exit(0);
        case 'z':
            mPos.z++;
            break;
        case 'x':
            mPos.z--;
            break;
        case 'r':
            g_angle1 = 0.0;
            g_angle2 = 0.1;
            g_distance = 100.0;
            mPos = {-12, 4, 0};
            break;
        case 'i':
            cout << "input anything moji you like" << endl;
            cin >> out_moji;
            cout << "your change was reflected." << endl;
            bullet.add_moji_box(out_moji, moji.GetTextSize(out_moji), btVector3(2.0, 2.0, 0.0));
            break;
            
    }
    glutPostRedisplay();

}

void glut_keyboard(int key, int x, int y){
    switch (key)
    {
        case GLUT_KEY_LEFT:
            mPos.x--;
            break;
        case GLUT_KEY_RIGHT:
            mPos.x++;
            break;
        case GLUT_KEY_DOWN:
            mPos.y--;
            break;
        case GLUT_KEY_UP:
            mPos.y++;
            break;
    }
    glutPostRedisplay();  // 「ディスプレイのコールバック関数を呼んで」と指示する。
}

void glut_mouse(int button, int state, int x, int y){
    if(button == GLUT_LEFT_BUTTON){
        if(state == GLUT_UP){
            g_isLeftButtonOn = false;
        }else if(state == GLUT_DOWN){
            g_isLeftButtonOn = true;
        }
    }
    
    if(button == GLUT_RIGHT_BUTTON){
        if(state == GLUT_UP){
            g_isRightButtonOn = false;
        }else if(state == GLUT_DOWN){
            g_isRightButtonOn = true;
        }
    }

}

void glut_motion(int x, int y){
    static int px = -1, py = -1;
    
    if(g_isLeftButtonOn == true){
        if(px >= 0 && py >= 0){
            g_angle1 += (double)-(x - px)/30;
            float tmp = g_angle2 + (double)(y - py)/30;
            //カメラワークを制限
            if(tmp<0.1){
                g_angle2 = 0.1;
            }else if(tmp>1.35){
                g_angle2 = 1.35;
            }else{
                g_angle2 += (double)(y - py)/10;
            }
        }
        px = x;
        py = y;
    }else if(g_isRightButtonOn == true){
        if(px >= 0 && py >= 0){
            g_distance += (double)(y - py)/10;
        }
        px = x;
        py = y;
    }else{
        px = -1;
        py = -1;
    }
    
    mouse.x = x;
    mouse.y = y;
    glutPostRedisplay();
}

void glut_reshape(int w, int h){
    WINDOW_WIDTH = w;
    WINDOW_HEIGHT = h;
    glutPostRedisplay();
}
