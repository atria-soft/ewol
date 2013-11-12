/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#include <vector>
#include <ewol/debug.h>
#include <ewol/renderer/openGL.h>

/**
 * @brief get the draw mutex (ewol render).
 * @note due ti the fact that the system can be called for multiple instance, for naw we just limit the acces to one process at a time.
 * @return the main inteface Mutex
 */
static etk::Mutex& mutexOpenGl(void) {
	static etk::Mutex s_drawMutex;
	return s_drawMutex;
}

std::vector<mat4> l_matrixList;
mat4 l_matrixCamera;
static uint32_t l_flagsCurrent = 0;
static uint32_t l_flagsMustBeSet = 0;
static uint32_t l_textureflags = 0;
static int32_t  l_programId = 0;


void ewol::openGL::lock(void) {
	mutexOpenGl().lock();
	l_matrixList.clear();
	mat4 tmpMat;
	l_matrixList.push_back(tmpMat);
	l_matrixCamera.identity();
	l_flagsCurrent = 0;
	l_flagsMustBeSet = 0;
	l_textureflags = 0;
	l_programId = -1;
}

void ewol::openGL::unLock(void) {
	mutexOpenGl().unLock();
}

void ewol::openGL::setBasicMatrix(const mat4& _newOne) {
	if (l_matrixList.size()!=1) {
		EWOL_ERROR("matrix is not corect size in the stack : " << l_matrixList.size());
	}
	l_matrixList.clear();
	l_matrixList.push_back(_newOne);
}

void ewol::openGL::setMatrix(const mat4& _newOne) {
	if (l_matrixList.size() == 0) {
		EWOL_ERROR("set matrix list is not corect size in the stack : " << l_matrixList.size());
		l_matrixList.push_back(_newOne);
		return;
	}
	l_matrixList[l_matrixList.size()-1] = _newOne;
}

void ewol::openGL::push(void) {
	if (l_matrixList.size() == 0) {
		EWOL_ERROR("set matrix list is not corect size in the stack : " << l_matrixList.size());
		mat4 tmp;
		l_matrixList.push_back(tmp);
		return;
	}
	mat4 tmp = l_matrixList[l_matrixList.size()-1];
	l_matrixList.push_back(tmp);
}

void ewol::openGL::pop(void) {
	if (l_matrixList.size() <= 1) {
		EWOL_ERROR("set matrix list is not corect size in the stack : " << l_matrixList.size());
		l_matrixList.clear();
		mat4 tmp;
		l_matrixList.push_back(tmp);
		l_matrixCamera.identity();
		return;
	}
	l_matrixList.pop_back();
	l_matrixCamera.identity();
}

const mat4& ewol::openGL::getMatrix(void) {
	if (l_matrixList.size() == 0) {
		EWOL_ERROR("set matrix list is not corect size in the stack : " << l_matrixList.size());
		mat4 tmp;
		l_matrixList.push_back(tmp);
	}
	return l_matrixList[l_matrixList.size()-1];
}

const mat4& ewol::openGL::getCameraMatrix(void) {
	return l_matrixCamera;
}

void ewol::openGL::setCameraMatrix(const mat4& _newOne) {
	l_matrixCamera = _newOne;
}

void ewol::openGL::finish(void) {
	l_programId = -1;
	l_textureflags = 0;
}

void ewol::openGL::flush(void) {
	l_programId = -1;
	l_textureflags = 0;
}

void ewol::openGL::swap(void) {
	
}

//#define DIRECT_MODE

typedef struct {
	uint32_t curentFlag;
	GLenum   OGlFlag;
} correspondenceTable_ts;

static correspondenceTable_ts basicFlag[] = {
	{(uint32_t)ewol::openGL::FLAG_BLEND, GL_BLEND},
	#if 0
	!(defined(__TARGET_OS__Android) || defined(__TARGET_OS__MacOs))
	{(uint32_t)ewol::openGL::FLAG_CLIP_DISTANCE_I, GL_CLIP_DISTANCE0},
	{(uint32_t)ewol::openGL::FLAG_COLOR_LOGIC_OP, GL_COLOR_LOGIC_OP},
	#endif
	{(uint32_t)ewol::openGL::FLAG_CULL_FACE, GL_CULL_FACE},
	#if 0
		!(defined(__TARGET_OS__Android) || defined(__TARGET_OS__MacOs))
	{(uint32_t)ewol::openGL::FLAG_DEBUG_OUTPUT, GL_DEBUG_OUTPUT},
	{(uint32_t)ewol::openGL::FLAG_DEBUG_OUTPUT_SYNCHRONOUS, GL_DEBUG_OUTPUT_SYNCHRONOUS},
	{(uint32_t)ewol::openGL::FLAG_DEPTH_CLAMP, GL_DEPTH_CLAMP},
	#endif
	{(uint32_t)ewol::openGL::FLAG_DEPTH_TEST, GL_DEPTH_TEST},
	{(uint32_t)ewol::openGL::FLAG_DITHER, GL_DITHER},
	#if 0
	!(defined(__TARGET_OS__Android) || defined(__TARGET_OS__MacOs))
	{(uint32_t)ewol::openGL::FLAG_FRAMEBUFFER_SRGB, GL_FRAMEBUFFER_SRGB},
	{(uint32_t)ewol::openGL::FLAG_LINE_SMOOTH, GL_LINE_SMOOTH},
	{(uint32_t)ewol::openGL::FLAG_MULTISAMPLE, GL_MULTISAMPLE},
	#endif
	{(uint32_t)ewol::openGL::FLAG_POLYGON_OFFSET_FILL, GL_POLYGON_OFFSET_FILL},
	#if 0
	!(defined(__TARGET_OS__Android) || defined(__TARGET_OS__MacOs))
	{(uint32_t)ewol::openGL::FLAG_POLYGON_OFFSET_LINE, GL_POLYGON_OFFSET_LINE},
	{(uint32_t)ewol::openGL::FLAG_POLYGON_OFFSET_POINT, GL_POLYGON_OFFSET_POINT},
	{(uint32_t)ewol::openGL::FLAG_POLYGON_SMOOTH, GL_POLYGON_SMOOTH},
	{(uint32_t)ewol::openGL::FLAG_PRIMITIVE_RESTART, GL_PRIMITIVE_RESTART},
	{(uint32_t)ewol::openGL::FLAG_PRIMITIVE_RESTART_FIXED_INDEX, GL_PRIMITIVE_RESTART_FIXED_INDEX},
	#endif
	{(uint32_t)ewol::openGL::FLAG_SAMPLE_ALPHA_TO_COVERAGE, GL_SAMPLE_ALPHA_TO_COVERAGE},
	#if 0
	!(defined(__TARGET_OS__Android) || defined(__TARGET_OS__MacOs))
	{(uint32_t)ewol::openGL::FLAG_SAMPLE_ALPHA_TO_ONE, GL_SAMPLE_ALPHA_TO_ONE},
	#endif
	{(uint32_t)ewol::openGL::FLAG_SAMPLE_COVERAGE, GL_SAMPLE_COVERAGE},
	#if 0
	!(defined(__TARGET_OS__Android) || defined(__TARGET_OS__MacOs))
	{(uint32_t)ewol::openGL::FLAG_SAMPLE_SHADING, GL_SAMPLE_SHADING},
	{(uint32_t)ewol::openGL::FLAG_SAMPLE_MASK, GL_SAMPLE_MASK},
	#endif
	{(uint32_t)ewol::openGL::FLAG_SCISSOR_TEST, GL_SCISSOR_TEST},
	{(uint32_t)ewol::openGL::FLAG_STENCIL_TEST, GL_STENCIL_TEST},
	#if 0
	!(defined(__TARGET_OS__Android) || defined(__TARGET_OS__MacOs))
	{(uint32_t)ewol::openGL::FLAG_PROGRAM_POINT_SIZE, GL_PROGRAM_POINT_SIZE},
	#endif
	{(uint32_t)ewol::openGL::FLAG_TEXTURE_2D, GL_TEXTURE_2D},
	#if 0
	!(defined(__TARGET_OS__Android) || defined(__TARGET_OS__MacOs))
	{(uint32_t)ewol::openGL::FLAG_ALPHA_TEST, GL_ALPHA_TEST},
	{(uint32_t)ewol::openGL::FLAG_FOG, GL_FOG}
	#endif
	//{(uint32_t)ewol::openGL::FLAG_, GL_}
};
static int32_t basicFlagCount = sizeof(basicFlag) / sizeof(correspondenceTable_ts);


void ewol::openGL::enable(enum ewol::openGL::openGlFlags _flagID) {
	#ifdef DIRECT_MODE
	for (int32_t iii=0; iii<basicFlagCount ; iii++) {
		if ( basicFlag[iii].curentFlag == (uint32_t)_flagID ) {
			glEnable(basicFlag[iii].OGlFlag);
		}
	}
	# else
		//EWOL_DEBUG("Enable FLAGS = " << l_flagsMustBeSet);
		l_flagsMustBeSet |= (uint32_t)_flagID;
		//EWOL_DEBUG("             == >" << l_flagsMustBeSet);
	#endif
}

void ewol::openGL::disable(enum ewol::openGL::openGlFlags _flagID) {
	#ifdef DIRECT_MODE
	for (int32_t iii=0; iii<basicFlagCount ; iii++) {
		if ( basicFlag[iii].curentFlag == (uint32_t)_flagID ) {
			glDisable(basicFlag[iii].OGlFlag);
		}
	}
	# else
		//EWOL_DEBUG("Disable FLAGS = " << l_flagsMustBeSet);
		l_flagsMustBeSet &= ~((uint32_t)_flagID);
		//EWOL_DEBUG("              == >" << l_flagsMustBeSet);
	#endif
}

void ewol::openGL::updateAllFlags(void) {
	#ifdef DIRECT_MODE
		return;
	#endif
	// check if fhags has change :
	if (l_flagsMustBeSet == l_flagsCurrent ) {
		return;
	}
	//EWOL_DEBUG("             == >" << l_flagsMustBeSet);
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


void ewol::openGL::activeTexture(uint32_t _flagID) {
	if (l_programId >= 0) {
		glActiveTexture(_flagID);
	}
}

void ewol::openGL::desActiveTexture(uint32_t _flagID) {
	if (l_programId >= 0) {
		
	}
}

void ewol::openGL::drawArrays(uint32_t _mode, int32_t _first, int32_t _count) {
	if (l_programId >= 0) {
		updateAllFlags();
		glDrawArrays(_mode, _first, _count);
	}
}

void ewol::openGL::drawElements(uint32_t _mode, const std::vector<uint32_t>& _indices) {
	if (l_programId >= 0) {
		updateAllFlags();
		//EWOL_DEBUG("Request draw of " << indices.size() << "elements");
		glDrawElements(_mode, _indices.size(), GL_UNSIGNED_INT, &_indices[0]);
	}
}

void ewol::openGL::drawElements16(uint32_t _mode, const std::vector<uint16_t>& _indices) {
	if (l_programId >= 0) {
		updateAllFlags();
		glDrawElements(_mode, _indices.size(), GL_UNSIGNED_SHORT, &_indices[0]);
	}
}

void ewol::openGL::drawElements8(uint32_t _mode, const std::vector<uint8_t>& _indices) {
	if (l_programId >= 0) {
		updateAllFlags();
		glDrawElements(_mode, _indices.size(), GL_UNSIGNED_BYTE, &_indices[0]);
	}
}

void ewol::openGL::useProgram(int32_t _id) {
	//EWOL_DEBUG("USE prog : " << id);
	#if 1
		// note : In normal openGL case, the system might call with the program ID and at the end with 0, 
		//        here, we wrap this use to prevent over call of glUseProgram  == > then we set -1 when the 
		//        user no more use this program, and just stop grnerating. (chen 0  == > this is an errored program ...
		if (-1 == _id) {
			// not used  == > because it is unneded
			return;
		}
		if (l_programId != _id) {
			l_programId = _id;
			glUseProgram(l_programId);
		}
	#else
		if (-1 == _id) {
			glUseProgram(0);
		} else {
			l_programId = _id;
			glUseProgram(_id);
		}
	#endif
}


