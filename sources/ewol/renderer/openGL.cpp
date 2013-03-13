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
static uint32_t l_flagsCurrent = 0;
static uint32_t l_flagsMustBeSet = 0;
static uint32_t l_textureflags = 0;
static int32_t  l_programId = 0;

void ewol::openGL::Init(void)
{
	// remove deprecated pb ...
	l_matrixList.Clear();
	mat4 tmpMat;
	l_matrixList.PushBack(tmpMat);
	l_matrixCamera.Identity();
	l_flagsCurrent = 0;
	l_flagsMustBeSet = 0;
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

//#define DIRECT_MODE

typedef struct {
	uint32_t curentFlag;
	GLenum   OGlFlag;
} correspondenceTable_ts;

static correspondenceTable_ts basicFlag[] = {
	{(uint32_t)ewol::openGL::FLAG_BLEND, GL_BLEND},
	#ifndef __TARGET_OS__Android
	{(uint32_t)ewol::openGL::FLAG_CLIP_DISTANCE_I, GL_CLIP_DISTANCE0},
	{(uint32_t)ewol::openGL::FLAG_COLOR_LOGIC_OP, GL_COLOR_LOGIC_OP},
	#endif
	{(uint32_t)ewol::openGL::FLAG_CULL_FACE, GL_CULL_FACE},
	#ifndef __TARGET_OS__Android
	{(uint32_t)ewol::openGL::FLAG_DEBUG_OUTPUT, GL_DEBUG_OUTPUT},
	{(uint32_t)ewol::openGL::FLAG_DEBUG_OUTPUT_SYNCHRONOUS, GL_DEBUG_OUTPUT_SYNCHRONOUS},
	{(uint32_t)ewol::openGL::FLAG_DEPTH_CLAMP, GL_DEPTH_CLAMP},
	#endif
	{(uint32_t)ewol::openGL::FLAG_DEPTH_TEST, GL_DEPTH_TEST},
	{(uint32_t)ewol::openGL::FLAG_DITHER, GL_DITHER},
	#ifndef __TARGET_OS__Android
	{(uint32_t)ewol::openGL::FLAG_FRAMEBUFFER_SRGB, GL_FRAMEBUFFER_SRGB},
	{(uint32_t)ewol::openGL::FLAG_LINE_SMOOTH, GL_LINE_SMOOTH},
	{(uint32_t)ewol::openGL::FLAG_MULTISAMPLE, GL_MULTISAMPLE},
	#endif
	{(uint32_t)ewol::openGL::FLAG_POLYGON_OFFSET_FILL, GL_POLYGON_OFFSET_FILL},
	#ifndef __TARGET_OS__Android
	{(uint32_t)ewol::openGL::FLAG_POLYGON_OFFSET_LINE, GL_POLYGON_OFFSET_LINE},
	{(uint32_t)ewol::openGL::FLAG_POLYGON_OFFSET_POINT, GL_POLYGON_OFFSET_POINT},
	{(uint32_t)ewol::openGL::FLAG_POLYGON_SMOOTH, GL_POLYGON_SMOOTH},
	{(uint32_t)ewol::openGL::FLAG_PRIMITIVE_RESTART, GL_PRIMITIVE_RESTART},
	{(uint32_t)ewol::openGL::FLAG_PRIMITIVE_RESTART_FIXED_INDEX, GL_PRIMITIVE_RESTART_FIXED_INDEX},
	#endif
	{(uint32_t)ewol::openGL::FLAG_SAMPLE_ALPHA_TO_COVERAGE, GL_SAMPLE_ALPHA_TO_COVERAGE},
	#ifndef __TARGET_OS__Android
	{(uint32_t)ewol::openGL::FLAG_SAMPLE_ALPHA_TO_ONE, GL_SAMPLE_ALPHA_TO_ONE},
	#endif
	{(uint32_t)ewol::openGL::FLAG_SAMPLE_COVERAGE, GL_SAMPLE_COVERAGE},
	#ifndef __TARGET_OS__Android
	{(uint32_t)ewol::openGL::FLAG_SAMPLE_SHADING, GL_SAMPLE_SHADING},
	{(uint32_t)ewol::openGL::FLAG_SAMPLE_MASK, GL_SAMPLE_MASK},
	#endif
	{(uint32_t)ewol::openGL::FLAG_SCISSOR_TEST, GL_SCISSOR_TEST},
	{(uint32_t)ewol::openGL::FLAG_STENCIL_TEST, GL_STENCIL_TEST},
	#ifndef __TARGET_OS__Android
	{(uint32_t)ewol::openGL::FLAG_PROGRAM_POINT_SIZE, GL_PROGRAM_POINT_SIZE},
	#endif
	{(uint32_t)ewol::openGL::FLAG_TEXTURE_2D, GL_TEXTURE_2D},
	#ifndef __TARGET_OS__Android
	{(uint32_t)ewol::openGL::FLAG_ALPHA_TEST, GL_ALPHA_TEST},
	{(uint32_t)ewol::openGL::FLAG_FOG, GL_FOG}
	#endif
	//{(uint32_t)ewol::openGL::FLAG_, GL_}
};
static int32_t basicFlagCount = sizeof(basicFlag) / sizeof(correspondenceTable_ts);


void ewol::openGL::Enable(ewol::openGL::openGlFlags_te flagID)
{
	#ifdef DIRECT_MODE
	for (int32_t iii=0; iii<basicFlagCount ; iii++) {
		if ( basicFlag[iii].curentFlag==(uint32_t)flagID ) {
			glEnable(basicFlag[iii].OGlFlag);
		}
	}
	# else
		//EWOL_DEBUG("Enable FLAGS = " << l_flagsMustBeSet);
		l_flagsMustBeSet |= (uint32_t)flagID;
		//EWOL_DEBUG("            ==>" << l_flagsMustBeSet);
	#endif
}

void ewol::openGL::Disable(ewol::openGL::openGlFlags_te flagID)
{
	#ifdef DIRECT_MODE
	for (int32_t iii=0; iii<basicFlagCount ; iii++) {
		if ( basicFlag[iii].curentFlag==(uint32_t)flagID ) {
			glDisable(basicFlag[iii].OGlFlag);
		}
	}
	# else
		//EWOL_DEBUG("Disable FLAGS = " << l_flagsMustBeSet);
		l_flagsMustBeSet &= ~((uint32_t)flagID);
		//EWOL_DEBUG("             ==>" << l_flagsMustBeSet);
	#endif
}



void ewol::openGL::UpdateAllFlags(void)
{
	#ifdef DIRECT_MODE
		return;
	#endif
	// check if fhags has change :
	if (l_flagsMustBeSet==l_flagsCurrent ) {
		return;
	}
	//EWOL_DEBUG("            ==>" << l_flagsMustBeSet);
	for (int32_t iii=0; iii<basicFlagCount ; iii++) {
		uint32_t CurrentFlag = basicFlag[iii].curentFlag;
		if ( (l_flagsMustBeSet&CurrentFlag)!=(l_flagsCurrent&CurrentFlag) ) {
			if ( (l_flagsMustBeSet&CurrentFlag) != 0) {
				glEnable(basicFlag[iii].OGlFlag);
			} else {
				glDisable(basicFlag[iii].OGlFlag);
			}
		}
	}
	l_flagsCurrent = l_flagsMustBeSet;
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
	UpdateAllFlags();
	glDrawArrays(mode, first, count);
}

void ewol::openGL::DrawElements(uint32_t mode, int32_t count, uint32_t type, const void*  indices)
{
	UpdateAllFlags();
	glDrawElements(mode, count,  type, indices);
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


