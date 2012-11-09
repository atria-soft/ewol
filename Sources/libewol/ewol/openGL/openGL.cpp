/**
 *******************************************************************************
 * @file ewol/openGL/openGL.cpp
 * @brief ewol openGl abstarction (sources)
 * @author Edouard DUPIN
 * @date 19/09/2012
 * @par Project
 * ewol
 *
 * @par Copyright
 * Copyright 2011 Edouard DUPIN, all right reserved
 *
 * This software is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY.
 *
 * Licence summary : 
 *    You can modify and redistribute the sources code and binaries.
 *    You can send me the bug-fix
 *
 * Term of the licence in in the file licence.txt.
 *
 *******************************************************************************
 */

#include <ewol/Debug.h>
#include <ewol/openGL/openGL.h>
#include <etk/Vector.h>

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

