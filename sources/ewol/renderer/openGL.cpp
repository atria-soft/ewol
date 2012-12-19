/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#include <etk/Vector.h>
#include <ewol/debug.h>
#include <ewol/renderer/openGL.h>

etk::Vector<mat4> l_matrixList;
mat4 l_matrixCamera;

void ewol::openGL::Init(void)
{
	// remove deprecated pb ...
	l_matrixList.Clear();
	mat4 tmpMat;
	l_matrixList.PushBack(tmpMat);
	l_matrixCamera.Identity();
}


void ewol::openGL::UnInit(void)
{
	l_matrixList.Clear();
	l_matrixCamera.Identity();
}

void ewol::openGL::SetBasicMatrix(mat4& newOne)
{
	if (l_matrixList.Size()!=1) {
		EWOL_ERROR("matrix is not corect size in the stack : " << l_matrixList.Size());
	}
	l_matrixList.Clear();
	l_matrixList.PushBack(newOne);
}

void ewol::openGL::SetMatrix(mat4& newOne)
{
	if (l_matrixList.Size()==0) {
		EWOL_ERROR("set matrix list is not corect size in the stack : " << l_matrixList.Size());
		l_matrixList.PushBack(newOne);
		return;
	}
	l_matrixList[l_matrixList.Size()-1] = newOne;
}

void ewol::openGL::Push(void)
{
	if (l_matrixList.Size()==0) {
		EWOL_ERROR("set matrix list is not corect size in the stack : " << l_matrixList.Size());
		mat4 tmp;
		l_matrixList.PushBack(tmp);
		return;
	}
	mat4 tmp = l_matrixList[l_matrixList.Size()-1];
	l_matrixList.PushBack(tmp);
}

void ewol::openGL::Pop(void)
{
	if (l_matrixList.Size()<=1) {
		EWOL_ERROR("set matrix list is not corect size in the stack : " << l_matrixList.Size());
		l_matrixList.Clear();
		mat4 tmp;
		l_matrixList.PushBack(tmp);
		l_matrixCamera.Identity();
		return;
	}
	l_matrixList.PopBack();
	l_matrixCamera.Identity();
}

mat4& ewol::openGL::GetMatrix(void)
{
	if (l_matrixList.Size()==0) {
		EWOL_ERROR("set matrix list is not corect size in the stack : " << l_matrixList.Size());
		mat4 tmp;
		l_matrixList.PushBack(tmp);
	}
	return l_matrixList[l_matrixList.Size()-1];
}

mat4& ewol::openGL::GetCameraMatrix(void)
{
	return l_matrixCamera;
}

void ewol::openGL::SetCameraMatrix(mat4& newOne)
{
	l_matrixCamera = newOne;
}
