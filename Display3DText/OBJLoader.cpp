#include "OBJLoader.h"

using std::ifstream;
using std::ofstream;
using std::ios;
using std::cout;
using std::endl;
using std::vector;
using std::string;

// RtlZeroMemory Macro
#ifndef RtlZeroMemory
#define RtlZeroMemory(Destination,Length) memset((Destination),0,(Length))
#endif

// ZeroMemory Macro
#ifndef ZeroMemory
#define ZeroMemory RtlZeroMemory
#endif

// SAFE_DELETE_ARRAY
#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(x) { if (x) { delete [] (x); (x) = NULL; } }
#endif


// �}�e���A���̏�����
void InitMaterial(OBJMATERIAL* pMaterial)
{
	ZeroMemory(pMaterial, sizeof(OBJMATERIAL));
	pMaterial->ambient = Vertex3Df(0.2f, 0.2f, 0.2f);
	pMaterial->diffuse = Vertex3Df(0.8f, 0.8f, 0.8f);
	pMaterial->specular = Vertex3Df(1.0f, 1.0f, 1.0f);
	pMaterial->shininess = 0.0f;
	pMaterial->alpha = 1.0f;
}

// �}�e���A����ݒ肷��
void SetMaterial(OBJMATERIAL* pMaterial)
{
	glColor4f(pMaterial->diffuse.x, pMaterial->diffuse.y, pMaterial->diffuse.z, pMaterial->alpha);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, pMaterial->ambient.X);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, pMaterial->diffuse.X);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, pMaterial->specular.X);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &pMaterial->shininess);
}


/////////////////////////////////////////////////////////////////////////
// OBJMESH
/////////////////////////////////////////////////////////////////////////

// �R���X�g���N�^
OBJMESH::OBJMESH()
{
	m_NumVertices = 0;
	m_NumMaterials = 0;
	m_NumSubsets = 0;
	m_NumIndices = 0;

}


// �f�X�g���N�^
OBJMESH::~OBJMESH()
{
	Release();
};

// ��������j��
void OBJMESH::Release()
{
	m_Vertices.clear();
	vector<OBJVERTEX>().swap(m_Vertices);
	m_Materials.clear();
	vector<OBJMATERIAL>().swap(m_Materials);
	m_Subsets.clear();
	vector<OBJSUBSET>().swap(m_Subsets);
	m_Indices.clear();
	vector<unsigned int>().swap(m_Indices);
	m_texID.clear();
	vector<GLuint>().swap(m_texID);

	m_NumVertices = 0;
	m_NumMaterials = 0;
	m_NumSubsets = 0;
	m_NumIndices = 0;
}

// OBJ�t�@�C���̓ǂݍ���
bool OBJMESH::LoadOBJFile(const string _filename)
{
	ifstream file;

	string mtlFileName;
	char buf[OBJ_BUFFER_LENGTH] = { 0 };
	vector<Vertex3Df> positions;
	vector<Vertex3Df> normals;
	vector<Vertex2Df> texcoords;
	vector<OBJVERTEX> t_vertices;
	vector<OBJSUBSET> t_subsets;
	vector<unsigned int> t_indices;
	bool initBox = false;
	int prevSize = 0;

	unsigned long total = 0;

	OBJMATERIAL material;
	unsigned int dwFaceIndex = 0;
	unsigned int dwFaceCount = 0;
	unsigned int dwCurSubset = 0;

	//�@�f�B���N�g����؂���
	int direPlace = (int)_filename.rfind("\\");
	m_directoryPath = _filename.substr(0, direPlace + 1);

	//�@�t�@�C�����J��
	file.open(_filename, ios::in);

	//�@�`�F�b�N
	if (!file.is_open())
	{
		cout << "Error : �t�@�C���I�[�v���Ɏ��s" << endl;
		cout << "File Name : " << _filename << endl;
		return false;
	}

	//�@���[�v
	for (;;)
	{
		file >> buf;
		if (!file)
			break;

		//�@�R�����g
		if (0 == strcmp(buf, "#"))
		{
		}

		//�@���_���W
		else if (0 == strcmp(buf, "v"))
		{
			float x, y, z;
			file >> x >> y >> z;
			Vertex3Df v(x, y, z);
			positions.push_back(v);

			//�@�o�E���f�B���O�{�b�N�X�̏�����
			if (!initBox)
			{
				m_Box = OBJBOUNDINGBOX(v);
				initBox = true;
			}

			//�@�o�E���f�B���O�{�b�N�X�̎Z�o
			m_Box.Merge(v);
		}

		//�@�e�N�X�`�����W
		else if (0 == strcmp(buf, "vt"))
		{
			float u, v;
			file >> u >> v;
			texcoords.push_back(Vertex2Df(u, v));
		}

		//�@�@���x�N�g��
		else if (0 == strcmp(buf, "vn"))
		{
			float x, y, z;
			file >> x >> y >> z;
			normals.push_back(Vertex3Df(x, y, z));
		}

		//�@��
		else if (0 == strcmp(buf, "f"))
		{
			unsigned int iPosition, iTexCoord, iNormal;
			unsigned int p[4] = { -1 }, t[4] = { -1 }, n[4] = { -1 };
			OBJVERTEX vertex;
			dwFaceIndex++;
			dwFaceCount++;
			int count = 0;
			unsigned int index = 0;

			//�@�O�p�`�E�l�p�`�̂ݑΉ�
			for (int iFace = 0; iFace < 4; iFace++)
			{
				count++;	//�@���_���𐔂���
				ZeroMemory(&vertex, sizeof(OBJVERTEX));

				file >> iPosition;
				vertex.position = positions[iPosition - 1];
				p[iFace] = iPosition - 1;

				if ('/' == file.peek())
				{
					file.ignore();

					//�@�e�N�X�`�����W�C���f�b�N�X
					if ('/' != file.peek())
					{
						file >> iTexCoord;
						vertex.texcoord = texcoords[iTexCoord - 1];
						t[iFace] = iTexCoord - 1;
					}

					//�@�@���x�N�g���C���f�b�N�X
					if ('/' == file.peek())
					{
						file.ignore();

						file >> iNormal;
						vertex.normal = normals[iNormal - 1];
						n[iFace] = iNormal - 1;
					}
				}

				//�@�J�E���g��3����
				if (iFace < 3)
				{
					t_vertices.push_back(vertex);
					index = t_vertices.size() - 1;
					t_indices.push_back(index);
				}

				//�@�������s��������I��
				if ('\n' == file.peek())
				{
					break;
				}

			}

			//�@�l�p�`�|���S���̏ꍇ�C�O�p�`��ǉ�����
			if (count > 3)
			{
				//�@�J�E���g
				dwFaceIndex++;
				dwFaceCount++;

				//�@���_�ƃC���f�b�N�X��ǉ�
				for (int iFace = 1; iFace < 4; iFace++)
				{
					int j = (iFace + 1) % 4;
					ZeroMemory(&vertex, sizeof(OBJVERTEX));

					if (p[j] != -1) vertex.position = positions[p[j]];
					if (t[j] != -1) vertex.texcoord = texcoords[t[j]];
					if (n[j] != -1) vertex.normal = normals[n[j]];

					t_vertices.push_back(vertex);
					index = t_vertices.size() - 1;
					t_indices.push_back(index);
				}

			}
		}

		//�@�}�e���A���t�@�C��
		else if (0 == strcmp(buf, "mtllib"))
		{
			file >> mtlFileName;
			//�@�}�e���A���t�@�C���̓ǂݍ���
			if (mtlFileName[0])
			{
				if (!LoadMTLFile(m_directoryPath + mtlFileName))
				{
					cout << "Error : �}�e���A���̃��[�h�Ɏ��s" << endl;
					return false;
				}
			}
		}

		//�@�}�e���A��
		else if (0 == strcmp(buf, "usemtl"))
		{
			char strName[OBJ_NAME_LENGTH] = { 0 };
			file >> strName;
			OBJSUBSET subset;
			subset.faceCount = 1;

			for (unsigned int i = 0; i < m_NumMaterials; i++)
			{
				if (0 == strcmp(m_Materials[i].name, strName))
				{
					dwCurSubset = i;
					break;
				}
			}

			subset.materialIndex = dwCurSubset;
			subset.faceStart = dwFaceIndex * 3;
			prevSize = t_subsets.size();
			t_subsets.push_back(subset);
			if (t_subsets.size() > 1)
			{
				t_subsets[prevSize - 1].faceCount = dwFaceCount * 3;
				dwFaceCount = 0;
			}
		}

		file.ignore(OBJ_BUFFER_LENGTH, '\n');
	}

	//�@�T�u�Z�b�g
	if (t_subsets.size() > 0)
	{
		int maxSize = t_subsets.size();
		t_subsets[maxSize - 1].faceCount = dwFaceCount * 3;
	}

	//�@�t�@�C�������
	file.close();

	//�@���_�f�[�^���R�s�[
	m_NumVertices = t_vertices.size();
	m_Vertices.resize(m_NumVertices);
	for (unsigned int i = 0; i<m_NumVertices; i++)
		m_Vertices[i] = t_vertices[i];

	//�@�T�u�Z�b�g�f�[�^���R�s�[
	m_NumSubsets = t_subsets.size();
	m_Subsets.resize(m_NumSubsets);
	for (unsigned int i = 0; i<m_NumSubsets; i++)
		m_Subsets[i] = t_subsets[i];

	//�@�C���f�b�N�X�f�[�^���R�s�[
	m_NumIndices = t_indices.size();
	m_Indices.resize(m_NumIndices);
	for (unsigned int i = 0; i<m_NumIndices; i++)
		m_Indices[i] = t_indices[i];

	//�@�o�E���f�B���O�X�t�B�A�̍쐬
	m_Sphere.Create(m_Box);

	//�@�������j��
	positions.clear();
	normals.clear();
	texcoords.clear();
	t_vertices.clear();
	t_subsets.clear();
	t_indices.clear();

	//�@����I��
	return true;
}

// MTL�t�@�C���̓ǂݍ���
bool OBJMESH::LoadMTLFile(const string _filename)
{
	char buf[OBJ_BUFFER_LENGTH] = { 0 };
	int iMtlCount = -1;
	ifstream file;
	vector<OBJMATERIAL> t_materials;
	OBJMATERIAL material;
	InitMaterial(&material);

	//�@�t�@�C�����J��
	file.open(_filename, ios::in);

	//�@�`�F�b�N
	if (!file.is_open())
	{
		cout << "Error : �t�@�C���I�[�v���Ɏ��s���܂���" << endl;
		cout << "File Name : " << _filename << endl;
		return false;
	}

	//�@���[�v
	for (;;)
	{
		file >> buf;
		if (!file)
			break;

		// New Material
		if (0 == strcmp(buf, "newmtl"))
		{
			iMtlCount++;
			t_materials.push_back(material);
			char strName[OBJ_NAME_LENGTH] = { 0 };
			file >> strName;
			strcpy(t_materials[iMtlCount].name, strName);
		}
		// Ambient Color
		else if (0 == strcmp(buf, "Ka"))
		{
			float r, g, b;
			file >> r >> g >> b;
			t_materials[iMtlCount].ambient = Vertex3Df(r, g, b);
		}
		// Diffuse Color
		else if (0 == strcmp(buf, "Kd"))
		{
			float r, g, b;
			file >> r >> g >> b;
			t_materials[iMtlCount].diffuse = Vertex3Df(r, g, b);
		}
		// Specular Color
		else if (0 == strcmp(buf, "Ks"))
		{
			float r, g, b;
			file >> r >> g >> b;
			t_materials[iMtlCount].specular = Vertex3Df(r, g, b);
		}
		// Alpha
		else if (0 == strcmp(buf, "d") ||
			0 == strcmp(buf, "Tr"))
		{
			file >> t_materials[iMtlCount].alpha;
		}
		// Shininess
		else if (0 == strcmp(buf, "Ns"))
		{
			file >> t_materials[iMtlCount].shininess;
		}
		// Ambient Map
		else if (0 == strcmp(buf, "map_Ka"))
		{
			string mapKaName;
			file >> mapKaName;
			t_materials[iMtlCount].ambientMapName = m_directoryPath + mapKaName;
		}
		// Diffuse Map
		else if (0 == strcmp(buf, "map_Kd"))
		{
			string mapKdName;
			file >> mapKdName;
			t_materials[iMtlCount].diffuseMapName = m_directoryPath + mapKdName;
		}
		// Specular Map
		else if (0 == strcmp(buf, "map_Ks"))
		{
			string mapKsName;
			file >> mapKsName;
			t_materials[iMtlCount].specularMapName = m_directoryPath + mapKsName;
		}
		// Bump Map
		else if (0 == strcmp(buf, "map_Bump"))
		{
			string mapBumpName;
			file >> mapBumpName;
			t_materials[iMtlCount].bumpMapName = m_directoryPath + mapBumpName;
		}

		file.ignore(OBJ_BUFFER_LENGTH, '\n');
	}

	//�@�t�@�C�������
	file.close();

	//�@�}�e���A���f�[�^���R�s�[
	m_NumMaterials = t_materials.size();
	m_Materials.resize(m_NumMaterials);
	for (unsigned int i = 0; i<m_NumMaterials; i++)
		m_Materials[i] = t_materials[i];

	// texture�̓ǂݍ���(diffuseMap�݂̂ɑΉ��B�����Ƃ��͂ł��Ȃ�)
	if (m_Materials.size()>0 && m_Materials[0].diffuseMapName.size() > 3){
		for (int t_mate = 0; t_mate < m_Materials.size(); t_mate++){
			cv::Mat img = cv::imread(m_Materials[t_mate].diffuseMapName, -1);
			if (img.cols < 10){
				cout << "image read error: "<<m_Materials[t_mate].diffuseMapName << endl;
				continue;
			}

			// �ނ���4�`�����l���摜�ɕύX
			cv::flip(img, img, 0);
			if (img.channels() == 3) cv::cvtColor(img, img, CV_BGR2RGBA);
			else if (img.channels() == 4) cv::cvtColor(img, img, CV_BGRA2RGBA);

			// texture����
			GLuint t_texID = -1;
			glEnable(GL_TEXTURE_2D);
			glGenTextures(1, &t_texID);
			glBindTexture(GL_TEXTURE_2D, t_texID);

			// �e�N�X�`���摜�̓o�C�g�P�ʂɋl�ߍ��܂�Ă���
			glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

			// �e�N�X�`���̊��蓖��
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img.cols, img.rows, 0,GL_RGBA, GL_UNSIGNED_BYTE, img.data);

			// �e�N�X�`�����g��E�k��������@�̎w��
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

			// �e�N�X�`���������@�e�N�X�`���ɉA�e�����ڂ��邩�ǂ���
			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

			glDisable(GL_TEXTURE_2D);
			m_texID.push_back(t_texID);
		}
	}

	//�@����I��
	return true;
}

// ���b�V���t�@�C���̓ǂݍ���
bool OBJMESH::LoadFile(const string _filename)
{
	cout << "OBJ�t�@�C���ǂݍ���(" << _filename << ") -> ";
	//�@OBJ, MTL�t�@�C����ǂݍ���
	if (!LoadOBJFile(_filename))
	{
		cout << "Error : ���b�V���t�@�C���̓ǂݍ��݂Ɏ��s���܂���" << endl;
		return false;
	}
	cout << "������" << endl;

	//�@����I��
	return true;
}


// �`�揈��
void OBJMESH::Draw()
{
	for (unsigned int i = 0; i<m_NumSubsets; i++)
	{
		//�@�}�e���A��
		OBJMATERIAL* pMat = &m_Materials[m_Subsets[i].materialIndex];
		SetMaterial(pMat);
		
		// texture
		if (pMat->diffuseMapName.size() > 3){
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, m_texID[m_Subsets[i].materialIndex]);
		}

		//�@�O�p�`�`��
		glInterleavedArrays(GL_T2F_N3F_V3F, 0, m_Vertices.data());
		glDrawElements(GL_TRIANGLES, m_Subsets[i].faceCount, GL_UNSIGNED_INT, &m_Indices[m_Subsets[i].faceStart]);

		// texture
		if (pMat->diffuseMapName.size() > 3){
			glDisable(GL_TEXTURE_2D);
		}
	}
}
