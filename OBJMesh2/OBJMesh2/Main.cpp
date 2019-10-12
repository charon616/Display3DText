#pragma region File Description
//-----------------------------------------------------------------------
// File : Main.cpp
// Desc : OBJ Mesh Loader Demo Program
// Date : Feb. 27, 2009
// Version : 2.0
// Author : Pocol
//-----------------------------------------------------------------------
#pragma endregion

#pragma region Includes
//
// Includes
//
#include <iostream>
#include <cmath>
#include <GL/glut.h>
#include "Mouse.h"
#include "OBJLoader.h"
#pragma endregion

#pragma region Using Statements
//
// Using Statements
//
using namespace std;
#pragma endregion

#pragma endregion Global Variables
//
// Global Variables
//
int WindowPositionX = 100;
int WindowPositionY = 100;
int WindowWidth = 512;
int WindowHeight = 512;
char WindowTitle[] = "���@OBJ�t�@�C���̓ǂݍ��݁@��";
ViewCamera camera(500.0);
bool wireframe_flag = false;
OBJMESH mesh;
#pragma endregion

#pragma region Forward Declarations
//
// Forward Declarations
//
void Initialize();
void Display();
void Idle();
void Shutdown();
void Reshape(int x, int y);
void Keyboard(unsigned char key, int x, int y);
void Special(int key, int x, int y);
void Mouse(int button, int state, int x, int y);
void Motion(int x, int y);
void PassiveMotion(int x, int y);
void SetLighting();
#pragma endregion

//----------------------------------------------------------------------------------------------------
// Name : main()
// Desc : ���C���G���g���|�C���g
//----------------------------------------------------------------------------------------------------
int main( int argc, char **argv )
{
	glutInit(&argc, argv);
	glutInitWindowPosition(WindowPositionX, WindowPositionY);
	glutInitWindowSize(WindowWidth, WindowHeight);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
	glutCreateWindow(WindowTitle);
	glutDisplayFunc(Display);
	glutReshapeFunc(Reshape);
	glutIdleFunc(Idle);
	glutMouseFunc(Mouse);
	glutMotionFunc(Motion);
	glutPassiveMotionFunc(PassiveMotion);
	glutKeyboardFunc(Keyboard);
	glutSpecialFunc(Special);

	Initialize();

	glutMainLoop();

	Shutdown();

	return 0;
}


//----------------------------------------------------------------------------------------------------
// Name : SetLighting()
// Desc : ���C�e�B���O
//----------------------------------------------------------------------------------------------------
void SetLighting()
{
	GLfloat lightAmbientColor[4] = { 0.2f, 0.2f, 0.2f, 1.0f };
	GLfloat lightDiffuseColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat lightSpecularColor[4] = { 0.8f, 0.8f, 0.8f, 1.0f };
	GLfloat lightPosition[4] = { 0.0f, 100.0f, 100.0f, 0.0f };

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
	glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbientColor);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuseColor);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecularColor);
}

//----------------------------------------------------------------------------------------------------
// Name : Initialize()
// Desc : ����������
//----------------------------------------------------------------------------------------------------
void Initialize()
{
	//�@�h��Ԃ��F
	glClearColor(0.3, 0.3, 1.0, 1.0);

	//�@�[�x�e�X�g�L��
	glEnable(GL_DEPTH_TEST);

	//�@�X���[�Y�V�F�C�f�B���O
	glShadeModel(GL_SMOOTH);

	//�@���C�e�B���O
	SetLighting();

	//�@���b�V�������[�h
	if ( !mesh.LoadFile( "Mesh/test2.obj" ) )
		Shutdown();
}

//---------------------------------------------------------------------------------------------------
// Name : Idle()
// Desc : �A�C�h�����O���̏���
//---------------------------------------------------------------------------------------------------
void Idle()
{
	//�@�ɂȂƂ��ɍĕ`��
	glutPostRedisplay();
}

//---------------------------------------------------------------------------------------------------
// Name : Reshape()
// Desc : �T�C�Y�ύX
//---------------------------------------------------------------------------------------------------
void Reshape(int x, int y)
{
	WindowWidth = x;
	WindowHeight = y;
	if ( WindowWidth < 1 ) WindowWidth = 1;
	if ( WindowHeight < 1 ) WindowHeight = 1;
}


//---------------------------------------------------------------------------------------------------
// Name : Display()
// Desc : �E�B���h�E�ւ̕`��
//---------------------------------------------------------------------------------------------------
void Display()
{
	//�@
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, WindowWidth, WindowHeight);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(40.0, (double)WindowWidth/(double)WindowHeight, 0.1, 1000.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//
	glPushMatrix();

	//�@���_�̕`��
	camera.Set();

	//�@���C�e�B���O
	SetLighting();

	//�@
	if ( wireframe_flag ) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	//�@���b�V����`��
	mesh.Draw();

	if ( wireframe_flag ) glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	
	//
	glPopMatrix();

	//�@�⏕���̕`��
	glPushMatrix();
	camera.RenderSubAxis(WindowWidth, WindowHeight);
	glPopMatrix();

	//�@�_�u���o�b�t�@
	glutSwapBuffers();

#if defined(DEBUG) || defined(_DEBUG)
	glutReportErrors();
#endif
}

//---------------------------------------------------------------------------------------------------
// Name : Mouse()
// Desc : �}�E�X����
//---------------------------------------------------------------------------------------------------
void Mouse(int button, int state, int x, int y)
{
	camera.MouseInput(button, state, x, y);
}

//--------------------------------------------------------------------------------------------------
// Name : Motion()
// Desc : �}�E�X�h���b�O��
//--------------------------------------------------------------------------------------------------
void Motion(int x, int y)
{
	camera.MouseMotion(x, y);
}

//--------------------------------------------------------------------------------------------------
// Name : PassiveMotion()
// Desc : �}�E�X�ړ���
//--------------------------------------------------------------------------------------------------
void PassiveMotion(int x, int y)
{
}

//--------------------------------------------------------------------------------------------------
// Name : Keyboard()
// Desc : �L�[�{�[�h����
//--------------------------------------------------------------------------------------------------
void Keyboard(unsigned char key, int x, int y)
{
	switch ( key )
	{
	case '\033':
		exit(0);
		break;

	case 'w':
		wireframe_flag ? wireframe_flag = false : wireframe_flag = true;
		break;

	default:
		break;
	}
}

//--------------------------------------------------------------------------------------------------
// Name : Special()
// Desc : ����L�[����
//--------------------------------------------------------------------------------------------------
void Special(int key, int x, int y)
{
	switch ( key )
	{
	case GLUT_KEY_F1:
		break;

	case GLUT_KEY_F2:
		break;

	case GLUT_KEY_F3:
		break;

	case GLUT_KEY_F4:
		break;

	case GLUT_KEY_F5:
		break;

	case GLUT_KEY_F6:
		break;

	case GLUT_KEY_F7:
		break;

	case GLUT_KEY_F8:
		break;

	case GLUT_KEY_F9:
		break;

	case GLUT_KEY_F10:
		break;

	case GLUT_KEY_F11:
		break;

	case GLUT_KEY_F12:
		break;

	case GLUT_KEY_LEFT:
		break;

	case GLUT_KEY_RIGHT:
		break;

	case GLUT_KEY_UP:
		break;

	case GLUT_KEY_DOWN:
		break;

	case GLUT_KEY_PAGE_UP:
		break;

	case GLUT_KEY_PAGE_DOWN:
		break;

	case GLUT_KEY_HOME:
		break;

	case GLUT_KEY_END:
		break;

	case GLUT_KEY_INSERT:
		break;
	}
}

//----------------------------------------------------------------------------------------------------
// Name : Shutdown()
// Desc : ��Еt��
//----------------------------------------------------------------------------------------------------
void Shutdown()
{
	//�@���b�V���t�@�C���̌�Еt��
	mesh.Release();

	exit( 0 );	
}