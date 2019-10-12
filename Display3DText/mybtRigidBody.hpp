#ifndef mybtRigidBody_h
#define mybtRigidBody_h

#include <stdio.h>
#include <btBulletDynamicsCommon.h>

class mybtRigidBody : public btRigidBody{
public:
    using btRigidBody::btRigidBody;
    //mybtRigidBody(btScalar mass, btMotionState* motionState, btCollisionShape* collisionShape, const btVector3& localInertia = btVector3(0, 0, 0));
    
    std::string getStr();
    void setStr(std::string);
private:
    std::string str;
};

#endif 
