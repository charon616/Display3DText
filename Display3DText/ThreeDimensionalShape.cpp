#include "ThreeDimensionalShape.hpp"

using namespace std;

ThreeDimensionalShape::ThreeDimensionalShape(){
    random_device rd;
    mt19937 mt(rd()); // シードをランダムに決定した擬似乱数生成器
    uniform_real_distribution<double> c(0.0, 1.0); //一様分布
    for(int j=0; j<6;j++){
        for(int i=0; i<3;i++){
            cube_color[j][i] = c(mt);
        }
    }
};
ThreeDimensionalShape::~ThreeDimensionalShape(){
};

void ThreeDimensionalShape::projection_transformation(int w, int h){
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();// 射影変換行列を復元
    glLoadIdentity();
    float aspect = (float)w/(float)h;
    gluPerspective(45.0, aspect, 1.0, 10000000);
}

void ThreeDimensionalShape::modelview_transformation(double angle1, double angle2, double distance){
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();// モデルビュー行列を復元
    glLoadIdentity();
 
        gluLookAt(distance * cos(angle2) * sin(angle1),
                  distance * sin(angle2),
                  distance * cos(angle2) * cos(angle1),
                  0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}

void ThreeDimensionalShape::modelview_transformation_win1(double angle1, double angle2, double distance){
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();// モデルビュー行列を復元
    glLoadIdentity();
    
    gluLookAt(distance * cos(angle2) * sin(angle1) - 20,
              distance * sin(angle2),
              distance * cos(angle2) * cos(angle1),
              -20.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}

void ThreeDimensionalShape::modelview_transformation_win2(double angle1, double angle2, double distance){
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();// モデルビュー行列を復元
    glLoadIdentity();
    
    gluLookAt(distance * cos(angle2) * sin(angle1) + 20,
              distance * sin(angle2),
              distance * cos(angle2) * cos(angle1),
              20.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}

void ThreeDimensionalShape::modelview_transformation2(double angle1, double angle2, double distance){
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();// モデルビュー行列を復元
    glLoadIdentity();
    
    gluLookAt(0.0, distance, distance, angle1, angle2, 0.0, 0.0, 1.0, 0.0);
}

void ThreeDimensionalShape::show_axes(){
    glBegin(GL_LINES);
    glColor3d(1.0, 0.0, 0.0);
    glVertex3d(100.0, 0.0, 0.0);
    glColor3d(0.5, 0.0, 0.0);
    glVertex3d(-100.0, 0.0, 0.0);
    
    glColor3d(0.0, 1.0, 0.0);
    glVertex3d(0.0, 100.0, 0.0);
    glColor3d(0.0, 0.5, 0.0);
    glVertex3d(0.0, -100.0, 0.0);
    
    glColor3d(0.0, 0.0, 1.0);
    glVertex3d(0.0, 0.0, 100.0);
    glColor3d(0.0, 0.0, 0.5);
    glVertex3d(0.0, 0.0, -100.0);
    glEnd();
};

void ThreeDimensionalShape::draw_pyramid()
{
    GLdouble pointO[] = {0.0, 1.0, 0.0};
    GLdouble pointA[] = {1.5, -1.0, 1.5};
    GLdouble pointB[] = {-1.5, -1.0, 1.5};
    GLdouble pointC[] = {-1.5, -1.0, -1.5};
    GLdouble pointD[] = {1.5, -1.0, -1.5};
    
    glColor3d(1.0, 0.0, 0.0);

    glBegin(GL_TRIANGLE_FAN);
    glVertex3dv(pointO);
    glColor3d(0.0, 1.0, 1.0);
    glVertex3dv(pointA);
    glColor3d(1.0, 0.0, 1.0);
    glVertex3dv(pointB);
    glColor3d(1.0, 1.0, 0.0);
    glVertex3dv(pointC);
    glColor3d(0.0, 0.0, 1.0);
    glVertex3dv(pointD);
    glColor3d(0.0, 1.0, 0.0);
    glVertex3dv(pointA);
    glEnd();
    
    glColor3d(1.0, 1.0, 1.0);
    glBegin(GL_POLYGON);
    glVertex3dv(pointA);
    glVertex3dv(pointB);
    glVertex3dv(pointC);
    glVertex3dv(pointD);
    glEnd();
}

void ThreeDimensionalShape::draw_pyramid_lt()
{
    GLdouble pointO[] = {0.0, 1.0, 0.0};
    GLdouble pointA[] = {1.5, -1.0, 1.5};
    GLdouble pointB[] = {-1.5, -1.0, 1.5};
    GLdouble pointC[] = {-1.5, -1.0, -1.5};
    GLdouble pointD[] = {1.5, -1.0, -1.5};
    
    GLfloat facecolor1[] = {1.0, 0.0, 0.0, 0.8};
    GLfloat facecolor2[] = {1.0, 1.0, 0.0, 0.8};
    GLfloat facecolor3[] = {0.0, 1.0, 1.0, 0.8};
    GLfloat facecolor4[] = {1.0, 0.0, 1.0, 0.8};
    GLfloat facecolor5[] = {1.0, 1.0, 1.0, 1.0};
    GLfloat specular[] = {0.0, 0.0, 0.0, 1.0};
    
    glMaterialfv(GL_FRONT, GL_AMBIENT, facecolor1);
    glNormal3d(0.0, 0.6, 0.8);
    glBegin(GL_TRIANGLES);
    glVertex3dv(pointO);
    glVertex3dv(pointA);
    glVertex3dv(pointB);
    glEnd();
    
    glMaterialfv(GL_FRONT, GL_DIFFUSE, facecolor2);
    glNormal3d(-0.8, 0.6, 0.0);
    glBegin(GL_TRIANGLES);
    glVertex3dv(pointO);
    glVertex3dv(pointB);
    glVertex3dv(pointC);
    glEnd();
    
    glMaterialfv(GL_FRONT, GL_DIFFUSE, facecolor3);
    glNormal3d(0.0, 0.6, -0.8);
    glBegin(GL_TRIANGLES);
    glVertex3dv(pointO);
    glVertex3dv(pointC);
    glVertex3dv(pointD);
    glEnd();
    
    glMaterialfv(GL_FRONT, GL_DIFFUSE, facecolor4);
    glNormal3d(0.8, 0.6, 0.0);
    glBegin(GL_TRIANGLES);
    glVertex3dv(pointO);
    glVertex3dv(pointD);
    glVertex3dv(pointA);
    glEnd();
    
    glMaterialfv(GL_FRONT, GL_DIFFUSE, facecolor5);
    glNormal3d(0.0, -1.0, 0.0);
    glBegin(GL_POLYGON);
    glVertex3dv(pointA);
    glVertex3dv(pointB);
    glVertex3dv(pointC);
    glVertex3dv(pointD);
    glEnd();
}

void ThreeDimensionalShape::draw_cube(){
    GLdouble vertices[][3] = {
        {0.0, 0.0, 0.0},
        {1.0, 0.0, 0.0},
        {1.0, 1.0, 0.0},
        {0.0, 1.0, 0.0},
        {0.0, 0.0, 1.0},
        {1.0, 0.0, 1.0},
        {1.0, 1.0, 1.0},
        {0.0, 1.0, 1.0},
    };
    
    int face[][4] = {//面の定義
        { 0, 1, 2, 3 },
        { 2, 3, 7, 6 },
        { 3, 7, 4, 0 },
        { 1, 2, 6, 5 },
        { 5, 6, 7, 4 },
        { 0, 4, 5, 1 }
    };

    
    glPushMatrix();
    glScalef(2.0, 2.0, 2.0);
    glTranslated(-0.5, -0.5, -0.5);//平行移動値の設定
    
    for (int j = 0; j < 6; ++j) {
        glBegin(GL_POLYGON);
        for (int i = 0; i < 4; ++i) {
            //glColor3d(cube_color[j][0], cube_color[j][1], cube_color[j][2]);
            glVertex3dv(vertices[face[j][i]]);
            
        }
        glEnd();
    }

    
    glPopMatrix();

}

void ThreeDimensionalShape::draw_ground_plane(){
    GLfloat face_color[] = {1.0, 1.0, 1.0, 1.0};
    glMaterialfv(GL_FRONT, GL_AMBIENT, face_color);
    glBegin(GL_QUADS);
    glTexCoord2d(0.0, 0.0);
    glVertex3f(-0.5 , 0 , -0.5);
    glTexCoord2d(0.0, 1.0);
    glVertex3f(-0.5 , 0 , 0.5);
    glTexCoord2d(1.0, 1.0);
    glVertex3f(0.5 , 0 , 0.5);
    glTexCoord2d(1.0, 0.0);
    glVertex3f(0.5 , 0 , -0.5);
    glEnd();
}

void ThreeDimensionalShape::draw_distant_plane(){
    GLfloat face_color[] = {1.0, 1.0, 1.0, 1.0};
    glMaterialfv(GL_FRONT, GL_AMBIENT, face_color);
    glBegin(GL_QUADS);
    glTexCoord2d(0.0, 0.0);
    glVertex3f(-100 , 0 , -100);
    glTexCoord2d(0.0, 1.0);
    glVertex3f(-100 , 0 , 100);
    glTexCoord2d(1.0, 1.0);
    glVertex3f(100 , 0 , 100);
    glTexCoord2d(1.0, 0.0);
    glVertex3f(100 , 0 , -100);
    glEnd();
}

void ThreeDimensionalShape::draw_tree(){
    GLfloat facecolor[] = {0.9, 0.9, 0.9, 1.0};
    glMaterialfv(GL_FRONT, GL_DIFFUSE, facecolor);
    glNormal3d(0.0, 1.0, 0.0);
    
    //glutSolidDodecahedron(1.0);
}

void ThreeDimensionalShape::my_scene(){
    /* draw Table */
    glColor3d(255/255.0, 133/255.0, 44/255.0);
    glPushMatrix();
    glTranslatef(-2.0 , 0.1, 2.0);
    glScalef(2.0, 0.1, 2.0);
    draw_cube();
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(-3.8 , -2.0, 3.8);
    glScalef(0.1, 2.0, 0.1);
    draw_cube();
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(-3.8 , -2.0, 0.2);
    glScalef(0.1, 2.0, 0.1);
    draw_cube();
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(-0.2 , -2.0, 3.8);
    glScalef(0.1, 2.0, 0.1);
    draw_cube();
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(-0.2 , -2.0, 0.2);
    glScalef(0.1, 2.0, 0.1);
    draw_cube();
    glPopMatrix();
    
    /* draw Chair */
    glColor3d(175/255.0, 148/255.0, 128/255.0);
    GLUquadricObj *cylinder;
    cylinder = gluNewQuadric();
    
    glPushMatrix();
    glTranslatef(1.3 , -1.0, 2.0);
    glRotatef(90, 1.0, 0.0, 0.0);
    gluCylinder(cylinder, 1.0, 1.0, 0.2, 10, 16);
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(1.3 , -1.0, 1.5);
    glRotatef(10, 1.0, 0.0, 0.0);
    glRotatef(90, 1.0, 0.0, 0.0);
    gluCylinder(cylinder, 0.1, 0.1, 3.0, 16, 16);
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(0.8 , -1.0, 2.4);
    glRotatef(-10, 0.5, 0.0, 0.5);
    glRotatef(90, 1.0, 0.0, 0.0);
    gluCylinder(cylinder, 0.1, 0.1, 3.0, 16, 16);
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(1.8 , -1.0, 2.4);
    glRotatef(10, -0.5, 0.0, 0.5);
    glRotatef(90, 1.0, 0.0, 0.0);
    gluCylinder(cylinder, 0.1, 0.1, 3.0, 16, 16);
    gluDeleteQuadric(cylinder);
    glPopMatrix();
    
    /* draw Teapot */
    glColor3d(57/255.0, 106/255.0, 114/255.0);
    glPushMatrix();
    glTranslatef(-3.0 , 0.6, 1.5);
    glutSolidTeapot(0.5);
    glPopMatrix();
    
}
