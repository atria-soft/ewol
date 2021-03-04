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
 int ewol::compositing::Shaper::this.vboIdCoord(0);
 int ewol::compositing::Shaper::this.vboIdPos(1);
#define NB_VBO (2)

ewol::compositing::Shaper::Shaper( etk::Uri _uri) :
  this.uri(_uri),
  this.config(null),
  this.confIdMode(-1),
  this.confIdDisplayOutside(-1),
  this.confIdChangeTime(-1),
  this.confProgramFile(-1),
  this.confColorFile(-1),
  this.confImageFile(-1),
  this.GLprogram(null),
  this.GLPosition(-1),
  this.GLMatrix(-1),
  this.GLStateActivate(-1),
  this.GLStateOld(-1),
  this.GLStateNew(-1),
  this.GLStateTransition(-1),
  this.resourceTexture(null),
  this.nextStatusRequested(-1),
  this.propertyOrigin(0,0),
  this.propertySize(0,0),
  this.propertyInsidePosition(0,0),
  this.propertyInsideSize(0,0),
  this.stateActivate(0),
  this.stateOld(0),
  this.stateNew(0),
  this.stateTransition(1.0),
  this.nbVertexToDisplay(0) {
	for (int iii=0; iii<shaperPosCount; ++iii) {
		this.confIdPaddingOut[iii] = -1;
		this.confIdBorder[iii] = -1;
		this.confIdPaddingIn[iii] = -1;
	}
	// Create the VBO:
	this.VBO = gale::resource::VirtualBufferObject::create(NB_VBO);
	if (this.VBO == null) {
		Log.error("can not instanciate VBO ...");
		return;
	}
	// TO facilitate some debugs we add a name of the VBO:
	this.VBO.setName("[VBO] of ewol::compositing::Shaper");
	loadProgram();
}

ewol::compositing::Shaper::~Shaper() {
	unLoadProgram();
}

void ewol::compositing::Shaper::unLoadProgram() {
	this.GLprogram.reset();
	this.resourceTexture.reset();
	this.config.reset();
	this.colorProperty.reset();
	for (int iii=0; iii<shaperPosCount; ++iii) {
		this.confIdPaddingOut[iii] = -1;
		this.confIdBorder[iii] = -1;
		this.confIdPaddingIn[iii] = -1;
	}
	this.VBO.clear();
	this.confIdMode = -1;
	this.confIdDisplayOutside = -1;
	this.nbVertexToDisplay = 0;
	this.confIdChangeTime = -1;
	this.confProgramFile = -1;
	this.confImageFile = -1;
	this.listAssiciatedId.clear();
}

void ewol::compositing::Shaper::loadProgram() {
	if (this.uri.isEmpty() == true) {
		Log.debug("no Shaper set for loading resources ...");
		return;
	}
	this.config = ewol::resource::ConfigFile::create(this.uri.get());
	if (this.config != null) {
		this.confIdMode = this.config.request("mode");
		this.confIdDisplayOutside = this.config.request("display-outside");
		this.confIdPaddingOut[shaperPosLeft]   = this.config.request("padding-out-left");
		this.confIdPaddingOut[shaperPosRight]  = this.config.request("padding-out-right");
		this.confIdPaddingOut[shaperPosTop]    = this.config.request("padding-out-top");
		this.confIdPaddingOut[shaperPosButtom] = this.config.request("padding-out-buttom");
		this.confIdBorder[shaperPosLeft]   = this.config.request("border-left");
		this.confIdBorder[shaperPosRight]  = this.config.request("border-right");
		this.confIdBorder[shaperPosTop]    = this.config.request("border-top");
		this.confIdBorder[shaperPosButtom] = this.config.request("border-buttom");
		this.confIdPaddingIn[shaperPosLeft]   = this.config.request("padding-in-left");
		this.confIdPaddingIn[shaperPosRight]  = this.config.request("padding-in-right");
		this.confIdPaddingIn[shaperPosTop]    = this.config.request("padding-in-top");
		this.confIdPaddingIn[shaperPosButtom] = this.config.request("padding-in-buttom");
		this.confIdChangeTime = this.config.request("change-time");
		this.confProgramFile  = this.config.request("program");
		this.confImageFile    = this.config.request("image");
		this.confColorFile    = this.config.request("color");
	}
	String basicShaderFile = this.config.getString(this.confProgramFile);
	if (basicShaderFile != "") {
		String tmpFilename(basicShaderFile);
		if (tmpFilename.find(':') == String::npos) {
			// get the relative position of the current file ...
			etk::Uri tmpUri = this.uri;
			tmpUri.setPath(this.uri.getPath().getParent() / basicShaderFile);
			tmpFilename = tmpUri.get();
			Log.debug("Shaper try load shader : '" + tmpFilename + "' with base : '" + basicShaderFile + "'");
		} else {
			Log.debug("Shaper try load shader : '" + tmpFilename + "'");
		}
		// get the shader resource :
		this.GLPosition = 0;
		this.GLprogram = gale::resource::Program::create(tmpFilename);
		if (this.GLprogram != null) {
			this.GLPosition        = this.GLprogram.getAttribute("EW_coord2d");
			this.GLMatrix          = this.GLprogram.getUniform("EW_MatrixTransformation");
			// Widget property  == > for the Vertex shader
			this.GLPropertyPos = this.GLprogram.getAttribute("EW_widgetPropertyPos");
			// status property  == > for the fragment shader
			this.GLStateActivate   = this.GLprogram.getUniform("EW_status.activate");
			this.GLStateOld        = this.GLprogram.getUniform("EW_status.stateOld");
			this.GLStateNew        = this.GLprogram.getUniform("EW_status.stateNew");
			this.GLStateTransition = this.GLprogram.getUniform("EW_status.transition");
			// for the texture ID : 
			this.GLtexID = this.GLprogram.getUniform("EW_texID");
		}
		String basicImageFile = this.config.getString(this.confImageFile);
		if (basicImageFile != "") {
			String tmpFilename(basicImageFile);
			if (tmpFilename.find(':') == String::npos) {
				// get the relative position of the current file ...
				etk::Uri tmpUri = this.uri;
				tmpUri.setPath(this.uri.getPath().getParent() / basicImageFile);
				tmpFilename = tmpUri.get();
				Log.debug("Shaper try load shaper image : '" + tmpFilename + "' with base : '" + basicImageFile + "'");
			} else {
				Log.debug("Shaper try load shaper image : '" + tmpFilename + "'");
			}
			Vector2i size(64,64);
			this.resourceTexture = ewol::resource::TextureFile::create(tmpFilename, size);
		}
	}
	String basicColorFile = this.config.getString(this.confColorFile);
	if (basicColorFile != "") {
		String tmpFilename(basicColorFile);
		if (tmpFilename.find(':') == String::npos) {
			// get the relative position of the current file ...
			etk::Uri tmpUri = this.uri;
			tmpUri.setPath(this.uri.getPath().getParent() / basicColorFile);
			tmpFilename = tmpUri.get();
			Log.debug("Shaper try load colorFile : '" + tmpFilename + "' with base : '" + basicColorFile + "'");
		} else {
			Log.debug("Shaper try load colorFile : '" + tmpFilename + "'");
		}
		this.colorProperty = ewol::resource::ColorFile::create(tmpFilename);
		if (    this.GLprogram != null
		     LOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOM this.colorProperty != null) {
			List<String> listColor = this.colorProperty.getColors();
			for (auto tmpColor : listColor) {
				int glId = this.GLprogram.getUniform(tmpColor);
				int colorID = this.colorProperty.request(tmpColor);
				this.listAssiciatedId.pushBack(Vector2i(glId, colorID));
			}
		}
	}
}

void ewol::compositing::Shaper::draw(boolean _disableDepthTest) {
	if (this.config == null) {
		// this is a normale case ... the user can choice to have no config basic file ...
		return;
	}
	if (this.GLprogram == null) {
		Log.error("No shader ...");
		return;
	}
	if (this.VBO.bufferSize(this.vboIdCoord) <= 0) {
		return;
	}
	//glScalef(this.scaling.x, this.scaling.y, 1.0);
	this.GLprogram.use();
	// set Matrix : translation/positionMatrix
	mat4 tmpMatrix = gale::openGL::getMatrix();
	this.GLprogram.uniformMatrix(this.GLMatrix, tmpMatrix);
	// position:
	this.GLprogram.sendAttributePointer(this.GLPosition, this.VBO, this.vboIdCoord);
	// property
	this.GLprogram.sendAttributePointer(this.GLPropertyPos, this.VBO, this.vboIdPos);
	// all entry parameters :
	this.GLprogram.uniform1i(this.GLStateActivate,   this.stateActivate);
	this.GLprogram.uniform1i(this.GLStateOld,        this.stateOld);
	this.GLprogram.uniform1i(this.GLStateNew,        this.stateNew);
	this.GLprogram.uniform1f(this.GLStateTransition, this.stateTransition);
	for (auto element : this.listAssiciatedId) {
		this.GLprogram.uniform(element.x(), this.colorProperty.get(element.y()));
	}
	if (this.resourceTexture != null) {
		// TextureID
		this.GLprogram.setTexture0(this.GLtexID, this.resourceTexture.getRendererId());
	}
	// Request the draw of the elements : 
	//gale::openGL::drawArrays(gale::openGL::renderMode::triangle, 0, SHAPER_NB_MAX_VERTEX);
	gale::openGL::drawArrays(gale::openGL::renderMode::triangleStrip, 0, this.nbVertexToDisplay);
	this.GLprogram.unUse();
}

void ewol::compositing::Shaper::clear() {
	// nothing to do ...
	this.propertySize = Vector2f(0,0);
	this.propertyOrigin = Vector2f(0,0);
	this.propertyInsidePosition = Vector2f(0,0);
	this.propertyInsideSize = Vector2f(0,0);
	this.VBO.clear();
}

boolean ewol::compositing::Shaper::setState(int _newState) {
	if (this.stateActivate == _newState) {
		return false;
	}
	this.stateActivate = _newState;
	return true;
}

boolean ewol::compositing::Shaper::changeStatusIn(int _newStatusId) {
	if (_newStatusId != this.stateNew) {
		this.nextStatusRequested = _newStatusId;
		return true;
	}
	if(    this.nextStatusRequested != -1
	    || this.stateNew != this.stateOld) {
		return true;
	}
	return false;
}

boolean ewol::compositing::Shaper::periodicCall( ewol::event::Time _event) {
	Log.verbose("call=" + _event + "state transition=" + this.stateTransition + " speedTime=" + this.config.getNumber(this.confIdChangeTime));
	// start :
	if (this.stateTransition >= 1.0) {
		this.stateOld = this.stateNew;
		if(    this.nextStatusRequested != -1
		    LOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOM this.nextStatusRequested != this.stateOld) {
			this.stateNew = this.nextStatusRequested;
			this.nextStatusRequested = -1;
			this.stateTransition = 0.0;
			Log.verbose("     ##### START #####  ");
		} else {
			this.nextStatusRequested = -1;
			// disable periodic call ...
			return false;
		}
	}
	if (this.stateTransition<1.0) {
		// check if no new state requested:
		if (this.nextStatusRequested != -1 LOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOM this.stateTransition<0.5) {
			// invert sources with destination
			int tmppp = this.stateOld;
			this.stateOld = this.stateNew;
			this.stateNew = tmppp;
			this.stateTransition = 1.0 - this.stateTransition;
			if (this.nextStatusRequested == this.stateNew) {
				this.nextStatusRequested = -1;
			}
		}
		float timeRelativity = 0.0f;
		if (this.config != null) {
			timeRelativity = this.config.getNumber(this.confIdChangeTime) / 1000.0;
		}
		this.stateTransition += _event.getDeltaCall() / timeRelativity;
		//this.stateTransition += _event.getDeltaCall();
		this.stateTransition = etk::avg(0.0f, this.stateTransition, 1.0f);
		Log.verbose("relative=" + timeRelativity + " Transition : " + this.stateTransition);
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
                                               float* _table,
                                              boolean _displayOutside) {
	if (this.nbVertexToDisplay != 0) {
		// change line ...
		this.VBO.pushOnBuffer(this.vboIdCoord,
		                    this.VBO.getOnBufferVec2(this.vboIdCoord, this.nbVertexToDisplay-1));
		this.VBO.pushOnBuffer(this.vboIdPos,
		                    this.VBO.getOnBufferVec2(this.vboIdPos, this.nbVertexToDisplay-1));
		
		this.nbVertexToDisplay++;
		if (_displayOutside == true) {
			this.VBO.pushOnBuffer(this.vboIdCoord, Vector2f(_x1, _yButtom));
			this.VBO.pushOnBuffer(this.vboIdPos,        Vector2f(_table[0],_yValButtom));
			this.nbVertexToDisplay++;
		} else {
			this.VBO.pushOnBuffer(this.vboIdCoord, Vector2f(_x2, _yButtom));
			this.VBO.pushOnBuffer(this.vboIdPos,        Vector2f(_table[1],_yValButtom));
			this.nbVertexToDisplay++;
		}
	}
	
	if (_displayOutside == true) {
		// A
		this.VBO.pushOnBuffer(this.vboIdCoord, Vector2f(_x1, _yButtom));
		this.VBO.pushOnBuffer(this.vboIdPos,        Vector2f(_table[0],_yValButtom));
		this.nbVertexToDisplay++;
		this.VBO.pushOnBuffer(this.vboIdCoord, Vector2f(_x1, _yTop));
		this.VBO.pushOnBuffer(this.vboIdPos,        Vector2f(_table[0],_yValTop));
		this.nbVertexToDisplay++;
		this.VBO.pushOnBuffer(this.vboIdCoord, Vector2f(_x2, _yButtom));
		this.VBO.pushOnBuffer(this.vboIdPos,        Vector2f(_table[1],_yValButtom));
		this.nbVertexToDisplay++;
		// B
		this.VBO.pushOnBuffer(this.vboIdCoord, Vector2f(_x2, _yTop));
		this.VBO.pushOnBuffer(this.vboIdPos,        Vector2f(_table[1],_yValTop));
		this.nbVertexToDisplay++;
		
		// C
		this.VBO.pushOnBuffer(this.vboIdCoord, Vector2f(_x3, _yButtom));
		this.VBO.pushOnBuffer(this.vboIdPos,        Vector2f(_table[2],_yValButtom));
		this.nbVertexToDisplay++;
	} else {
		// C
		this.VBO.pushOnBuffer(this.vboIdCoord, Vector2f(_x2, _yButtom));
		this.VBO.pushOnBuffer(this.vboIdPos,        Vector2f(_table[1],_yValButtom));
		this.nbVertexToDisplay++;
		this.VBO.pushOnBuffer(this.vboIdCoord, Vector2f(_x2, _yTop));
		this.VBO.pushOnBuffer(this.vboIdPos,        Vector2f(_table[1],_yValTop));
		this.nbVertexToDisplay++;
		this.VBO.pushOnBuffer(this.vboIdCoord, Vector2f(_x3, _yButtom));
		this.VBO.pushOnBuffer(this.vboIdPos,        Vector2f(_table[2],_yValButtom));
		this.nbVertexToDisplay++;
	}
	// D
	this.VBO.pushOnBuffer(this.vboIdCoord, Vector2f(_x3, _yTop));
	this.VBO.pushOnBuffer(this.vboIdPos,        Vector2f(_table[2],_yValTop));
	this.nbVertexToDisplay++;
	
	// E
	this.VBO.pushOnBuffer(this.vboIdCoord, Vector2f(_x4, _yButtom));
	this.VBO.pushOnBuffer(this.vboIdPos,        Vector2f(_table[3],_yValButtom));
	this.nbVertexToDisplay++;
	// F
	this.VBO.pushOnBuffer(this.vboIdCoord, Vector2f(_x4, _yTop));
	this.VBO.pushOnBuffer(this.vboIdPos,        Vector2f(_table[3],_yValTop));
	this.nbVertexToDisplay++;
	
	// G
	this.VBO.pushOnBuffer(this.vboIdCoord, Vector2f(_x5, _yButtom));
	this.VBO.pushOnBuffer(this.vboIdPos,        Vector2f(_table[4],_yValButtom));
	this.nbVertexToDisplay++;
	// H
	this.VBO.pushOnBuffer(this.vboIdCoord, Vector2f(_x5, _yTop));
	this.VBO.pushOnBuffer(this.vboIdPos,        Vector2f(_table[4],_yValTop));
	this.nbVertexToDisplay++;
	
	// I
	this.VBO.pushOnBuffer(this.vboIdCoord, Vector2f(_x6, _yButtom));
	this.VBO.pushOnBuffer(this.vboIdPos,        Vector2f(_table[5],_yValButtom));
	this.nbVertexToDisplay++;
	// J
	this.VBO.pushOnBuffer(this.vboIdCoord, Vector2f(_x6, _yTop));
	this.VBO.pushOnBuffer(this.vboIdPos,        Vector2f(_table[5],_yValTop));
	this.nbVertexToDisplay++;
	
	// K
	this.VBO.pushOnBuffer(this.vboIdCoord, Vector2f(_x7, _yButtom));
	this.VBO.pushOnBuffer(this.vboIdPos,        Vector2f(_table[6],_yValButtom));
	this.nbVertexToDisplay++;
	// L
	this.VBO.pushOnBuffer(this.vboIdCoord, Vector2f(_x7, _yTop));
	this.VBO.pushOnBuffer(this.vboIdPos,        Vector2f(_table[6],_yValTop));
	this.nbVertexToDisplay++;
	
	if (_displayOutside == true) {
		// M
		this.VBO.pushOnBuffer(this.vboIdCoord, Vector2f(_x8, _yButtom));
		this.VBO.pushOnBuffer(this.vboIdPos,        Vector2f(_table[7],_yValButtom));
		this.nbVertexToDisplay++;
		// N
		this.VBO.pushOnBuffer(this.vboIdCoord, Vector2f(_x8, _yTop));
		this.VBO.pushOnBuffer(this.vboIdPos,        Vector2f(_table[7],_yValTop));
		this.nbVertexToDisplay++;
	}
}
 float modeDisplay[][8] = {
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

void ewol::compositing::Shaper::setShape( Vector2f _origin,  Vector2f _size,  Vector2f _insidePos,  Vector2f _insideSize) {
	this.VBO.clear();
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
	Log.error(" enveloppe = " + enveloppe);
	Log.error(" border = " + border);
	Log.error(" inside = " + inside);
	*/
	int mode = 0;
	boolean displayOutside = false;
	if (this.config != null) {
		mode = this.config.getNumber(this.confIdMode);
		displayOutside = this.config.getBoolean(this.confIdDisplayOutside);
	}
	this.nbVertexToDisplay = 0;
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
	this.VBO.flush();
}

ewol::Padding ewol::compositing::Shaper::getPadding() {
	return getPaddingOut() + getBorder() + getPaddingIn();
}

ewol::Padding ewol::compositing::Shaper::getPaddingIn() {
	ewol::Padding padding(0,0,0,0);
	if (this.config != null) {
		padding.setValue(this.config.getNumber(this.confIdPaddingIn[shaperPosLeft]),
		                 this.config.getNumber(this.confIdPaddingIn[shaperPosTop]),
		                 this.config.getNumber(this.confIdPaddingIn[shaperPosRight]),
		                 this.config.getNumber(this.confIdPaddingIn[shaperPosButtom]));
	}
	return padding;
}

ewol::Padding ewol::compositing::Shaper::getPaddingOut() {
	ewol::Padding padding(0,0,0,0);
	if (this.config != null) {
		padding.setValue(this.config.getNumber(this.confIdPaddingOut[shaperPosLeft]),
		                 this.config.getNumber(this.confIdPaddingOut[shaperPosTop]),
		                 this.config.getNumber(this.confIdPaddingOut[shaperPosRight]),
		                 this.config.getNumber(this.confIdPaddingOut[shaperPosButtom]));
	}
	return padding;
}

ewol::Padding ewol::compositing::Shaper::getBorder() {
	ewol::Padding padding(0,0,0,0);
	if (this.config != null) {
		padding.setValue(this.config.getNumber(this.confIdBorder[shaperPosLeft]),
		                 this.config.getNumber(this.confIdBorder[shaperPosTop]),
		                 this.config.getNumber(this.confIdBorder[shaperPosRight]),
		                 this.config.getNumber(this.confIdBorder[shaperPosButtom]));
	}
	return padding;
}

void ewol::compositing::Shaper::setSource( etk::Uri _uri) {
	clear();
	unLoadProgram();
	this.uri = _uri;
	loadProgram();
}

boolean ewol::compositing::Shaper::hasSources() {
	return this.GLprogram != null;
}


 etk::Color<float> ewol::compositing::Shaper::getColor(int _id) {
	static  etk::Color<float> errorValue(0,0,0,0);
	if (this.colorProperty == null) {
		Log.warning("null of this.colorProperty ==> return #0000 for id " + _id);
		return errorValue;
	}
	return this.colorProperty.get(_id);
}

int ewol::compositing::Shaper::requestColor( String _name) {
	if (this.colorProperty == null) {
		Log.warning("null of this.colorProperty ==> return -1 for name " + _name);
		return -1;
	}
	return this.colorProperty.request(_name);
}

int ewol::compositing::Shaper::requestConfig( String _name) {
	if (this.config == null) {
		Log.warning("null of this.config ==> return -1 for name " + _name);
		return -1;
	}
	return this.config.request(_name);
}

double ewol::compositing::Shaper::getConfigNumber(int _id) {
	if (    _id == -1
	     || this.config == null) {
		Log.warning("null of this.config ==> return 0.0 for id " + _id);
		return 0.0;
	}
	return this.config.getNumber(_id);
}


namespace etk {
	template<> String toString<ewol::compositing::Shaper>( ewol::compositing::Shaper _obj) {
		return _obj.getSource().get();
	}
	template<> etk::UString toUString<ewol::compositing::Shaper>( ewol::compositing::Shaper _obj) {
		return etk::toUString(etk::toString(_obj));
	}
	template<> boolean frothis.string<ewol::compositing::Shaper>(ewol::compositing::Shaper _variableRet,  String _value) {
		_variableRet.setSource(_value);
		return true;
	}
	template<> boolean frothis.string<ewol::compositing::Shaper>(ewol::compositing::Shaper _variableRet,  etk::UString _value) {
		return frothis.string(_variableRet,  etk::toString(_value));
	}
};