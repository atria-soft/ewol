/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license APACHE v2.0 (see license file)
 */

#include <etk/os/FSNode.h>
#include <ewol/debug.h>
#include <ewol/compositing/Shaper.h>

#undef __class__
#define __class__	"ewol::compositing::Shaper"

ewol::compositing::Shaper::Shaper(const std::string& _shaperName) :
  m_name(_shaperName),
  m_config(nullptr),
  m_confIdMode(-1),
  m_confIdDisplayOutside(-1),
  m_confIdChangeTime(-1),
  m_confProgramFile(-1),
  m_confColorFile(-1),
  m_confImageFile(-1),
  m_GLprogram(nullptr),
  m_GLPosition(-1),
  m_GLMatrix(-1),
  m_GLStateActivate(-1),
  m_GLStateOld(-1),
  m_GLStateNew(-1),
  m_GLStateTransition(-1),
  m_resourceTexture(nullptr),
  m_nextStatusRequested(-1),
  m_propertyOrigin(0,0),
  m_propertySize(0,0),
  m_propertyInsidePosition(0,0),
  m_propertyInsideSize(0,0),
  m_stateActivate(0),
  m_stateOld(0),
  m_stateNew(0),
  m_stateTransition(1.0),
  m_nbVertexToDisplay(0) {
	for (size_t iii=0; iii<shaperPosCount; ++iii) {
		m_confIdPaddingOut[iii] = -1;
		m_confIdBorder[iii] = -1;
		m_confIdPaddingIn[iii] = -1;
	}
	for (size_t iii=0; iii<SHAPER_NB_MAX_VERTEX; ++iii) {
		m_coord[iii] = vec2(0,0);
		m_pos[iii] = vec2(0,0);
	}
	loadProgram();
}

ewol::compositing::Shaper::~Shaper() {
	unLoadProgram();
}

void ewol::compositing::Shaper::unLoadProgram() {
	m_GLprogram.reset();
	m_resourceTexture.reset();
	m_config.reset();
	m_colorProperty.reset();
	for (size_t iii=0; iii<shaperPosCount; ++iii) {
		m_confIdPaddingOut[iii] = -1;
		m_confIdBorder[iii] = -1;
		m_confIdPaddingIn[iii] = -1;
	}
	for (size_t iii=0; iii<SHAPER_NB_MAX_VERTEX; ++iii) {
		m_coord[iii] = vec2(0,0);
		m_pos[iii] = vec2(0,0);
	}
	m_confIdMode = -1;
	m_confIdDisplayOutside = -1;
	m_nbVertexToDisplay = 0;
	m_confIdChangeTime = -1;
	m_confProgramFile = -1;
	m_confImageFile = -1;
	m_listAssiciatedId.clear();
}

void ewol::compositing::Shaper::loadProgram() {
	if (m_name == "") {
		EWOL_DEBUG("no Shaper set for loading resources ...");
		return;
	}
	m_config = ewol::resource::ConfigFile::create(m_name);
	if (m_config != nullptr) {
		m_confIdMode = m_config->request("mode");
		m_confIdDisplayOutside = m_config->request("display-outside");
		m_confIdPaddingOut[shaperPosLeft]   = m_config->request("padding-out-left");
		m_confIdPaddingOut[shaperPosRight]  = m_config->request("padding-out-right");
		m_confIdPaddingOut[shaperPosTop]    = m_config->request("padding-out-top");
		m_confIdPaddingOut[shaperPosButtom] = m_config->request("padding-out-buttom");
		m_confIdBorder[shaperPosLeft]   = m_config->request("border-left");
		m_confIdBorder[shaperPosRight]  = m_config->request("border-right");
		m_confIdBorder[shaperPosTop]    = m_config->request("border-top");
		m_confIdBorder[shaperPosButtom] = m_config->request("border-buttom");
		m_confIdPaddingIn[shaperPosLeft]   = m_config->request("padding-in-left");
		m_confIdPaddingIn[shaperPosRight]  = m_config->request("padding-in-right");
		m_confIdPaddingIn[shaperPosTop]    = m_config->request("padding-in-top");
		m_confIdPaddingIn[shaperPosButtom] = m_config->request("padding-in-buttom");
		m_confIdChangeTime = m_config->request("change-time");
		m_confProgramFile  = m_config->request("program");
		m_confImageFile    = m_config->request("image");
		m_confColorFile    = m_config->request("color");
	}
	std::string basicShaderFile = m_config->getString(m_confProgramFile);
	if (basicShaderFile != "") {
		std::string tmpFilename(basicShaderFile);
		if (tmpFilename.find(':') == std::string::npos) {
			// get the relative position of the current file ...
			etk::FSNode file(m_name);
			tmpFilename = file.getRelativeFolder() + basicShaderFile;
			EWOL_DEBUG("Shaper try load shader : '" << tmpFilename << "' with base : '" << basicShaderFile << "'");
		} else {
			EWOL_DEBUG("Shaper try load shader : '" << tmpFilename << "'");
		}
		// get the shader resource :
		m_GLPosition = 0;
		m_GLprogram = gale::resource::Program::create(tmpFilename);
		if (m_GLprogram != nullptr) {
			m_GLPosition        = m_GLprogram->getAttribute("EW_coord2d");
			m_GLMatrix          = m_GLprogram->getUniform("EW_MatrixTransformation");
			// Widget property  == > for the Vertex shader
			m_GLPropertyPos = m_GLprogram->getAttribute("EW_widgetPropertyPos");
			// status property  == > for the fragment shader
			m_GLStateActivate   = m_GLprogram->getUniform("EW_status.activate");
			m_GLStateOld        = m_GLprogram->getUniform("EW_status.stateOld");
			m_GLStateNew        = m_GLprogram->getUniform("EW_status.stateNew");
			m_GLStateTransition = m_GLprogram->getUniform("EW_status.transition");
			// for the texture ID : 
			m_GLtexID = m_GLprogram->getUniform("EW_texID");
		}
		std::string basicImageFile = m_config->getString(m_confImageFile);
		if (basicImageFile != "") {
			std::string tmpFilename(basicImageFile);
			if (tmpFilename.find(':') == std::string::npos) {
				// get the relative position of the current file ...
				etk::FSNode file(m_name);
				tmpFilename = file.getRelativeFolder() + basicImageFile;
				EWOL_DEBUG("Shaper try load shaper image : '" << tmpFilename << "' with base : '" << basicImageFile << "'");
			} else {
				EWOL_DEBUG("Shaper try load shaper image : '" << tmpFilename << "'");
			}
			ivec2 size(64,64);
			m_resourceTexture = ewol::resource::TextureFile::create(tmpFilename, size);
		}
	}
	std::string basicColorFile = m_config->getString(m_confColorFile);
	if (basicColorFile != "") {
		std::string tmpFilename(basicColorFile);
		if (tmpFilename.find(':') == std::string::npos) {
			// get the relative position of the current file ...
			etk::FSNode file(m_name);
			tmpFilename = file.getRelativeFolder() + basicColorFile;
			EWOL_DEBUG("Shaper try load colorFile : '" << tmpFilename << "' with base : '" << basicColorFile << "'");
		} else {
			EWOL_DEBUG("Shaper try load colorFile : '" << tmpFilename << "'");
		}
		m_colorProperty = ewol::resource::ColorFile::create(tmpFilename);
		if (    m_GLprogram != nullptr
		     && m_colorProperty != nullptr) {
			std::vector<std::string> listColor = m_colorProperty->getColors();
			for (auto tmpColor : listColor) {
				int32_t glId = m_GLprogram->getUniform(tmpColor);
				int32_t colorID = m_colorProperty->request(tmpColor);
				m_listAssiciatedId.push_back(ivec2(glId, colorID));
			}
		}
	}
}

void ewol::compositing::Shaper::draw(bool _disableDepthTest) {
	if (m_config == nullptr) {
		// this is a normale case ... the user can choice to have no config basic file ...
		return;
	}
	if (m_GLprogram == nullptr) {
		EWOL_ERROR("No shader ...");
		return;
	}
	//glScalef(m_scaling.x, m_scaling.y, 1.0);
	m_GLprogram->use();
	// set Matrix : translation/positionMatrix
	mat4 tmpMatrix = gale::openGL::getMatrix();
	m_GLprogram->uniformMatrix(m_GLMatrix, tmpMatrix);
	// position :
	m_GLprogram->sendAttribute(m_GLPosition, 2/*x,y*/, m_coord);
	// property
	m_GLprogram->sendAttribute(m_GLPropertyPos, 2/*x,y*/, m_pos);
	// all entry parameters :
	m_GLprogram->uniform1i(m_GLStateActivate,   m_stateActivate);
	m_GLprogram->uniform1i(m_GLStateOld,        m_stateOld);
	m_GLprogram->uniform1i(m_GLStateNew,        m_stateNew);
	m_GLprogram->uniform1f(m_GLStateTransition, m_stateTransition);
	for (auto element : m_listAssiciatedId) {
		m_GLprogram->uniform(element.x(), m_colorProperty->get(element.y()));
	}
	if (nullptr!=m_resourceTexture) {
		// TextureID
		m_GLprogram->setTexture0(m_GLtexID, m_resourceTexture->getRendererId());
	}
	// Request the draw of the elements : 
	//gale::openGL::drawArrays(gale::openGL::render_triangle, 0, SHAPER_NB_MAX_VERTEX);
	gale::openGL::drawArrays(gale::openGL::render_triangleStrip, 0, m_nbVertexToDisplay);
	m_GLprogram->unUse();
}

void ewol::compositing::Shaper::clear() {
	// nothing to do ...
	m_propertySize = vec2(0,0);
	m_propertyOrigin = vec2(0,0);
	m_propertyInsidePosition = vec2(0,0);
	m_propertyInsideSize = vec2(0,0);
	memset(m_coord, 0, sizeof(m_coord));
}

bool ewol::compositing::Shaper::setState(int32_t _newState) {
	if (m_stateActivate == _newState) {
		return false;
	}
	m_stateActivate = _newState;
	return true;
}

bool ewol::compositing::Shaper::changeStatusIn(int32_t _newStatusId) {
	if (_newStatusId != m_stateNew) {
		m_nextStatusRequested = _newStatusId;
		return true;
	}
	if(    m_nextStatusRequested != -1
	    || m_stateNew != m_stateOld) {
		return true;
	}
	return false;
}

bool ewol::compositing::Shaper::periodicCall(const ewol::event::Time& _event) {
	EWOL_VERBOSE("call=" << _event << "state transition=" << m_stateTransition << " speedTime=" << m_config->getNumber(m_confIdChangeTime));
	// start :
	if (m_stateTransition >= 1.0) {
		m_stateOld = m_stateNew;
		if(    m_nextStatusRequested != -1
		    && m_nextStatusRequested != m_stateOld) {
			m_stateNew = m_nextStatusRequested;
			m_nextStatusRequested = -1;
			m_stateTransition = 0.0;
			EWOL_VERBOSE("     ##### START #####  ");
		} else {
			m_nextStatusRequested = -1;
			// disable periodic call ...
			return false;
		}
	}
	if (m_stateTransition<1.0) {
		// check if no new state requested:
		if (m_nextStatusRequested != -1 && m_stateTransition<0.5) {
			// invert sources with destination
			int32_t tmppp = m_stateOld;
			m_stateOld = m_stateNew;
			m_stateNew = tmppp;
			m_stateTransition = 1.0 - m_stateTransition;
			if (m_nextStatusRequested == m_stateNew) {
				m_nextStatusRequested = -1;
			}
		}
		float timeRelativity = 0.0f;
		if (m_config != nullptr) {
			timeRelativity = m_config->getNumber(m_confIdChangeTime) / 1000.0;
		}
		m_stateTransition += _event.getDeltaCall() / timeRelativity;
		//m_stateTransition += _event.getDeltaCall();
		m_stateTransition = std::avg(0.0f, m_stateTransition, 1.0f);
		EWOL_VERBOSE("relative=" << timeRelativity << " Transition : " << m_stateTransition);
	}
	return true;
}

//Create Line:
void ewol::compositing::Shaper::addVertexLine(float _yTop,
                                              float _yButtom,
                                              float _x1,
                                              float _x2,
                                              float _x3,
                                              float _x4,
                                              float _x5,
                                              float _x6,
                                              float _x7,
                                              float _x8,
                                              float _yValTop,
                                              float _yValButtom,
                                              const float* _table,
                                              bool _displayOutside) {
	if (m_nbVertexToDisplay != 0) {
		// change line ...
		m_coord[m_nbVertexToDisplay] = m_coord[m_nbVertexToDisplay-1];
		m_pos[m_nbVertexToDisplay] = m_pos[m_nbVertexToDisplay-1];
		m_nbVertexToDisplay++;
		if (_displayOutside == true) {
			m_coord[m_nbVertexToDisplay].setValue(_x1, _yButtom);
			m_pos[m_nbVertexToDisplay].setValue(_table[0],_yValButtom);
			m_nbVertexToDisplay++;
		} else {
			m_coord[m_nbVertexToDisplay].setValue(_x2, _yButtom);
			m_pos[m_nbVertexToDisplay].setValue(_table[1],_yValButtom);
			m_nbVertexToDisplay++;
		}
	}
	
	if (_displayOutside == true) {
		// A
		m_coord[m_nbVertexToDisplay].setValue(_x1, _yButtom);
		m_pos[m_nbVertexToDisplay].setValue(_table[0],_yValButtom);
		m_nbVertexToDisplay++;
		m_coord[m_nbVertexToDisplay].setValue(_x1, _yTop);
		m_pos[m_nbVertexToDisplay].setValue(_table[0],_yValTop);
		m_nbVertexToDisplay++;
		m_coord[m_nbVertexToDisplay].setValue(_x2, _yButtom);
		m_pos[m_nbVertexToDisplay].setValue(_table[1],_yValButtom);
		m_nbVertexToDisplay++;
		// B
		m_coord[m_nbVertexToDisplay].setValue(_x2, _yTop);
		m_pos[m_nbVertexToDisplay].setValue(_table[1],_yValTop);
		m_nbVertexToDisplay++;
		
		// C
		m_coord[m_nbVertexToDisplay].setValue(_x3, _yButtom);
		m_pos[m_nbVertexToDisplay].setValue(_table[2],_yValButtom);
		m_nbVertexToDisplay++;
	} else {
		// C
		m_coord[m_nbVertexToDisplay].setValue(_x2, _yButtom);
		m_pos[m_nbVertexToDisplay].setValue(_table[1],_yValButtom);
		m_nbVertexToDisplay++;
		m_coord[m_nbVertexToDisplay].setValue(_x2, _yTop);
		m_pos[m_nbVertexToDisplay].setValue(_table[1],_yValTop);
		m_nbVertexToDisplay++;
		m_coord[m_nbVertexToDisplay].setValue(_x3, _yButtom);
		m_pos[m_nbVertexToDisplay].setValue(_table[2],_yValButtom);
		m_nbVertexToDisplay++;
	}
	// D
	m_coord[m_nbVertexToDisplay].setValue(_x3, _yTop);
	m_pos[m_nbVertexToDisplay].setValue(_table[2],_yValTop);
	m_nbVertexToDisplay++;
	
	// E
	m_coord[m_nbVertexToDisplay].setValue(_x4, _yButtom);
	m_pos[m_nbVertexToDisplay].setValue(_table[3],_yValButtom);
	m_nbVertexToDisplay++;
	// F
	m_coord[m_nbVertexToDisplay].setValue(_x4, _yTop);
	m_pos[m_nbVertexToDisplay].setValue(_table[3],_yValTop);
	m_nbVertexToDisplay++;
	
	// G
	m_coord[m_nbVertexToDisplay].setValue(_x5, _yButtom);
	m_pos[m_nbVertexToDisplay].setValue(_table[4],_yValButtom);
	m_nbVertexToDisplay++;
	// H
	m_coord[m_nbVertexToDisplay].setValue(_x5, _yTop);
	m_pos[m_nbVertexToDisplay].setValue(_table[4],_yValTop);
	m_nbVertexToDisplay++;
	
	// I
	m_coord[m_nbVertexToDisplay].setValue(_x6, _yButtom);
	m_pos[m_nbVertexToDisplay].setValue(_table[5],_yValButtom);
	m_nbVertexToDisplay++;
	// J
	m_coord[m_nbVertexToDisplay].setValue(_x6, _yTop);
	m_pos[m_nbVertexToDisplay].setValue(_table[5],_yValTop);
	m_nbVertexToDisplay++;
	
	// K
	m_coord[m_nbVertexToDisplay].setValue(_x7, _yButtom);
	m_pos[m_nbVertexToDisplay].setValue(_table[6],_yValButtom);
	m_nbVertexToDisplay++;
	// L
	m_coord[m_nbVertexToDisplay].setValue(_x7, _yTop);
	m_pos[m_nbVertexToDisplay].setValue(_table[6],_yValTop);
	m_nbVertexToDisplay++;
	
	if (_displayOutside == true) {
		// M
		m_coord[m_nbVertexToDisplay].setValue(_x8, _yButtom);
		m_pos[m_nbVertexToDisplay].setValue(_table[7],_yValButtom);
		m_nbVertexToDisplay++;
		// N
		m_coord[m_nbVertexToDisplay].setValue(_x8, _yTop);
		m_pos[m_nbVertexToDisplay].setValue(_table[7],_yValTop);
		m_nbVertexToDisplay++;
	}
}
const float modeDisplay[][8] = {
	/* !! 0 !!
	 *                    / *******
	 *          / ****** /
	 *  ****** /
	 */
	{ 0.0f, 0.0f, 0.5f, 0.5f, 0.5f, 0.5f, 1.0f, 1.0f },
	/* !! 1 !!
	 *  ****** \
	 *          \ ****** \
	 *                    \ *******
	 */
	{ 1.0f, 1.0f, 0.5f, 0.5f, 0.5f, 0.5f, 0.0f, 0.0f },
	/* !! 2 !!
	 *          / ****** \
	 *  ****** /          \ *******
	 */
	{ 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f },
	/* !! 3 !!
	 *  ****** \          / *******
	 *          \ ****** /
	 */
	{ 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f },
	/* !! 4 !!
	 *                    / *******
	 *          / ****** /
	 *  ****** /
	 */
	{ -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f },
	/* !! 5 !!
	 *  ****** \
	 *          \ ****** \
	 *                    \ *******
	 */
	{ 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, -1.0f },
	/* !! 6 !!
	 *          / ****** \
	 *  ****** /          \ *******
	 */
	{ -1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, -1.0f, -1.0f },
	/* !! 7 !!
	 *  ****** \          / *******
	 *          \ ****** /
	 */
	{ 1.0f, 1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, 1.0f }
};

void ewol::compositing::Shaper::setShape(const vec2& _origin, const vec2& _size, const vec2& _insidePos, const vec2& _insideSize) {
	ewol::Padding borderTmp = getBorder();
	ewol::Padding paddingIn = getPaddingIn();
	ewol::Padding paddingOut = getPaddingOut();
	ewol::Padding padding = paddingIn + borderTmp + paddingOut;
	ewol::Padding enveloppe(_origin.x(),
	                        _origin.y() + _size.y(),
	                        _origin.x() + _size.x(),
	                        _origin.y());
	#if 0
		ewol::Padding inside(_insidePos.x(),
		                     _insidePos.y() + _insideSize.y(),
		                     _insidePos.x() + _insideSize.x(),
		                     _insidePos.y());
		ewol::Padding insideBorder(inside.xLeft()   - paddingIn.xLeft(),
		                           inside.yTop()    + paddingIn.yTop(),
		                           inside.xRight()  + paddingIn.xRight(),
		                           inside.yButtom() - paddingIn.yButtom());
		ewol::Padding border(insideBorder.xLeft()   - borderTmp.xLeft(),
		                     insideBorder.yTop()    + borderTmp.yTop(),
		                     insideBorder.xRight()  + borderTmp.xRight(),
		                     insideBorder.yButtom() - borderTmp.yButtom());
	#else
		ewol::Padding border(_insidePos.x()                   - padding.xLeft()   + paddingOut.xLeft(),
		                     _insidePos.y() + _insideSize.y() + padding.yTop()    - paddingOut.yTop(),
		                     _insidePos.x() + _insideSize.x() + padding.xRight()  - paddingOut.xRight(),
		                     _insidePos.y()                   - padding.yButtom() + paddingOut.yButtom());
		ewol::Padding insideBorder(border.xLeft()   + borderTmp.xLeft(),
		                           border.yTop()    - borderTmp.yTop(),
		                           border.xRight()  - borderTmp.xRight(),
		                           border.yButtom() + borderTmp.yButtom());
		ewol::Padding inside(insideBorder.xLeft()   + std::max(0.0f, paddingIn.xLeft()),
		                     insideBorder.yTop()    - std::max(0.0f, paddingIn.yTop()),
		                     insideBorder.xRight()  - std::max(0.0f, paddingIn.xRight()),
		                     insideBorder.yButtom() + std::max(0.0f, paddingIn.yButtom()));
		
	#endif
	/*
	EWOL_ERROR(" enveloppe = " << enveloppe);
	EWOL_ERROR(" border = " << border);
	EWOL_ERROR(" inside = " << inside);
	*/
	int32_t mode = 0;
	bool displayOutside = false;
	if (m_config != nullptr) {
		mode = m_config->getNumber(m_confIdMode);
		displayOutside = m_config->getBoolean(m_confIdDisplayOutside);
	}
	m_nbVertexToDisplay = 0;
	if (displayOutside == true) {
		addVertexLine(enveloppe.yTop(), border.yTop(),
		              enveloppe.xLeft(),
		              border.xLeft(),
		              insideBorder.xLeft(),
		              inside.xLeft(),
		              inside.xRight(),
		              insideBorder.xRight(),
		              border.xRight(),
		              enveloppe.xRight(),
		              modeDisplay[mode][7], modeDisplay[mode][6],
		              modeDisplay[mode],
		              displayOutside);
	}
	addVertexLine(border.yTop(), insideBorder.yTop(),
	              enveloppe.xLeft(),
	              border.xLeft(),
	              insideBorder.xLeft(),
	              inside.xLeft(),
	              inside.xRight(),
	              insideBorder.xRight(),
	              border.xRight(),
	              enveloppe.xRight(),
	              modeDisplay[mode][6], modeDisplay[mode][5],
	              modeDisplay[mode],
	              displayOutside);
	addVertexLine(insideBorder.yTop(), inside.yTop(),
	              enveloppe.xLeft(),
	              border.xLeft(),
	              insideBorder.xLeft(),
	              inside.xLeft(),
	              inside.xRight(),
	              insideBorder.xRight(),
	              border.xRight(),
	              enveloppe.xRight(),
	              modeDisplay[mode][5], modeDisplay[mode][4],
	              modeDisplay[mode],
	              displayOutside);
	addVertexLine(inside.yTop(), inside.yButtom(),
	              enveloppe.xLeft(),
	              border.xLeft(),
	              insideBorder.xLeft(),
	              inside.xLeft(),
	              inside.xRight(),
	              insideBorder.xRight(),
	              border.xRight(),
	              enveloppe.xRight(),
	              modeDisplay[mode][4], modeDisplay[mode][3],
	              modeDisplay[mode],
	              displayOutside);
	addVertexLine(inside.yButtom(), insideBorder.yButtom(),
	              enveloppe.xLeft(),
	              border.xLeft(),
	              insideBorder.xLeft(),
	              inside.xLeft(),
	              inside.xRight(),
	              insideBorder.xRight(),
	              border.xRight(),
	              enveloppe.xRight(),
	              modeDisplay[mode][3], modeDisplay[mode][2],
	              modeDisplay[mode],
	              displayOutside);
	addVertexLine(insideBorder.yButtom(), border.yButtom(),
	              enveloppe.xLeft(),
	              border.xLeft(),
	              insideBorder.xLeft(),
	              inside.xLeft(),
	              inside.xRight(),
	              insideBorder.xRight(),
	              border.xRight(),
	              enveloppe.xRight(),
	              modeDisplay[mode][2], modeDisplay[mode][1],
	              modeDisplay[mode],
	              displayOutside);
	if (displayOutside == true) {
		addVertexLine(border.yButtom(), enveloppe.yButtom(),
		              enveloppe.xLeft(),
		              border.xLeft(),
		              insideBorder.xLeft(),
		              inside.xLeft(),
		              inside.xRight(),
		              insideBorder.xRight(),
		              border.xRight(),
		              enveloppe.xRight(),
		              modeDisplay[mode][1], modeDisplay[mode][0],
		              modeDisplay[mode],
		              displayOutside);
	}
}

ewol::Padding ewol::compositing::Shaper::getPadding() {
	return getPaddingOut() + getBorder() + getPaddingIn();
}

ewol::Padding ewol::compositing::Shaper::getPaddingIn() {
	ewol::Padding padding(0,0,0,0);
	if (m_config != nullptr) {
		padding.setValue(m_config->getNumber(m_confIdPaddingIn[shaperPosLeft]),
		                 m_config->getNumber(m_confIdPaddingIn[shaperPosTop]),
		                 m_config->getNumber(m_confIdPaddingIn[shaperPosRight]),
		                 m_config->getNumber(m_confIdPaddingIn[shaperPosButtom]));
	}
	return padding;
}

ewol::Padding ewol::compositing::Shaper::getPaddingOut() {
	ewol::Padding padding(0,0,0,0);
	if (m_config != nullptr) {
		padding.setValue(m_config->getNumber(m_confIdPaddingOut[shaperPosLeft]),
		                 m_config->getNumber(m_confIdPaddingOut[shaperPosTop]),
		                 m_config->getNumber(m_confIdPaddingOut[shaperPosRight]),
		                 m_config->getNumber(m_confIdPaddingOut[shaperPosButtom]));
	}
	return padding;
}

ewol::Padding ewol::compositing::Shaper::getBorder() {
	ewol::Padding padding(0,0,0,0);
	if (m_config != nullptr) {
		padding.setValue(m_config->getNumber(m_confIdBorder[shaperPosLeft]),
		                 m_config->getNumber(m_confIdBorder[shaperPosTop]),
		                 m_config->getNumber(m_confIdBorder[shaperPosRight]),
		                 m_config->getNumber(m_confIdBorder[shaperPosButtom]));
	}
	return padding;
}

void ewol::compositing::Shaper::setSource(const std::string& _newFile) {
	clear();
	unLoadProgram();
	m_name = _newFile;
	loadProgram();
}

bool ewol::compositing::Shaper::hasSources() {
	return m_GLprogram!=nullptr;
}


const etk::Color<float>& ewol::compositing::Shaper::getColor(int32_t _id) {
	static const etk::Color<float> errorValue(0,0,0,0);
	if (m_colorProperty == nullptr) {
		EWOL_WARNING("nullptr of m_colorProperty ==> return #0000 for id " << _id);
		return errorValue;
	}
	return m_colorProperty->get(_id);
}

int32_t ewol::compositing::Shaper::requestColor(const std::string& _name) {
	if (m_colorProperty == nullptr) {
		EWOL_WARNING("nullptr of m_colorProperty ==> return -1 for name " << _name);
		return -1;
	}
	return m_colorProperty->request(_name);
}

int32_t ewol::compositing::Shaper::requestConfig(const std::string& _name) {
	if (m_config == nullptr) {
		EWOL_WARNING("nullptr of m_config ==> return -1 for name " << _name);
		return -1;
	}
	return m_config->request(_name);
}

double ewol::compositing::Shaper::getConfigNumber(int32_t _id) {
	if (    _id == -1
	     || m_config == nullptr) {
		EWOL_WARNING("nullptr of m_config ==> return 0.0 for id " << _id);
		return 0.0;
	}
	return m_config->getNumber(_id);
}


namespace etk {
	template<> std::string to_string<ewol::compositing::Shaper>(const ewol::compositing::Shaper& _obj) {
		return _obj.getSource();
	}
	template<> std::u32string to_u32string<ewol::compositing::Shaper>(const ewol::compositing::Shaper& _obj) {
		return etk::to_u32string(etk::to_string(_obj));
	}
	template<> bool from_string<ewol::compositing::Shaper>(ewol::compositing::Shaper& _variableRet, const std::string& _value) {
		_variableRet.setSource(_value);
		return true;
	}
	template<> bool from_string<ewol::compositing::Shaper>(ewol::compositing::Shaper& _variableRet, const std::u32string& _value) {
		return from_string(_variableRet,  etk::to_string(_value));
	}
};