/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license APACHE v2.0 (see license file)
 */

#include <vector>
#include <ewol/debug.h>
#include <ewol/openGL/openGL.h>
#include <etk/stdTools.h>
#include <mutex>
//#define DIRECT_MODE
/**
 * @brief get the draw mutex (ewol render).
 * @note due ti the fact that the system can be called for multiple instance, for naw we just limit the acces to one process at a time.
 * @return the main inteface Mutex
 */
static std::mutex& mutexOpenGl() {
	static std::mutex s_drawMutex;
	return s_drawMutex;
}

std::vector<mat4> l_matrixList;
mat4 l_matrixCamera;
static uint32_t l_flagsCurrent = 0;
static uint32_t l_flagsMustBeSet = 0;
static uint32_t l_textureflags = 0;
static int32_t  l_programId = 0;


void ewol::openGL::lock() {
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

void ewol::openGL::unLock() {
	mutexOpenGl().unlock();
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

void ewol::openGL::push() {
	if (l_matrixList.size() == 0) {
		EWOL_ERROR("set matrix list is not corect size in the stack : " << l_matrixList.size());
		mat4 tmp;
		l_matrixList.push_back(tmp);
		return;
	}
	mat4 tmp = l_matrixList[l_matrixList.size()-1];
	l_matrixList.push_back(tmp);
}

void ewol::openGL::pop() {
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

const mat4& ewol::openGL::getMatrix() {
	if (l_matrixList.size() == 0) {
		EWOL_ERROR("set matrix list is not corect size in the stack : " << l_matrixList.size());
		mat4 tmp;
		l_matrixList.push_back(tmp);
	}
	return l_matrixList[l_matrixList.size()-1];
}

const mat4& ewol::openGL::getCameraMatrix() {
	return l_matrixCamera;
}

void ewol::openGL::setCameraMatrix(const mat4& _newOne) {
	l_matrixCamera = _newOne;
}

void ewol::openGL::finish() {
	l_programId = -1;
	l_textureflags = 0;
}

void ewol::openGL::flush() {
	l_programId = -1;
	l_textureflags = 0;
}

void ewol::openGL::swap() {
	
}


std::ostream& ewol::operator <<(std::ostream& _os, const enum openGL::openGlFlags& _obj) {
	static std::vector<std::pair<enum openGL::openGlFlags, const char*>> list = {
		std::make_pair(openGL::FLAG_BLEND, "FLAG_BLEND"),
		std::make_pair(openGL::FLAG_CLIP_DISTANCE_I, "FLAG_CLIP_DISTANCE_I"),
		std::make_pair(openGL::FLAG_COLOR_LOGIC_OP, "FLAG_COLOR_LOGIC_OP"),
		std::make_pair(openGL::FLAG_CULL_FACE, "FLAG_CULL_FACE"),
		std::make_pair(openGL::FLAG_DEBUG_OUTPUT, "FLAG_DEBUG_OUTPUT"),
		std::make_pair(openGL::FLAG_DEBUG_OUTPUT_SYNCHRONOUS, "FLAG_DEBUG_OUTPUT_SYNCHRONOUS"),
		std::make_pair(openGL::FLAG_DEPTH_CLAMP, "FLAG_DEPTH_CLAMP"),
		std::make_pair(openGL::FLAG_DEPTH_TEST, "FLAG_DEPTH_TEST"),
		std::make_pair(openGL::FLAG_DITHER, "FLAG_DITHER"),
		std::make_pair(openGL::FLAG_FRAMEBUFFER_SRGB, "FLAG_FRAMEBUFFER_SRGB"),
		std::make_pair(openGL::FLAG_LINE_SMOOTH, "FLAG_LINE_SMOOTH"),
		std::make_pair(openGL::FLAG_MULTISAMPLE, "FLAG_MULTISAMPLE"),
		std::make_pair(openGL::FLAG_POLYGON_OFFSET_FILL, "FLAG_POLYGON_OFFSET_FILL"),
		std::make_pair(openGL::FLAG_POLYGON_OFFSET_LINE, "FLAG_POLYGON_OFFSET_LINE"),
		std::make_pair(openGL::FLAG_POLYGON_OFFSET_POINT, "FLAG_POLYGON_OFFSET_POINT"),
		std::make_pair(openGL::FLAG_POLYGON_SMOOTH, "FLAG_POLYGON_SMOOTH"),
		std::make_pair(openGL::FLAG_PRIMITIVE_RESTART, "FLAG_PRIMITIVE_RESTART"),
		std::make_pair(openGL::FLAG_PRIMITIVE_RESTART_FIXED_INDEX, "FLAG_PRIMITIVE_RESTART_FIXED_INDEX"),
		std::make_pair(openGL::FLAG_SAMPLE_ALPHA_TO_COVERAGE, "FLAG_SAMPLE_ALPHA_TO_COVERAGE"),
		std::make_pair(openGL::FLAG_SAMPLE_ALPHA_TO_ONE, "FLAG_SAMPLE_ALPHA_TO_ONE"),
		std::make_pair(openGL::FLAG_SAMPLE_COVERAGE, "FLAG_SAMPLE_COVERAGE"),
		std::make_pair(openGL::FLAG_SAMPLE_SHADING, "FLAG_SAMPLE_SHADING"),
		std::make_pair(openGL::FLAG_SAMPLE_MASK, "FLAG_SAMPLE_MASK"),
		std::make_pair(openGL::FLAG_SCISSOR_TEST, "FLAG_SCISSOR_TEST"),
		std::make_pair(openGL::FLAG_STENCIL_TEST, "FLAG_STENCIL_TEST"),
		std::make_pair(openGL::FLAG_PROGRAM_POINT_SIZE, "FLAG_PROGRAM_POINT_SIZE"),
		std::make_pair(openGL::FLAG_TEXTURE_2D, "FLAG_TEXTURE_2D"),
		std::make_pair(openGL::FLAG_ALPHA_TEST, "FLAG_ALPHA_TEST"),
		std::make_pair(openGL::FLAG_FOG, "FLAG_FOG")
	};
	_os << "{";
	bool hasOne = false;
	for (auto &it : list) {
		if ((_obj & it.first) != 0) {
			if (hasOne==true) {
				_os << ",";
			}
			_os << it.second;
			hasOne = true;
		}
	}
	_os << "}";
	return _os;
}


std::vector<std::pair<enum ewol::openGL::renderMode, std::string>>& getListRenderMode() {
	static std::vector<std::pair<enum ewol::openGL::renderMode, std::string>> list = {
		std::make_pair(ewol::openGL::renderPoint, "POINTS"),
		std::make_pair(ewol::openGL::renderLine, "LINES"),
		std::make_pair(ewol::openGL::renderLineStrip, "LINES_STRIP"),
		std::make_pair(ewol::openGL::renderLineLoop, "LINE_LOOP"),
		std::make_pair(ewol::openGL::renderTriangle, "TRIANGLE"),
		std::make_pair(ewol::openGL::renderTriangleStrip, "TRIANGLE_STRIP"),
		std::make_pair(ewol::openGL::renderTriangleFan, "TRIANGLE_FAN"),
		std::make_pair(ewol::openGL::renderQuad, "QUAD"),
		std::make_pair(ewol::openGL::renderQuadStrip, "QUAD_STRIP"),
		std::make_pair(ewol::openGL::renderPolygon, "POLYGON"),
	};
	return list;
}


namespace etk {
	template<> std::string to_string<ewol::openGL::renderMode>(const ewol::openGL::renderMode& _obj) {
		for (auto &it : getListRenderMode()) {
			if (it.first == _obj) {
				return it.second;
			}
		}
		EWOL_ERROR("Can not convert : " << static_cast<int32_t>(_obj) << " return UNKNOW");
		return "UNKNOW";
	}
	template<> std::u32string to_u32string<ewol::openGL::renderMode>(const ewol::openGL::renderMode& _obj) {
		return etk::to_u32string(etk::to_string(_obj));
	}
	template<> bool from_string<ewol::openGL::renderMode>(ewol::openGL::renderMode& _variableRet, const std::string& _value) {
		for (auto &it : getListRenderMode()) {
			if (it.second == _value) {
				_variableRet = it.first;
				return true;
			}
		}
		EWOL_WARNING("Can not parse : '" << _value << "' set Triangle default value");
		_variableRet = ewol::openGL::renderTriangle;
		return false;
	}
	template<> bool from_string<ewol::openGL::renderMode>(ewol::openGL::renderMode& _variableRet, const std::u32string& _value) {
		return from_string(_variableRet, etk::to_string(_value));
	}
};

std::ostream& ewol::operator <<(std::ostream& _os, const enum openGL::renderMode& _obj) {
	_os << etk::to_string(_obj);
	return _os;
}


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

void ewol::openGL::reset() {
	#ifdef DIRECT_MODE
		EWOL_TODO("...");
	#else
		l_flagsMustBeSet = 0;
		l_programId = -1;
		l_textureflags = 0;
		updateAllFlags();
	#endif
}

void ewol::openGL::enable(enum ewol::openGL::openGlFlags _flagID) {
	//EWOL_INFO("Enable : " << _flagID);
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
	//EWOL_INFO("Disable : " << _flagID);
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

void ewol::openGL::updateAllFlags() {
	#ifdef DIRECT_MODE
		return;
	#endif
	// check if fhags has change :
	if (l_flagsMustBeSet == l_flagsCurrent ) {
		//EWOL_INFO("real flag set : " << l_flagsMustBeSet << " " << l_flagsCurrent);
		return;
	}
	//EWOL_INFO("real flag set : " << l_flagsMustBeSet);
	for (int32_t iii=0; iii<basicFlagCount ; iii++) {
		uint32_t CurrentFlag = basicFlag[iii].curentFlag;
		if ( (l_flagsMustBeSet&CurrentFlag)!=(l_flagsCurrent&CurrentFlag) ) {
			if ( (l_flagsMustBeSet&CurrentFlag) != 0) {
				glEnable(basicFlag[iii].OGlFlag);
				//EWOL_INFO("    enable : " << basicFlag[iii].OGlFlag);
			} else {
				glDisable(basicFlag[iii].OGlFlag);
				//EWOL_INFO("    disable : " << basicFlag[iii].OGlFlag);
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


