#ifndef ThreeDimensionalShape_h
#define ThreeDimensionalShape_h

#define _USE_MATH_DEFINES
#include <iostream>
#include <iomanip>
#include <math.h>
#include <random>
#include <stdio.h>
#include <GLUT/glut.h>
#define GL_SILENCE_DEPRECATION

class ThreeDimensionalShape{
    
    public:
        ThreeDimensionalShape();
        ~ThreeDimensionalShape();
    
        void projection_transformation(int w, int h);
        void modelview_transformation(double angle1, double angle2, double distance);
        void modelview_transformation_win1(double angle1, double angle2, double distance);
        void modelview_transformation_win2(double angle1, double angle2, double distance);
        void modelview_transformation2(double angle1, double angle2, double distance);
        void show_axes();
    
        void draw_pyramid();
        void draw_pyramid_lt();
        void draw_cube();
        void draw_ground_plane();
        void draw_distant_plane();
        void draw_tree();
    
        void my_scene();
    
    private:
        float cube_color[6][3];

};

#endif
