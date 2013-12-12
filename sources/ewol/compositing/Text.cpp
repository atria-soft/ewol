/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#include <ewol/debug.h>
#include <ewol/compositing/Text.h>
#include <ewol/context/Context.h>
#include <etk/UString.h>

#undef __class__
#define __class__	"ewol::compositing::Text"


ewol::compositing::Text::Text(const std::string& _fontName, int32_t _fontSize) :
  m_position(0.0, 0.0, 0.0),
  m_clippingPosStart(0.0, 0.0, 0.0),
  m_clippingPosStop(0.0, 0.0, 0.0),
  m_clippingEnable(false),
  m_color(etk::color::black),
  m_colorBg(etk::color::none),
  m_colorCursor(etk::color::black),
  m_colorSelection(etk::color::olive),
  m_mode(ewol::font::Regular),
  m_kerning(true),
  m_distanceField(false),
  m_previousCharcode(0),
  m_startTextpos(0),
  m_stopTextPos(0),
  m_alignement(alignDisable),
  m_GLprogram(NULL),
  m_GLPosition(-1),
  m_GLMatrix(-1),
  m_GLColor(-1),
  m_GLtexture(-1),
  m_GLtexID(-1),
  m_selectionStartPos(-100),
  m_cursorPos(-100),
  m_font(NULL) {
	setFont(_fontName, _fontSize);
	loadProgram();
}


ewol::compositing::Text::~Text(void) {
	ewol::resource::TexturedFont::release(m_font);
	ewol::resource::Program::release(m_GLprogram);
}

void ewol::compositing::Text::loadProgram(void) {
	// get the shader resource :
	m_GLPosition = 0;
	m_GLprogram = ewol::resource::Program::keep("DATA:text.prog");
	if (m_GLprogram != NULL) {
		m_GLPosition = m_GLprogram->getAttribute("EW_coord2d");
		m_GLColor    = m_GLprogram->getAttribute("EW_color");
		m_GLtexture  = m_GLprogram->getAttribute("EW_texture2d");
		m_GLMatrix   = m_GLprogram->getUniform("EW_MatrixTransformation");
		m_GLtexID    = m_GLprogram->getUniform("EW_texID");
	}
}

void ewol::compositing::Text::draw(const mat4& _transformationMatrix, bool _enableDepthTest) {
	
	// draw BG in any case:
	m_vectorialDraw.draw();
	
	if (m_coord.size() <= 0 || m_font == NULL) {
		// TODO : a remÃštre ...
		//EWOL_WARNING("Nothink to draw...");
		return;
	}
	if (m_font == NULL) {
		EWOL_WARNING("no font...");
		return;
	}
	if (m_GLprogram == NULL) {
		EWOL_ERROR("No shader ...");
		return;
	}
	if (_enableDepthTest == true) {
		ewol::openGL::enable(ewol::openGL::FLAG_DEPTH_TEST);
	}
	// set Matrix : translation/positionMatrix
	mat4 projMatrix = ewol::openGL::getMatrix();
	mat4 camMatrix = ewol::openGL::getCameraMatrix();
	mat4 tmpMatrix = projMatrix * camMatrix * _transformationMatrix;
	m_GLprogram->use(); 
	m_GLprogram->uniformMatrix4fv(m_GLMatrix, 1, tmpMatrix.m_mat);
	// TextureID
	m_GLprogram->setTexture0(m_GLtexID, m_font->getId());
	// position :
	m_GLprogram->sendAttribute(m_GLPosition, 2/*x,y*/, &m_coord[0]);
	// Texture :
	m_GLprogram->sendAttribute(m_GLtexture, 2/*u,v*/, &m_coordTex[0]);
	// color :
	m_GLprogram->sendAttribute(m_GLColor, 4/*r,g,b,a*/, &m_coordColor[0]);
	// Request the draw od the elements : 
	ewol::openGL::drawArrays(GL_TRIANGLES, 0, m_coord.size());
	m_GLprogram->unUse();
	if (_enableDepthTest == true) {
		ewol::openGL::disable(ewol::openGL::FLAG_DEPTH_TEST);
	}
}


void ewol::compositing::Text::draw(bool _disableDepthTest) {
	// draw BG in any case:
	m_vectorialDraw.draw();
	
	if (m_coord.size() <= 0 || m_font == NULL) {
		// TODO : a remètre ...
		//EWOL_WARNING("Nothink to draw...");
		return;
	}
	if (m_font == NULL) {
		EWOL_WARNING("no font...");
		return;
	}
	if (m_GLprogram == NULL) {
		EWOL_ERROR("No shader ...");
		return;
	}
	// set Matrix : translation/positionMatrix
	mat4 tmpMatrix = ewol::openGL::getMatrix()*m_matrixApply;
	m_GLprogram->use(); 
	m_GLprogram->uniformMatrix4fv(m_GLMatrix, 1, tmpMatrix.m_mat);
	// TextureID
	m_GLprogram->setTexture0(m_GLtexID, m_font->getId());
	// position :
	m_GLprogram->sendAttribute(m_GLPosition, 2/*x,y*/, &m_coord[0]);
	// Texture :
	m_GLprogram->sendAttribute(m_GLtexture, 2/*u,v*/, &m_coordTex[0]);
	// color :
	m_GLprogram->sendAttribute(m_GLColor, 4/*r,g,b,a*/, &m_coordColor[0]);
	// Request the draw od the elements : 
	ewol::openGL::drawArrays(GL_TRIANGLES, 0, m_coord.size());
	m_GLprogram->unUse();
}

void ewol::compositing::Text::translate(const vec3& _vect) {
	ewol::Compositing::translate(_vect);
	m_vectorialDraw.translate(_vect);
}

void ewol::compositing::Text::rotate(const vec3& _vect, float _angle) {
	ewol::Compositing::rotate(_vect, _angle);
	m_vectorialDraw.rotate(_vect, _angle);
}

void ewol::compositing::Text::scale(const vec3& _vect) {
	ewol::Compositing::scale(_vect);
	m_vectorialDraw.scale(_vect);
}

void ewol::compositing::Text::clear(void) {
	// call upper class
	ewol::Compositing::clear();
	// remove sub draw system
	m_vectorialDraw.clear();
	// reset Buffer :
	m_coord.clear();
	m_coordTex.clear();
	m_coordColor.clear();
	// reset temporal variables :
	reset();
}

void ewol::compositing::Text::reset(void) {
	m_position = vec3(0,0,0);
	m_clippingPosStart = vec3(0,0,0);
	m_clippingPosStop = vec3(0,0,0);
	m_sizeDisplayStart = m_position;
	m_sizeDisplayStop = m_position;
	m_nbCharDisplayed = 0;
	m_clippingEnable = false;
	m_color = etk::color::black;
	m_colorBg = etk::color::none;
	m_mode = ewol::font::Regular;
	m_previousCharcode = 0;
	m_startTextpos = 0;
	m_stopTextPos = 0;
	m_alignement = ewol::Text::alignDisable;
	m_htmlCurrrentLine = "";
	m_selectionStartPos = -100;
	m_cursorPos = -100;
	m_htmlDecoration.clear();
	m_needDisplay = true;
	m_nbCharDisplayed = 0;
}

void ewol::compositing::Text::setPos(const vec3& _pos) {
	// check min max for display area
	if (m_nbCharDisplayed != 0) {
		EWOL_VERBOSE("update size 1 " << m_sizeDisplayStart << " " << m_sizeDisplayStop);
		m_sizeDisplayStop.setX(etk_max(m_position.x(), m_sizeDisplayStop.x()));
		m_sizeDisplayStop.setY(etk_max(m_position.y(), m_sizeDisplayStop.y()));
		m_sizeDisplayStart.setX(etk_min(m_position.x(), m_sizeDisplayStart.x()));
		m_sizeDisplayStart.setY(etk_min(m_position.y(), m_sizeDisplayStart.y()));
		EWOL_VERBOSE("update size 2 " << m_sizeDisplayStart << " " << m_sizeDisplayStop);
	}
	// update position
	m_position = _pos;
	m_previousCharcode = 0;
	m_vectorialDraw.setPos(m_position);
	// update min max of the display area:
	if (m_nbCharDisplayed == 0) {
		m_sizeDisplayStart = m_position;
		m_sizeDisplayStop = m_position;
		m_sizeDisplayStop.setY( m_sizeDisplayStop.y()+ m_font->getHeight(m_mode));
		EWOL_VERBOSE("update size 0 " << m_sizeDisplayStart << " " << m_sizeDisplayStop);
	} else {
		EWOL_VERBOSE("update size 3 " << m_sizeDisplayStart << " " << m_sizeDisplayStop);
		m_sizeDisplayStop.setX(etk_max(m_position.x(), m_sizeDisplayStop.x()));
		m_sizeDisplayStop.setY(etk_max(m_position.y(), m_sizeDisplayStop.y()));
		m_sizeDisplayStart.setX(etk_min(m_position.x(), m_sizeDisplayStart.x()));
		m_sizeDisplayStart.setY(etk_min(m_position.y(), m_sizeDisplayStart.y()));
		EWOL_VERBOSE("update size 4 " << m_sizeDisplayStart << " " << m_sizeDisplayStop);
	}
}

void ewol::compositing::Text::setRelPos(const vec3& _pos) {
	m_position += _pos;
	m_previousCharcode = 0;
	m_vectorialDraw.setPos(m_position);
}

void ewol::compositing::Text::setColorBg(const etk::Color<>& _color) {
	m_colorBg = _color;
	m_vectorialDraw.setColor(_color);
}

void ewol::compositing::Text::setClipping(const vec3& _pos, const vec3& _posEnd) {
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
	//m_vectorialDraw.setClipping(m_clippingPosStart, m_clippingPosStop);
}

void ewol::compositing::Text::setClippingMode(bool _newMode) {
	m_clippingEnable = _newMode;
	//m_vectorialDraw.setClippingMode(m_clippingEnable);
}

void ewol::compositing::Text::setFontSize(int32_t _fontSize) {
	// get old size
	std::string fontName = "";
	if (m_font != NULL) {
		fontName = m_font->getName();
		// Remove the :XX for the size ...
		size_t pos = fontName.rfind(':');
		fontName.erase(pos, fontName.size()-pos);
	}
	setFont(fontName, _fontSize);
}

void ewol::compositing::Text::setFontName(const std::string& _fontName) {
	// get old size
	int32_t fontSize = -1;
	if (m_font != NULL) {
		fontSize = m_font->getFontSize();
	}
	setFont(_fontName, fontSize);
}

void ewol::compositing::Text::setFont(std::string _fontName, int32_t _fontSize) {
	clear();
	// remove old one
	ewol::TexturedFont * previousFont = m_font;
	if (_fontSize <= 0) {
		_fontSize = ewol::getContext().getFontDefault().getSize();
	}
	if (_fontName == "") {
		_fontName = ewol::getContext().getFontDefault().getName();
	}
	_fontName += ":";
	_fontName += std::to_string(_fontSize);
	EWOL_VERBOSE("plop : " << _fontName << " size=" << _fontSize << " result :" << _fontName);
	// link to new one
	m_font = ewol::TexturedFont::keep(_fontName);
	if (m_font == NULL) {
		EWOL_ERROR("Can not get font resource");
		m_font = previousFont;
	} else {
		ewol::TexturedFont::release(previousFont);
	}
}

void ewol::compositing::Text::setFontMode(enum ewol::font::mode _mode) {
	if (m_font != NULL) {
		m_mode = m_font->getWrappingMode(_mode);
	}
}

enum ewol::font::mode ewol::Text::getFontMode(void) {
	return m_mode;
}

void ewol::compositing::Text::setFontBold(bool _status) {
	if (_status == true) {
		// enable
		if (m_mode == ewol::font::Regular) {
			setFontMode(ewol::font::Bold);
		} else if (m_mode == ewol::font::Italic) {
			setFontMode(ewol::font::BoldItalic);
		}
	} else {
		// disable
		if (m_mode == ewol::font::Bold) {
			setFontMode(ewol::font::Regular);
		} else if (m_mode == ewol::font::BoldItalic) {
			setFontMode(ewol::font::Italic);
		}
	}
}

void ewol::compositing::Text::setFontItalic(bool _status) {
	if (_status == true) {
		// enable
		if (m_mode == ewol::font::Regular) {
			setFontMode(ewol::font::Italic);
		} else if (m_mode == ewol::font::Bold) {
			setFontMode(ewol::font::BoldItalic);
		}
	} else {
		// disable
		if (m_mode == ewol::font::Italic) {
			setFontMode(ewol::font::Regular);
		} else if (m_mode == ewol::font::BoldItalic) {
			setFontMode(ewol::font::Bold);
		}
	}
}

void ewol::compositing::Text::setKerningMode(bool _newMode) {
	m_kerning = _newMode;
}

void ewol::compositing::Text::setDistanceFieldMode(bool _newMode) {
	m_distanceField = _newMode;
	EWOL_TODO("The Distance field mode is not availlable for now ...");
}

void ewol::compositing::Text::print(const std::u32string& _text) {
	std::vector<TextDecoration> decorationEmpty;
	print(_text, decorationEmpty);
}

void ewol::compositing::Text::print(const std::string& _text) {
	std::vector<TextDecoration> decorationEmpty;
	print(_text, decorationEmpty);
}


void ewol::compositing::Text::parseHtmlNode(exml::Element* _element) {
	// get the static real pointer
	if (_element == NULL) {
		EWOL_ERROR( "Error Input node does not existed ...");
	}
	for(size_t iii=0; iii< _element->size(); iii++) {
		if (_element->getType(iii) == exml::typeComment) {
			// nothing to do ...
		} else if (_element->getType(iii) == exml::typeText) {
			exml::Node* child = _element->getNode(iii);
			htmlAddData(child->getValue() );
			EWOL_VERBOSE("XML add : " << child->getValue());
			continue;
		} else if (_element->getType(iii)!=exml::typeElement) {
			EWOL_ERROR("(l "<< _element->getNode(iii)->getPos() << ") node not suported type : " << _element->getType(iii) << " val=\""<< _element->getNode(iii)->getValue() << "\"" );
			continue;
		}
		exml::Element* elem = _element->getElement(iii);
		if (elem == NULL) {
			EWOL_ERROR("Cast error ...");
			continue;
		}
		if(compare_no_case(elem->getValue(), "br") == true) {
			htmlFlush();
			EWOL_VERBOSE("XML flush & newLine");
			forceLineReturn();
		} else if (compare_no_case(elem->getValue(), "font") == true) {
			EWOL_VERBOSE("XML Font ...");
			TextDecoration tmpDeco = m_htmlDecoTmp;
			std::string colorValue = elem->getAttribute("color");
			m_htmlDecoTmp.m_colorFg = colorValue;
			colorValue = elem->getAttribute("colorBg");
			m_htmlDecoTmp.m_colorBg = colorValue;
			parseHtmlNode(elem);
			m_htmlDecoTmp = tmpDeco;
		} else if(    compare_no_case(elem->getValue(), "b") == true
		           || compare_no_case(elem->getValue(), "bold") == true) {
			EWOL_VERBOSE("XML bold ...");
			TextDecoration tmpDeco = m_htmlDecoTmp;
			if (m_htmlDecoTmp.m_mode == ewol::font::Regular) {
				m_htmlDecoTmp.m_mode = ewol::font::Bold;
			} else if (m_htmlDecoTmp.m_mode == ewol::font::Italic) {
				m_htmlDecoTmp.m_mode = ewol::font::BoldItalic;
			} 
			parseHtmlNode(elem);
			m_htmlDecoTmp = tmpDeco;
		} else if(    compare_no_case(elem->getValue(), "i") == true
		           || compare_no_case(elem->getValue(), "italic") == true) {
			EWOL_VERBOSE("XML italic ...");
			TextDecoration tmpDeco = m_htmlDecoTmp;
			if (m_htmlDecoTmp.m_mode == ewol::font::Regular) {
				m_htmlDecoTmp.m_mode = ewol::font::Italic;
			} else if (m_htmlDecoTmp.m_mode == ewol::font::Bold) {
				m_htmlDecoTmp.m_mode = ewol::font::BoldItalic;
			} 
			parseHtmlNode(elem);
			m_htmlDecoTmp = tmpDeco;
		} else if(    compare_no_case(elem->getValue(), "u") == true
		           || compare_no_case(elem->getValue(), "underline") == true) {
			EWOL_VERBOSE("XML underline ...");
			parseHtmlNode(elem);
		} else if(    compare_no_case(elem->getValue(), "p") == true
		           || compare_no_case(elem->getValue(), "paragraph") == true) {
			EWOL_VERBOSE("XML paragraph ...");
			htmlFlush();
			m_alignement = ewol::Text::alignLeft;
			forceLineReturn();
			parseHtmlNode(elem);
			forceLineReturn();
		} else if (compare_no_case(elem->getValue(), "center") == true) {
			EWOL_VERBOSE("XML center ...");
			htmlFlush();
			m_alignement = ewol::Text::alignCenter;
			parseHtmlNode(elem);
		} else if (compare_no_case(elem->getValue(), "left") == true) {
			EWOL_VERBOSE("XML left ...");
			htmlFlush();
			m_alignement = ewol::Text::alignLeft;
			parseHtmlNode(elem);
		} else if (compare_no_case(elem->getValue(), "right") == true) {
			EWOL_VERBOSE("XML right ...");
			htmlFlush();
			m_alignement = ewol::Text::alignRight;
			parseHtmlNode(elem);
		} else if (compare_no_case(elem->getValue(), "justify") == true) {
			EWOL_VERBOSE("XML justify ...");
			htmlFlush();
			m_alignement = ewol::Text::alignJustify;
			parseHtmlNode(elem);
		} else {
			EWOL_ERROR("(l "<< elem->getPos() << ") node not suported type : " << elem->getType() << " val=\""<< elem->getValue() << "\"" );
		}
	}
}

void ewol::compositing::Text::printDecorated(const std::string& _text) {
	std::string tmpData("<html>\n<body>\n");
	tmpData+=_text;
	tmpData+="\n</body>\n</html>\n";
	//EWOL_DEBUG("plop : " << tmpData);
	printHTML(tmpData);
}

void ewol::compositing::Text::printHTML(const std::string& _text) {
	exml::Document doc;
	
	// reset parameter :
	m_htmlDecoTmp.m_colorBg = etk::color::none;
	m_htmlDecoTmp.m_colorFg = etk::color::black;
	m_htmlDecoTmp.m_mode = ewol::font::Regular;
	
	if (doc.parse(_text) == false) {
		EWOL_ERROR( "can not load XML: PARSING error: Decorated text ");
		return;
	}
	
	exml::Element* root = (exml::Element*)doc.getNamed( "html" );
	if (root == NULL) {
		EWOL_ERROR( "can not load XML: main node not find: \"html\"");
		doc.display();
		return;
	}
	exml::Element* bodyNode = (exml::Element*)root->getNamed( "body" );
	if (root == NULL) {
		EWOL_ERROR( "can not load XML: main node not find: \"body\"");
		return;
	}
	(void)parseHtmlNode(bodyNode);
	htmlFlush();
}

void ewol::compositing::Text::print(const std::string& _text, const std::vector<TextDecoration>& _decoration) {
	if (m_font == NULL) {
		EWOL_ERROR("Font Id is not corectly defined");
		return;
	}
	etk::Color<> tmpFg(m_color);
	etk::Color<> tmpBg(m_colorBg);
	if (m_alignement == ewol::Text::alignDisable) {
		//EWOL_DEBUG(" 1 print in not alligned mode : start=" << m_sizeDisplayStart << " stop=" << m_sizeDisplayStop << " pos=" << m_position);
		// display the cursor if needed (if it is at the start position...)
		if (m_needDisplay == true) {
			if (0 == m_cursorPos) {
				m_vectorialDraw.setPos(m_position);
				setColorBg(m_colorCursor);
				printCursor(false);
			}
		}
		// note this is faster when nothing is requested ...
		for(size_t iii=0; iii<_text.size(); iii++) {
			// check if ve have decoration
			if (iii<_decoration.size()) {
				tmpFg = _decoration[iii].m_colorFg;
				tmpBg = _decoration[iii].m_colorBg;
				setFontMode(_decoration[iii].m_mode);
			}
			// if real display : ( not display is for size calculation)
			if (m_needDisplay == true) {
				if(    (    m_selectionStartPos-1 < (int64_t)iii
				         && (int64_t)iii  <= m_cursorPos-1)
				    || (    m_selectionStartPos-1 >= (int64_t)iii
				         && (int64_t)iii > m_cursorPos-1) ) {
					setColor(  0x000000FF);
					setColorBg(m_colorSelection);
				} else {
					setColor(  tmpFg);
					setColorBg(tmpBg);
				}
			}
			if(    m_needDisplay == true
			    && m_colorBg.a() != 0) {
				vec3 pos = m_position;
				m_vectorialDraw.setPos(pos);
				print(_text[iii]);
				float fontHeigh = m_font->getHeight(m_mode);
				m_vectorialDraw.rectangleWidth(vec3(m_position.x()-pos.x(),fontHeigh,0.0f) );
				m_nbCharDisplayed++;
			} else {
				print(_text[iii]);
				m_nbCharDisplayed++;
			}
			// display the cursor if needed (if it is at the other position...)
			if (m_needDisplay == true) {
				if ((int64_t)iii == m_cursorPos-1) {
					m_vectorialDraw.setPos(m_position);
					setColorBg(m_colorCursor);
					printCursor(false);
				}
			}
		}
		//EWOL_DEBUG(" 2 print in not alligned mode : start=" << m_sizeDisplayStart << " stop=" << m_sizeDisplayStop << " pos=" << m_position);
	} else {
		//EWOL_DEBUG(" 3 print in not alligned mode : start=" << m_sizeDisplayStart << " stop=" << m_sizeDisplayStop << " pos=" << m_position);
		// special start case at the right of the endpoint :
		if (m_stopTextPos < m_position.x()) {
			forceLineReturn();
		}
		float basicSpaceWidth = calculateSize(char32_t(' ')).x();
		int32_t currentId = 0;
		int32_t stop;
		int32_t space;
		int32_t freeSpace;
		while (currentId < (int64_t)_text.size()) {
			bool needNoJustify = extrapolateLastId(_text, currentId, stop, space, freeSpace);
			float interpolation = basicSpaceWidth;
			switch (m_alignement) {
				case ewol::Text::alignJustify:
					if (needNoJustify == false) {
						interpolation += (float)freeSpace / (float)(space-1);
					}
					break;
				case ewol::Text::alignDisable: // must not came from here ...
				case ewol::Text::alignLeft:
					// nothing to do ...
					break;
				case ewol::Text::alignRight:
					if (m_needDisplay == true) {
						// Move the first char at the right :
						setPos(vec3(m_position.x() + freeSpace,
						            m_position.y(),
						            m_position.z()) );
					}
					break;
				case ewol::Text::alignCenter:
					if (m_needDisplay == true) {
						// Move the first char at the right :
						setPos(vec3(m_position.x() + freeSpace/2,
						            m_position.y(),
						            m_position.z()) );
					}
					break;
			}
			// display all the elements
			if(    m_needDisplay == true
			    && m_cursorPos == 0) {
				m_vectorialDraw.setPos(m_position);
				setColorBg(m_colorCursor);
				printCursor(false);
			}
			for(size_t iii=currentId; (int64_t)iii<stop && iii<_text.size(); iii++) {
				float fontHeigh = m_font->getHeight(m_mode);
				// get specific decoration if provided
				if (iii<_decoration.size()) {
					tmpFg = _decoration[iii].m_colorFg;
					tmpBg = _decoration[iii].m_colorBg;
					setFontMode(_decoration[iii].m_mode);
				}
				if (m_needDisplay == true) {
					if(    (    m_selectionStartPos-1<(int64_t)iii
					         && (int64_t)iii  <= m_cursorPos-1)
					    || (    m_selectionStartPos-1 >= (int64_t)iii
					         && (int64_t)iii > m_cursorPos-1) ) {
						setColor(  0x000000FF);
						setColorBg(m_colorSelection);
					} else {
						setColor(  tmpFg);
						setColorBg(tmpBg);
					}
				}
				// special for the justify mode
				if ((char32_t)_text[iii] == etk::UChar::Space) {
					//EWOL_DEBUG(" generateString : \" \"");
					if(    m_needDisplay == true
					    && m_colorBg.a() != 0) {
						m_vectorialDraw.setPos(m_position);
					}
					// Must generate a dynamic space : 
					setPos(vec3(m_position.x() + interpolation,
					            m_position.y(),
					            m_position.z()) );
					if(    m_needDisplay == true
					    && m_colorBg.a() != 0) {
						m_vectorialDraw.rectangleWidth(vec3(interpolation,fontHeigh,0.0f) );
					}
				} else {
					//EWOL_DEBUG(" generateString : \"" << (char)text[iii] << "\"");
					if(    m_needDisplay == true
					    && m_colorBg.a() != 0) {
						vec3 pos = m_position;
						m_vectorialDraw.setPos(pos);
						print(_text[iii]);
						m_vectorialDraw.rectangleWidth(vec3(m_position.x()-pos.x(),fontHeigh,0.0f) );
						m_nbCharDisplayed++;
					} else {
						print(_text[iii]);
						m_nbCharDisplayed++;
					}
				}
				if (m_needDisplay == true) {
					if ((int64_t)iii == m_cursorPos-1) {
						m_vectorialDraw.setPos(m_position);
						setColorBg(m_colorCursor);
						printCursor(false);
					}
				}
			}
			if (currentId == stop) {
				currentId++;
			} else if((char32_t)_text[stop] == etk::UChar::Space) {
				currentId = stop+1;
				// reset position :
				setPos(vec3(m_startTextpos,
				            (float)(m_position.y() - m_font->getHeight(m_mode)),
				            m_position.z()) );
				m_nbCharDisplayed++;
			} else if((char32_t)_text[stop] == etk::UChar::Return) {
				currentId = stop+1;
				// reset position :
				setPos(vec3(m_startTextpos,
				            (float)(m_position.y() - m_font->getHeight(m_mode)),
				            m_position.z()) );
				m_nbCharDisplayed++;
			} else {
				currentId = stop;
			}
		}
		//EWOL_DEBUG(" 4 print in not alligned mode : start=" << m_sizeDisplayStart << " stop=" << m_sizeDisplayStop << " pos=" << m_position);
	}
}

void ewol::compositing::Text::print(const std::u32string& _text, const std::vector<TextDecoration>& _decoration) {
	if (m_font == NULL) {
		EWOL_ERROR("Font Id is not corectly defined");
		return;
	}
	etk::Color<> tmpFg(m_color);
	etk::Color<> tmpBg(m_colorBg);
	if (m_alignement == ewol::Text::alignDisable) {
		//EWOL_DEBUG(" 1 print in not alligned mode : start=" << m_sizeDisplayStart << " stop=" << m_sizeDisplayStop << " pos=" << m_position);
		// display the cursor if needed (if it is at the start position...)
		if (m_needDisplay == true) {
			if (0 == m_cursorPos) {
				m_vectorialDraw.setPos(m_position);
				setColorBg(m_colorCursor);
				printCursor(false);
			}
		}
		// note this is faster when nothing is requested ...
		for(size_t iii=0; iii<_text.size(); iii++) {
			// check if ve have decoration
			if (iii<_decoration.size()) {
				tmpFg = _decoration[iii].m_colorFg;
				tmpBg = _decoration[iii].m_colorBg;
				setFontMode(_decoration[iii].m_mode);
			}
			// if real display : ( not display is for size calculation)
			if (m_needDisplay == true) {
				if(    (    m_selectionStartPos-1<(int64_t)iii
				         && (int64_t)iii  <= m_cursorPos-1)
				    || (    m_selectionStartPos-1 >= (int64_t)iii
				         && (int64_t)iii > m_cursorPos-1) ) {
					setColor(  0x000000FF);
					setColorBg(m_colorSelection);
				} else {
					setColor(  tmpFg);
					setColorBg(tmpBg);
				}
			}
			if(    m_needDisplay == true
			    && m_colorBg.a() != 0) {
				vec3 pos = m_position;
				m_vectorialDraw.setPos(pos);
				print(_text[iii]);
				float fontHeigh = m_font->getHeight(m_mode);
				m_vectorialDraw.rectangleWidth(vec3(m_position.x()-pos.x(),fontHeigh,0.0f) );
				m_nbCharDisplayed++;
			} else {
				print(_text[iii]);
				m_nbCharDisplayed++;
			}
			// display the cursor if needed (if it is at the other position...)
			if (m_needDisplay == true) {
				if ((int64_t)iii == m_cursorPos-1) {
					m_vectorialDraw.setPos(m_position);
					setColorBg(m_colorCursor);
					printCursor(false);
				}
			}
		}
		//EWOL_DEBUG(" 2 print in not alligned mode : start=" << m_sizeDisplayStart << " stop=" << m_sizeDisplayStop << " pos=" << m_position);
	} else {
		//EWOL_DEBUG(" 3 print in not alligned mode : start=" << m_sizeDisplayStart << " stop=" << m_sizeDisplayStop << " pos=" << m_position);
		// special start case at the right of the endpoint :
		if (m_stopTextPos < m_position.x()) {
			forceLineReturn();
		}
		float basicSpaceWidth = calculateSize(char32_t(' ')).x();
		int32_t currentId = 0;
		int32_t stop;
		int32_t space;
		int32_t freeSpace;
		while (currentId < (int64_t)_text.size()) {
			bool needNoJustify = extrapolateLastId(_text, currentId, stop, space, freeSpace);
			float interpolation = basicSpaceWidth;
			switch (m_alignement) {
				case ewol::Text::alignJustify:
					if (needNoJustify == false) {
						interpolation += (float)freeSpace / (float)(space-1);
					}
					break;
				case ewol::Text::alignDisable: // must not came from here ...
				case ewol::Text::alignLeft:
					// nothing to do ...
					break;
				case ewol::Text::alignRight:
					if (m_needDisplay == true) {
						// Move the first char at the right :
						setPos(vec3(m_position.x() + freeSpace,
						            m_position.y(),
						            m_position.z()) );
					}
					break;
				case ewol::Text::alignCenter:
					if (m_needDisplay == true) {
						// Move the first char at the right :
						setPos(vec3(m_position.x() + freeSpace/2,
						            m_position.y(),
						            m_position.z()) );
					}
					break;
			}
			// display all the elements
			if(    m_needDisplay == true
			    && m_cursorPos == 0) {
				m_vectorialDraw.setPos(m_position);
				setColorBg(m_colorCursor);
				printCursor(false);
			}
			for(size_t iii=currentId; (int64_t)iii<stop && iii<_text.size(); iii++) {
				float fontHeigh = m_font->getHeight(m_mode);
				// get specific decoration if provided
				if (iii<_decoration.size()) {
					tmpFg = _decoration[iii].m_colorFg;
					tmpBg = _decoration[iii].m_colorBg;
					setFontMode(_decoration[iii].m_mode);
				}
				if (m_needDisplay == true) {
					if(    (    m_selectionStartPos-1<(int64_t)iii
					         && (int64_t)iii  <= m_cursorPos-1)
					    || (    m_selectionStartPos-1 >= (int64_t)iii
					         && (int64_t)iii > m_cursorPos-1) ) {
						setColor(  0x000000FF);
						setColorBg(m_colorSelection);
					} else {
						setColor(  tmpFg);
						setColorBg(tmpBg);
					}
				}
				// special for the justify mode
				if ((char32_t)_text[iii] == etk::UChar::Space) {
					//EWOL_DEBUG(" generateString : \" \"");
					if(    m_needDisplay == true
					    && m_colorBg.a() != 0) {
						m_vectorialDraw.setPos(m_position);
					}
					// Must generate a dynamic space : 
					setPos(vec3(m_position.x() + interpolation,
					            m_position.y(),
					            m_position.z()) );
					if(    m_needDisplay == true
					    && m_colorBg.a() != 0) {
						m_vectorialDraw.rectangleWidth(vec3(interpolation,fontHeigh,0.0f) );
					}
				} else {
					//EWOL_DEBUG(" generateString : \"" << (char)text[iii] << "\"");
					if(    m_needDisplay == true
					    && m_colorBg.a() != 0) {
						vec3 pos = m_position;
						m_vectorialDraw.setPos(pos);
						print(_text[iii]);
						m_vectorialDraw.rectangleWidth(vec3(m_position.x()-pos.x(),fontHeigh,0.0f) );
						m_nbCharDisplayed++;
					} else {
						print(_text[iii]);
						m_nbCharDisplayed++;
					}
				}
				if (m_needDisplay == true) {
					if ((int64_t)iii == m_cursorPos-1) {
						m_vectorialDraw.setPos(m_position);
						setColorBg(m_colorCursor);
						printCursor(false);
					}
				}
			}
			if (currentId == stop) {
				currentId++;
			} else if(_text[stop] == etk::UChar::Space) {
				currentId = stop+1;
				// reset position :
				setPos(vec3(m_startTextpos,
				            (float)(m_position.y() - m_font->getHeight(m_mode)),
				            m_position.z()) );
				m_nbCharDisplayed++;
			} else if(_text[stop] == etk::UChar::Return) {
				currentId = stop+1;
				// reset position :
				setPos(vec3(m_startTextpos,
				            (float)(m_position.y() - m_font->getHeight(m_mode)),
				            m_position.z()) );
				m_nbCharDisplayed++;
			} else {
				currentId = stop;
			}
		}
		//EWOL_DEBUG(" 4 print in not alligned mode : start=" << m_sizeDisplayStart << " stop=" << m_sizeDisplayStop << " pos=" << m_position);
	}
}


void ewol::compositing::Text::print(const char32_t& _charcode) {
	if (NULL == m_font) {
		EWOL_ERROR("Font Id is not corectly defined");
		return;
	}
	// get a pointer on the glyph property : 
	ewol::GlyphProperty* myGlyph = m_font->getGlyphPointer(_charcode, m_mode);
	if (NULL == myGlyph) {
		EWOL_ERROR(" font does not really existed ...");
		return;
	}
	int32_t fontSize = m_font->getFontSize();
	int32_t fontHeigh = m_font->getHeight(m_mode);
	
	// get the kerning ofset :
	float kerningOffset = 0;
	if (true == m_kerning) {
		kerningOffset = myGlyph->kerningGet(m_previousCharcode);
		if (kerningOffset != 0) {
			//EWOL_DEBUG("Kerning between : '" << m_previousCharcode << "'&'" << myGlyph->m_UVal << "' value : " << kerningOffset);
		}
	}
	// 0x01 == 0x20 == ' ';
	if (_charcode != 0x01) {
		/* Bitmap position
		 *      xA     xB
		 *   yC *------*
		 *      |      |
		 *      |      |
		 *   yD *------*
		 */
		float dxA = m_position.x() + myGlyph->m_bearing.x() + kerningOffset;
		float dxB = dxA + myGlyph->m_sizeTexture.x();
		float dyC = m_position.y() + myGlyph->m_bearing.y() + fontHeigh - fontSize;
		float dyD = dyC - myGlyph->m_sizeTexture.y();
		
		float tuA = myGlyph->m_texturePosStart.x();
		float tuB = tuA + myGlyph->m_texturePosSize.x();
		float tvC = myGlyph->m_texturePosStart.y();
		float tvD = tvC + myGlyph->m_texturePosSize.y();
		
		
		// Clipping and drawing area
		if(    m_clippingEnable == true
		    && (    dxB < m_clippingPosStart.x()
		         || dxA > m_clippingPosStop.x()
		         || dyC < m_clippingPosStart.y()
		         || dyD > m_clippingPosStop.y() ) ) {
			// Nothing to diplay ...
		} else {
			if (m_clippingEnable == true) {
				// generata positions...
				float TexSizeX = tuB - tuA;
				if (dxA < m_clippingPosStart.x()) {
					// clip display
					float drawSize = m_clippingPosStart.x() - dxA;
					// update element start display
					dxA = m_clippingPosStart.x();
					float addElement = TexSizeX * drawSize / (float)myGlyph->m_sizeTexture.x();
					// update texture start X Pos
					tuA += addElement;
				}
				if (dxB > m_clippingPosStop.x()) {
					// clip display
					float drawSize = dxB - m_clippingPosStop.x();
					// update element start display
					dxB = m_clippingPosStop.x();
					float addElement = TexSizeX * drawSize / (float)myGlyph->m_sizeTexture.x();
					// update texture start X Pos
					tuB -= addElement;
				}
				float TexSizeY = tvC - tvD;
				if (dyC > m_clippingPosStop.y()) {
					// clip display
					float drawSize = dyC - m_clippingPosStop.y();
					// update element start display
					dyC = m_clippingPosStop.y();
					float addElement = TexSizeY * drawSize / (float)myGlyph->m_sizeTexture.y();
					// update texture start X Pos
					tvC -= addElement;
				}
				if (dyD < m_clippingPosStart.y()) {
					// clip display
					float drawSize = m_clippingPosStart.y() - dyD;
					// update element start display
					dyD = m_clippingPosStart.y();
					float addElement = TexSizeY * drawSize / (float)myGlyph->m_sizeTexture.y();
					// update texture start X Pos
					tvD += addElement;
				}
			}
			if(    dxB <= dxA
			    || dyD >= dyC) {
				// nothing to do ...
			} else {
				/* Bitmap position
				 *   0------1
				 *   |      |
				 *   |      |
				 *   3------2
				 */
				if (m_needDisplay == true) {
					ivec2 bitmapDrawPos[4];
					bitmapDrawPos[0].setValue((int32_t)dxA, (int32_t)dyC);
					bitmapDrawPos[1].setValue((int32_t)dxB, (int32_t)dyC);
					bitmapDrawPos[2].setValue((int32_t)dxB, (int32_t)dyD);
					bitmapDrawPos[3].setValue((int32_t)dxA, (int32_t)dyD);
					/* texture Position : 
					 *   0------1
					 *   |      |
					 *   |      |
					 *   3------2
					 */
					vec2 texturePos[4];
					texturePos[0].setValue(tuA+m_mode, tvC);
					texturePos[1].setValue(tuB+m_mode, tvC);
					texturePos[2].setValue(tuB+m_mode, tvD);
					texturePos[3].setValue(tuA+m_mode, tvD);
					
					// NOTE : Android does not support the Quads elements ...
					/* Step 1 : 
					 *   ********     
					 *     ******     
					 *       ****     
					 *         **     
					 *                
					 */
					// set texture coordonates :
					m_coordTex.push_back(texturePos[0]);
					m_coordTex.push_back(texturePos[1]);
					m_coordTex.push_back(texturePos[2]);
					// set display positions :
					m_coord.push_back(bitmapDrawPos[0]);
					m_coord.push_back(bitmapDrawPos[1]);
					m_coord.push_back(bitmapDrawPos[2]);
					// set the color
					m_coordColor.push_back(m_color);
					m_coordColor.push_back(m_color);
					m_coordColor.push_back(m_color);
					/* Step 2 : 
					 *              
					 *   **         
					 *   ****       
					 *   ******     
					 *   ********   
					 */
					// set texture coordonates :
					m_coordTex.push_back(texturePos[0]);
					m_coordTex.push_back(texturePos[2]);
					m_coordTex.push_back(texturePos[3]);
					// set display positions :
					m_coord.push_back(bitmapDrawPos[0]);
					m_coord.push_back(bitmapDrawPos[2]);
					m_coord.push_back(bitmapDrawPos[3]);
					// set the color
					m_coordColor.push_back(m_color);
					m_coordColor.push_back(m_color);
					m_coordColor.push_back(m_color);
				}
			}
		}
	}
	// move the position :
	//EWOL_DEBUG(" 5 pos=" << m_position << " advance=" << myGlyph->m_advance.x() << " kerningOffset=" << kerningOffset);
	m_position.setX(m_position.x() + myGlyph->m_advance.x() + kerningOffset);
	//EWOL_DEBUG(" 6 print '" << charcode << "' : start=" << m_sizeDisplayStart << " stop=" << m_sizeDisplayStop << " pos=" << m_position);
	// Register the previous character
	m_previousCharcode = _charcode;
	return;
}

void ewol::compositing::Text::forceLineReturn(void) {
	// reset position : 
	setPos(vec3(m_startTextpos, m_position.y() - m_font->getHeight(m_mode), 0) );
}

void ewol::compositing::Text::setTextAlignement(float _startTextpos, float _stopTextPos, enum ewol::Text::aligneMode _alignement) {
	m_startTextpos = _startTextpos;
	m_stopTextPos = _stopTextPos+1;
	m_alignement = _alignement;
	if (m_startTextpos >= m_stopTextPos) {
		EWOL_ERROR("Request allignement with Borne position error : " << _startTextpos << " => " << _stopTextPos);
	}
}

enum ewol::compositing::Text::aligneMode ewol::compositing::Text::getAlignement(void) {
	return m_alignement;
}

void ewol::compositing::Text::disableAlignement(void) {
	m_alignement = ewol::Text::alignDisable;
}

vec3 ewol::compositing::Text::calculateSizeHTML(const std::string& _text) {
	// remove intermediate result 
	reset();
	//EWOL_DEBUG("        0 size for=\n" << text);
	// disable display system
	m_needDisplay = false;
	
	setPos(vec3(0,0,0) );
	// same as print without the end display ...
	printHTML(_text);
	//EWOL_DEBUG("        1 Start pos=" << m_sizeDisplayStart);
	//EWOL_DEBUG("        1 Stop pos=" << m_sizeDisplayStop);
	
	// get the last elements
	m_sizeDisplayStop.setValue(etk_max(m_position.x(), m_sizeDisplayStop.x()) ,
	                           etk_max(m_position.y(), m_sizeDisplayStop.y()) ,
	                           0);
	m_sizeDisplayStart.setValue(etk_min(m_position.x(), m_sizeDisplayStart.x()) ,
	                            etk_min(m_position.y(), m_sizeDisplayStart.y()) ,
	                            0);
	
	//EWOL_DEBUG("        2 Start pos=" << m_sizeDisplayStart);
	//EWOL_DEBUG("        2 Stop pos=" << m_sizeDisplayStop);
	// set back the display system
	m_needDisplay = true;
	
	return vec3( m_sizeDisplayStop.x()-m_sizeDisplayStart.x(),
	             m_sizeDisplayStop.y()-m_sizeDisplayStart.y(),
	             m_sizeDisplayStop.z()-m_sizeDisplayStart.z());
}

vec3 ewol::compositing::Text::calculateSizeDecorated(const std::string& _text) {
	if (_text.size() == 0) {
		return vec3(0,0,0);
	}
	std::string tmpData("<html><body>\n");
	tmpData+=_text;
	tmpData+="\n</body></html>\n";
	vec3 tmpVal = calculateSizeHTML(tmpData);
	return tmpVal;
}

vec3 ewol::compositing::Text::calculateSize(const std::string& _text) {
	if (m_font == NULL) {
		EWOL_ERROR("Font Id is not corectly defined");
		return vec3(0,0,0);
	}
	vec3 outputSize(0, 0, 0);
	for(auto element : _text) {
		vec3 tmpp = calculateSize(element);
		if (outputSize.y() == 0) {
			outputSize.setY(tmpp.y());
		}
		outputSize.setX( outputSize.x() + tmpp.x());
	}
	return outputSize;
}

vec3 ewol::compositing::Text::calculateSize(const char32_t& _charcode) {
	if (m_font == NULL) {
		EWOL_ERROR("Font Id is not corectly defined");
		return vec3(0,0,0);
	}
	// get a pointer on the glyph property : 
	ewol::GlyphProperty * myGlyph = m_font->getGlyphPointer(_charcode, m_mode);
	int32_t fontHeigh = m_font->getHeight(m_mode);
	
	// get the kerning ofset :
	float kerningOffset = 0.0;
	if (true == m_kerning) {
		kerningOffset = myGlyph->kerningGet(m_previousCharcode);
	}
	
	vec3 outputSize((float)(myGlyph->m_advance.x() + kerningOffset),
	                (float)(fontHeigh),
	                (float)(0.0));
	// Register the previous character
	m_previousCharcode = _charcode;
	return outputSize;
}

void ewol::compositing::Text::printCursor(bool _isInsertMode, float _cursorSize) {
	int32_t fontHeigh = m_font->getHeight(m_mode);
	if (true == _isInsertMode) {
		m_vectorialDraw.rectangleWidth(vec3(_cursorSize, fontHeigh, 0) );
	} else {
		m_vectorialDraw.setThickness(2);
		m_vectorialDraw.lineRel( vec3(0, fontHeigh, 0) );
		m_vectorialDraw.setThickness(0);
	}
}

bool ewol::compositing::Text::extrapolateLastId(const std::string& _text,
                                                const int32_t _start,
                                                int32_t& _stop,
                                                int32_t& _space,
                                                int32_t& _freeSpace) {
	// store previous :
	char32_t storePrevious = m_previousCharcode;
	
	_stop = _text.size();
	_space = 0;
	
	int32_t lastSpacePosition = _start;
	int32_t lastSpacefreeSize;
	
	float endPos = m_position.x();
	bool endOfLine = false;
	
	float stopPosition = m_stopTextPos;
	if(    m_needDisplay == false
	    || m_stopTextPos == m_startTextpos) {
		stopPosition = m_startTextpos + 3999999999.0;
	}
	
	for (size_t iii=_start; iii<_text.size(); iii++) {
		vec3 tmpSize = calculateSize(_text[iii]);
		// check oveflow :
		if (endPos + tmpSize.x() > stopPosition) {
			_stop = iii;
			break;
		}
		// save number of space :
		if ((char32_t)_text[iii] == etk::UChar::Space) {
			_space++;
			lastSpacePosition = iii;
			lastSpacefreeSize = stopPosition - endPos;
		} else if ((char32_t)_text[iii] == etk::UChar::Return) {
			_stop = iii;
			endOfLine = true;
			break;
		}
		// update local size :
		endPos += tmpSize.x();
	}
	_freeSpace = stopPosition - endPos;
	// retore previous :
	m_previousCharcode = storePrevious;
	// need to align left or right ...
	if(_stop == (int64_t)_text.size()) {
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

bool ewol::compositing::Text::extrapolateLastId(const std::u32string& _text,
                                                const int32_t _start,
                                                int32_t& _stop,
                                                int32_t& _space,
                                                int32_t& _freeSpace) {
	// store previous :
	char32_t storePrevious = m_previousCharcode;
	
	_stop = _text.size();
	_space = 0;
	
	int32_t lastSpacePosition = _start;
	int32_t lastSpacefreeSize;
	
	float endPos = m_position.x();
	bool endOfLine = false;
	
	float stopPosition = m_stopTextPos;
	if(    m_needDisplay == false
	    || m_stopTextPos == m_startTextpos) {
		stopPosition = m_startTextpos + 3999999999.0;
	}
	
	for (size_t iii=_start; iii<_text.size(); iii++) {
		vec3 tmpSize = calculateSize(_text[iii]);
		// check oveflow :
		if (endPos + tmpSize.x() > stopPosition) {
			_stop = iii;
			break;
		}
		// save number of space :
		if (_text[iii] == etk::UChar::Space) {
			_space++;
			lastSpacePosition = iii;
			lastSpacefreeSize = stopPosition - endPos;
		} else if (_text[iii] == etk::UChar::Return) {
			_stop = iii;
			endOfLine = true;
			break;
		}
		// update local size :
		endPos += tmpSize.x();
	}
	_freeSpace = stopPosition - endPos;
	// retore previous :
	m_previousCharcode = storePrevious;
	// need to align left or right ...
	if(_stop == (int64_t)_text.size()) {
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

void ewol::compositing::Text::htmlAddData(const std::string& _data) {
	if(    m_htmlCurrrentLine.size()>0
	    && m_htmlCurrrentLine[m_htmlCurrrentLine.size()-1] != ' ') {
		m_htmlCurrrentLine+=" ";
		if(m_htmlDecoration.size()>0) {
			TextDecoration tmp = m_htmlDecoration[m_htmlDecoration.size()-1];
			m_htmlDecoration.push_back(tmp);
		} else {
			m_htmlDecoration.push_back(m_htmlDecoTmp);
		}
	}
	m_htmlCurrrentLine += _data;
	for(size_t iii=0; iii<_data.size() ; iii++) {
		m_htmlDecoration.push_back(m_htmlDecoTmp);
	}
}

void ewol::compositing::Text::htmlFlush(void) {
	if (m_htmlCurrrentLine.size()>0) {
		print(m_htmlCurrrentLine, m_htmlDecoration);
	}
	m_htmlCurrrentLine = "";
	m_htmlDecoration.clear();
}

void ewol::compositing::Text::disableCursor(void) {
	m_selectionStartPos = -100;
	m_cursorPos = -100;
}

void ewol::compositing::Text::setCursorPos(int32_t _cursorPos) {
	m_selectionStartPos = _cursorPos;
	m_cursorPos = _cursorPos;
}

void ewol::compositing::Text::setCursorSelection(int32_t _cursorPos, int32_t _selectionStartPos) {
	m_selectionStartPos = _selectionStartPos;
	m_cursorPos = _cursorPos;
}

void ewol::compositing::Text::setSelectionColor(const etk::Color<>& _color) {
	m_colorSelection = _color;
}

void ewol::compositing::Text::setCursorColor(const etk::Color<>& _color) {
	m_colorCursor = _color;
}
