/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */


#include <ewol/Debug.h>
#include <ewol/compositing/Text.h>
#include <ewol/font/FontManager.h>
#include <tinyXML/tinyxml.h>


/*
			// curent Drawing position
			etk::Vector3D<float> m_position;         //!< the next position to draw the text
			// clipping section
			etk::Vector3D<float> m_clippingPosition;
			etk::Vector3D<float> m_clippingSize;
			bool                 m_clippingEnable;
			// Basic color
			etk::Color           m_color;
*/

ewol::Text::Text(void) :
	m_position(0.0, 0.0, 0.0),
	m_clippingPosStart(0.0, 0.0, 0.0),
	m_clippingPosStop(0.0, 0.0, 0.0),
	m_clippingEnable(false),
	m_color(draw::color::black),
	m_colorBg(draw::color::none),
	m_mode(ewol::font::Regular),
	m_kerning(true),
	m_distanceField(false),
	m_previousCharcode(0),
	m_startTextpos(0),
	m_stopTextPos(0),
	m_alignement(ewol::Text::alignDisable),
	m_GLprogram(NULL),
	m_GLPosition(-1),
	m_GLMatrix(-1),
	m_GLColor(-1),
	m_GLtexture(-1),
	m_GLtexID(-1),
	m_font(NULL)
{
	SetFont(ewol::font::GetDefaultFont(), ewol::font::GetDefaultSize());
	LoadProgram();
}


ewol::Text::Text(etk::UString fontName, int32_t fontSize) :
	m_position(0.0, 0.0, 0.0),
	m_clippingPosStart(0.0, 0.0, 0.0),
	m_clippingPosStop(0.0, 0.0, 0.0),
	m_clippingEnable(false),
	m_color(draw::color::black),
	m_colorBg(draw::color::none),
	m_mode(ewol::font::Regular),
	m_kerning(true),
	m_distanceField(false),
	m_previousCharcode(0),
	m_startTextpos(0),
	m_stopTextPos(0),
	m_alignement(ewol::Text::alignDisable),
	m_GLprogram(NULL),
	m_GLPosition(-1),
	m_GLMatrix(-1),
	m_GLColor(-1),
	m_GLtexture(-1),
	m_GLtexID(-1),
	m_font(NULL)
{
	SetFont(fontName, fontSize);
	LoadProgram();
}


ewol::Text::~Text(void)
{
	
	if (NULL != m_font) {
		ewol::resource::Release(m_font);
		m_font = NULL;
	}
	ewol::resource::Release(m_GLprogram);
}

void ewol::Text::LoadProgram(void)
{
	etk::UString tmpString("DATA:text.prog");
	// get the shader resource :
	m_GLPosition = 0;
	if (true == ewol::resource::Keep(tmpString, m_GLprogram) ) {
		m_GLPosition = m_GLprogram->GetAttribute("EW_coord2d");
		m_GLColor    = m_GLprogram->GetAttribute("EW_color");
		m_GLtexture  = m_GLprogram->GetAttribute("EW_texture2d");
		m_GLMatrix   = m_GLprogram->GetUniform("EW_MatrixTransformation");
		m_GLtexID    = m_GLprogram->GetUniform("EW_texID");
	}
}

void ewol::Text::Draw(void)
{
	if (m_coord.Size()<=0 || NULL == m_font) {
		// TODO : a remètre ...
		//EWOL_WARNING("Nothink to draw...");
		return;
	}
	if (m_font == NULL) {
		EWOL_WARNING("no font...");
		return;
	}
	if (m_GLprogram==NULL) {
		EWOL_ERROR("No shader ...");
		return;
	}
	m_vectorialDraw.Draw();
	// set Matrix : translation/positionMatrix
	etk::Matrix4 tmpMatrix = ewol::openGL::GetMatrix()*m_matrixApply;
	m_GLprogram->Use(); 
	m_GLprogram->UniformMatrix4fv(m_GLMatrix, 1, tmpMatrix.m_mat);
	// TextureID
	m_GLprogram->SetTexture0(m_GLtexID, m_font->GetId());
	// position :
	m_GLprogram->SendAttribute(m_GLPosition, 2/*x,y*/, &m_coord[0]);
	// Texture :
	m_GLprogram->SendAttribute(m_GLtexture, 2/*u,v*/, &m_coordTex[0]);
	// color :
	m_GLprogram->SendAttribute(m_GLColor, 4/*r,g,b,a*/, &m_coordColor[0]);
	// Request the draw od the elements : 
	glDrawArrays(GL_TRIANGLES, 0, m_coord.Size());
	m_GLprogram->UnUse();
}

void ewol::Text::Translate(etk::Vector3D<float> vect)
{
	ewol::Compositing::Translate(vect);
	m_vectorialDraw.Translate(vect);
}


void ewol::Text::Rotate(etk::Vector3D<float> vect, float angle)
{
	ewol::Compositing::Rotate(vect, angle);
	m_vectorialDraw.Rotate(vect, angle);
}


void ewol::Text::Scale(etk::Vector3D<float> vect)
{
	ewol::Compositing::Scale(vect);
	m_vectorialDraw.Scale(vect);
}


void ewol::Text::Clear(void)
{
	// call upper class
	ewol::Compositing::Clear();
	// remove sub draw system
	m_vectorialDraw.Clear();
	// Reset Buffer :
	m_coord.Clear();
	m_coordTex.Clear();
	m_coordColor.Clear();
	// Reset temporal variables :
	m_position = etk::Vector3D<float>(0.0, 0.0, 0.0);
	m_clippingPosStart = etk::Vector3D<float>(0.0, 0.0, 0.0);
	m_clippingPosStop = etk::Vector3D<float>(0.0, 0.0, 0.0);
	m_clippingEnable = false;
	m_color = draw::color::black;
	m_colorBg = draw::color::none;
	m_mode = ewol::font::Regular;
	m_kerning = false;
	m_previousCharcode = 0;
	m_startTextpos = 0;
	m_stopTextPos = 0;
	m_alignement = ewol::Text::alignDisable;
	m_htmlCurrrentLine = "";
	m_htmlDecoration.Clear();
}


etk::Vector3D<float> ewol::Text::GetPos(void)
{
	return m_position;
}


void ewol::Text::SetPos(etk::Vector3D<float> pos)
{
	m_position = pos;
	m_previousCharcode = 0;
	m_vectorialDraw.SetPos(m_position);
}


void ewol::Text::SetRelPos(etk::Vector3D<float> pos)
{
	m_position += pos;
	m_previousCharcode = 0;
	m_vectorialDraw.SetPos(m_position);
}


void ewol::Text::SetColor(draw::Color color)
{
	m_color = color;
}


void ewol::Text::SetColorBg(draw::Color color)
{
	m_colorBg = color;
	m_vectorialDraw.SetColor(color);
}


void ewol::Text::SetClippingWidth(etk::Vector3D<float> pos, etk::Vector3D<float> width)
{
	SetClipping(pos, pos+width);
}

void ewol::Text::SetClipping(etk::Vector3D<float> pos, etk::Vector3D<float> posEnd)
{
	// note the internal system all time request to have a bounding all time in the same order
	if (pos.x <= posEnd.x) {
		m_clippingPosStart.x = pos.x;
		m_clippingPosStop.x = posEnd.x;
	} else {
		m_clippingPosStart.x = posEnd.x;
		m_clippingPosStop.x = pos.x;
	}
	if (pos.y <= posEnd.y) {
		m_clippingPosStart.y = pos.y;
		m_clippingPosStop.y = posEnd.y;
	} else {
		m_clippingPosStart.y = posEnd.y;
		m_clippingPosStop.y = pos.y;
	}
	if (pos.z <= posEnd.z) {
		m_clippingPosStart.z = pos.z;
		m_clippingPosStop.z = posEnd.z;
	} else {
		m_clippingPosStart.z = posEnd.z;
		m_clippingPosStop.z = pos.z;
	}
	m_clippingEnable = true;
	//m_vectorialDraw.SetClipping(m_clippingPosStart, m_clippingPosStop);
}


void ewol::Text::SetClippingMode(bool newMode)
{
	m_clippingEnable = newMode;
	//m_vectorialDraw.SetClippingMode(m_clippingEnable);
}


void ewol::Text::SetFontSize(int32_t fontSize)
{
	// get old size
	etk::UString fontName = ewol::font::GetDefaultFont();
	if (NULL != m_font) {
		fontName = m_font->GetName();
	}
	SetFont(fontName, fontSize);
}


void ewol::Text::SetFontName(etk::UString fontName)
{
	// get old size
	int32_t fontSize = ewol::font::GetDefaultSize();
	if (NULL != m_font) {
		fontSize = m_font->GetFontSize();
	}
	SetFont(fontName, fontSize);
}


void ewol::Text::SetFont(etk::UString fontName, int32_t fontSize)
{
	// remove old one
	if (NULL != m_font) {
		ewol::resource::Release(m_font);
		m_font = NULL;
	}
	etk::UString tmpName = fontName;
	tmpName += ":";
	tmpName += fontSize;
	// link to new One
	if (false == ewol::resource::Keep(tmpName, m_font)) {
		EWOL_ERROR("Can not get font resource");
	}
}


void ewol::Text::SetFontMode(ewol::font::mode_te mode)
{
	if (NULL!=m_font) {
		m_mode = m_font->GetWrappingMode(mode);
	}
}


ewol::font::mode_te ewol::Text::GetFontMode(void)
{
	return m_mode;
}


void ewol::Text::SetFontBold(bool status)
{
	if (true == status) {
		// enable
		if (m_mode == ewol::font::Regular) {
			SetFontMode(ewol::font::Bold);
		} else if (m_mode == ewol::font::Italic) {
			SetFontMode(ewol::font::BoldItalic);
		}
	} else {
		// disable
		if (m_mode == ewol::font::Bold) {
			SetFontMode(ewol::font::Regular);
		} else if (m_mode == ewol::font::BoldItalic) {
			SetFontMode(ewol::font::Italic);
		}
	}
}

void ewol::Text::SetFontItalic(bool status)
{
	if (true == status) {
		// enable
		if (m_mode == ewol::font::Regular) {
			SetFontMode(ewol::font::Italic);
		} else if (m_mode == ewol::font::Bold) {
			SetFontMode(ewol::font::BoldItalic);
		}
	} else {
		// disable
		if (m_mode == ewol::font::Italic) {
			SetFontMode(ewol::font::Regular);
		} else if (m_mode == ewol::font::BoldItalic) {
			SetFontMode(ewol::font::Bold);
		}
	}
}


void ewol::Text::SetKerningMode(bool newMode)
{
	m_kerning = newMode;
}


void ewol::Text::SetDistanceFieldMode(bool newMode)
{
	m_distanceField = newMode;
	EWOL_TODO("The Distance field mode is not availlable for now ...");
}


void ewol::Text::Print(const etk::UString& text)
{
	etk::Vector<TextDecoration> decorationEmpty;
	Print(text, decorationEmpty);
}


void ewol::Text::ParseHtmlNode(void* element2)
{
	TiXmlNode* element = static_cast<TiXmlNode*>(element2);
	if (NULL != element) {
		/*const char *myData = element->ToElement()->GetText();
		if (NULL != myData) {
			etk::UString outputData(myData);
			Print(myData);
		} else {
		*/
			for (TiXmlNode * child = element->FirstChild(); NULL != child ; child = child->NextSibling() ) {
				if (child->Type()==TiXmlNode::TINYXML_COMMENT) {
					// nothing to do ...
				} else if (child->Type()==TiXmlNode::TINYXML_TEXT) {
					HtmlAddData(child->Value() );
				} else if (!strcmp(child->Value(), "br")) {
					HtmlFlush();
					ForceLineReturn();
				} else if (!strcmp(child->Value(), "font")) {
					TextDecoration tmpDeco = m_htmlDecoTmp;
					const char *colorValue = child->ToElement()->Attribute("color");
					if (NULL != colorValue) {
						draw::ParseColor(colorValue, m_htmlDecoTmp.m_colorFg);
					}
					colorValue = child->ToElement()->Attribute("colorBg");
					if (NULL != colorValue) {
						draw::ParseColor(colorValue, m_htmlDecoTmp.m_colorBg);
					}
					ParseHtmlNode(child);
					m_htmlDecoTmp = tmpDeco;
				} else if (!strcmp(child->Value(), "b")) {
					TextDecoration tmpDeco = m_htmlDecoTmp;
					if (m_htmlDecoTmp.m_mode == ewol::font::Regular) {
						m_htmlDecoTmp.m_mode = ewol::font::Bold;
					} else if (m_htmlDecoTmp.m_mode == ewol::font::Italic) {
						m_htmlDecoTmp.m_mode = ewol::font::BoldItalic;
					} 
					ParseHtmlNode(child);
					m_htmlDecoTmp = tmpDeco;
				} else if (!strcmp(child->Value(), "i")) {
					TextDecoration tmpDeco = m_htmlDecoTmp;
					if (m_htmlDecoTmp.m_mode == ewol::font::Regular) {
						m_htmlDecoTmp.m_mode = ewol::font::Italic;
					} else if (m_htmlDecoTmp.m_mode == ewol::font::Bold) {
						m_htmlDecoTmp.m_mode = ewol::font::BoldItalic;
					} 
					ParseHtmlNode(child);
					m_htmlDecoTmp = tmpDeco;
				} else if (!strcmp(child->Value(), "u")) {
					ParseHtmlNode(child);
				} else if (!strcmp(child->Value(), "p")) {
					HtmlFlush();
					m_alignement = ewol::Text::alignLeft;
					ForceLineReturn();
					ParseHtmlNode(child);
					ForceLineReturn();
				} else if (!strcmp(child->Value(), "center")) {
					HtmlFlush();
					m_alignement = ewol::Text::alignCenter;
					ParseHtmlNode(child);
				} else if (!strcmp(child->Value(), "left")) {
					HtmlFlush();
					m_alignement = ewol::Text::alignLeft;
					ParseHtmlNode(child);
				} else if (!strcmp(child->Value(), "right")) {
					HtmlFlush();
					m_alignement = ewol::Text::alignRight;
					ParseHtmlNode(child);
				} else if (!strcmp(child->Value(), "justify")) {
					HtmlFlush();
					m_alignement = ewol::Text::alignJustify;
					ParseHtmlNode(child);
				} else {
					EWOL_ERROR("(l "<< child->Row() << ") node not suported type : " << child->Type() << " val=\""<< child->Value() << "\"" );
				}
			//}
		}
	}
}

void ewol::Text::PrintDecorated(etk::UString& text)
{
	TiXmlDocument XmlDocument;
	
	// reset parameter :
	m_htmlDecoTmp.m_colorBg = draw::color::none;
	m_htmlDecoTmp.m_colorFg = draw::color::black;
	m_htmlDecoTmp.m_mode = ewol::font::Regular;
	
	
	// load the XML from the memory
	bool loadError = XmlDocument.Parse((const char*)text.c_str(), 0, TIXML_ENCODING_UTF8);
	if (false == loadError) {
		EWOL_ERROR( "can not load Hightlight XML: PARSING error: Decorated text ");
		return;
	}

	TiXmlElement* root = XmlDocument.FirstChildElement( "html" );
	if (NULL == root) {
		EWOL_ERROR( "can not load Hightlight XML: main node not find: \"html\"");
		return;
	}
	TiXmlElement* bodyNode = root->FirstChildElement( "body" );
	ParseHtmlNode(bodyNode);
	HtmlFlush();
}


void ewol::Text::Print(const etk::UString& text, const etk::Vector<TextDecoration>& decoration)
{
	if (m_font == NULL) {
		EWOL_ERROR("Font Id is not corectly defined");
		return;
	}
	if (m_alignement == ewol::Text::alignDisable) {
		// note this is faster when nothing is requested ...
		for(int32_t iii=0; iii<text.Size(); iii++) {
			if (iii<decoration.Size()) {
				SetColor(decoration[iii].m_colorFg);
				SetColorBg(decoration[iii].m_colorBg);
				SetFontMode(decoration[iii].m_mode);
			}
			if (m_colorBg.a != 0) {
				etk::Vector3D<float> pos = m_position;
				m_vectorialDraw.SetPos(pos);
				Print(text[iii]);
				float fontHeigh = m_font->GetHeight(m_mode);
				m_vectorialDraw.RectangleWidth(etk::Vector3D<float>(m_position.x-pos.x,fontHeigh,0.0f) );
			} else {
				Print(text[iii]);
			}
		}
	} else {
		// special start case at the right of the endpoint :
		if (m_stopTextPos <= m_position.x) {
			ForceLineReturn();
		}
		float basicSpaceWidth = CalculateSize(' ').x;
		int32_t currentId = 0;
		int32_t stop;
		int32_t space;
		int32_t freeSpace;
		while (currentId < text.Size()) {
			bool needNoJustify = ExtrapolateLastId(text, currentId, stop, space, freeSpace);
			
			float interpolation = basicSpaceWidth;
			switch (m_alignement)
			{
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
					// Move the first char at the right :
					SetPos(etk::Vector3D<float>(m_position.x + freeSpace,
					                            m_position.y,
					                            m_position.z) );
					break;
				case ewol::Text::alignCenter:
					// Move the first char at the right :
					SetPos(etk::Vector3D<float>(m_position.x + freeSpace/2,
					                            m_position.y,
					                            m_position.z) );
					break;
			}
			// display all the elements
			for(int32_t iii=currentId; iii<stop && iii<text.Size(); iii++) {
				float fontHeigh = m_font->GetHeight(m_mode);
				// Get specific decoration if provided
				if (iii<decoration.Size()) {
					SetColor(decoration[iii].m_colorFg);
					SetColorBg(decoration[iii].m_colorBg);
					SetFontMode(decoration[iii].m_mode);
				}
				// special for the justify mode
				if (text[iii] == (uniChar_t)' ') {
					if (m_colorBg.a != 0) {
						m_vectorialDraw.SetPos(m_position);
					}
					// Must generate a dynamic space : 
					SetPos(etk::Vector3D<float>(m_position.x + interpolation,
					                            m_position.y,
					                            m_position.z) );
					if (m_colorBg.a != 0) {
						m_vectorialDraw.RectangleWidth(etk::Vector3D<float>(interpolation,fontHeigh,0.0f) );
					}
				} else {
					if (m_colorBg.a != 0) {
						etk::Vector3D<float> pos = m_position;
						m_vectorialDraw.SetPos(pos);
						Print(text[iii]);
						m_vectorialDraw.RectangleWidth(etk::Vector3D<float>(m_position.x-pos.x,fontHeigh,0.0f) );
					} else {
						Print(text[iii]);
					}
				}
			}
			if (currentId == stop) {
				currentId++;
			} else if(text[stop] == (uniChar_t)' ') {
				currentId = stop+1;
				// Reset position : 
				SetPos(etk::Vector3D<float>(m_startTextpos,
				                            (float)(m_position.y - m_font->GetHeight(m_mode)),
				                            m_position.z) );
			} else if(text[stop] == (uniChar_t)'\n') {
				currentId = stop+1;
				// Reset position : 
				SetPos(etk::Vector3D<float>(m_startTextpos,
				                            (float)(m_position.y - m_font->GetHeight(m_mode)),
				                            m_position.z) );
			} else {
				currentId = stop;
			}
		}
	}
}


void ewol::Text::Print(const uniChar_t charcode)
{
	if (m_font == NULL) {
		EWOL_ERROR("Font Id is not corectly defined");
		return;
	}
	// get a pointer on the glyph property : 
	ewol::GlyphProperty * myGlyph = m_font->GetGlyphPointer(charcode, m_mode);
	int32_t fontSize = m_font->GetFontSize();
	int32_t fontHeigh = m_font->GetHeight(m_mode);
	
	// Get the kerning ofset :
	float kerningOffset = 0.0;
	if (true==m_kerning) {
		kerningOffset = myGlyph->KerningGet(m_previousCharcode);
		if (kerningOffset != 0) {
			//EWOL_DEBUG("Kerning between : '" << (char)m_previousCharcode << "'&'" << (char)myGlyph->m_UVal << "' value : " << kerningOffset);
		}
	}
	// 0x01 == 0x20 == ' ';
	if (charcode != 0x01) {
		/* Bitmap position
		 *      xA     xB
		 *   yC *------*
		 *      |      |
		 *      |      |
		 *   yD *------*
		 */
		float dxA = m_position.x + myGlyph->m_bearing.x + kerningOffset;
		float dxB = dxA + myGlyph->m_sizeTexture.x;
		float dyC = m_position.y + myGlyph->m_bearing.y + fontHeigh - fontSize;
		float dyD = dyC - myGlyph->m_sizeTexture.y;
		
		float tuA = myGlyph->m_texturePosStart.u;
		float tuB = myGlyph->m_texturePosStop.u;
		float tvC = myGlyph->m_texturePosStart.v;
		float tvD = myGlyph->m_texturePosStop.v;
		
		
		// Clipping and drawing area
		if(     true == m_clippingEnable
		    && (    dxB < m_clippingPosStart.x
		         || dxA > m_clippingPosStop.x
		         || dyC < m_clippingPosStart.y
		         || dyD > m_clippingPosStop.y ) ) {
			// Nothing to diplay ...
		} else {
			if (true == m_clippingEnable) {
				// generata positions...
				float TexSizeX = tuB - tuA;
				if (dxA < m_clippingPosStart.x) {
					// clip display
					float drawSize = m_clippingPosStart.x - dxA;
					// Update element start display
					dxA = m_clippingPosStart.x;
					float addElement = TexSizeX * drawSize / (float)myGlyph->m_sizeTexture.x;
					// update texture start X Pos
					tuA += addElement;
				}
				if (dxB > m_clippingPosStop.x) {
					// clip display
					float drawSize = dxB - m_clippingPosStop.x;
					// Update element start display
					dxB = m_clippingPosStop.x;
					float addElement = TexSizeX * drawSize / (float)myGlyph->m_sizeTexture.x;
					// update texture start X Pos
					tuB -= addElement;
				}
				float TexSizeY = tvC - tvD;
				if (dyC > m_clippingPosStop.y) {
					// clip display
					float drawSize = dyC - m_clippingPosStop.y;
					// Update element start display
					dyC = m_clippingPosStop.y;
					float addElement = TexSizeY * drawSize / (float)myGlyph->m_sizeTexture.y;
					// update texture start X Pos
					tvC -= addElement;
				}
				if (dyD < m_clippingPosStart.y) {
					// clip display
					float drawSize = m_clippingPosStart.y - dyD;
					// Update element start display
					dyD = m_clippingPosStart.y;
					float addElement = TexSizeY * drawSize / (float)myGlyph->m_sizeTexture.y;
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
				etk::Vector2D<int32_t> bitmapDrawPos[4];
				bitmapDrawPos[0].x = (int32_t)dxA;
				bitmapDrawPos[1].x = (int32_t)dxB;
				bitmapDrawPos[2].x = (int32_t)dxB;
				bitmapDrawPos[3].x = (int32_t)dxA;
				
				bitmapDrawPos[0].y = (int32_t)dyC;
				bitmapDrawPos[1].y = (int32_t)dyC;
				bitmapDrawPos[2].y = (int32_t)dyD;
				bitmapDrawPos[3].y = (int32_t)dyD;
				/* texture Position : 
				 *   0------1
				 *   |      |
				 *   |      |
				 *   3------2
				 */
				texCoord_ts texturePos[4];
				texturePos[0].u = tuA+m_mode;
				texturePos[1].u = tuB+m_mode;
				texturePos[2].u = tuB+m_mode;
				texturePos[3].u = tuA+m_mode;
				
				texturePos[0].v = tvC;
				texturePos[1].v = tvC;
				texturePos[2].v = tvD;
				texturePos[3].v = tvD;
				
				// NOTE : Android does not support the Quads elements ...
				/* Step 1 : 
				 *   ********     
				 *     ******     
				 *       ****     
				 *         **     
				 *                
				 */
				// set texture coordonates :
				m_coordTex.PushBack(texturePos[0]);
				m_coordTex.PushBack(texturePos[1]);
				m_coordTex.PushBack(texturePos[2]);
				// set display positions :
				m_coord.PushBack(bitmapDrawPos[0]);
				m_coord.PushBack(bitmapDrawPos[1]);
				m_coord.PushBack(bitmapDrawPos[2]);
				// set the color
				m_coordColor.PushBack(m_color);
				m_coordColor.PushBack(m_color);
				m_coordColor.PushBack(m_color);
				/* Step 2 : 
				 *              
				 *   **         
				 *   ****       
				 *   ******     
				 *   ********   
				 */
				// set texture coordonates :
				m_coordTex.PushBack(texturePos[0]);
				m_coordTex.PushBack(texturePos[2]);
				m_coordTex.PushBack(texturePos[3]);
				// set display positions :
				m_coord.PushBack(bitmapDrawPos[0]);
				m_coord.PushBack(bitmapDrawPos[2]);
				m_coord.PushBack(bitmapDrawPos[3]);
				// set the color
				m_coordColor.PushBack(m_color);
				m_coordColor.PushBack(m_color);
				m_coordColor.PushBack(m_color);
			}
		}
	}
	// move the position :
	m_position.x += myGlyph->m_advance.x + kerningOffset;
	// Register the previous character
	m_previousCharcode = charcode;
	return;
}

void ewol::Text::ForceLineReturn(void)
{
	// Reset position : 
	SetPos(etk::Vector3D<float>(m_startTextpos,
	                            (float)(m_position.y - m_font->GetHeight(m_mode)),
	                            m_position.z) );
}


void ewol::Text::SetTextAlignement(float startTextpos, float stopTextPos, ewol::Text::aligneMode_te alignement)
{
	m_startTextpos = startTextpos;
	m_stopTextPos = stopTextPos;
	m_alignement = alignement;
	if (m_startTextpos >= m_stopTextPos) {
		EWOL_ERROR("Request allignement with Borne position error : " << startTextpos << " => " << stopTextPos);
	}
}


ewol::Text::aligneMode_te ewol::Text::GetAlignement(void)
{
	return m_alignement;
}


void ewol::Text::DisableAlignement(void)
{
	m_alignement = ewol::Text::alignDisable;
}

etk::Vector3D<float> ewol::Text::CalculateSize(const etk::UString& text)
{
	if (m_font == NULL) {
		EWOL_ERROR("Font Id is not corectly defined");
		return etk::Vector3D<float>(0,0,0);
	}
	etk::Vector3D<float> outputSize(0, 0, 0);
	for(int32_t iii=0; iii<text.Size(); iii++) {
		etk::Vector3D<float> tmpp = CalculateSize(text[iii]);
		if (outputSize.y == 0) {
			outputSize.y += tmpp.y;
		}
		outputSize.x += tmpp.x;
	}
	return outputSize;
}

etk::Vector3D<float> ewol::Text::CalculateSize(const uniChar_t charcode)
{
	if (m_font == NULL) {
		EWOL_ERROR("Font Id is not corectly defined");
		return etk::Vector3D<float>(0,0,0);
	}
	// get a pointer on the glyph property : 
	ewol::GlyphProperty * myGlyph = m_font->GetGlyphPointer(charcode, m_mode);
	int32_t fontHeigh = m_font->GetHeight(m_mode);
	
	// Get the kerning ofset :
	float kerningOffset = 0.0;
	if (true==m_kerning) {
		kerningOffset = myGlyph->KerningGet(m_previousCharcode);
	}
	
	etk::Vector3D<float> outputSize((float)(myGlyph->m_advance.x + kerningOffset),
	                                (float)(fontHeigh),
	                                (float)(0.0));
	// Register the previous character
	m_previousCharcode = charcode;
	return outputSize;
}


void ewol::Text::PrintCursor(bool isInsertMode)
{
	int32_t fontHeigh = m_font->GetHeight(m_mode);
	if (true == isInsertMode) {
		m_vectorialDraw.RectangleWidth(etk::Vector3D<float>(20, fontHeigh, 0) );
	} else {
		m_vectorialDraw.SetThickness(2);
		m_vectorialDraw.LineRel( etk::Vector3D<float>(0, fontHeigh, 0) );
		m_vectorialDraw.SetThickness(0);
	}
}


bool ewol::Text::ExtrapolateLastId(const etk::UString& text, const int32_t start, int32_t& stop, int32_t& space, int32_t& freeSpace)
{
	// store previous :
	uniChar_t storePrevious = m_previousCharcode;
	
	stop = text.Size();
	space = 0;
	
	int32_t lastSpacePosition = start;
	int32_t lastSpacefreeSize;
	
	float endPos = m_position.x;
	bool endOfLine = false;
	for (int32_t iii=start; iii<text.Size(); iii++) {
		etk::Vector3D<float> tmpSize = CalculateSize(text[iii]);
		// check oveflow :
		if (endPos + tmpSize.x > m_stopTextPos) {
			stop = iii;
			break;
		}
		// save number of space :
		if (text[iii] == (uniChar_t)' ') {
			space++;
			lastSpacePosition = iii;
			lastSpacefreeSize = m_stopTextPos - endPos;
		} else if (text[iii] == (uniChar_t)'\n') {
			stop = iii;
			endOfLine = true;
			break;
		}
		// update local size :
		endPos += tmpSize.x;
	}
	freeSpace = m_stopTextPos - endPos;
	// retore previous :
	m_previousCharcode = storePrevious;
	// need to align left or right ...
	if(stop == text.Size()) {
		return true;
	} else {
		if (endOfLine) {
			return true;
		} else {
			if (space == 0) {
				return true;
			}
			stop = lastSpacePosition;
			freeSpace = lastSpacefreeSize;
			return false;
		}
	}
}

void ewol::Text::HtmlAddData(etk::UString data)
{
	if(    m_htmlCurrrentLine.Size()>0
	    && m_htmlCurrrentLine[m_htmlCurrrentLine.Size()-1] != ' ') {
		m_htmlCurrrentLine+=" ";
		if(m_htmlDecoration.Size()>0) {
			TextDecoration tmp = m_htmlDecoration[m_htmlDecoration.Size()-1];
			m_htmlDecoration.PushBack(tmp);
		} else {
			m_htmlDecoration.PushBack(m_htmlDecoTmp);
		}
	}
	m_htmlCurrrentLine += data;
	for(int32_t iii=0; iii<data.Size() ; iii++) {
		m_htmlDecoration.PushBack(m_htmlDecoTmp);
	}
}


void ewol::Text::HtmlFlush(void)
{
	if (m_htmlCurrrentLine.Size()>0) {
		Print(m_htmlCurrrentLine, m_htmlDecoration);
	}
	m_htmlCurrrentLine = "";
	m_htmlDecoration.Clear();
}
