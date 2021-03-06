/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */

#include <ewol/debug.hpp>
#include <ewol/compositing/Drawing.hpp>

#include <etk/typeInfo.hpp>
ETK_DECLARE_TYPE(ewol::compositing::Drawing);

// VBO table property:
const int32_t ewol::compositing::Drawing::m_vboIdCoord(0);
const int32_t ewol::compositing::Drawing::m_vboIdColor(1);
#define NB_VBO (2)

#if 0

static void generatePolyGone(etk::Vector<vec2 > & input, etk::Vector<vec2 > & output )
{
	if (input.size()<3) {
		return;
	}
	// TODO : Regenerate a linear poligone generation
	for (int32_t iii=1; iii<input.size()-1; iii++) {
		output.pushBack(input[0]);
		output.pushBack(input[iii]);
		output.pushBack(input[iii+1]);
	}
	//EWOL_DEBUG("generate Plygone : " << input.size() << "  == > " << output.size() );
}

static void SutherlandHodgman(etk::Vector<vec2 > & input, etk::Vector<vec2 > & output, float sx, float sy, float ex, float ey)
{
	// with Sutherland-Hodgman-Algorithm
	if (input.size() <0) {
		return;
	}
	//int32_t sizeInit=input.size();
	// last element :
	vec2 destPoint;
	vec2 lastElement = input[input.size()-1];
	bool inside = true;
	if (lastElement.x < sx) {
		inside = false;
	}
	//EWOL_DEBUG("generate an crop : ");
	for(int32_t iii=0; iii<input.size(); iii++) {
		if(input[iii].x < sx) {
			if(true == inside) {
				//EWOL_DEBUG("element IN  == > OUT ");
				//new point intersection ...
				//y=aaax+bbb
				float aaa = (lastElement.y-input[iii].y) / (lastElement.x-input[iii].x);
				float bbb = lastElement.y - (aaa*lastElement.x);
				destPoint.y = aaa*sx + bbb;
				destPoint.x = sx;
				output.pushBack(destPoint);
			} else {
				//EWOL_DEBUG("element OUT  == > OUT ");
			}
			inside = false;
		} else {
			if(true == inside) {
				//EWOL_DEBUG("element IN  == > IN ");
				output.pushBack(input[iii]);
			} else {
				//EWOL_DEBUG("element OUT  == > IN ");
				//new point intersection ...
				//y=aaax+bbb
				float aaa = (lastElement.y-input[iii].y) / (lastElement.x-input[iii].x);
				float bbb = lastElement.y - (aaa*lastElement.x);
				destPoint.y = aaa*sx + bbb;
				destPoint.x = sx;
				output.pushBack(destPoint);
				output.pushBack(input[iii]);
			}
			inside = true;
		}
		// update the last point position :
		lastElement.x = input[iii].x;
		lastElement.y = input[iii].y;
	}
	
	//EWOL_DEBUG("generate an crop on element : " << sizeInit << "  == > " << output.size() << "intermediate (1)");
	input = output;
	output.clear();
	lastElement = input[input.size()-1];
	inside = true;
	if (lastElement.y < sy) {
		inside = false;
	}
	for(int32_t iii=0; iii<input.size(); iii++) {
		if(input[iii].y < sy) {
			if(true == inside) {
				//EWOL_DEBUG("element IN  == > OUT ");
				//new point intersection ...
				//x=aaay+bbb
				float aaa = (lastElement.x-input[iii].x) / (lastElement.y-input[iii].y);
				float bbb = lastElement.x - (aaa*lastElement.y);
				destPoint.y = sy;
				destPoint.x = sy*aaa + bbb;
				output.pushBack(destPoint);
			} else {
				//EWOL_DEBUG("element OUT  == > OUT ");
			}
			inside = false;
		} else {
			if(true == inside) {
				//EWOL_DEBUG("element IN  == > IN ");
				output.pushBack(input[iii]);
			} else {
				//EWOL_DEBUG("element OUT  == > IN ");
				//new point intersection ...
				//y=aaax+bbb
				float aaa = (lastElement.x-input[iii].x) / (lastElement.y-input[iii].y);
				float bbb = lastElement.x - (aaa*lastElement.y);
				destPoint.y = sy;
				destPoint.x = sy*aaa + bbb;
				output.pushBack(destPoint);
				output.pushBack(input[iii]);
			}
			inside = true;
		}
		// update the last point position :
		lastElement.x = input[iii].x;
		lastElement.y = input[iii].y;
	}
	
	input = output;
	output.clear();
	lastElement = input[input.size()-1];
	inside = true;
	if (lastElement.x > ex) {
		inside = false;
	}
	//EWOL_DEBUG("generate an crop : ");
	for(int32_t iii=0; iii<input.size(); iii++) {
		if(input[iii].x > ex) {
			if(true == inside) {
				//EWOL_DEBUG("element IN  == > OUT ");
				//new point intersection ...
				//y=aaax+bbb
				float aaa = (lastElement.y-input[iii].y) / (lastElement.x-input[iii].x);
				float bbb = lastElement.y - (aaa*lastElement.x);
				destPoint.y = aaa*ex + bbb;
				destPoint.x = ex;
				output.pushBack(destPoint);
			} else {
				//EWOL_DEBUG("element OUT  == > OUT ");
			}
			inside = false;
		} else {
			if(true == inside) {
				//EWOL_DEBUG("element IN  == > IN ");
				output.pushBack(input[iii]);
			} else {
				//EWOL_DEBUG("element OUT  == > IN ");
				//new point intersection ...
				//y=aaax+bbb
				float aaa = (lastElement.y-input[iii].y) / (lastElement.x-input[iii].x);
				float bbb = lastElement.y - (aaa*lastElement.x);
				destPoint.y = aaa*ex + bbb;
				destPoint.x = ex;
				output.pushBack(destPoint);
				output.pushBack(input[iii]);
			}
			inside = true;
		}
		// update the last point position :
		lastElement.x = input[iii].x;
		lastElement.y = input[iii].y;
	}
	
	input = output;
	output.clear();
	lastElement = input[input.size()-1];
	inside = true;
	if (lastElement.y > ey) {
		inside = false;
	}
	for(int32_t iii=0; iii<input.size(); iii++) {
		if(input[iii].y > ey) {
			if(true == inside) {
				//EWOL_DEBUG("element IN  == > OUT ");
				//new point intersection ...
				//x=aaay+bbb
				float aaa = (lastElement.x-input[iii].x) / (lastElement.y-input[iii].y);
				float bbb = lastElement.x - (aaa*lastElement.y);
				destPoint.y = ey;
				destPoint.x = ey*aaa + bbb;
				output.pushBack(destPoint);
			} else {
				//EWOL_DEBUG("element OUT  == > OUT ");
			}
			inside = false;
		} else {
			if(true == inside) {
				//EWOL_DEBUG("element IN  == > IN ");
				output.pushBack(input[iii]);
			} else {
				//EWOL_DEBUG("element OUT  == > IN ");
				//new point intersection ...
				//y=aaax+bbb
				float aaa = (lastElement.x-input[iii].x) / (lastElement.y-input[iii].y);
				float bbb = lastElement.x - (aaa*lastElement.y);
				destPoint.y = ey;
				destPoint.x = ey*aaa + bbb;
				output.pushBack(destPoint);
				output.pushBack(input[iii]);
			}
			inside = true;
		}
		// update the last point position :
		lastElement.x = input[iii].x;
		lastElement.y = input[iii].y;
	}
	
	
	//EWOL_DEBUG("generate an crop on element : " << sizeInit << "  == > " << output.size() );
}
#endif

ewol::compositing::Drawing::Drawing() :
  m_position(0.0, 0.0, 0.0),
  m_clippingPosStart(0.0, 0.0, 0.0),
  m_clippingPosStop(0.0, 0.0, 0.0),
  m_clippingEnable(false),
  m_color(etk::color::black),
  m_colorBg(etk::color::none),
  m_GLprogram(null),
  m_GLPosition(-1),
  m_GLMatrix(-1),
  m_GLMatrixPosition(-1),
  m_GLColor(-1),
  m_thickness(0.0),
  m_triElement(0) {
	loadProgram();
	for (int32_t iii=0; iii<3; iii++) {
		m_triangle[iii] = m_position;
		m_tricolor[iii] = m_color;
	}
	// Create the VBO:
	m_VBO = gale::resource::VirtualBufferObject::create(NB_VBO);
	if (m_VBO == null) {
		EWOL_ERROR("can not instanciate VBO ...");
		return;
	}
	// TO facilitate some debugs we add a name of the VBO:
	m_VBO->setName("[VBO] of ewol::compositing::Area");
}

ewol::compositing::Drawing::~Drawing() {
	unLoadProgram();
}

void ewol::compositing::Drawing::generateTriangle() {
	m_triElement = 0;
	
	m_VBO->pushOnBuffer(m_vboIdCoord, m_triangle[0]);
	m_VBO->pushOnBuffer(m_vboIdColor, m_tricolor[0]);
	m_VBO->pushOnBuffer(m_vboIdCoord, m_triangle[1]);
	m_VBO->pushOnBuffer(m_vboIdColor, m_tricolor[1]);
	m_VBO->pushOnBuffer(m_vboIdCoord, m_triangle[2]);
	m_VBO->pushOnBuffer(m_vboIdColor, m_tricolor[2]);
}

void ewol::compositing::Drawing::internalSetColor(const etk::Color<>& _color) {
	if (m_triElement < 1) {
		m_tricolor[0] = _color;
	}
	if (m_triElement < 2) {
		m_tricolor[1] = _color;
	}
	if (m_triElement < 3) {
		m_tricolor[2] = _color;
	}
}

void ewol::compositing::Drawing::setPoint(const vec3& _point) {
	m_triangle[m_triElement] = _point;
	m_triElement++;
	if (m_triElement >= 3) {
		generateTriangle();
	}
	m_VBO->flush();
}

void ewol::compositing::Drawing::resetCount() {
	m_triElement = 0;
}

void ewol::compositing::Drawing::unLoadProgram() {
	m_GLprogram.reset();
}

void ewol::compositing::Drawing::loadProgram() {
	// remove previous loading ... in case
	unLoadProgram();
	// oad the new ...
	m_GLprogram = gale::resource::Program::create("DATA:///color3.prog?lib=ewol");
	// get the shader resource :
	if (m_GLprogram != null) {
		m_GLPosition = m_GLprogram->getAttribute("EW_coord3d");
		m_GLColor = m_GLprogram->getAttribute("EW_color");
		m_GLMatrix = m_GLprogram->getUniform("EW_MatrixTransformation");
		m_GLMatrixPosition = m_GLprogram->getUniform("EW_MatrixPosition");
	}
}

void ewol::compositing::Drawing::draw(bool _disableDepthTest) {
	if (m_VBO->bufferSize(m_vboIdCoord) <= 0) {
		// TODO : set it back ...
		//EWOL_WARNING("Nothink to draw...");
		return;
	}
	if (m_GLprogram == null) {
		EWOL_ERROR("No shader ...");
		return;
	}
	// set Matrix : translation/positionMatrix
	mat4 tmpMatrix = gale::openGL::getMatrix()*m_matrixApply;
	m_GLprogram->use();
	m_GLprogram->uniformMatrix(m_GLMatrix, tmpMatrix);
	mat4 tmpMatrix2;
	m_GLprogram->uniformMatrix(m_GLMatrixPosition, tmpMatrix2);
	// position:
	m_GLprogram->sendAttributePointer(m_GLPosition, m_VBO, m_vboIdCoord);
	// color:
	m_GLprogram->sendAttributePointer(m_GLColor, m_VBO, m_vboIdColor);
	// Request the draw od the elements : 
	gale::openGL::drawArrays(gale::openGL::renderMode::triangle, 0, m_VBO->bufferSize(m_vboIdCoord));
	m_GLprogram->unUse();
}

void ewol::compositing::Drawing::clear() {
	// call upper class
	ewol::Compositing::clear();
	// reset Buffer :
	m_VBO->clear();
	// reset temporal variables :
	m_position = vec3(0.0, 0.0, 0.0);
	
	m_clippingPosStart = vec3(0.0, 0.0, 0.0);
	m_clippingPosStop = vec3(0.0, 0.0, 0.0);
	m_clippingEnable = false;
	
	m_color = etk::color::black;
	m_colorBg = etk::color::none;
	
	for (int32_t iii=0; iii<3; iii++) {
		m_triangle[iii] = m_position;
		m_tricolor[iii] = m_color;
	}
}

void ewol::compositing::Drawing::setClipping(const vec3& _pos, const vec3& _posEnd) {
	// note the internal system all time request to have a bounding all time in the same order
	if (_pos.x() <= _posEnd.x()) {
		m_clippingPosStart.setX(_pos.x());
		m_clippingPosStop.setX(_posEnd.x());
	} else {
		m_clippingPosStart.setX(_posEnd.x());
		m_clippingPosStop.setX(_pos.x());
	}
	if (_pos.y() <= _posEnd.y()) {
		m_clippingPosStart.setY(_pos.y());
		m_clippingPosStop.setY(_posEnd.y());
	} else {
		m_clippingPosStart.setY(_posEnd.y());
		m_clippingPosStop.setY(_pos.y());
	}
	if (_pos.z() <= _posEnd.z()) {
		m_clippingPosStart.setZ(_pos.z());
		m_clippingPosStop.setZ(_posEnd.z());
	} else {
		m_clippingPosStart.setZ(_posEnd.z());
		m_clippingPosStop.setZ(_pos.z());
	}
	m_clippingEnable = true;
}

void ewol::compositing::Drawing::setThickness(float _thickness) {
	m_thickness = _thickness;
	// thickness must be positive
	if (m_thickness < 0) {
		m_thickness *= -1;
	}
}

void ewol::compositing::Drawing::addVertex() {
	internalSetColor(m_color);
	setPoint(m_position);
}

void ewol::compositing::Drawing::lineTo(const vec3& _dest) {
	resetCount();
	internalSetColor(m_color);
	//EWOL_VERBOSE("DrawLine : " << m_position << " to " << _dest);
	if (m_position.x() == _dest.x() && m_position.y() == _dest.y() && m_position.z() == _dest.z()) {
		//EWOL_WARNING("Try to draw a line width 0");
		return;
	}
	//teta = tan-1(oposer/adjacent)
	float teta = 0;
	if (m_position.x() <= _dest.x()) {
		teta = atan((_dest.y()-m_position.y())/(_dest.x()-m_position.x()));
	} else {
		teta = M_PI + atan((_dest.y()-m_position.y())/(_dest.x()-m_position.x()));
	}
	if (teta < 0) {
		teta += 2*M_PI;
	} else if (teta > 2*M_PI) {
		teta -= 2*M_PI;
	}
	//EWOL_DEBUG("teta = " << (teta*180/(M_PI)) << " deg." );
	float offsety = sin(teta-M_PI/2) * (m_thickness/2);
	float offsetx = cos(teta-M_PI/2) * (m_thickness/2);
	setPoint(vec3(m_position.x() - offsetx, m_position.y() - offsety, m_position.z()) );
	setPoint(vec3(m_position.x() + offsetx, m_position.y() + offsety, m_position.z()) );
	setPoint(vec3(_dest.x()      + offsetx, _dest.y()      + offsety, m_position.z()) );
	
	setPoint(vec3(_dest.x()      + offsetx, _dest.y()      + offsety, _dest.z()) );
	setPoint(vec3(_dest.x()      - offsetx, _dest.y()      - offsety, _dest.z()) );
	setPoint(vec3(m_position.x() - offsetx, m_position.y() - offsety, _dest.z()) );
	// update the system position :
	m_position = _dest;
}

void ewol::compositing::Drawing::rectangle(const vec3& _dest) {
	resetCount();
	internalSetColor(m_color);
	/* Bitmap position
	 *      xA     xB
	 *   yC *------*
	 *      |      |
	 *      |      |
	 *   yD *------*
	 */
	float dxA = m_position.x();
	float dxB = _dest.x();
	if (dxA > dxB) {
		// inverse order : 
		float tmp = dxA;
		dxA = dxB;
		dxB = tmp;
	}
	float dyC = m_position.y();
	float dyD = _dest.y();
	if (dyC > dyD) {
		// inverse order : 
		float tmp = dyC;
		dyC = dyD;
		dyD = tmp;
	}
	if (true == m_clippingEnable) {
		if (dxA < m_clippingPosStart.x()) {
			dxA = m_clippingPosStart.x();
		}
		if (dxB > m_clippingPosStop.x()) {
			dxB = m_clippingPosStop.x();
		}
		if (dyC < m_clippingPosStart.y()) {
			dyC = m_clippingPosStart.y();
		}
		if (dyD > m_clippingPosStop.y()) {
			dyD = m_clippingPosStop.y();
		}
	}
	if(    dyC >= dyD
	    || dxA >= dxB) {
		return;
	}
	setPoint(vec3(dxA, dyD, 0) );
	setPoint(vec3(dxA, dyC, 0) );
	setPoint(vec3(dxB, dyC, 0) );

	setPoint(vec3(dxB, dyC, 0) );
	setPoint(vec3(dxB, dyD, 0) );
	setPoint(vec3(dxA, dyD, 0) );
}

void ewol::compositing::Drawing::cube(const vec3& _dest) {
	
}

void ewol::compositing::Drawing::circle(float _radius, float _angleStart, float _angleStop) {
	resetCount();
	
	if (_radius<0) {
		_radius *= -1;
	}
	_angleStop = _angleStop-_angleStart;
	
	
	int32_t nbOcurence = _radius;
	if (nbOcurence < 10)
	{
		nbOcurence = 10;
	}
	
	// display background :
	if (m_colorBg.a()!=0) {
		internalSetColor(m_colorBg);
		for (int32_t iii=0; iii<nbOcurence; iii++) {
			setPoint(vec3(m_position.x(),
			              m_position.y(),
			              0) );
			
			float angleOne = _angleStart + (_angleStop* iii / nbOcurence) ;
			float offsety = sin(angleOne) * _radius;
			float offsetx = cos(angleOne) * _radius;
			
			setPoint(vec3(m_position.x() + offsetx,
			              m_position.y() + offsety,
			              0) );
			
			float angleTwo = _angleStart + (_angleStop* (iii+1) / nbOcurence) ;
			offsety = sin(angleTwo) * _radius;
			offsetx = cos(angleTwo) * _radius;
			
			setPoint(vec3(m_position.x() + offsetx,
			              m_position.y() + offsety,
			              0) );
		}
	}
	
	// show if we have a border :
	if(    m_thickness == 0
	    || m_color.a() == 0) {
		return;
	}
	internalSetColor(m_color);
	for (int32_t iii=0; iii<nbOcurence; iii++) {
		
		float angleOne =  _angleStart + (_angleStop* iii     / nbOcurence) ;
		float offsetExty = sin(angleOne) * (_radius+m_thickness/2);
		float offsetExtx = cos(angleOne) * (_radius+m_thickness/2);
		float offsetInty = sin(angleOne) * (_radius-m_thickness/2);
		float offsetIntx = cos(angleOne) * (_radius-m_thickness/2);
		
		float angleTwo =  _angleStart + (_angleStop*  (iii+1) / nbOcurence );
		float offsetExt2y = sin(angleTwo) * (_radius+m_thickness/2);
		float offsetExt2x = cos(angleTwo) * (_radius+m_thickness/2);
		float offsetInt2y = sin(angleTwo) * (_radius-m_thickness/2);
		float offsetInt2x = cos(angleTwo) * (_radius-m_thickness/2);
		
		setPoint(vec3(m_position.x() + offsetIntx,  m_position.y() + offsetInty,  0));
		setPoint(vec3(m_position.x() + offsetExtx,  m_position.y() + offsetExty,  0));
		setPoint(vec3(m_position.x() + offsetExt2x, m_position.y() + offsetExt2y, 0));
		
		setPoint(vec3(m_position.x() + offsetExt2x, m_position.y() + offsetExt2y, 0));
		setPoint(vec3(m_position.x() + offsetInt2x, m_position.y() + offsetInt2y, 0));
		setPoint(vec3(m_position.x() + offsetIntx,  m_position.y() + offsetInty,  0));
	}
}

