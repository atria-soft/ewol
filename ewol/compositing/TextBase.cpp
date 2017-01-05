/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */

#include <ewol/debug.hpp>
#include <ewol/compositing/Text.hpp>
#include <ewol/context/Context.hpp>
#include <etk/types.hpp>


const int32_t ewol::compositing::TextBase::m_vboIdCoord(0);
const int32_t ewol::compositing::TextBase::m_vboIdCoordText(1);
const int32_t ewol::compositing::TextBase::m_vboIdColor(2);
const int32_t ewol::compositing::TextBase::m_vboIdGlyphLevel(3);
#define NB_VBO (4)

ewol::compositing::TextBase::TextBase(const std::string& _shaderName, bool _loadProgram) :
  m_position(0.0, 0.0, 0.0),
  m_clippingPosStart(0.0, 0.0, 0.0),
  m_clippingPosStop(0.0, 0.0, 0.0),
  m_clippingEnable(false),
  m_defaultColorFg(etk::color::black),
  m_defaultColorBg(etk::color::none),
  m_color(etk::color::black),
  m_colorBg(etk::color::none),
  m_colorCursor(etk::color::black),
  m_colorSelection(etk::color::olive),
  m_mode(ewol::font::Regular),
  m_kerning(true),
  m_previousCharcode(0),
  m_startTextpos(0),
  m_stopTextPos(0),
  m_alignement(alignDisable),
  m_GLprogram(nullptr),
  m_GLPosition(-1),
  m_GLMatrix(-1),
  m_GLColor(-1),
  m_GLtexture(-1),
  m_GLtexID(-1),
  m_selectionStartPos(-100),
  m_cursorPos(-100) {
	if (_loadProgram == true) {
		loadProgram(_shaderName);
	}
	// Create the VBO:
	m_VBO = gale::resource::VirtualBufferObject::create(NB_VBO);
	if (m_VBO == nullptr) {
		EWOL_ERROR("can not instanciate VBO ...");
		return;
	}
	// TO facilitate some debugs we add a name of the VBO:
	m_VBO->setName("[VBO] of ewol::compositing::TextBase");
}


ewol::compositing::TextBase::~TextBase() {
	
}

void ewol::compositing::TextBase::loadProgram(const std::string& _shaderName) {
	// get the shader resource:
	m_GLPosition = 0;
	ememory::SharedPtr<gale::resource::Program> old = m_GLprogram;
	m_GLprogram = gale::resource::Program::create(_shaderName);
	if (m_GLprogram != nullptr) {
		m_GLPosition   = m_GLprogram->getAttribute("EW_coord3d");
		m_GLColor      = m_GLprogram->getAttribute("EW_color");
		m_GLtexture    = m_GLprogram->getAttribute("EW_texture2d");
		m_GLMatrix     = m_GLprogram->getUniform("EW_MatrixTransformation");
		m_GLtexID      = m_GLprogram->getUniform("EW_texID");
		m_GLtextWidth  = m_GLprogram->getUniform("EW_texWidth");
		m_GLtextHeight = m_GLprogram->getUniform("EW_texHeight");
	} else {
		EWOL_ERROR("Can not load the program => create previous one...");
		m_GLprogram = old;
		old = nullptr;
	}
}

void ewol::compositing::TextBase::translate(const vec3& _vect) {
	ewol::Compositing::translate(_vect);
	m_vectorialDraw.translate(_vect);
}

void ewol::compositing::TextBase::rotate(const vec3& _vect, float _angle) {
	ewol::Compositing::rotate(_vect, _angle);
	m_vectorialDraw.rotate(_vect, _angle);
}

void ewol::compositing::TextBase::scale(const vec3& _vect) {
	ewol::Compositing::scale(_vect);
	m_vectorialDraw.scale(_vect);
}

void ewol::compositing::TextBase::clear() {
	// call upper class
	ewol::Compositing::clear();
	// remove sub draw system
	m_vectorialDraw.clear();
	// reset Buffer:
	m_VBO->clear();
	// reset temporal variables:
	reset();
}

void ewol::compositing::TextBase::reset() {
	m_position = vec3(0,0,0);
	m_clippingPosStart = vec3(0,0,0);
	m_clippingPosStop = vec3(0,0,0);
	m_sizeDisplayStart = m_position;
	m_sizeDisplayStop = m_position;
	m_nbCharDisplayed = 0;
	m_clippingEnable = false;
	m_color = m_defaultColorFg;
	m_colorBg = m_defaultColorBg;
	m_mode = ewol::font::Regular;
	m_previousCharcode = 0;
	m_startTextpos = 0;
	m_stopTextPos = 0;
	m_alignement = alignDisable;
	m_htmlCurrrentLine = U"";
	m_selectionStartPos = -100;
	m_cursorPos = -100;
	m_htmlDecoration.clear();
	m_needDisplay = true;
	m_nbCharDisplayed = 0;
}

void ewol::compositing::TextBase::setPos(const vec3& _pos) {
	// check min max for display area
	if (m_nbCharDisplayed != 0) {
		EWOL_VERBOSE("update size 1 " << m_sizeDisplayStart << " " << m_sizeDisplayStop);
		m_sizeDisplayStop.setX(std::max(m_position.x(), m_sizeDisplayStop.x()));
		m_sizeDisplayStop.setY(std::max(m_position.y(), m_sizeDisplayStop.y()));
		m_sizeDisplayStart.setX(std::min(m_position.x(), m_sizeDisplayStart.x()));
		m_sizeDisplayStart.setY(std::min(m_position.y(), m_sizeDisplayStart.y()));
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
		m_sizeDisplayStop.setY( m_sizeDisplayStop.y()+ getHeight());
		EWOL_VERBOSE("update size 0 " << m_sizeDisplayStart << " " << m_sizeDisplayStop);
	} else {
		EWOL_VERBOSE("update size 3 " << m_sizeDisplayStart << " " << m_sizeDisplayStop);
		m_sizeDisplayStop.setX(std::max(m_position.x(), m_sizeDisplayStop.x()));
		m_sizeDisplayStop.setY(std::max(m_position.y(), m_sizeDisplayStop.y()));
		m_sizeDisplayStart.setX(std::min(m_position.x(), m_sizeDisplayStart.x()));
		m_sizeDisplayStart.setY(std::min(m_position.y(), m_sizeDisplayStart.y()));
		EWOL_VERBOSE("update size 4 " << m_sizeDisplayStart << " " << m_sizeDisplayStop);
	}
}

void ewol::compositing::TextBase::setRelPos(const vec3& _pos) {
	m_position += _pos;
	m_previousCharcode = 0;
	m_vectorialDraw.setPos(m_position);
}

void ewol::compositing::TextBase::setColorBg(const etk::Color<>& _color) {
	m_colorBg = _color;
	m_vectorialDraw.setColor(_color);
}

void ewol::compositing::TextBase::setClipping(const vec3& _pos, const vec3& _posEnd) {
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

void ewol::compositing::TextBase::setClippingMode(bool _newMode) {
	m_clippingEnable = _newMode;
	//m_vectorialDraw.setClippingMode(m_clippingEnable);
}

void ewol::compositing::TextBase::setFontBold(bool _status) {
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

void ewol::compositing::TextBase::setFontItalic(bool _status) {
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

void ewol::compositing::TextBase::setKerningMode(bool _newMode) {
	m_kerning = _newMode;
}

void ewol::compositing::TextBase::print(const std::u32string& _text) {
	std::vector<TextDecoration> decorationEmpty;
	print(_text, decorationEmpty);
}

void ewol::compositing::TextBase::print(const std::string& _text) {
	std::vector<TextDecoration> decorationEmpty;
	print(_text, decorationEmpty);
}


void ewol::compositing::TextBase::parseHtmlNode(const exml::Element& _element) {
	// get the static real pointer
	if (_element.exist() == false) {
		EWOL_ERROR( "Error Input node does not existed ...");
		return;
	}
	for(auto it : _element.nodes) {
		if (it.isComment() == true) {
			// nothing to do ...
			continue;
		} else if (it.isText() == true) {
			htmlAddData(etk::to_u32string(it.getValue()));
			EWOL_VERBOSE("XML add : " << it.getValue());
			continue;
		} else if (it.isElement() == false) {
			EWOL_ERROR("(l "<< it.getPos() << ") node not suported type : " << it.getType() << " val='"<< it.getValue() << "'" );
			continue;
		}
		exml::Element elem = it.toElement();
		if (elem.exist() == false) {
			EWOL_ERROR("Cast error ...");
			continue;
		}
		if(etk::compare_no_case(elem.getValue(), "br") == true) {
			htmlFlush();
			EWOL_VERBOSE("XML flush & newLine");
			forceLineReturn();
		} else if (etk::compare_no_case(elem.getValue(), "font") == true) {
			EWOL_VERBOSE("XML Font ...");
			TextDecoration tmpDeco = m_htmlDecoTmp;
			std::string colorValue = elem.attributes["color"];
			if (colorValue.size() != 0) {
				m_htmlDecoTmp.m_colorFg = colorValue;
			}
			colorValue = elem.attributes["colorBg"];
			if (colorValue.size() != 0) {
				m_htmlDecoTmp.m_colorBg = colorValue;
			}
			parseHtmlNode(elem);
			m_htmlDecoTmp = tmpDeco;
		} else if(    etk::compare_no_case(elem.getValue(), "b") == true
		           || etk::compare_no_case(elem.getValue(), "bold") == true) {
			EWOL_VERBOSE("XML bold ...");
			TextDecoration tmpDeco = m_htmlDecoTmp;
			if (m_htmlDecoTmp.m_mode == ewol::font::Regular) {
				m_htmlDecoTmp.m_mode = ewol::font::Bold;
			} else if (m_htmlDecoTmp.m_mode == ewol::font::Italic) {
				m_htmlDecoTmp.m_mode = ewol::font::BoldItalic;
			} 
			parseHtmlNode(elem);
			m_htmlDecoTmp = tmpDeco;
		} else if(    etk::compare_no_case(elem.getValue(), "i") == true
		           || etk::compare_no_case(elem.getValue(), "italic") == true) {
			EWOL_VERBOSE("XML italic ...");
			TextDecoration tmpDeco = m_htmlDecoTmp;
			if (m_htmlDecoTmp.m_mode == ewol::font::Regular) {
				m_htmlDecoTmp.m_mode = ewol::font::Italic;
			} else if (m_htmlDecoTmp.m_mode == ewol::font::Bold) {
				m_htmlDecoTmp.m_mode = ewol::font::BoldItalic;
			} 
			parseHtmlNode(elem);
			m_htmlDecoTmp = tmpDeco;
		} else if(    etk::compare_no_case(elem.getValue(), "u") == true
		           || etk::compare_no_case(elem.getValue(), "underline") == true) {
			EWOL_VERBOSE("XML underline ...");
			parseHtmlNode(elem);
		} else if(    etk::compare_no_case(elem.getValue(), "p") == true
		           || etk::compare_no_case(elem.getValue(), "paragraph") == true) {
			EWOL_VERBOSE("XML paragraph ...");
			htmlFlush();
			m_alignement = alignLeft;
			forceLineReturn();
			parseHtmlNode(elem);
			forceLineReturn();
		} else if (etk::compare_no_case(elem.getValue(), "center") == true) {
			EWOL_VERBOSE("XML center ...");
			htmlFlush();
			m_alignement = alignCenter;
			parseHtmlNode(elem);
		} else if (etk::compare_no_case(elem.getValue(), "left") == true) {
			EWOL_VERBOSE("XML left ...");
			htmlFlush();
			m_alignement = alignLeft;
			parseHtmlNode(elem);
		} else if (etk::compare_no_case(elem.getValue(), "right") == true) {
			EWOL_VERBOSE("XML right ...");
			htmlFlush();
			m_alignement = alignRight;
			parseHtmlNode(elem);
		} else if (etk::compare_no_case(elem.getValue(), "justify") == true) {
			EWOL_VERBOSE("XML justify ...");
			htmlFlush();
			m_alignement = alignJustify;
			parseHtmlNode(elem);
		} else {
			EWOL_ERROR("(l "<< elem.getPos() << ") node not suported type: " << elem.getType() << " val='"<< elem.getValue() << "'" );
		}
	}
}

void ewol::compositing::TextBase::printDecorated(const std::string& _text) {
	std::string tmpData("<html>\n<body>\n");
	tmpData += _text;
	tmpData += "\n</body>\n</html>\n";
	//EWOL_DEBUG("plop : " << tmpData);
	printHTML(tmpData);
}

void ewol::compositing::TextBase::printDecorated(const std::u32string& _text) {
	std::u32string tmpData(U"<html>\n<body>\n");
	tmpData += _text;
	tmpData += U"\n</body>\n</html>\n";
	//EWOL_DEBUG("plop : " << tmpData);
	printHTML(tmpData);
}

void ewol::compositing::TextBase::printHTML(const std::string& _text) {
	exml::Document doc;
	
	// reset parameter :
	m_htmlDecoTmp.m_colorBg = m_defaultColorBg;
	m_htmlDecoTmp.m_colorFg = m_defaultColorFg;
	m_htmlDecoTmp.m_mode = ewol::font::Regular;
	
	if (doc.parse(_text) == false) {
		EWOL_ERROR( "can not load XML: PARSING error: Decorated text ");
		return;
	}
	
	exml::Element root = doc.nodes["html"];
	if (root.exist() == false) {
		EWOL_ERROR( "can not load XML: main node not find: 'html'");
		doc.display();
		return;
	}
	exml::Element bodyNode = root.nodes["body"];
	if (root.exist() == false) {
		EWOL_ERROR( "can not load XML: main node not find: 'body'");
		return;
	}
	parseHtmlNode(bodyNode);
	htmlFlush();
}

void ewol::compositing::TextBase::printHTML(const std::u32string& _text) {
	exml::Document doc;
	
	// reset parameter :
	m_htmlDecoTmp.m_colorBg = m_defaultColorBg;
	m_htmlDecoTmp.m_colorFg = m_defaultColorFg;
	m_htmlDecoTmp.m_mode = ewol::font::Regular;
	// TODO : Create an instance of xml parser to manage std::u32string...
	if (doc.parse(etk::to_string(_text)) == false) {
		EWOL_ERROR( "can not load XML: PARSING error: Decorated text ");
		return;
	}
	
	exml::Element root = doc.nodes["html"];
	if (root.exist() == false) {
		EWOL_ERROR( "can not load XML: main node not find: 'html'");
		doc.display();
		return;
	}
	exml::Element bodyNode = root.nodes["body"];
	if (root.exist() == false) {
		EWOL_ERROR( "can not load XML: main node not find: 'body'");
		return;
	}
	parseHtmlNode(bodyNode);
	htmlFlush();
}

void ewol::compositing::TextBase::print(const std::string& _text, const std::vector<TextDecoration>& _decoration) {
	etk::Color<> tmpFg(m_color);
	etk::Color<> tmpBg(m_colorBg);
	if (m_alignement == alignDisable) {
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
				printChar(_text[iii]);
				float fontHeigh = getHeight();
				m_vectorialDraw.rectangleWidth(vec3(m_position.x()-pos.x(),fontHeigh,0.0f) );
				m_nbCharDisplayed++;
			} else {
				printChar(_text[iii]);
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
					if (m_needDisplay == true) {
						// Move the first char at the right :
						setPos(vec3(m_position.x() + freeSpace,
						            m_position.y(),
						            m_position.z()) );
					}
					break;
				case alignCenter:
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
				float fontHeigh = getHeight();
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
				if ((char32_t)_text[iii] == u32char::Space) {
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
						printChar(_text[iii]);
						m_vectorialDraw.rectangleWidth(vec3(m_position.x()-pos.x(),fontHeigh,0.0f) );
						m_nbCharDisplayed++;
					} else {
						printChar(_text[iii]);
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
			} else if((char32_t)_text[stop] == u32char::Space) {
				currentId = stop+1;
				// reset position :
				setPos(vec3(m_startTextpos,
				            (float)(m_position.y() - getHeight()),
				            m_position.z()) );
				m_nbCharDisplayed++;
			} else if((char32_t)_text[stop] == u32char::Return) {
				currentId = stop+1;
				// reset position :
				setPos(vec3(m_startTextpos,
				            (float)(m_position.y() - getHeight()),
				            m_position.z()) );
				m_nbCharDisplayed++;
			} else {
				currentId = stop;
			}
		}
		//EWOL_DEBUG(" 4 print in not alligned mode : start=" << m_sizeDisplayStart << " stop=" << m_sizeDisplayStop << " pos=" << m_position);
	}
}

void ewol::compositing::TextBase::print(const std::u32string& _text, const std::vector<TextDecoration>& _decoration) {
	etk::Color<> tmpFg(m_color);
	etk::Color<> tmpBg(m_colorBg);
	if (m_alignement == alignDisable) {
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
				printChar(_text[iii]);
				float fontHeigh = getHeight();
				m_vectorialDraw.rectangleWidth(vec3(m_position.x()-pos.x(),fontHeigh,0.0f) );
				m_nbCharDisplayed++;
			} else {
				printChar(_text[iii]);
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
					if (m_needDisplay == true) {
						// Move the first char at the right :
						setPos(vec3(m_position.x() + freeSpace,
						            m_position.y(),
						            m_position.z()) );
					}
					break;
				case alignCenter:
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
				float fontHeigh = getHeight();
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
				if ((char32_t)_text[iii] == u32char::Space) {
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
						printChar(_text[iii]);
						m_vectorialDraw.rectangleWidth(vec3(m_position.x()-pos.x(),fontHeigh,0.0f) );
						m_nbCharDisplayed++;
					} else {
						printChar(_text[iii]);
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
			} else if(_text[stop] == u32char::Space) {
				currentId = stop+1;
				// reset position :
				setPos(vec3(m_startTextpos,
				            (float)(m_position.y() - getHeight()),
				            m_position.z()) );
				m_nbCharDisplayed++;
			} else if(_text[stop] == u32char::Return) {
				currentId = stop+1;
				// reset position :
				setPos(vec3(m_startTextpos,
				            (float)(m_position.y() - getHeight()),
				            m_position.z()) );
				m_nbCharDisplayed++;
			} else {
				currentId = stop;
			}
		}
		//EWOL_DEBUG(" 4 print in not alligned mode : start=" << m_sizeDisplayStart << " stop=" << m_sizeDisplayStop << " pos=" << m_position);
	}
}




void ewol::compositing::TextBase::forceLineReturn() {
	// reset position : 
	setPos(vec3(m_startTextpos, m_position.y() - getHeight(), 0) );
}

void ewol::compositing::TextBase::setTextAlignement(float _startTextpos, float _stopTextPos, enum ewol::compositing::aligneMode _alignement) {
	m_startTextpos = _startTextpos;
	m_stopTextPos = _stopTextPos+1;
	m_alignement = _alignement;
	if (m_startTextpos >= m_stopTextPos) {
		EWOL_ERROR("Request allignement with Borne position error : " << _startTextpos << " => " << _stopTextPos);
	}
}

enum ewol::compositing::aligneMode ewol::compositing::TextBase::getAlignement() {
	return m_alignement;
}

void ewol::compositing::TextBase::disableAlignement() {
	m_alignement = alignDisable;
}

vec3 ewol::compositing::TextBase::calculateSizeHTML(const std::string& _text) {
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
	m_sizeDisplayStop.setValue(std::max(m_position.x(), m_sizeDisplayStop.x()) ,
	                           std::max(m_position.y(), m_sizeDisplayStop.y()) ,
	                           0);
	m_sizeDisplayStart.setValue(std::min(m_position.x(), m_sizeDisplayStart.x()) ,
	                            std::min(m_position.y(), m_sizeDisplayStart.y()) ,
	                            0);
	
	//EWOL_DEBUG("        2 Start pos=" << m_sizeDisplayStart);
	//EWOL_DEBUG("        2 Stop pos=" << m_sizeDisplayStop);
	// set back the display system
	m_needDisplay = true;
	
	return vec3( m_sizeDisplayStop.x()-m_sizeDisplayStart.x(),
	             m_sizeDisplayStop.y()-m_sizeDisplayStart.y(),
	             m_sizeDisplayStop.z()-m_sizeDisplayStart.z());
}

vec3 ewol::compositing::TextBase::calculateSizeHTML(const std::u32string& _text) {
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
	m_sizeDisplayStop.setValue(std::max(m_position.x(), m_sizeDisplayStop.x()) ,
	                           std::max(m_position.y(), m_sizeDisplayStop.y()) ,
	                           0);
	m_sizeDisplayStart.setValue(std::min(m_position.x(), m_sizeDisplayStart.x()) ,
	                            std::min(m_position.y(), m_sizeDisplayStart.y()) ,
	                            0);
	
	//EWOL_DEBUG("        2 Start pos=" << m_sizeDisplayStart);
	//EWOL_DEBUG("        2 Stop pos=" << m_sizeDisplayStop);
	// set back the display system
	m_needDisplay = true;
	
	return vec3( m_sizeDisplayStop.x()-m_sizeDisplayStart.x(),
	             m_sizeDisplayStop.y()-m_sizeDisplayStart.y(),
	             m_sizeDisplayStop.z()-m_sizeDisplayStart.z());
}

vec3 ewol::compositing::TextBase::calculateSizeDecorated(const std::string& _text) {
	if (_text.size() == 0) {
		return vec3(0,0,0);
	}
	std::string tmpData("<html><body>\n");
	tmpData+=_text;
	tmpData+="\n</body></html>\n";
	vec3 tmpVal = calculateSizeHTML(tmpData);
	return tmpVal;
}

vec3 ewol::compositing::TextBase::calculateSizeDecorated(const std::u32string& _text) {
	if (_text.size() == 0) {
		return vec3(0,0,0);
	}
	std::u32string tmpData(U"<html><body>\n");
	tmpData += _text;
	tmpData += U"\n</body></html>\n";
	vec3 tmpVal = calculateSizeHTML(tmpData);
	return tmpVal;
}

vec3 ewol::compositing::TextBase::calculateSize(const std::string& _text) {
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

vec3 ewol::compositing::TextBase::calculateSize(const std::u32string& _text) {
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

void ewol::compositing::TextBase::printCursor(bool _isInsertMode, float _cursorSize) {
	int32_t fontHeigh = getHeight();
	if (true == _isInsertMode) {
		m_vectorialDraw.rectangleWidth(vec3(_cursorSize, fontHeigh, 0) );
	} else {
		m_vectorialDraw.setThickness(2);
		m_vectorialDraw.lineRel( vec3(0, fontHeigh, 0) );
		m_vectorialDraw.setThickness(0);
	}
}

bool ewol::compositing::TextBase::extrapolateLastId(const std::string& _text,
                                                const int32_t _start,
                                                int32_t& _stop,
                                                int32_t& _space,
                                                int32_t& _freeSpace) {
	// store previous :
	char32_t storePrevious = m_previousCharcode;
	
	_stop = _text.size();
	_space = 0;
	
	int32_t lastSpacePosition = _start;
	int32_t lastSpacefreeSize = 0;
	
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
		if ((char32_t)_text[iii] == u32char::Space) {
			_space++;
			lastSpacePosition = iii;
			lastSpacefreeSize = stopPosition - endPos;
		} else if ((char32_t)_text[iii] == u32char::Return) {
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

bool ewol::compositing::TextBase::extrapolateLastId(const std::u32string& _text,
                                                const int32_t _start,
                                                int32_t& _stop,
                                                int32_t& _space,
                                                int32_t& _freeSpace) {
	// store previous :
	char32_t storePrevious = m_previousCharcode;
	
	_stop = _text.size();
	_space = 0;
	
	int32_t lastSpacePosition = _start;
	int32_t lastSpacefreeSize = 0;
	
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

void ewol::compositing::TextBase::htmlAddData(const std::u32string& _data) {
	if(    m_htmlCurrrentLine.size()>0
	    && m_htmlCurrrentLine[m_htmlCurrrentLine.size()-1] != ' ') {
		m_htmlCurrrentLine += U" ";
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

void ewol::compositing::TextBase::htmlFlush() {
	if (m_htmlCurrrentLine.size()>0) {
		print(m_htmlCurrrentLine, m_htmlDecoration);
	}
	m_htmlCurrrentLine = U"";
	m_htmlDecoration.clear();
}

void ewol::compositing::TextBase::disableCursor() {
	m_selectionStartPos = -100;
	m_cursorPos = -100;
}

void ewol::compositing::TextBase::setCursorPos(int32_t _cursorPos) {
	m_selectionStartPos = _cursorPos;
	m_cursorPos = _cursorPos;
}

void ewol::compositing::TextBase::setCursorSelection(int32_t _cursorPos, int32_t _selectionStartPos) {
	m_selectionStartPos = _selectionStartPos;
	m_cursorPos = _cursorPos;
}

void ewol::compositing::TextBase::setSelectionColor(const etk::Color<>& _color) {
	m_colorSelection = _color;
}

void ewol::compositing::TextBase::setCursorColor(const etk::Color<>& _color) {
	m_colorCursor = _color;
}
