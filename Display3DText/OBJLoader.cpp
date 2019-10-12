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


// マテリアルの初期化
void InitMaterial(OBJMATERIAL* pMaterial)
{
	ZeroMemory(pMaterial, sizeof(OBJMATERIAL));
	pMaterial->ambient = Vertex3Df(0.2f, 0.2f, 0.2f);
	pMaterial->diffuse = Vertex3Df(0.8f, 0.8f, 0.8f);
	pMaterial->specular = Vertex3Df(1.0f, 1.0f, 1.0f);
	pMaterial->shininess = 0.0f;
	pMaterial->alpha = 1.0f;
}

// マテリアルを設定する
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

// コンストラクタ
OBJMESH::OBJMESH()
{
	m_NumVertices = 0;
	m_NumMaterials = 0;
	m_NumSubsets = 0;
	m_NumIndices = 0;

}


// デストラクタ
OBJMESH::~OBJMESH()
{
	Release();
};

// メモリを破棄
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

// OBJファイルの読み込み
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

	//　ディレクトリを切り取り
	int direPlace = (int)_filename.rfind("\\");
	m_directoryPath = _filename.substr(0, direPlace + 1);

	//　ファイルを開く
	file.open(_filename, ios::in);

	//　チェック
	if (!file.is_open())
	{
		cout << "Error : ファイルオープンに失敗" << endl;
		cout << "File Name : " << _filename << endl;
		return false;
	}

	//　ループ
	for (;;)
	{
		file >> buf;
		if (!file)
			break;

		//　コメント
		if (0 == strcmp(buf, "#"))
		{
		}

		//　頂点座標
		else if (0 == strcmp(buf, "v"))
		{
			float x, y, z;
			file >> x >> y >> z;
			Vertex3Df v(x, y, z);
			positions.push_back(v);

			//　バウンディングボックスの初期化
			if (!initBox)
			{
				m_Box = OBJBOUNDINGBOX(v);
				initBox = true;
			}

			//　バウンディングボックスの算出
			m_Box.Merge(v);
		}

		//　テクスチャ座標
		else if (0 == strcmp(buf, "vt"))
		{
			float u, v;
			file >> u >> v;
			texcoords.push_back(Vertex2Df(u, v));
		}

		//　法線ベクトル
		else if (0 == strcmp(buf, "vn"))
		{
			float x, y, z;
			file >> x >> y >> z;
			normals.push_back(Vertex3Df(x, y, z));
		}

		//　面
		else if (0 == strcmp(buf, "f"))
		{
			unsigned int iPosition, iTexCoord, iNormal;
			unsigned int p[4] = { -1 }, t[4] = { -1 }, n[4] = { -1 };
			OBJVERTEX vertex;
			dwFaceIndex++;
			dwFaceCount++;
			int count = 0;
			unsigned int index = 0;

			//　三角形・四角形のみ対応
			for (int iFace = 0; iFace < 4; iFace++)
			{
				count++;	//　頂点数を数える
				ZeroMemory(&vertex, sizeof(OBJVERTEX));

				file >> iPosition;
				vertex.position = positions[iPosition - 1];
				p[iFace] = iPosition - 1;

				if ('/' == file.peek())
				{
					file.ignore();

					//　テクスチャ座標インデックス
					if ('/' != file.peek())
					{
						file >> iTexCoord;
						vertex.texcoord = texcoords[iTexCoord - 1];
						t[iFace] = iTexCoord - 1;
					}

					//　法線ベクトルインデックス
					if ('/' == file.peek())
					{
						file.ignore();

						file >> iNormal;
						vertex.normal = normals[iNormal - 1];
						n[iFace] = iNormal - 1;
					}
				}

				//　カウントが3未満
				if (iFace < 3)
				{
					t_vertices.push_back(vertex);
					index = t_vertices.size() - 1;
					t_indices.push_back(index);
				}

				//　次が改行だったら終了
				if ('\n' == file.peek())
				{
					break;
				}

			}

			//　四角形ポリゴンの場合，三角形を追加する
			if (count > 3)
			{
				//　カウント
				dwFaceIndex++;
				dwFaceCount++;

				//　頂点とインデックスを追加
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

		//　マテリアルファイル
		else if (0 == strcmp(buf, "mtllib"))
		{
			file >> mtlFileName;
			//　マテリアルファイルの読み込み
			if (mtlFileName[0])
			{
				if (!LoadMTLFile(m_directoryPath + mtlFileName))
				{
					cout << "Error : マテリアルのロードに失敗" << endl;
					return false;
				}
			}
		}

		//　マテリアル
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

	//　サブセット
	if (t_subsets.size() > 0)
	{
		int maxSize = t_subsets.size();
		t_subsets[maxSize - 1].faceCount = dwFaceCount * 3;
	}

	//　ファイルを閉じる
	file.close();

	//　頂点データをコピー
	m_NumVertices = t_vertices.size();
	m_Vertices.resize(m_NumVertices);
	for (unsigned int i = 0; i<m_NumVertices; i++)
		m_Vertices[i] = t_vertices[i];

	//　サブセットデータをコピー
	m_NumSubsets = t_subsets.size();
	m_Subsets.resize(m_NumSubsets);
	for (unsigned int i = 0; i<m_NumSubsets; i++)
		m_Subsets[i] = t_subsets[i];

	//　インデックスデータをコピー
	m_NumIndices = t_indices.size();
	m_Indices.resize(m_NumIndices);
	for (unsigned int i = 0; i<m_NumIndices; i++)
		m_Indices[i] = t_indices[i];

	//　バウンディングスフィアの作成
	m_Sphere.Create(m_Box);

	//　メモリ破棄
	positions.clear();
	normals.clear();
	texcoords.clear();
	t_vertices.clear();
	t_subsets.clear();
	t_indices.clear();

	//　正常終了
	return true;
}

// MTLファイルの読み込み
bool OBJMESH::LoadMTLFile(const string _filename)
{
	char buf[OBJ_BUFFER_LENGTH] = { 0 };
	int iMtlCount = -1;
	ifstream file;
	vector<OBJMATERIAL> t_materials;
	OBJMATERIAL material;
	InitMaterial(&material);

	//　ファイルを開く
	file.open(_filename, ios::in);

	//　チェック
	if (!file.is_open())
	{
		cout << "Error : ファイルオープンに失敗しました" << endl;
		cout << "File Name : " << _filename << endl;
		return false;
	}

	//　ループ
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

	//　ファイルを閉じる
	file.close();

	//　マテリアルデータをコピー
	m_NumMaterials = t_materials.size();
	m_Materials.resize(m_NumMaterials);
	for (unsigned int i = 0; i<m_NumMaterials; i++)
		m_Materials[i] = t_materials[i];

	// textureの読み込み(diffuseMapのみに対応。合成とかはできない)
	if (m_Materials.size()>0 && m_Materials[0].diffuseMapName.size() > 3){
		for (int t_mate = 0; t_mate < m_Materials.size(); t_mate++){
			cv::Mat img = cv::imread(m_Materials[t_mate].diffuseMapName, -1);
			if (img.cols < 10){
				cout << "image read error: "<<m_Materials[t_mate].diffuseMapName << endl;
				continue;
			}

			// むりやり4チャンネル画像に変更
			cv::flip(img, img, 0);
			if (img.channels() == 3) cv::cvtColor(img, img, CV_BGR2RGBA);
			else if (img.channels() == 4) cv::cvtColor(img, img, CV_BGRA2RGBA);

			// texture準備
			GLuint t_texID = -1;
			glEnable(GL_TEXTURE_2D);
			glGenTextures(1, &t_texID);
			glBindTexture(GL_TEXTURE_2D, t_texID);

			// テクスチャ画像はバイト単位に詰め込まれている
			glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

			// テクスチャの割り当て
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img.cols, img.rows, 0,GL_RGBA, GL_UNSIGNED_BYTE, img.data);

			// テクスチャを拡大・縮小する方法の指定
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

			// テクスチャ合成環境　テクスチャに陰影情報を載せるかどうか
			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

			glDisable(GL_TEXTURE_2D);
			m_texID.push_back(t_texID);
		}
	}

	//　正常終了
	return true;
}

// メッシュファイルの読み込み
bool OBJMESH::LoadFile(const string _filename)
{
	cout << "OBJファイル読み込み(" << _filename << ") -> ";
	//　OBJ, MTLファイルを読み込み
	if (!LoadOBJFile(_filename))
	{
		cout << "Error : メッシュファイルの読み込みに失敗しました" << endl;
		return false;
	}
	cout << "おしり" << endl;

	//　正常終了
	return true;
}


// 描画処理
void OBJMESH::Draw()
{
	for (unsigned int i = 0; i<m_NumSubsets; i++)
	{
		//　マテリアル
		OBJMATERIAL* pMat = &m_Materials[m_Subsets[i].materialIndex];
		SetMaterial(pMat);
		
		// texture
		if (pMat->diffuseMapName.size() > 3){
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, m_texID[m_Subsets[i].materialIndex]);
		}

		//　三角形描画
		glInterleavedArrays(GL_T2F_N3F_V3F, 0, m_Vertices.data());
		glDrawElements(GL_TRIANGLES, m_Subsets[i].faceCount, GL_UNSIGNED_INT, &m_Indices[m_Subsets[i].faceStart]);

		// texture
		if (pMat->diffuseMapName.size() > 3){
			glDisable(GL_TEXTURE_2D);
		}
	}
}
