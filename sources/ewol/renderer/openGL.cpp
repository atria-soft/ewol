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

etk::Vector<etk::Matrix4> l_matrixList;

void ewol::openGL::Init(void)
{
	// remove deprecated pb ...
	l_matrixList.Clear();
	etk::Matrix4 tmpMat;
	l_matrixList.PushBack(tmpMat);
}


void ewol::openGL::UnInit(void)
{
	l_matrixList.Clear();
}

void ewol::openGL::SetBasicMatrix(etk::Matrix4& newOne)
{
	if (l_matrixList.Size()!=1) {
		EWOL_ERROR("matrix is not corect size in the stack : " << l_matrixList.Size());
	}
	l_matrixList.Clear();
	l_matrixList.PushBack(newOne);
}

void ewol::openGL::SetMatrix(etk::Matrix4& newOne)
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
		etk::Matrix4 tmp;
		l_matrixList.PushBack(tmp);
		return;
	}
	etk::Matrix4 tmp = l_matrixList[l_matrixList.Size()-1];
	l_matrixList.PushBack(tmp);
}

void ewol::openGL::Pop(void)
{
	if (l_matrixList.Size()<=1) {
		EWOL_ERROR("set matrix list is not corect size in the stack : " << l_matrixList.Size());
		l_matrixList.Clear();
		etk::Matrix4 tmp;
		l_matrixList.PushBack(tmp);
		return;
	}
	l_matrixList.PopBack();
}

etk::Matrix4& ewol::openGL::GetMatrix(void)
{
	if (l_matrixList.Size()==0) {
		EWOL_ERROR("set matrix list is not corect size in the stack : " << l_matrixList.Size());
		etk::Matrix4 tmp;
		l_matrixList.PushBack(tmp);
	}
	return l_matrixList[l_matrixList.Size()-1];
}

