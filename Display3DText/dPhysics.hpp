#ifndef dPhysics_h
#define dPhysics_h

#include <iostream>
#include <stdio.h>
#include <btBulletDynamicsCommon.h>
#include "mybtRigidBody.hpp"
#include "dMoji.hpp"

class dPhysics{
    public:
    
        void init_bullet();
//        void add_ground(btRigidBody* ground);
        void add_ground();
        void add_sphere();
        void add_box();
        void add_moji_box(std::string m, btVector3 m_size, btVector3 impulse);
    
        void idle();
        void clean_Bullet();
        btRigidBody* ground;
        btRigidBody* sphere;
        btRigidBody* box;
        std::vector<mybtRigidBody*> moji_boxes;
        
    private:
        btDiscreteDynamicsWorld* world;
        btAlignedObjectArray<btCollisionShape*> collisionShapes;
        int g_iStep = 0;
    
};

#endif
