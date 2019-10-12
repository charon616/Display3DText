#ifndef dMaterial_h
#define dMaterial_h

#include <stdio.h>
#include <GLUT/glut.h>

struct MaterialStruct {
    GLfloat ambient[4];
    GLfloat diffuse[4];
    GLfloat specular[4];
    GLfloat shininess;
};
//jade(翡翠)
static MaterialStruct ms_jade = {
    {0.135,     0.2225,   0.1575,   1.0},
    {0.54,      0.89,     0.63,     1.0},
    {0.316228,  0.316228, 0.316228, 1.0},
    12.8
};
//ruby(ルビー)
static MaterialStruct ms_ruby  = {
    {0.1745,   0.01175,  0.01175,   1.0},
    {0.61424,  0.04136,  0.04136,   1.0},
    {0.727811, 0.626959, 0.626959,  1.0},
    76.8
};

static GLdouble gui_color[] = {255/255.0, 204/255.0, 51/255.0,  1.0};
static GLfloat bgcolor[] = {145/255.0, 232/255.0, 251/255.0, 0.0};

#endif
