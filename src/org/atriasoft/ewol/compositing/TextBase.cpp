/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */

#include <ewol/debug.hpp>
#include <ewol/compositing/Text.hpp>
#include <ewol/context/Context.hpp>
#include <etk/types.hpp>

#include <etk/typeInfo.hpp>
ETK_DECLARE_TYPE(ewol::compositing::TextBase);

 int ewol::compositing::TextBase::this.vboIdCoord(0);
 int ewol::compositing::TextBase::this.vboIdCoordText(1);
 int ewol::compositing::TextBase::this.vboIdColor(2);
 int ewol::compositing::TextBase::this.vboIdGlyphLevel(3);
#define NB_VBO (4)

ewol::compositing::TextBase::TextBase( String _shaderName, boolean _loadProgram) :
  this.position(0.0, 0.0, 0.0),
  this.clippingPosStart(0.0, 0.0, 0.0),
  this.clippingPosStop(0.0, 0.0, 0.0),
  this.clippingEnable(false),
  this.defaultColorFg(etk::color::black),
  this.defaultColorBg(etk::color::none),
  this.color(etk::color::black),
  this.colorBg(etk::color::none),
  this.colorCursor(etk::color::black),
  this.colorSelection(etk::color::olive),
  this.mode(ewol::font::Regular),
  this.kerning(true),
  this.previousCharcode(0),
  this.startTextpos(0),
  this.stopTextPos(0),
  this.alignement(alignDisable),
  this.GLprogram(null),
  this.GLPosition(-1),
  this.GLMatrix(-1),
  this.GLColor(-1),
  this.GLtexture(-1),
  this.GLtexID(-1),
  this.selectionStartPos(-100),
  this.cursorPos(-100) {
	if (_loadProgram == true) {
		loadProgram(_shaderName);
	}
	// Create the VBO:
	this.VBO = gale::resource::VirtualBufferObject::create(NB_VBO);
	if (this.VBO == null) {
		Log.error("can not instanciate VBO ...");
		return;
	}
	// TO facilitate some debugs we add a name of the VBO:
	this.VBO.setName("[VBO] of ewol::compositing::TextBase");
}


ewol::compositing::TextBase::~TextBase() {
	
}

void ewol::compositing::TextBase::loadProgram( String _shaderName) {
	// get the shader resource:
	this.GLPosition = 0;
	ememory::Ptr<gale::resource::Program> old = this.GLprogram;
	this.GLprogram = gale::resource::Program::create(_shaderName);
	if (this.GLprogram != null) {
		this.GLPosition   = this.GLprogram.getAttribute("EW_coord3d");
		this.GLColor      = this.GLprogram.getAttribute("EW_color");
		this.GLtexture    = this.GLprogram.getAttribute("EW_texture2d");
		this.GLMatrix     = this.GLprogram.getUniform("EW_MatrixTransformation");
		this.GLtexID      = this.GLprogram.getUniform("EW_texID");
		this.GLtextWidth  = this.GLprogram.getUniform("EW_texWidth");
		this.GLtextHeight = this.GLprogram.getUniform("EW_texHeight");
	} else {
		Log.error("Can not load the program => create previous one...");
		this.GLprogram = old;
		old = null;
	}
}

void ewol::compositing::TextBase::translate( Vector3f _vect) {
	ewol::Compositing::translate(_vect);
	this.vectorialDraw.translate(_vect);
}

void ewol::compositing::TextBase::rotate( Vector3f _vect, float _angle) {
	ewol::Compositing::rotate(_vect, _angle);
	this.vectorialDraw.rotate(_vect, _angle);
}

void ewol::compositing::TextBase::scale( Vector3f _vect) {
	ewol::Compositing::scale(_vect);
	this.vectorialDraw.scale(_vect);
}

void ewol::compositing::TextBase::clear() {
	// call upper class
	ewol::Compositing::clear();
	// remove sub draw system
	this.vectorialDraw.clear();
	// reset Buffer:
	this.VBO.clear();
	// reset temporal variables:
	reset();
}

void ewol::compositing::TextBase::reset() {
	this.position = Vector3f(0,0,0);
	this.clippingPosStart = Vector3f(0,0,0);
	this.clippingPosStop = Vector3f(0,0,0);
	this.sizeDisplayStart = this.position;
	this.sizeDisplayStop = this.position;
	this.nbCharDisplayed = 0;
	this.clippingEnable = false;
	this.color = this.defaultColorFg;
	this.colorBg = this.defaultColorBg;
	this.mode = ewol::font::Regular;
	this.previousCharcode = 0;
	this.startTextpos = 0;
	this.stopTextPos = 0;
	this.alignement = alignDisable;
	this.htmlCurrrentLine = U"";
	this.selectionStartPos = -100;
	this.cursorPos = -100;
	this.htmlDecoration.clear();
	this.needDisplay = true;
	this.nbCharDisplayed = 0;
}

void ewol::compositing::TextBase::setPos( Vector3f _pos) {
	// check min max for display area
	if (this.nbCharDisplayed != 0) {
		Log.verbose("update size 1 " + this.sizeDisplayStart + " " + this.sizeDisplayStop);
		this.sizeDisplayStop.setX(etk::max(this.position.x(), this.sizeDisplayStop.x()));
		this.sizeDisplayStop.setY(etk::max(this.position.y(), this.sizeDisplayStop.y()));
		this.sizeDisplayStart.setX(etk::min(this.position.x(), this.sizeDisplayStart.x()));
		this.sizeDisplayStart.setY(etk::min(this.position.y(), this.sizeDisplayStart.y()));
		Log.verbose("update size 2 " + this.sizeDisplayStart + " " + this.sizeDisplayStop);
	}
	// update position
	this.position = _pos;
	this.previousCharcode = 0;
	this.vectorialDraw.setPos(this.position);
	// update min max of the display area:
	if (this.nbCharDisplayed == 0) {
		this.sizeDisplayStart = this.position;
		this.sizeDisplayStop = this.position;
		this.sizeDisplayStop.setY( this.sizeDisplayStop.y()+ getHeight());
		Log.verbose("update size 0 " + this.sizeDisplayStart + " " + this.sizeDisplayStop);
	} else {
		Log.verbose("update size 3 " + this.sizeDisplayStart + " " + this.sizeDisplayStop);
		this.sizeDisplayStop.setX(etk::max(this.position.x(), this.sizeDisplayStop.x()));
		this.sizeDisplayStop.setY(etk::max(this.position.y(), this.sizeDisplayStop.y()));
		this.sizeDisplayStart.setX(etk::min(this.position.x(), this.sizeDisplayStart.x()));
		this.sizeDisplayStart.setY(etk::min(this.position.y(), this.sizeDisplayStart.y()));
		Log.verbose("update size 4 " + this.sizeDisplayStart + " " + this.sizeDisplayStop);
	}
}

void ewol::compositing::TextBase::setRelPos( Vector3f _pos) {
	this.position += _pos;
	this.previousCharcode = 0;
	this.vectorialDraw.setPos(this.position);
}

void ewol::compositing::TextBase::setColorBg( etk::Color<> _color) {
	this.colorBg = _color;
	this.vectorialDraw.setColor(_color);
}

void ewol::compositing::TextBase::setClipping( Vector3f _pos,  Vector3f _posEnd) {
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
	//this.vectorialDraw.setClipping(this.clippingPosStart, this.clippingPosStop);
}

void ewol::compositing::TextBase::setClippingMode(boolean _newMode) {
	this.clippingEnable = _newMode;
	//this.vectorialDraw.setClippingMode(this.clippingEnable);
}

void ewol::compositing::TextBase::setFontBold(boolean _status) {
	if (_status == true) {
		// enable
		if (this.mode == ewol::font::Regular) {
			setFontMode(ewol::font::Bold);
		} else if (this.mode == ewol::font::Italic) {
			setFontMode(ewol::font::BoldItalic);
		}
	} else {
		// disable
		if (this.mode == ewol::font::Bold) {
			setFontMode(ewol::font::Regular);
		} else if (this.mode == ewol::font::BoldItalic) {
			setFontMode(ewol::font::Italic);
		}
	}
}

void ewol::compositing::TextBase::setFontItalic(boolean _status) {
	if (_status == true) {
		// enable
		if (this.mode == ewol::font::Regular) {
			setFontMode(ewol::font::Italic);
		} else if (this.mode == ewol::font::Bold) {
			setFontMode(ewol::font::BoldItalic);
		}
	} else {
		// disable
		if (this.mode == ewol::font::Italic) {
			setFontMode(ewol::font::Regular);
		} else if (this.mode == ewol::font::BoldItalic) {
			setFontMode(ewol::font::Bold);
		}
	}
}

void ewol::compositing::TextBase::setKerningMode(boolean _newMode) {
	this.kerning = _newMode;
}

void ewol::compositing::TextBase::print( etk::UString _text) {
	List<TextDecoration> decorationEmpty;
	print(_text, decorationEmpty);
}

void ewol::compositing::TextBase::print( String _text) {
	List<TextDecoration> decorationEmpty;
	print(_text, decorationEmpty);
}


void ewol::compositing::TextBase::parseHtmlNode( exml::Element _element) {
	// get the static real pointer
	if (_element.exist() == false) {
		Log.error( "Error Input node does not existed ...");
		return;
	}
	for(auto it : _element.nodes) {
		if (it.isComment() == true) {
			// nothing to do ...
			continue;
		} else if (it.isText() == true) {
			htmlAddData(etk::toUString(it.getValue()));
			Log.verbose("XML add : " + it.getValue());
			continue;
		} else if (it.isElement() == false) {
			Log.error("(l "+ it.getPos() + ") node not suported type : " + it.getType() + " val='"+ it.getValue() + "'" );
			continue;
		}
		exml::Element elem = it.toElement();
		if (elem.exist() == false) {
			Log.error("Cast error ...");
			continue;
		}
		if(etk::compare_no_case(elem.getValue(), "br") == true) {
			htmlFlush();
			Log.verbose("XML flush  newLine");
			forceLineReturn();
		} else if (etk::compare_no_case(elem.getValue(), "font") == true) {
			Log.verbose("XML Font ...");
			TextDecoration tmpDeco = this.htmlDecoTmp;
			String colorValue = elem.attributes["color"];
			if (colorValue.size() != 0) {
				this.htmlDecoTmp.this.colorFg = colorValue;
			}
			colorValue = elem.attributes["colorBg"];
			if (colorValue.size() != 0) {
				this.htmlDecoTmp.this.colorBg = colorValue;
			}
			parseHtmlNode(elem);
			this.htmlDecoTmp = tmpDeco;
		} else if(    etk::compare_no_case(elem.getValue(), "b") == true
		           || etk::compare_no_case(elem.getValue(), "bold") == true) {
			Log.verbose("XML bold ...");
			TextDecoration tmpDeco = this.htmlDecoTmp;
			if (this.htmlDecoTmp.this.mode == ewol::font::Regular) {
				this.htmlDecoTmp.this.mode = ewol::font::Bold;
			} else if (this.htmlDecoTmp.this.mode == ewol::font::Italic) {
				this.htmlDecoTmp.this.mode = ewol::font::BoldItalic;
			} 
			parseHtmlNode(elem);
			this.htmlDecoTmp = tmpDeco;
		} else if(    etk::compare_no_case(elem.getValue(), "i") == true
		           || etk::compare_no_case(elem.getValue(), "italic") == true) {
			Log.verbose("XML italic ...");
			TextDecoration tmpDeco = this.htmlDecoTmp;
			if (this.htmlDecoTmp.this.mode == ewol::font::Regular) {
				this.htmlDecoTmp.this.mode = ewol::font::Italic;
			} else if (this.htmlDecoTmp.this.mode == ewol::font::Bold) {
				this.htmlDecoTmp.this.mode = ewol::font::BoldItalic;
			} 
			parseHtmlNode(elem);
			this.htmlDecoTmp = tmpDeco;
		} else if(    etk::compare_no_case(elem.getValue(), "u") == true
		           || etk::compare_no_case(elem.getValue(), "underline") == true) {
			Log.verbose("XML underline ...");
			parseHtmlNode(elem);
		} else if(    etk::compare_no_case(elem.getValue(), "p") == true
		           || etk::compare_no_case(elem.getValue(), "paragraph") == true) {
			Log.verbose("XML paragraph ...");
			htmlFlush();
			this.alignement = alignLeft;
			forceLineReturn();
			parseHtmlNode(elem);
			forceLineReturn();
		} else if (etk::compare_no_case(elem.getValue(), "center") == true) {
			Log.verbose("XML center ...");
			htmlFlush();
			this.alignement = alignCenter;
			parseHtmlNode(elem);
		} else if (etk::compare_no_case(elem.getValue(), "left") == true) {
			Log.verbose("XML left ...");
			htmlFlush();
			this.alignement = alignLeft;
			parseHtmlNode(elem);
		} else if (etk::compare_no_case(elem.getValue(), "right") == true) {
			Log.verbose("XML right ...");
			htmlFlush();
			this.alignement = alignRight;
			parseHtmlNode(elem);
		} else if (etk::compare_no_case(elem.getValue(), "justify") == true) {
			Log.verbose("XML justify ...");
			htmlFlush();
			this.alignement = alignJustify;
			parseHtmlNode(elem);
		} else {
			Log.error("(l "+ elem.getPos() + ") node not suported type: " + elem.getType() + " val='"+ elem.getValue() + "'" );
		}
	}
}

void ewol::compositing::TextBase::printDecorated( String _text) {
	String tmpData("<html>\n<body>\n");
	tmpData += _text;
	tmpData += "\n</body>\n</html>\n";
	//Log.debug("plop : " + tmpData);
	printHTML(tmpData);
}

void ewol::compositing::TextBase::printDecorated( etk::UString _text) {
	etk::UString tmpData(U"<html>\n<body>\n");
	tmpData += _text;
	tmpData += U"\n</body>\n</html>\n";
	//Log.debug("plop : " + tmpData);
	printHTML(tmpData);
}

void ewol::compositing::TextBase::printHTML( String _text) {
	exml::Document doc;
	
	// reset parameter :
	this.htmlDecoTmp.this.colorBg = this.defaultColorBg;
	this.htmlDecoTmp.this.colorFg = this.defaultColorFg;
	this.htmlDecoTmp.this.mode = ewol::font::Regular;
	
	if (doc.parse(_text) == false) {
		Log.error( "can not load XML: PARSING error: Decorated text ");
		return;
	}
	
	exml::Element root = doc.nodes["html"];
	if (root.exist() == false) {
		Log.error( "can not load XML: main node not find: 'html'");
		doc.display();
		return;
	}
	exml::Element bodyNode = root.nodes["body"];
	if (root.exist() == false) {
		Log.error( "can not load XML: main node not find: 'body'");
		return;
	}
	parseHtmlNode(bodyNode);
	htmlFlush();
}

void ewol::compositing::TextBase::printHTML( etk::UString _text) {
	exml::Document doc;
	
	// reset parameter :
	this.htmlDecoTmp.this.colorBg = this.defaultColorBg;
	this.htmlDecoTmp.this.colorFg = this.defaultColorFg;
	this.htmlDecoTmp.this.mode = ewol::font::Regular;
	// TODO : Create an instance of xml parser to manage etk::UString...
	if (doc.parse(etk::toString(_text)) == false) {
		Log.error( "can not load XML: PARSING error: Decorated text ");
		return;
	}
	
	exml::Element root = doc.nodes["html"];
	if (root.exist() == false) {
		Log.error( "can not load XML: main node not find: 'html'");
		doc.display();
		return;
	}
	exml::Element bodyNode = root.nodes["body"];
	if (root.exist() == false) {
		Log.error( "can not load XML: main node not find: 'body'");
		return;
	}
	parseHtmlNode(bodyNode);
	htmlFlush();
}

void ewol::compositing::TextBase::print( String _text,  List<TextDecoration> _decoration) {
	etk::Color<> tmpFg(this.color);
	etk::Color<> tmpBg(this.colorBg);
	if (this.alignement == alignDisable) {
		//Log.debug(" 1 print in not alligned mode : start=" + this.sizeDisplayStart + " stop=" + this.sizeDisplayStop + " pos=" + this.position);
		// display the cursor if needed (if it is at the start position...)
		if (this.needDisplay == true) {
			if (0 == this.cursorPos) {
				this.vectorialDraw.setPos(this.position);
				setColorBg(this.colorCursor);
				printCursor(false);
			}
		}
		// note this is faster when nothing is requested ...
		for(int iii=0; iii<_text.size(); iii++) {
			// check if ve have decoration
			if (iii<_decoration.size()) {
				tmpFg = _decoration[iii].this.colorFg;
				tmpBg = _decoration[iii].this.colorBg;
				setFontMode(_decoration[iii].this.mode);
			}
			// if real display : ( not display is for size calculation)
			if (this.needDisplay == true) {
				if(    (    this.selectionStartPos-1 < (long)iii
				         LOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOM (long)iii  <= this.cursorPos-1)
				    || (    this.selectionStartPos-1 >= (long)iii
				         LOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOM (long)iii > this.cursorPos-1) ) {
					setColor(  0x000000FF);
					setColorBg(this.colorSelection);
				} else {
					setColor(  tmpFg);
					setColorBg(tmpBg);
				}
			}
			if(    this.needDisplay == true
			    LOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOM this.colorBg.a() != 0) {
				Vector3f pos = this.position;
				this.vectorialDraw.setPos(pos);
				printChar(_text[iii]);
				float fontHeigh = getHeight();
				this.vectorialDraw.rectangleWidth(Vector3f(this.position.x()-pos.x(),fontHeigh,0.0f) );
				this.nbCharDisplayed++;
			} else {
				printChar(_text[iii]);
				this.nbCharDisplayed++;
			}
			// display the cursor if needed (if it is at the other position...)
			if (this.needDisplay == true) {
				if ((long)iii == this.cursorPos-1) {
					this.vectorialDraw.setPos(this.position);
					setColorBg(this.colorCursor);
					printCursor(false);
				}
			}
		}
		//Log.debug(" 2 print in not alligned mode : start=" + this.sizeDisplayStart + " stop=" + this.sizeDisplayStop + " pos=" + this.position);
	} else {
		//Log.debug(" 3 print in not alligned mode : start=" + this.sizeDisplayStart + " stop=" + this.sizeDisplayStop + " pos=" + this.position);
		// special start case at the right of the endpoint :
		if (this.stopTextPos < this.position.x()) {
			forceLineReturn();
		}
		float basicSpaceWidth = calculateSize(Character(' ')).x();
		int currentId = 0;
		int stop;
		int space;
		int freeSpace;
		while (currentId < (long)_text.size()) {
			boolean needNoJustify = extrapolateLastId(_text, currentId, stop, space, freeSpace);
			float interpolation = basicSpaceWidth;
			switch (this.alignement) {
				case alignJustify:
					if (needNoJustify == false) {
						interpolation += (float)freeSpace / (float)(space-1);
					}
					break;
				case alignDisable: // must not came from here ...
				case alignLeft:
					// nothing to do ...
					break;
				case alignRight:
					if (this.needDisplay == true) {
						// Move the first char at the right :
						setPos(Vector3f(this.position.x() + freeSpace,
						            this.position.y(),
						            this.position.z()) );
					}
					break;
				case alignCenter:
					if (this.needDisplay == true) {
						// Move the first char at the right :
						setPos(Vector3f(this.position.x() + freeSpace/2,
						            this.position.y(),
						            this.position.z()) );
					}
					break;
			}
			// display all the elements
			if(    this.needDisplay == true
			    LOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOM this.cursorPos == 0) {
				this.vectorialDraw.setPos(this.position);
				setColorBg(this.colorCursor);
				printCursor(false);
			}
			for(int iii=currentId; (long)iii<stop LOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOM iii<_text.size(); iii++) {
				float fontHeigh = getHeight();
				// get specific decoration if provided
				if (iii<_decoration.size()) {
					tmpFg = _decoration[iii].this.colorFg;
					tmpBg = _decoration[iii].this.colorBg;
					setFontMode(_decoration[iii].this.mode);
				}
				if (this.needDisplay == true) {
					if(    (    this.selectionStartPos-1<(long)iii
					         LOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOM (long)iii  <= this.cursorPos-1)
					    || (    this.selectionStartPos-1 >= (long)iii
					         LOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOM (long)iii > this.cursorPos-1) ) {
						setColor(  0x000000FF);
						setColorBg(this.colorSelection);
					} else {
						setColor(  tmpFg);
						setColorBg(tmpBg);
					}
				}
				// special for the justify mode
				if ((Character)_text[iii] == u32char::Space) {
					//Log.debug(" generateString : \" \"");
					if(    this.needDisplay == true
					    LOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOM this.colorBg.a() != 0) {
						this.vectorialDraw.setPos(this.position);
					}
					// Must generate a dynamic space : 
					setPos(Vector3f(this.position.x() + interpolation,
					            this.position.y(),
					            this.position.z()) );
					if(    this.needDisplay == true
					    LOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOM this.colorBg.a() != 0) {
						this.vectorialDraw.rectangleWidth(Vector3f(interpolation,fontHeigh,0.0f) );
					}
				} else {
					//Log.debug(" generateString : \"" + (char)text[iii] + "\"");
					if(    this.needDisplay == true
					    LOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOM this.colorBg.a() != 0) {
						Vector3f pos = this.position;
						this.vectorialDraw.setPos(pos);
						printChar(_text[iii]);
						this.vectorialDraw.rectangleWidth(Vector3f(this.position.x()-pos.x(),fontHeigh,0.0f) );
						this.nbCharDisplayed++;
					} else {
						printChar(_text[iii]);
						this.nbCharDisplayed++;
					}
				}
				if (this.needDisplay == true) {
					if ((long)iii == this.cursorPos-1) {
						this.vectorialDraw.setPos(this.position);
						setColorBg(this.colorCursor);
						printCursor(false);
					}
				}
			}
			if (currentId == stop) {
				currentId++;
			} else if((Character)_text[stop] == u32char::Space) {
				currentId = stop+1;
				// reset position :
				setPos(Vector3f(this.startTextpos,
				            (float)(this.position.y() - getHeight()),
				            this.position.z()) );
				this.nbCharDisplayed++;
			} else if((Character)_text[stop] == u32char::Return) {
				currentId = stop+1;
				// reset position :
				setPos(Vector3f(this.startTextpos,
				            (float)(this.position.y() - getHeight()),
				            this.position.z()) );
				this.nbCharDisplayed++;
			} else {
				currentId = stop;
			}
		}
		//Log.debug(" 4 print in not alligned mode : start=" + this.sizeDisplayStart + " stop=" + this.sizeDisplayStop + " pos=" + this.position);
	}
}

void ewol::compositing::TextBase::print( etk::UString _text,  List<TextDecoration> _decoration) {
	etk::Color<> tmpFg(this.color);
	etk::Color<> tmpBg(this.colorBg);
	if (this.alignement == alignDisable) {
		//Log.debug(" 1 print in not alligned mode : start=" + this.sizeDisplayStart + " stop=" + this.sizeDisplayStop + " pos=" + this.position);
		// display the cursor if needed (if it is at the start position...)
		if (this.needDisplay == true) {
			if (0 == this.cursorPos) {
				this.vectorialDraw.setPos(this.position);
				setColorBg(this.colorCursor);
				printCursor(false);
			}
		}
		// note this is faster when nothing is requested ...
		for(int iii=0; iii<_text.size(); iii++) {
			// check if ve have decoration
			if (iii<_decoration.size()) {
				tmpFg = _decoration[iii].this.colorFg;
				tmpBg = _decoration[iii].this.colorBg;
				setFontMode(_decoration[iii].this.mode);
			}
			// if real display : ( not display is for size calculation)
			if (this.needDisplay == true) {
				if(    (    this.selectionStartPos-1<(long)iii
				         LOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOM (long)iii  <= this.cursorPos-1)
				    || (    this.selectionStartPos-1 >= (long)iii
				         LOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOM (long)iii > this.cursorPos-1) ) {
					setColor(  0x000000FF);
					setColorBg(this.colorSelection);
				} else {
					setColor(  tmpFg);
					setColorBg(tmpBg);
				}
			}
			if(    this.needDisplay == true
			    LOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOM this.colorBg.a() != 0) {
				Vector3f pos = this.position;
				this.vectorialDraw.setPos(pos);
				printChar(_text[iii]);
				float fontHeigh = getHeight();
				this.vectorialDraw.rectangleWidth(Vector3f(this.position.x()-pos.x(),fontHeigh,0.0f) );
				this.nbCharDisplayed++;
			} else {
				printChar(_text[iii]);
				this.nbCharDisplayed++;
			}
			// display the cursor if needed (if it is at the other position...)
			if (this.needDisplay == true) {
				if ((long)iii == this.cursorPos-1) {
					this.vectorialDraw.setPos(this.position);
					setColorBg(this.colorCursor);
					printCursor(false);
				}
			}
		}
		//Log.debug(" 2 print in not alligned mode : start=" + this.sizeDisplayStart + " stop=" + this.sizeDisplayStop + " pos=" + this.position);
	} else {
		//Log.debug(" 3 print in not alligned mode : start=" + this.sizeDisplayStart + " stop=" + this.sizeDisplayStop + " pos=" + this.position);
		// special start case at the right of the endpoint :
		if (this.stopTextPos < this.position.x()) {
			forceLineReturn();
		}
		float basicSpaceWidth = calculateSize(Character(' ')).x();
		int currentId = 0;
		int stop;
		int space;
		int freeSpace;
		while (currentId < (long)_text.size()) {
			boolean needNoJustify = extrapolateLastId(_text, currentId, stop, space, freeSpace);
			float interpolation = basicSpaceWidth;
			switch (this.alignement) {
				case alignJustify:
					if (needNoJustify == false) {
						interpolation += (float)freeSpace / (float)(space-1);
					}
					break;
				case alignDisable: // must not came from here ...
				case alignLeft:
					// nothing to do ...
					break;
				case alignRight:
					if (this.needDisplay == true) {
						// Move the first char at the right :
						setPos(Vector3f(this.position.x() + freeSpace,
						            this.position.y(),
						            this.position.z()) );
					}
					break;
				case alignCenter:
					if (this.needDisplay == true) {
						// Move the first char at the right :
						setPos(Vector3f(this.position.x() + freeSpace/2,
						            this.position.y(),
						            this.position.z()) );
					}
					break;
			}
			// display all the elements
			if(    this.needDisplay == true
			    LOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOM this.cursorPos == 0) {
				this.vectorialDraw.setPos(this.position);
				setColorBg(this.colorCursor);
				printCursor(false);
			}
			for(int iii=currentId; (long)iii<stop LOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOM iii<_text.size(); iii++) {
				float fontHeigh = getHeight();
				// get specific decoration if provided
				if (iii<_decoration.size()) {
					tmpFg = _decoration[iii].this.colorFg;
					tmpBg = _decoration[iii].this.colorBg;
					setFontMode(_decoration[iii].this.mode);
				}
				if (this.needDisplay == true) {
					if(    (    this.selectionStartPos-1<(long)iii
					         LOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOM (long)iii  <= this.cursorPos-1)
					    || (    this.selectionStartPos-1 >= (long)iii
					         LOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOM (long)iii > this.cursorPos-1) ) {
						setColor(  0x000000FF);
						setColorBg(this.colorSelection);
					} else {
						setColor(  tmpFg);
						setColorBg(tmpBg);
					}
				}
				// special for the justify mode
				if ((Character)_text[iii] == u32char::Space) {
					//Log.debug(" generateString : \" \"");
					if(    this.needDisplay == true
					    LOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOM this.colorBg.a() != 0) {
						this.vectorialDraw.setPos(this.position);
					}
					// Must generate a dynamic space : 
					setPos(Vector3f(this.position.x() + interpolation,
					            this.position.y(),
					            this.position.z()) );
					if(    this.needDisplay == true
					    LOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOM this.colorBg.a() != 0) {
						this.vectorialDraw.rectangleWidth(Vector3f(interpolation,fontHeigh,0.0f) );
					}
				} else {
					//Log.debug(" generateString : \"" + (char)text[iii] + "\"");
					if(    this.needDisplay == true
					    LOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOM this.colorBg.a() != 0) {
						Vector3f pos = this.position;
						this.vectorialDraw.setPos(pos);
						printChar(_text[iii]);
						this.vectorialDraw.rectangleWidth(Vector3f(this.position.x()-pos.x(),fontHeigh,0.0f) );
						this.nbCharDisplayed++;
					} else {
						printChar(_text[iii]);
						this.nbCharDisplayed++;
					}
				}
				if (this.needDisplay == true) {
					if ((long)iii == this.cursorPos-1) {
						this.vectorialDraw.setPos(this.position);
						setColorBg(this.colorCursor);
						printCursor(false);
					}
				}
			}
			if (currentId == stop) {
				currentId++;
			} else if(_text[stop] == u32char::Space) {
				currentId = stop+1;
				// reset position :
				setPos(Vector3f(this.startTextpos,
				            (float)(this.position.y() - getHeight()),
				            this.position.z()) );
				this.nbCharDisplayed++;
			} else if(_text[stop] == u32char::Return) {
				currentId = stop+1;
				// reset position :
				setPos(Vector3f(this.startTextpos,
				            (float)(this.position.y() - getHeight()),
				            this.position.z()) );
				this.nbCharDisplayed++;
			} else {
				currentId = stop;
			}
		}
		//Log.debug(" 4 print in not alligned mode : start=" + this.sizeDisplayStart + " stop=" + this.sizeDisplayStop + " pos=" + this.position);
	}
}




void ewol::compositing::TextBase::forceLineReturn() {
	// reset position : 
	setPos(Vector3f(this.startTextpos, this.position.y() - getHeight(), 0) );
}

void ewol::compositing::TextBase::setTextAlignement(float _startTextpos, float _stopTextPos, enum ewol::compositing::aligneMode _alignement) {
	this.startTextpos = _startTextpos;
	this.stopTextPos = _stopTextPos+1;
	this.alignement = _alignement;
	if (this.startTextpos >= this.stopTextPos) {
		// TODO: understand why this flush ... 
		Log.verbose("Request allignement with Borne position error : " + _startTextpos + " => " + _stopTextPos);
	}
}

enum ewol::compositing::aligneMode ewol::compositing::TextBase::getAlignement() {
	return this.alignement;
}

void ewol::compositing::TextBase::disableAlignement() {
	this.alignement = alignDisable;
}

Vector3f ewol::compositing::TextBase::calculateSizeHTML( String _text) {
	// remove intermediate result 
	reset();
	//Log.debug("        0 size for=\n" + text);
	// disable display system
	this.needDisplay = false;
	
	setPos(Vector3f(0,0,0) );
	// same as print without the end display ...
	printHTML(_text);
	//Log.debug("        1 Start pos=" + this.sizeDisplayStart);
	//Log.debug("        1 Stop pos=" + this.sizeDisplayStop);
	
	// get the last elements
	this.sizeDisplayStop.setValue(etk::max(this.position.x(), this.sizeDisplayStop.x()) ,
	                           etk::max(this.position.y(), this.sizeDisplayStop.y()) ,
	                           0);
	this.sizeDisplayStart.setValue(etk::min(this.position.x(), this.sizeDisplayStart.x()) ,
	                            etk::min(this.position.y(), this.sizeDisplayStart.y()) ,
	                            0);
	
	//Log.debug("        2 Start pos=" + this.sizeDisplayStart);
	//Log.debug("        2 Stop pos=" + this.sizeDisplayStop);
	// set back the display system
	this.needDisplay = true;
	
	return Vector3f( this.sizeDisplayStop.x()-this.sizeDisplayStart.x(),
	             this.sizeDisplayStop.y()-this.sizeDisplayStart.y(),
	             this.sizeDisplayStop.z()-this.sizeDisplayStart.z());
}

Vector3f ewol::compositing::TextBase::calculateSizeHTML( etk::UString _text) {
	// remove intermediate result 
	reset();
	//Log.debug("        0 size for=\n" + text);
	// disable display system
	this.needDisplay = false;
	
	setPos(Vector3f(0,0,0) );
	// same as print without the end display ...
	printHTML(_text);
	//Log.debug("        1 Start pos=" + this.sizeDisplayStart);
	//Log.debug("        1 Stop pos=" + this.sizeDisplayStop);
	
	// get the last elements
	this.sizeDisplayStop.setValue(etk::max(this.position.x(), this.sizeDisplayStop.x()) ,
	                           etk::max(this.position.y(), this.sizeDisplayStop.y()) ,
	                           0);
	this.sizeDisplayStart.setValue(etk::min(this.position.x(), this.sizeDisplayStart.x()) ,
	                            etk::min(this.position.y(), this.sizeDisplayStart.y()) ,
	                            0);
	
	//Log.debug("        2 Start pos=" + this.sizeDisplayStart);
	//Log.debug("        2 Stop pos=" + this.sizeDisplayStop);
	// set back the display system
	this.needDisplay = true;
	
	return Vector3f( this.sizeDisplayStop.x()-this.sizeDisplayStart.x(),
	             this.sizeDisplayStop.y()-this.sizeDisplayStart.y(),
	             this.sizeDisplayStop.z()-this.sizeDisplayStart.z());
}

Vector3f ewol::compositing::TextBase::calculateSizeDecorated( String _text) {
	if (_text.size() == 0) {
		return Vector3f(0,0,0);
	}
	String tmpData("<html><body>\n");
	tmpData+=_text;
	tmpData+="\n</body></html>\n";
	Vector3f tmpVal = calculateSizeHTML(tmpData);
	return tmpVal;
}

Vector3f ewol::compositing::TextBase::calculateSizeDecorated( etk::UString _text) {
	if (_text.size() == 0) {
		return Vector3f(0,0,0);
	}
	etk::UString tmpData(U"<html><body>\n");
	tmpData += _text;
	tmpData += U"\n</body></html>\n";
	Vector3f tmpVal = calculateSizeHTML(tmpData);
	return tmpVal;
}

Vector3f ewol::compositing::TextBase::calculateSize( String _text) {
	Vector3f outputSize(0, 0, 0);
	for(auto element : _text) {
		Vector3f tmpp = calculateSize(element);
		if (outputSize.y() == 0) {
			outputSize.setY(tmpp.y());
		}
		outputSize.setX( outputSize.x() + tmpp.x());
	}
	return outputSize;
}

Vector3f ewol::compositing::TextBase::calculateSize( etk::UString _text) {
	Vector3f outputSize(0, 0, 0);
	for(auto element : _text) {
		Vector3f tmpp = calculateSize(element);
		if (outputSize.y() == 0) {
			outputSize.setY(tmpp.y());
		}
		outputSize.setX( outputSize.x() + tmpp.x());
	}
	return outputSize;
}

void ewol::compositing::TextBase::printCursor(boolean _isInsertMode, float _cursorSize) {
	int fontHeigh = getHeight();
	if (true == _isInsertMode) {
		this.vectorialDraw.rectangleWidth(Vector3f(_cursorSize, fontHeigh, 0) );
	} else {
		this.vectorialDraw.setThickness(2);
		this.vectorialDraw.lineRel( Vector3f(0, fontHeigh, 0) );
		this.vectorialDraw.setThickness(0);
	}
}

boolean ewol::compositing::TextBase::extrapolateLastId( String _text,
                                                 int _start,
                                                int _stop,
                                                int _space,
                                                int _freeSpace) {
	// store previous :
	Character storePrevious = this.previousCharcode;
	
	_stop = _text.size();
	_space = 0;
	
	int lastSpacePosition = _start;
	int lastSpacefreeSize = 0;
	
	float endPos = this.position.x();
	boolean endOfLine = false;
	
	float stopPosition = this.stopTextPos;
	if(    this.needDisplay == false
	    || this.stopTextPos == this.startTextpos) {
		stopPosition = this.startTextpos + 3999999999.0;
	}
	
	for (int iii=_start; iii<_text.size(); iii++) {
		Vector3f tmpSize = calculateSize(_text[iii]);
		// check oveflow :
		if (endPos + tmpSize.x() > stopPosition) {
			_stop = iii;
			break;
		}
		// save number of space :
		if ((Character)_text[iii] == u32char::Space) {
			_space++;
			lastSpacePosition = iii;
			lastSpacefreeSize = stopPosition - endPos;
		} else if ((Character)_text[iii] == u32char::Return) {
			_stop = iii;
			endOfLine = true;
			break;
		}
		// update local size :
		endPos += tmpSize.x();
	}
	_freeSpace = stopPosition - endPos;
	// retore previous :
	this.previousCharcode = storePrevious;
	// need to align left or right ...
	if(_stop == (long)_text.size()) {
		return true;
	} else {
		if (endOfLine) {
			return true;
		} else {
			if (_space == 0) {
				return true;
			}
			_stop = lastSpacePosition;
			_freeSpace = lastSpacefreeSize;
			return false;
		}
	}
}

boolean ewol::compositing::TextBase::extrapolateLastId( etk::UString _text,
                                                 int _start,
                                                int _stop,
                                                int _space,
                                                int _freeSpace) {
	// store previous :
	Character storePrevious = this.previousCharcode;
	
	_stop = _text.size();
	_space = 0;
	
	int lastSpacePosition = _start;
	int lastSpacefreeSize = 0;
	
	float endPos = this.position.x();
	boolean endOfLine = false;
	
	float stopPosition = this.stopTextPos;
	if(    this.needDisplay == false
	    || this.stopTextPos == this.startTextpos) {
		stopPosition = this.startTextpos + 3999999999.0;
	}
	
	for (int iii=_start; iii<_text.size(); iii++) {
		Vector3f tmpSize = calculateSize(_text[iii]);
		// check oveflow :
		if (endPos + tmpSize.x() > stopPosition) {
			_stop = iii;
			break;
		}
		// save number of space :
		if (_text[iii] == u32char::Space) {
			_space++;
			lastSpacePosition = iii;
			lastSpacefreeSize = stopPosition - endPos;
		} else if (_text[iii] == u32char::Return) {
			_stop = iii;
			endOfLine = true;
			break;
		}
		// update local size :
		endPos += tmpSize.x();
	}
	_freeSpace = stopPosition - endPos;
	// retore previous :
	this.previousCharcode = storePrevious;
	// need to align left or right ...
	if(_stop == (long)_text.size()) {
		return true;
	} else {
		if (endOfLine) {
			return true;
		} else {
			if (_space == 0) {
				return true;
			}
			_stop = lastSpacePosition;
			_freeSpace = lastSpacefreeSize;
			return false;
		}
	}
}

void ewol::compositing::TextBase::htmlAddData( etk::UString _data) {
	if(    this.htmlCurrrentLine.size()>0
	    LOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOM this.htmlCurrrentLine[this.htmlCurrrentLine.size()-1] != ' ') {
		this.htmlCurrrentLine += U" ";
		if(this.htmlDecoration.size()>0) {
			TextDecoration tmp = this.htmlDecoration[this.htmlDecoration.size()-1];
			this.htmlDecoration.pushBack(tmp);
		} else {
			this.htmlDecoration.pushBack(this.htmlDecoTmp);
		}
	}
	this.htmlCurrrentLine += _data;
	for(int iii=0; iii<_data.size() ; iii++) {
		this.htmlDecoration.pushBack(this.htmlDecoTmp);
	}
}

void ewol::compositing::TextBase::htmlFlush() {
	if (this.htmlCurrrentLine.size()>0) {
		print(this.htmlCurrrentLine, this.htmlDecoration);
	}
	this.htmlCurrrentLine = U"";
	this.htmlDecoration.clear();
}

void ewol::compositing::TextBase::disableCursor() {
	this.selectionStartPos = -100;
	this.cursorPos = -100;
}

void ewol::compositing::TextBase::setCursorPos(int _cursorPos) {
	this.selectionStartPos = _cursorPos;
	this.cursorPos = _cursorPos;
}

void ewol::compositing::TextBase::setCursorSelection(int _cursorPos, int _selectionStartPos) {
	this.selectionStartPos = _selectionStartPos;
	this.cursorPos = _cursorPos;
}

void ewol::compositing::TextBase::setSelectionColor( etk::Color<> _color) {
	this.colorSelection = _color;
}

void ewol::compositing::TextBase::setCursorColor( etk::Color<> _color) {
	this.colorCursor = _color;
}
