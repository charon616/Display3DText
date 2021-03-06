#ifndef dOBJ_hpp
#define dOBJ_hpp

#include <stdio.h>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <GLUT/glut.h>

#define OBJ_BUFFER_LENGTH 1024
#define OBJ_NAME_LENGTH 256

using namespace std;

/////////////////////////////////////////////////////////////////////////
// OBJVEC2
/////////////////////////////////////////////////////////////////////////
struct OBJVEC2
{
    float x;
    float y;
    OBJVEC2( float nx=0.0f, float ny=0.0f );
    operator float* ();
    operator const float* () const;
};

/////////////////////////////////////////////////////////////////////////
// OBJVEC3
/////////////////////////////////////////////////////////////////////////
struct OBJVEC3
{
    float x;
    float y;
    float z;
    OBJVEC3( float nx=0.0f, float ny=0.0f, float nz=0.0f );
    operator float* ();
    operator const float* () const;
};

/////////////////////////////////////////////////////////////////////////
// OBJVERTEX
/////////////////////////////////////////////////////////////////////////
struct OBJVERTEX
{
    OBJVEC2 texcoord;
    OBJVEC3 normal;
    OBJVEC3 position;
    OBJVERTEX(){}
};

/////////////////////////////////////////////////////////////////////////
// OBJSUBSET
/////////////////////////////////////////////////////////////////////////
struct OBJSUBSET
{
    unsigned int materialIndex;
    unsigned int faceStart;
    unsigned int faceCount;
    OBJSUBSET(){}
};

/////////////////////////////////////////////////////////////////////////
// OBJMATERIAL
/////////////////////////////////////////////////////////////////////////
struct OBJMATERIAL
{
    char name[OBJ_NAME_LENGTH];
    OBJVEC3 ambient;
    OBJVEC3 diffuse;
    OBJVEC3 specular;
    float shininess;
    float alpha;
    char ambientMapName[OBJ_NAME_LENGTH];
    char diffuseMapName[OBJ_NAME_LENGTH];
    char specularMapName[OBJ_NAME_LENGTH];
    char bumpMapName[OBJ_NAME_LENGTH];
    OBJMATERIAL(){}
};

/////////////////////////////////////////////////////////////////////////
// OBJBOUNDINGBOX
/////////////////////////////////////////////////////////////////////////
struct OBJBOUNDINGBOX
{
    OBJVEC3 maximum;
    OBJVEC3 minimum;
    OBJVEC3 size;
    void Merge( OBJVEC3 value );
    OBJBOUNDINGBOX() {}
    OBJBOUNDINGBOX( OBJVEC3 value );
};

/////////////////////////////////////////////////////////////////////////
// OBJBOUNDINGSPHERE
/////////////////////////////////////////////////////////////////////////
struct OBJBOUNDINGSPHERE
{
    OBJVEC3 center;
    float radius;
    void Create( OBJBOUNDINGBOX box );
    OBJBOUNDINGSPHERE() {}
};

/////////////////////////////////////////////////////////////////////////
// OBJMESH
/////////////////////////////////////////////////////////////////////////
class OBJMESH
{
private:
    OBJVERTEX* m_Vertices;
    OBJSUBSET* m_Subsets;
    OBJMATERIAL* m_Materials;
    unsigned int* m_Indices;
    unsigned int m_NumVertices;
    unsigned int m_NumSubsets;
    unsigned int m_NumMaterials;
    unsigned int m_NumIndices;
    OBJBOUNDINGBOX m_Box;
    OBJBOUNDINGSPHERE m_Sphere;
    char m_directoryPath[OBJ_NAME_LENGTH];
    
    bool LoadMTLFile( const char* filename );
    bool LoadOBJFile( const char* filename );
    
public:
    OBJMESH();
    ~OBJMESH();
    
    bool LoadFile( const char* filename );
    void Release();
    void Draw();
    unsigned int GetNumVertices();
    unsigned int GetNumSubsets();
    unsigned int GetNumMaterials();
    unsigned int GetNumIndices();
    unsigned int  GetIndexData( unsigned int index );
    unsigned int* GetIndices();
    OBJVERTEX  GetVertex( unsigned int index );
    OBJVERTEX* GetVertices();
    OBJSUBSET  GetSubset( unsigned int index );
    OBJSUBSET* GetSubsets();
    OBJMATERIAL  GetMaterial( unsigned int index );
    OBJMATERIAL* GetMaterials();
    OBJBOUNDINGBOX GetBox();
    OBJBOUNDINGSPHERE GetSphere();
};

#endif /* dOBJ_hpp */

