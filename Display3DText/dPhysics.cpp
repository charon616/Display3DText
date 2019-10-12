#include "dPhysics.hpp"

void dPhysics::init_bullet(){
    // ブロードフェーズ法の設定(Dynamic AABB tree method)
    btBroadphaseInterface* broadphase = new btDbvtBroadphase();
    // 衝突検出方法の選択(デフォルトを選択)
    btDefaultCollisionConfiguration* config = new btDefaultCollisionConfiguration();
    btCollisionDispatcher* dispatcher = new btCollisionDispatcher(config);
    // 拘束(剛体間リンク)のソルバ設定
    btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver();
    // Bulletのワールド作成
    world = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, config);
    world->setGravity(btVector3(0, -10, 0));
}
//
//void dPhysics::add_ground(btRigidBody* ground){

void dPhysics::add_ground(){
    /* 地面 */
    btCollisionShape *ground_shape = new btBoxShape(btVector3(btScalar(50.), btScalar(1.), btScalar(50.)));
    collisionShapes.push_back(ground_shape);
    
    btTransform groundTransform;
    groundTransform.setIdentity();
    groundTransform.setOrigin(btVector3(0, 0, 0));
    
    btScalar mass(0.);
    
    btVector3 inertia(0, 0, 0);
    
    btDefaultMotionState* motion_state = new btDefaultMotionState(groundTransform);
    
    ground = new btRigidBody(mass, motion_state, ground_shape, inertia);
    ground->setRestitution(0.3);
    // ワールドに剛体オブジェクトを追加
    world->addRigidBody(ground);
    
    ground->setFriction(0.2);
}

void dPhysics::add_sphere(){
    /* 球体 */
    btVector3 pos = btVector3(0.0, 80, 0.0);    //!< 中心座標
    btScalar mass = 0.05;                    //!< 質量
    btScalar restitution = 0.8;                //!< 反発係数
    // 球体形状を設定
    btCollisionShape *sphere_shape = new btSphereShape(3.0);
    collisionShapes.push_back(sphere_shape);
    // 球体の初期位置・姿勢
    btQuaternion qrot(0, 0, 0, 1);
    btDefaultMotionState* motion_state = new btDefaultMotionState(btTransform(qrot, pos));
    // 慣性モーメントの計算
    btVector3 inertia(0, 0, 0);
    sphere_shape->calculateLocalInertia(mass, inertia);
    // 剛体オブジェクト生成(質量，位置姿勢，形状，慣性モーメントを設定)
    sphere = new btRigidBody(mass, motion_state, sphere_shape, inertia);
    sphere->setRestitution(restitution);
    sphere->setFriction(0.3);
    // ワールドに剛体オブジェクトを追加
    world->addRigidBody(sphere);
}

void dPhysics::add_box(){
    /* 立方体 */
    btVector3 pos = btVector3(0.0, 80, 0.0);    //!< 中心座標
    btScalar mass = 0.05;                    //!< 質量
    btScalar restitution = 0.8;                //!< 反発係数
    
    // 形状を設定
    btCollisionShape *box_shape = new btBoxShape(btVector3(btScalar(1.), btScalar(1.), btScalar(1.)));
    collisionShapes.push_back(box_shape);
    
    // 直方体の初期位置・姿勢
    btQuaternion qrot(0, 0, 0, 1);
    btDefaultMotionState* motion_state = new btDefaultMotionState(btTransform(qrot, pos));
    // 慣性モーメントの計算
    btVector3 inertia(0, 0, 0);
    box_shape->calculateLocalInertia(mass, inertia);
    // 剛体オブジェクト生成(質量，位置姿勢，形状，慣性モーメントを設定)
    box = new btRigidBody(mass, motion_state, box_shape, inertia);
    box->setRestitution(restitution);
    box->setFriction(0.3);
    // ワールドに剛体オブジェクトを追加
    world->addRigidBody(box);
}

void dPhysics::add_moji_box(std::string m, btVector3 m_size, btVector3 impulse){
    /* 文字と同じサイズの直方体 */
    btVector3 pos = btVector3(-20.0, 80.0, 0.0);    //!< 中心座標
    btScalar mass = 0.8;                    //!< 質量
    btScalar restitution = 0.3;                //!< 反発係数
    
    // 形状を設定
    //文字の厚みと同じ厚みの立方体を作成
    btCollisionShape *moji_shape = new btBoxShape(btVector3(m_size.x()/2, m_size.y()/2, m_size.z()/2));
    collisionShapes.push_back(moji_shape);
    
    // 直方体の初期位置・姿勢
    btQuaternion qrot(0.0, 0.6, 0, 0.7);
    btDefaultMotionState* motion_state = new btDefaultMotionState(btTransform(qrot, pos));
    // 慣性モーメントの計算
    btVector3 inertia(0, 0, 0);
    moji_shape->calculateLocalInertia(mass, inertia);
    // 剛体オブジェクト生成(質量，位置姿勢，形状，慣性モーメントを設定)
    mybtRigidBody* tmp;
    tmp = new mybtRigidBody(mass, motion_state, moji_shape, inertia);
    tmp->setStr(m);
    tmp->setRestitution(restitution);
    tmp->setFriction(0.3);
    // ワールドに剛体オブジェクトを追加
    world->addRigidBody(tmp);
    moji_boxes.push_back(tmp);
    
    tmp->applyCentralImpulse(impulse);
}

void dPhysics::idle(){
    if(g_iStep == 0){
    }
    
    // bulletのステップを進める
    if(world){
        world->stepSimulation(1.f / 60.f, 1);
    }
    
    g_iStep++;
}

void dPhysics::clean_Bullet(){
    // オブジェクト破棄
    for (int i = world->getNumCollisionObjects() - 1; i >= 0; i--)
    {
        btCollisionObject* obj = world->getCollisionObjectArray()[i];
        btRigidBody* body = btRigidBody::upcast(obj);
        if (body && body->getMotionState())
        {
            delete body->getMotionState();
        }
        world->removeCollisionObject(obj);
        delete obj;
    }
    
    for (int j = 0; j < collisionShapes.size(); j++)
    {
        btCollisionShape* shape = collisionShapes[j];
        collisionShapes[j] = 0;
        delete shape;
    }
    collisionShapes.clear();
    
    // ワールド破棄
    delete world->getBroadphase();
    delete world;
}




