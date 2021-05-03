/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */

#include <etk/uri/uri.hpp>
#include <ewol/debug.hpp>
#include <ewol/compositing/Shaper.hpp>
#include <etk/typeInfo.hpp>
ETK_DECLARE_TYPE(ewol::compositing::Shaper);

// VBO table property:
const int32_t ewol::compositing::Shaper::m_vboIdCoord(0);
const int32_t ewol::compositing::Shaper::m_vboIdPos(1);
#define NB_VBO (2)

ewol::compositing::Shaper::Shaper(const etk::Uri& _uri) :
  m_uri(_uri),
  m_config(null),
  m_confIdMode(-1),
  m_confIdDisplayOutside(-1),
  m_confIdChangeTime(-1),
  m_confProgramFile(-1),
  m_confColorFile(-1),
  m_confImageFile(-1),
  m_GLprogram(null),
  m_GLPosition(-1),
  m_GLMatrix(-1),
  m_GLStateActivate(-1),
  m_GLStateOld(-1),
  m_GLStateNew(-1),
  m_GLStateTransition(-1),
  m_resourceTexture(null),
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
	// Create the VBO:
	m_VBO = gale::resource::VirtualBufferObject::create(NB_VBO);
	if (m_VBO == null) {
		EWOL_ERROR("can not instanciate VBO ...");
		return;
	}
	// TO facilitate some debugs we add a name of the VBO:
	m_VBO->setName("[VBO] of ewol::compositing::Shaper");
	loadProgram();
}

ewol::compositing::Shaper::~Shaper() {
	unLoadProgram();
}

void ewol::compositing::Shaper::unLoadProgram() 

void ewol::compositing::Shaper::loadProgram() 

void ewol::compositing::Shaper::draw(bool _disableDepthTest) 
void ewol::compositing::Shaper::clear() 

bool ewol::compositing::Shaper::setState(int32_t _newState) 

bool ewol::compositing::Shaper::changeStatusIn(int32_t _newStatusId) 

bool ewol::compositing::Shaper::periodicCall(const ewol::event::Time& _event) 

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
		m_VBO->pushOnBuffer(m_vboIdCoord,
		                    m_VBO->getOnBufferVec2(m_vboIdCoord, m_nbVertexToDisplay-1));
		m_VBO->pushOnBuffer(m_vboIdPos,
		                    m_VBO->getOnBufferVec2(m_vboIdPos, m_nbVertexToDisplay-1));
		
		m_nbVertexToDisplay++;
		if (_displayOutside == true) {
			m_VBO->pushOnBuffer(m_vboIdCoord, vec2(_x1, _yButtom));
			m_VBO->pushOnBuffer(m_vboIdPos,        vec2(_table[0],_yValButtom));
			m_nbVertexToDisplay++;
		} else {
			m_VBO->pushOnBuffer(m_vboIdCoord, vec2(_x2, _yButtom));
			m_VBO->pushOnBuffer(m_vboIdPos,        vec2(_table[1],_yValButtom));
			m_nbVertexToDisplay++;
		}
	}
	
	if (_displayOutside == true) {
		// A
		m_VBO->pushOnBuffer(m_vboIdCoord, vec2(_x1, _yButtom));
		m_VBO->pushOnBuffer(m_vboIdPos,        vec2(_table[0],_yValButtom));
		m_nbVertexToDisplay++;
		m_VBO->pushOnBuffer(m_vboIdCoord, vec2(_x1, _yTop));
		m_VBO->pushOnBuffer(m_vboIdPos,        vec2(_table[0],_yValTop));
		m_nbVertexToDisplay++;
		m_VBO->pushOnBuffer(m_vboIdCoord, vec2(_x2, _yButtom));
		m_VBO->pushOnBuffer(m_vboIdPos,        vec2(_table[1],_yValButtom));
		m_nbVertexToDisplay++;
		// B
		m_VBO->pushOnBuffer(m_vboIdCoord, vec2(_x2, _yTop));
		m_VBO->pushOnBuffer(m_vboIdPos,        vec2(_table[1],_yValTop));
		m_nbVertexToDisplay++;
		
		// C
		m_VBO->pushOnBuffer(m_vboIdCoord, vec2(_x3, _yButtom));
		m_VBO->pushOnBuffer(m_vboIdPos,        vec2(_table[2],_yValButtom));
		m_nbVertexToDisplay++;
	} else {
		// C
		m_VBO->pushOnBuffer(m_vboIdCoord, vec2(_x2, _yButtom));
		m_VBO->pushOnBuffer(m_vboIdPos,        vec2(_table[1],_yValButtom));
		m_nbVertexToDisplay++;
		m_VBO->pushOnBuffer(m_vboIdCoord, vec2(_x2, _yTop));
		m_VBO->pushOnBuffer(m_vboIdPos,        vec2(_table[1],_yValTop));
		m_nbVertexToDisplay++;
		m_VBO->pushOnBuffer(m_vboIdCoord, vec2(_x3, _yButtom));
		m_VBO->pushOnBuffer(m_vboIdPos,        vec2(_table[2],_yValButtom));
		m_nbVertexToDisplay++;
	}
	// D
	m_VBO->pushOnBuffer(m_vboIdCoord, vec2(_x3, _yTop));
	m_VBO->pushOnBuffer(m_vboIdPos,        vec2(_table[2],_yValTop));
	m_nbVertexToDisplay++;
	
	// E
	m_VBO->pushOnBuffer(m_vboIdCoord, vec2(_x4, _yButtom));
	m_VBO->pushOnBuffer(m_vboIdPos,        vec2(_table[3],_yValButtom));
	m_nbVertexToDisplay++;
	// F
	m_VBO->pushOnBuffer(m_vboIdCoord, vec2(_x4, _yTop));
	m_VBO->pushOnBuffer(m_vboIdPos,        vec2(_table[3],_yValTop));
	m_nbVertexToDisplay++;
	
	// G
	m_VBO->pushOnBuffer(m_vboIdCoord, vec2(_x5, _yButtom));
	m_VBO->pushOnBuffer(m_vboIdPos,        vec2(_table[4],_yValButtom));
	m_nbVertexToDisplay++;
	// H
	m_VBO->pushOnBuffer(m_vboIdCoord, vec2(_x5, _yTop));
	m_VBO->pushOnBuffer(m_vboIdPos,        vec2(_table[4],_yValTop));
	m_nbVertexToDisplay++;
	
	// I
	m_VBO->pushOnBuffer(m_vboIdCoord, vec2(_x6, _yButtom));
	m_VBO->pushOnBuffer(m_vboIdPos,        vec2(_table[5],_yValButtom));
	m_nbVertexToDisplay++;
	// J
	m_VBO->pushOnBuffer(m_vboIdCoord, vec2(_x6, _yTop));
	m_VBO->pushOnBuffer(m_vboIdPos,        vec2(_table[5],_yValTop));
	m_nbVertexToDisplay++;
	
	// K
	m_VBO->pushOnBuffer(m_vboIdCoord, vec2(_x7, _yButtom));
	m_VBO->pushOnBuffer(m_vboIdPos,        vec2(_table[6],_yValButtom));
	m_nbVertexToDisplay++;
	// L
	m_VBO->pushOnBuffer(m_vboIdCoord, vec2(_x7, _yTop));
	m_VBO->pushOnBuffer(m_vboIdPos,        vec2(_table[6],_yValTop));
	m_nbVertexToDisplay++;
	
	if (_displayOutside == true) {
		// M
		m_VBO->pushOnBuffer(m_vboIdCoord, vec2(_x8, _yButtom));
		m_VBO->pushOnBuffer(m_vboIdPos,        vec2(_table[7],_yValButtom));
		m_nbVertexToDisplay++;
		// N
		m_VBO->pushOnBuffer(m_vboIdCoord, vec2(_x8, _yTop));
		m_VBO->pushOnBuffer(m_vboIdPos,        vec2(_table[7],_yValTop));
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
	m_VBO->clear();
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
		ewol::Padding inside(insideBorder.xLeft()   + etk::max(0.0f, paddingIn.xLeft()),
		                     insideBorder.yTop()    - etk::max(0.0f, paddingIn.yTop()),
		                     insideBorder.xRight()  - etk::max(0.0f, paddingIn.xRight()),
		                     insideBorder.yButtom() + etk::max(0.0f, paddingIn.yButtom()));
		
	#endif
	/*
	EWOL_ERROR(" enveloppe = " << enveloppe);
	EWOL_ERROR(" border = " << border);
	EWOL_ERROR(" inside = " << inside);
	*/
	int32_t mode = 0;
	bool displayOutside = false;
	if (m_config != null) {
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
	m_VBO->flush();
}

ewol::Padding ewol::compositing::Shaper::getPadding() 

ewol::Padding ewol::compositing::Shaper::getPaddingIn() 

ewol::Padding ewol::compositing::Shaper::getPaddingOut()

ewol::Padding ewol::compositing::Shaper::getBorder() 

void ewol::compositing::Shaper::setSource(const etk::Uri& _uri) 

bool ewol::compositing::Shaper::hasSources() 

const etk::Color<float>& ewol::compositing::Shaper::getColor(int32_t _id) {
	static const etk::Color<float> errorValue(0,0,0,0);
	if (m_colorProperty == null) {
		EWOL_WARNING("null of m_colorProperty ==> return #0000 for id " << _id);
		return errorValue;
	}
	return m_colorProperty->get(_id);
}

int32_t ewol::compositing::Shaper::requestColor(const etk::String& _name) {
	if (m_colorProperty == null) {
		EWOL_WARNING("null of m_colorProperty ==> return -1 for name " << _name);
		return -1;
	}
	return m_colorProperty->request(_name);
}

int32_t ewol::compositing::Shaper::requestConfig(const etk::String& _name) {
	if (m_config == null) {
		EWOL_WARNING("null of m_config ==> return -1 for name " << _name);
		return -1;
	}
	return m_config->request(_name);
}

double ewol::compositing::Shaper::getConfigNumber(int32_t _id) {
	if (    _id == -1
	     || m_config == null) {
		EWOL_WARNING("null of m_config ==> return 0.0 for id " << _id);
		return 0.0;
	}
	return m_config->getNumber(_id);
}


namespace etk {
	template<> etk::String toString<ewol::compositing::Shaper>(const ewol::compositing::Shaper& _obj) {
		return _obj.getSource().get();
	}
	template<> etk::UString toUString<ewol::compositing::Shaper>(const ewol::compositing::Shaper& _obj) {
		return etk::toUString(etk::toString(_obj));
	}
	template<> bool from_string<ewol::compositing::Shaper>(ewol::compositing::Shaper& _variableRet, const etk::String& _value) {
		_variableRet.setSource(_value);
		return true;
	}
	template<> bool from_string<ewol::compositing::Shaper>(ewol::compositing::Shaper& _variableRet, const etk::UString& _value) {
		return from_string(_variableRet,  etk::toString(_value));
	}
};