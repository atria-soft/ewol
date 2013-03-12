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
static uint32_t l_flags = 0;
static uint32_t l_textureflags = 0;
static int32_t  l_programId = 0;

void ewol::openGL::Init(void)
{
	// remove deprecated pb ...
	l_matrixList.Clear();
	mat4 tmpMat;
	l_matrixList.PushBack(tmpMat);
	l_matrixCamera.Identity();
	l_flags = 0;
	l_textureflags = 0;
	l_programId = -1;
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

void ewol::openGL::Finish(void)
{
	l_programId = -1;
	l_textureflags = 0;
}

void ewol::openGL::Flush(void)
{
	l_programId = -1;
	l_textureflags = 0;
	
}

void ewol::openGL::Swap(void)
{
	
}

void ewol::openGL::Enable(uint32_t flagID)
{
	glEnable(flagID);
}

void ewol::openGL::Disable(uint32_t flagID)
{
	glDisable(flagID);
}

void ewol::openGL::ActiveTexture(uint32_t flagID)
{
	glActiveTexture(flagID);
}

void ewol::openGL::DesActiveTexture(uint32_t flagID)
{
	
}

void ewol::openGL::DrawArrays(uint32_t mode, int32_t first, int32_t count)
{
	glDrawArrays(mode, first, count);
}

void ewol::openGL::DrawArraysInstanced(uint32_t mode, int32_t first, int32_t count, int32_t primcount)
{
	glDrawArraysInstanced(mode, first, count, primcount);
}

void ewol::openGL::DrawElements(uint32_t mode, int32_t count, uint32_t type, const void*  indices)
{
	glDrawElements(mode, count,  type, indices);
}

void ewol::openGL::DrawRangeElements(uint32_t mode, int32_t start, int32_t end, int32_t count, uint32_t type, const void* indices)
{
	glDrawRangeElements(mode, start, end, count, type, indices);
}

void ewol::openGL::UseProgram(int32_t id)
{
	if (0==id) {
		// not used ==> because it is unneded
		return;
	}
	if (l_programId != id) {
		l_programId = id;
		glUseProgram(l_programId);
	}
}


