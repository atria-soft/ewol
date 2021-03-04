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
 int ewol::compositing::Drawing::this.vboIdCoord(0);
 int ewol::compositing::Drawing::this.vboIdColor(1);
#define NB_VBO (2)

#if 0

static void generatePolyGone(List<Vector2f >  input, List<Vector2f >  output )
{
	if (input.size()<3) {
		return;
	}
	// TODO : Regenerate a linear poligone generation
	for (int iii=1; iii<input.size()-1; iii++) {
		output.pushBack(input[0]);
		output.pushBack(input[iii]);
		output.pushBack(input[iii+1]);
	}
	//Log.debug("generate Plygone : " + input.size() + "  == > " + output.size() );
}

static void SutherlandHodgman(List<Vector2f >  input, List<Vector2f >  output, float sx, float sy, float ex, float ey)
{
	// with Sutherland-Hodgman-Algorithm
	if (input.size() <0) {
		return;
	}
	//int sizeInit=input.size();
	// last element :
	Vector2f destPoint;
	Vector2f lastElement = input[input.size()-1];
	boolean inside = true;
	if (lastElement.x < sx) {
		inside = false;
	}
	//Log.debug("generate an crop : ");
	for(int iii=0; iii<input.size(); iii++) {
		if(input[iii].x < sx) {
			if(true == inside) {
				//Log.debug("element IN  == > OUT ");
				//new point intersection ...
				//y=aaax+bbb
				float aaa = (lastElement.y-input[iii].y) / (lastElement.x-input[iii].x);
				float bbb = lastElement.y - (aaa*lastElement.x);
				destPoint.y = aaa*sx + bbb;
				destPoint.x = sx;
				output.pushBack(destPoint);
			} else {
				//Log.debug("element OUT  == > OUT ");
			}
			inside = false;
		} else {
			if(true == inside) {
				//Log.debug("element IN  == > IN ");
				output.pushBack(input[iii]);
			} else {
				//Log.debug("element OUT  == > IN ");
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
	
	//Log.debug("generate an crop on element : " + sizeInit + "  == > " + output.size() + "intermediate (1)");
	input = output;
	output.clear();
	lastElement = input[input.size()-1];
	inside = true;
	if (lastElement.y < sy) {
		inside = false;
	}
	for(int iii=0; iii<input.size(); iii++) {
		if(input[iii].y < sy) {
			if(true == inside) {
				//Log.debug("element IN  == > OUT ");
				//new point intersection ...
				//x=aaay+bbb
				float aaa = (lastElement.x-input[iii].x) / (lastElement.y-input[iii].y);
				float bbb = lastElement.x - (aaa*lastElement.y);
				destPoint.y = sy;
				destPoint.x = sy*aaa + bbb;
				output.pushBack(destPoint);
			} else {
				//Log.debug("element OUT  == > OUT ");
			}
			inside = false;
		} else {
			if(true == inside) {
				//Log.debug("element IN  == > IN ");
				output.pushBack(input[iii]);
			} else {
				//Log.debug("element OUT  == > IN ");
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
	//Log.debug("generate an crop : ");
	for(int iii=0; iii<input.size(); iii++) {
		if(input[iii].x > ex) {
			if(true == inside) {
				//Log.debug("element IN  == > OUT ");
				//new point intersection ...
				//y=aaax+bbb
				float aaa = (lastElement.y-input[iii].y) / (lastElement.x-input[iii].x);
				float bbb = lastElement.y - (aaa*lastElement.x);
				destPoint.y = aaa*ex + bbb;
				destPoint.x = ex;
				output.pushBack(destPoint);
			} else {
				//Log.debug("element OUT  == > OUT ");
			}
			inside = false;
		} else {
			if(true == inside) {
				//Log.debug("element IN  == > IN ");
				output.pushBack(input[iii]);
			} else {
				//Log.debug("element OUT  == > IN ");
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
	for(int iii=0; iii<input.size(); iii++) {
		if(input[iii].y > ey) {
			if(true == inside) {
				//Log.debug("element IN  == > OUT ");
				//new point intersection ...
				//x=aaay+bbb
				float aaa = (lastElement.x-input[iii].x) / (lastElement.y-input[iii].y);
				float bbb = lastElement.x - (aaa*lastElement.y);
				destPoint.y = ey;
				destPoint.x = ey*aaa + bbb;
				output.pushBack(destPoint);
			} else {
				//Log.debug("element OUT  == > OUT ");
			}
			inside = false;
		} else {
			if(true == inside) {
				//Log.debug("element IN  == > IN ");
				output.pushBack(input[iii]);
			} else {
				//Log.debug("element OUT  == > IN ");
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
	
	
	//Log.debug("generate an crop on element : " + sizeInit + "  == > " + output.size() );
}
#endif

ewol::compositing::Drawing::Drawing() :
  this.position(0.0, 0.0, 0.0),
  this.clippingPosStart(0.0, 0.0, 0.0),
  this.clippingPosStop(0.0, 0.0, 0.0),
  this.clippingEnable(false),
  this.color(etk::color::black),
  this.colorBg(etk::color::none),
  this.GLprogram(null),
  this.GLPosition(-1),
  this.GLMatrix(-1),
  this.GLMatrixPosition(-1),
  this.GLColor(-1),
  this.thickness(0.0),
  this.triElement(0) {
	loadProgram();
	for (int iii=0; iii<3; iii++) {
		this.triangle[iii] = this.position;
		this.tricolor[iii] = this.color;
	}
	// Create the VBO:
	this.VBO = gale::resource::VirtualBufferObject::create(NB_VBO);
	if (this.VBO == null) {
		Log.error("can not instanciate VBO ...");
		return;
	}
	// TO facilitate some debugs we add a name of the VBO:
	this.VBO.setName("[VBO] of ewol::compositing::Area");
}

ewol::compositing::Drawing::~Drawing() {
	unLoadProgram();
}

void ewol::compositing::Drawing::generateTriangle() {
	this.triElement = 0;
	
	this.VBO.pushOnBuffer(this.vboIdCoord, this.triangle[0]);
	this.VBO.pushOnBuffer(this.vboIdColor, this.tricolor[0]);
	this.VBO.pushOnBuffer(this.vboIdCoord, this.triangle[1]);
	this.VBO.pushOnBuffer(this.vboIdColor, this.tricolor[1]);
	this.VBO.pushOnBuffer(this.vboIdCoord, this.triangle[2]);
	this.VBO.pushOnBuffer(this.vboIdColor, this.tricolor[2]);
}

void ewol::compositing::Drawing::internalSetColor( etk::Color<> _color) {
	if (this.triElement < 1) {
		this.tricolor[0] = _color;
	}
	if (this.triElement < 2) {
		this.tricolor[1] = _color;
	}
	if (this.triElement < 3) {
		this.tricolor[2] = _color;
	}
}

void ewol::compositing::Drawing::setPoint( Vector3f _point) {
	this.triangle[this.triElement] = _point;
	this.triElement++;
	if (this.triElement >= 3) {
		generateTriangle();
	}
	this.VBO.flush();
}

void ewol::compositing::Drawing::resetCount() {
	this.triElement = 0;
}

void ewol::compositing::Drawing::unLoadProgram() {
	this.GLprogram.reset();
}

void ewol::compositing::Drawing::loadProgram() {
	// remove previous loading ... in case
	unLoadProgram();
	// oad the new ...
	this.GLprogram = gale::resource::Program::create("DATA:///color3.prog?lib=ewol");
	// get the shader resource :
	if (this.GLprogram != null) {
		this.GLPosition = this.GLprogram.getAttribute("EW_coord3d");
		this.GLColor = this.GLprogram.getAttribute("EW_color");
		this.GLMatrix = this.GLprogram.getUniform("EW_MatrixTransformation");
		this.GLMatrixPosition = this.GLprogram.getUniform("EW_MatrixPosition");
	}
}

void ewol::compositing::Drawing::draw(boolean _disableDepthTest) {
	if (this.VBO.bufferSize(this.vboIdCoord) <= 0) {
		// TODO : set it back ...
		//Log.warning("Nothink to draw...");
		return;
	}
	if (this.GLprogram == null) {
		Log.error("No shader ...");
		return;
	}
	// set Matrix : translation/positionMatrix
	mat4 tmpMatrix = gale::openGL::getMatrix()*this.matrixApply;
	this.GLprogram.use();
	this.GLprogram.uniformMatrix(this.GLMatrix, tmpMatrix);
	mat4 tmpMatrix2;
	this.GLprogram.uniformMatrix(this.GLMatrixPosition, tmpMatrix2);
	// position:
	this.GLprogram.sendAttributePointer(this.GLPosition, this.VBO, this.vboIdCoord);
	// color:
	this.GLprogram.sendAttributePointer(this.GLColor, this.VBO, this.vboIdColor);
	// Request the draw od the elements : 
	gale::openGL::drawArrays(gale::openGL::renderMode::triangle, 0, this.VBO.bufferSize(this.vboIdCoord));
	this.GLprogram.unUse();
}

void ewol::compositing::Drawing::clear() {
	// call upper class
	ewol::Compositing::clear();
	// reset Buffer :
	this.VBO.clear();
	// reset temporal variables :
	this.position = Vector3f(0.0, 0.0, 0.0);
	
	this.clippingPosStart = Vector3f(0.0, 0.0, 0.0);
	this.clippingPosStop = Vector3f(0.0, 0.0, 0.0);
	this.clippingEnable = false;
	
	this.color = etk::color::black;
	this.colorBg = etk::color::none;
	
	for (int iii=0; iii<3; iii++) {
		this.triangle[iii] = this.position;
		this.tricolor[iii] = this.color;
	}
}

void ewol::compositing::Drawing::setClipping( Vector3f _pos,  Vector3f _posEnd) {
	// note the internal system all time request to have a bounding all time in the same order
	if (_pos.x() <= _posEnd.x()) {
		this.clippingPosStart.setX(_pos.x());
		this.clippingPosStop.setX(_posEnd.x());
	} else {
		this.clippingPosStart.setX(_posEnd.x());
		this.clippingPosStop.setX(_pos.x());
	}
	if (_pos.y() <= _posEnd.y()) {
		this.clippingPosStart.setY(_pos.y());
		this.clippingPosStop.setY(_posEnd.y());
	} else {
		this.clippingPosStart.setY(_posEnd.y());
		this.clippingPosStop.setY(_pos.y());
	}
	if (_pos.z() <= _posEnd.z()) {
		this.clippingPosStart.setZ(_pos.z());
		this.clippingPosStop.setZ(_posEnd.z());
	} else {
		this.clippingPosStart.setZ(_posEnd.z());
		this.clippingPosStop.setZ(_pos.z());
	}
	this.clippingEnable = true;
}

void ewol::compositing::Drawing::setThickness(float _thickness) {
	this.thickness = _thickness;
	// thickness must be positive
	if (this.thickness < 0) {
		this.thickness *= -1;
	}
}

void ewol::compositing::Drawing::addVertex() {
	internalSetColor(this.color);
	setPoint(this.position);
}

void ewol::compositing::Drawing::lineTo( Vector3f _dest) {
	resetCount();
	internalSetColor(this.color);
	//Log.verbose("DrawLine : " + this.position + " to " + _dest);
	if (this.position.x() == _dest.x() LOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOM this.position.y() == _dest.y() LOMLOMLOMLOMLOM this.position.z() == _dest.z()) {
		//Log.warning("Try to draw a line width 0");
		return;
	}
	//teta = tan-1(oposer/adjacent)
	float teta = 0;
	if (this.position.x() <= _dest.x()) {
		teta = atan((_dest.y()-this.position.y())/(_dest.x()-this.position.x()));
	} else {
		teta = M_PI + atan((_dest.y()-this.position.y())/(_dest.x()-this.position.x()));
	}
	if (teta < 0) {
		teta += 2*M_PI;
	} else if (teta > 2*M_PI) {
		teta -= 2*M_PI;
	}
	//Log.debug("teta = " + (teta*180/(M_PI)) + " deg." );
	float offsety = sin(teta-M_PI/2) * (this.thickness/2);
	float offsetx = cos(teta-M_PI/2) * (this.thickness/2);
	setPoint(Vector3f(this.position.x() - offsetx, this.position.y() - offsety, this.position.z()) );
	setPoint(Vector3f(this.position.x() + offsetx, this.position.y() + offsety, this.position.z()) );
	setPoint(Vector3f(_dest.x()      + offsetx, _dest.y()      + offsety, this.position.z()) );
	
	setPoint(Vector3f(_dest.x()      + offsetx, _dest.y()      + offsety, _dest.z()) );
	setPoint(Vector3f(_dest.x()      - offsetx, _dest.y()      - offsety, _dest.z()) );
	setPoint(Vector3f(this.position.x() - offsetx, this.position.y() - offsety, _dest.z()) );
	// update the system position :
	this.position = _dest;
}

void ewol::compositing::Drawing::rectangle( Vector3f _dest) {
	resetCount();
	internalSetColor(this.color);
	/* Bitmap position
	 *      xA     xB
	 *   yC *------*
	 *      |      |
	 *      |      |
	 *   yD *------*
	 */
	float dxA = this.position.x();
	float dxB = _dest.x();
	if (dxA > dxB) {
		// inverse order : 
		float tmp = dxA;
		dxA = dxB;
		dxB = tmp;
	}
	float dyC = this.position.y();
	float dyD = _dest.y();
	if (dyC > dyD) {
		// inverse order : 
		float tmp = dyC;
		dyC = dyD;
		dyD = tmp;
	}
	if (true == this.clippingEnable) {
		if (dxA < this.clippingPosStart.x()) {
			dxA = this.clippingPosStart.x();
		}
		if (dxB > this.clippingPosStop.x()) {
			dxB = this.clippingPosStop.x();
		}
		if (dyC < this.clippingPosStart.y()) {
			dyC = this.clippingPosStart.y();
		}
		if (dyD > this.clippingPosStop.y()) {
			dyD = this.clippingPosStop.y();
		}
	}
	if(    dyC >= dyD
	    || dxA >= dxB) {
		return;
	}
	setPoint(Vector3f(dxA, dyD, 0) );
	setPoint(Vector3f(dxA, dyC, 0) );
	setPoint(Vector3f(dxB, dyC, 0) );

	setPoint(Vector3f(dxB, dyC, 0) );
	setPoint(Vector3f(dxB, dyD, 0) );
	setPoint(Vector3f(dxA, dyD, 0) );
}

void ewol::compositing::Drawing::cube( Vector3f _dest) {
	
}

void ewol::compositing::Drawing::circle(float _radius, float _angleStart, float _angleStop) {
	resetCount();
	
	if (_radius<0) {
		_radius *= -1;
	}
	_angleStop = _angleStop-_angleStart;
	
	
	int nbOcurence = _radius;
	if (nbOcurence < 10)
	{
		nbOcurence = 10;
	}
	
	// display background :
	if (this.colorBg.a()!=0) {
		internalSetColor(this.colorBg);
		for (int iii=0; iii<nbOcurence; iii++) {
			setPoint(Vector3f(this.position.x(),
			              this.position.y(),
			              0) );
			
			float angleOne = _angleStart + (_angleStop* iii / nbOcurence) ;
			float offsety = sin(angleOne) * _radius;
			float offsetx = cos(angleOne) * _radius;
			
			setPoint(Vector3f(this.position.x() + offsetx,
			              this.position.y() + offsety,
			              0) );
			
			float angleTwo = _angleStart + (_angleStop* (iii+1) / nbOcurence) ;
			offsety = sin(angleTwo) * _radius;
			offsetx = cos(angleTwo) * _radius;
			
			setPoint(Vector3f(this.position.x() + offsetx,
			              this.position.y() + offsety,
			              0) );
		}
	}
	
	// show if we have a border :
	if(    this.thickness == 0
	    || this.color.a() == 0) {
		return;
	}
	internalSetColor(this.color);
	for (int iii=0; iii<nbOcurence; iii++) {
		
		float angleOne =  _angleStart + (_angleStop* iii     / nbOcurence) ;
		float offsetExty = sin(angleOne) * (_radius+this.thickness/2);
		float offsetExtx = cos(angleOne) * (_radius+this.thickness/2);
		float offsetInty = sin(angleOne) * (_radius-this.thickness/2);
		float offsetIntx = cos(angleOne) * (_radius-this.thickness/2);
		
		float angleTwo =  _angleStart + (_angleStop*  (iii+1) / nbOcurence );
		float offsetExt2y = sin(angleTwo) * (_radius+this.thickness/2);
		float offsetExt2x = cos(angleTwo) * (_radius+this.thickness/2);
		float offsetInt2y = sin(angleTwo) * (_radius-this.thickness/2);
		float offsetInt2x = cos(angleTwo) * (_radius-this.thickness/2);
		
		setPoint(Vector3f(this.position.x() + offsetIntx,  this.position.y() + offsetInty,  0));
		setPoint(Vector3f(this.position.x() + offsetExtx,  this.position.y() + offsetExty,  0));
		setPoint(Vector3f(this.position.x() + offsetExt2x, this.position.y() + offsetExt2y, 0));
		
		setPoint(Vector3f(this.position.x() + offsetExt2x, this.position.y() + offsetExt2y, 0));
		setPoint(Vector3f(this.position.x() + offsetInt2x, this.position.y() + offsetInt2y, 0));
		setPoint(Vector3f(this.position.x() + offsetIntx,  this.position.y() + offsetInty,  0));
	}
}

