#pragma once

// OpenCV---------------------------------------------------------------------------------------
//#include <opencv2/opencv.hpp>        // インクルードファイル指定
//#include <opencv2/opencv_lib.hpp>    // 静的リンクライブラリの指定

// OpenGL --------------------------------------------------------------------------------------
#include <GL/glew.h>
#include <GLUT/glut.h>
#pragma comment(lib, "glew.lib")
#pragma comment(lib, "glut32.lib")
//// MyHeader ------------------------------------------------------------------------------------
//#include "AsaVertex.h"
//#include "AsaTrackball.h"

// Defines
#define OBJ_BUFFER_LENGTH 1024
#define OBJ_NAME_LENGTH 256


/////////////////////////////////////////////////////////////////////////
// OBJVERTEX
/////////////////////////////////////////////////////////////////////////
struct OBJVERTEX
{
	glVertex2d texcoord;
	glVertex3d normal;
	glVertex3d position;
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
	glVertex3d ambient;
	glVertex3d diffuse;
	glVertex3d specular;
	float shininess;
	double alpha;
	std::string ambientMapName;
	std::string diffuseMapName;
	std::string specularMapName;
	std::string bumpMapName;
	OBJMATERIAL(){}
};

/////////////////////////////////////////////////////////////////////////
// OBJBOUNDINGBOX
/////////////////////////////////////////////////////////////////////////
struct OBJBOUNDINGBOX
{
	glVertex3d maximum;
	glVertex3d minimum;
	glVertex3d size;

	// 最大値を求める
	glVertex3d OBJVEC3Max(glVertex3d v, glVertex3d _max)
	{
		glVertex3d result;
		result.x = (v.x > _max.x ? v.x : _max.x);
		result.y = (v.y > _max.y ? v.y : _max.y);
		result.z = (v.z > _max.z ? v.z : _max.z);
		return result;
	}

	// 最小値を求める
	glVertex3d OBJVEC3Min(glVertex3d v, glVertex3d _min)
	{
		glVertex3d result;
		result.x = (v.x < _min.x ? v.x : _min.x);
		result.y = (v.y < _min.y ? v.y : _min.y);
		result.z = (v.z < _min.z ? v.z : _min.z);
		return result;
	}

	void Merge(glVertex3d value){
		maximum = OBJVEC3Max(value, maximum);
		minimum = OBJVEC3Min(value, minimum);
		size = maximum - minimum;
	}
	OBJBOUNDINGBOX() {}
	OBJBOUNDINGBOX(glVertex3d value) : minimum(value), maximum(value){}
};

/////////////////////////////////////////////////////////////////////////
// OBJBOUNDINGSPHERE
/////////////////////////////////////////////////////////////////////////
struct OBJBOUNDINGSPHERE
{
	glVertex3d center;
	double radius;
	void Create(OBJBOUNDINGBOX box){
		center = box.maximum + box.minimum;
		center = center / 2.0;

		glVertex3d half;
		half = box.maximum - center;
		radius = half.abs();
	}
	OBJBOUNDINGSPHERE() {}
};

/////////////////////////////////////////////////////////////////////////
// OBJMESH
/////////////////////////////////////////////////////////////////////////
class OBJMESH
{
private:
	std::vector<OBJVERTEX> m_Vertices;
	std::vector<OBJSUBSET> m_Subsets;
	std::vector<OBJMATERIAL> m_Materials;
	std::vector<GLuint> m_texID;
	std::vector<unsigned int> m_Indices;
	unsigned int m_NumVertices;
	unsigned int m_NumSubsets;
	unsigned int m_NumMaterials;
	unsigned int m_NumIndices;
	OBJBOUNDINGBOX m_Box;
	OBJBOUNDINGSPHERE m_Sphere;
	std::string m_directoryPath;

	bool LoadMTLFile(const std::string _filename);
	bool LoadOBJFile(const std::string _filename);

public:
	OBJMESH();
	~OBJMESH();

	bool LoadFile(const std::string _filename);
	void Release();
	void Draw();
	unsigned int GetNumVertices(){ return m_NumVertices; }
	unsigned int GetNumSubsets(){ return m_NumSubsets; }
	unsigned int GetNumMaterials(){ return m_NumMaterials; }
	unsigned int GetNumIndices(){ return m_NumIndices; }
	unsigned int  GetIndexData(unsigned int index){ return m_Indices[index]; }
	unsigned int* GetIndices(){ return m_Indices.data(); }
	OBJVERTEX  GetVertex(unsigned int index){ return m_Vertices[index]; }
	OBJVERTEX* GetVertices(){ return m_Vertices.data(); }
	OBJSUBSET  GetSubset(unsigned int index){ return m_Subsets[index]; }
	OBJSUBSET* GetSubsets(){ return m_Subsets.data(); }
	OBJMATERIAL  GetMaterial(unsigned int index){ return m_Materials[index]; }
	OBJMATERIAL* GetMaterials(){ return m_Materials.data(); }
	OBJBOUNDINGBOX GetBox(){ return m_Box; }
	OBJBOUNDINGSPHERE GetSphere(){ return m_Sphere; }
};
