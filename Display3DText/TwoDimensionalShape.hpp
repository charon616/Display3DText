#ifndef TwoDimensionalShape_h
#define TwoDimensionalShape_h

#define _USE_MATH_DEFINES
#include <iostream>
#include <iomanip>
#include <math.h>
#include <random>
#include <stdio.h>
#include <GLUT/glut.h>
#define GL_SILENCE_DEPRECATION

class TwoDimensionalShape{
    public:
        TwoDimensionalShape();
        ~TwoDimensionalShape();
    
        void projection_transformation();
        void draw_square1();
        void draw_square2();
        void draw_square3();
        void draw_n_square(int n);
        void draw_cursor_circle(int x, int y);
    
    private:
        const float radius = 0.9; // n角形の外接円の半径
    
};

#endif
