/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#include <etk/unicode.h>
#include <ewol/widget/Entry.h>
#include <ewol/widget/WidgetManager.h>
#include <ewol/ewol.h>


extern const char * const ewolEventEntryClick      = "ewol-Entry-click";
extern const char * const ewolEventEntryEnter      = "ewol-Entry-Enter";
extern const char * const ewolEventEntryModify     = "ewol-Entry-Modify";

const char * const ewolEventEntryCut               = "ewol-Entry-Cut";
const char * const ewolEventEntryCopy              = "ewol-Entry-Copy";
const char * const ewolEventEntryPaste             = "ewol-Entry-Paste";
const char * const ewolEventEntryClean             = "ewol-Entry-Clean";
const char * const ewolEventEntrySelect            = "ewol-Entry-Select";


#undef __class__
#define __class__	"Entry"


void widget::Entry::Init(void)
{
	AddEventId(ewolEventEntryClick);
	AddEventId(ewolEventEntryEnter);
	AddEventId(ewolEventEntryModify);
	m_displayStartPosition = 0;
	m_displayCursorPos = 0;
	m_displayCursorPosSelection = 0;
	m_userSize = 50;
	m_borderSize = 2;
	m_paddingSize = 3;
	m_displayCursor = false;
	m_textColorFg = draw::color::black;
	
	m_textColorBg = draw::color::white;
	m_textColorBg.a = 0xAF;
	SetCanHaveFocus(true);
	ShortCutAdd("ctrl+w",       ewolEventEntryClean);
	ShortCutAdd("ctrl+x",       ewolEventEntryCut);
	ShortCutAdd("ctrl+c",       ewolEventEntryCopy);
	ShortCutAdd("ctrl+v",       ewolEventEntryPaste);
	ShortCutAdd("ctrl+a",       ewolEventEntrySelect, "ALL");
	ShortCutAdd("ctrl+shift+a", ewolEventEntrySelect, "NONE");
	etk::UString tmpString("THEME:GUI:widgetEntry.prog");
	// get the shader resource :
	m_GLPosition = 0;
	if (true == ewol::resource::Keep(tmpString, m_GLprogram) ) {
		m_GLPosition     = m_GLprogram->GetAttribute("EW_coord2d");
		m_GLMatrix       = m_GLprogram->GetUniform("EW_MatrixTransformation");
		m_GLsizeBorder   = m_GLprogram->GetUniform("EW_sizeBorder");
		m_GLsizePadding  = m_GLprogram->GetUniform("EW_sizePadding");
		m_GLsize         = m_GLprogram->GetUniform("EW_size");
		m_GLposText      = m_GLprogram->GetUniform("EW_posText");
		m_GLstate        = m_GLprogram->GetUniform("EW_state");
	}
}

widget::Entry::Entry(void)
{
	Init();
	m_data = "";
	UpdateTextPosition();
	MarkToRedraw();
}

widget::Entry::Entry(etk::UString newData)
{
	Init();
	SetValue(newData);
	UpdateTextPosition();
	MarkToRedraw();
}


widget::Entry::~Entry(void)
{
	
}


bool widget::Entry::CalculateMinSize(void)
{
	int32_t minHeight = m_oObjectText.CalculateSize('A').y;
	m_minSize.x = m_userSize;
	m_minSize.y = minHeight + 2*(m_borderSize + 2*m_paddingSize);
	UpdateTextPosition();
	MarkToRedraw();
	return true;
}


void widget::Entry::SetValue(etk::UString newData)
{
	m_data = newData;
	m_displayCursorPos = m_data.Size();
	m_displayCursorPosSelection = m_displayCursorPos;
	EWOL_DEBUG("Set ... " << newData);
	MarkToRedraw();
}

etk::UString widget::Entry::GetValue(void)
{
	return m_data;
}



void widget::Entry::SetPoint(float x, float y)
{
	etk::Vector2D<float> triangle(x, y);
	m_coord.PushBack(triangle);
}

void widget::Entry::Rectangle(float x, float y, float w, float h)
{
	m_coord.Clear();
	/* Bitmap position
	 *      xA     xB
	 *   yC *------*
	 *      |      |
	 *      |      |
	 *   yD *------*
	 */
	float dxA = x;
	float dxB = x + w;
	float dyC = y;
	float dyD = y + h;
	SetPoint(dxA, dyD);
	SetPoint(dxA, dyC);
	SetPoint(dxB, dyC);

	SetPoint(dxB, dyC);
	SetPoint(dxB, dyD);
	SetPoint(dxA, dyD);
}


void widget::Entry::OnDraw(ewol::DrawProperty& displayProp)
{
	if (m_GLprogram==NULL) {
		EWOL_ERROR("No shader ...");
		return;
	}
	//glScalef(m_scaling.x, m_scaling.y, 1.0);
	m_GLprogram->Use();
	// set Matrix : translation/positionMatrix
	etk::Matrix4 tmpMatrix = ewol::openGL::GetMatrix();
	m_GLprogram->UniformMatrix4fv(m_GLMatrix, 1, tmpMatrix.m_mat);
	// position :
	m_GLprogram->SendAttribute(m_GLPosition, 2/*x,y*/, &m_coord[0]);
	// all entry parameters :
	m_GLprogram->Uniform1f(m_GLsizeBorder, m_borderSize);
	m_GLprogram->Uniform1f(m_GLsizePadding, m_paddingSize);
	m_GLprogram->Uniform2fv(m_GLsize, 1, &m_size.x);
	m_GLprogram->Uniform4fv(m_GLposText, 1, m_pos);
	m_GLprogram->Uniform1i(m_GLstate, 0);
	// Request the draw of the elements : 
	glDrawArrays(GL_TRIANGLES, 0, m_coord.Size());
	m_GLprogram->UnUse();
	m_oObjectDecoration.Draw();
	m_oObjectText.Draw();
}


void widget::Entry::OnRegenerateDisplay(void)
{
	if (true == NeedRedraw()) {
		m_oObjectDecoration.Clear();
		m_oObjectText.Clear();
		UpdateTextPosition();
		
		int32_t tmpSizeX = m_minSize.x;
		int32_t tmpSizeY = m_minSize.y;
		int32_t tmpOriginX = 0;
		int32_t tmpOriginY = (m_size.y - tmpSizeY) / 2;
		// no change for the text orogin : 
		int32_t tmpTextOriginX = m_borderSize + 2*m_paddingSize;
		int32_t tmpTextOriginY = tmpOriginY + m_borderSize + 2*m_paddingSize;
		
		if (true==m_userFill.x) {
			tmpSizeX = m_size.x;
		}
		if (true==m_userFill.y) {
			//tmpSizeY = m_size.y;
			tmpOriginY = 0;
			tmpTextOriginY = tmpOriginY + m_borderSize + 2*m_paddingSize;
		}
		tmpOriginX += m_paddingSize;
		tmpOriginY += m_paddingSize;
		tmpSizeX -= 2*m_paddingSize;
		tmpSizeY -= 2*m_paddingSize;
		
		
		etk::Vector3D<float> textPos( tmpTextOriginX + m_displayStartPosition,
		                              tmpTextOriginY,
		                              0 );
		etk::Vector3D<float> drawClippingPos( 2*m_paddingSize + m_borderSize,
		                                      2*m_paddingSize + m_borderSize,
		                                      -1 );
		etk::Vector3D<float> drawClippingSize( m_size.x - 2*drawClippingPos.x,
		                                       m_size.y - 2*drawClippingPos.y,
		                                       1 );
		m_oObjectText.SetClippingWidth(drawClippingPos, drawClippingSize);
		m_oObjectText.SetPos(textPos);
		m_oObjectText.Print(m_data);
		m_oObjectText.SetClippingMode(false);

		m_pos[0] = m_borderSize+2*drawClippingPos.x;
		m_pos[1] = m_borderSize+2*drawClippingPos.y;
		m_pos[2] = m_size.x - 2*(m_borderSize+2*drawClippingPos.x);
		m_pos[3] = m_size.y - 2*(m_borderSize+2*drawClippingPos.y);
		Rectangle(0, 0, m_size.x, m_size.y);

		/*
		  Must be rework corectly ==> selection and Cursor are integrated at the system ...
		int32_t pos1 = m_displayCursorPosSelection;
		int32_t pos2 = m_displayCursorPos;
		if(m_displayCursorPosSelection>m_displayCursorPos) {
			pos2 = m_displayCursorPosSelection;
			pos1 = m_displayCursorPos;
		}
		if(pos1!=pos2) {
			m_oObjectDecoration.SetColor(m_textColorFg);
			m_oObjectDecoration.clippingSet(drawClipping);
			etk::UString tmpDisplay = m_data.Extract(0, pos1);
			etk::Vector2D<int32_t> minSize = m_oObjectText.GetSize(tmpDisplay);
			tmpDisplay = m_data.Extract(0, pos2);
			etk::Vector2D<int32_t> maxSize = m_oObjectText.GetSize(tmpDisplay);
			
			int32_t XPos    = minSize.x + m_borderSize + 2*m_paddingSize + m_displayStartPosition;
			int32_t XPosEnd = maxSize.x + m_borderSize + 2*m_paddingSize + m_displayStartPosition;
			XPos    = etk_avg(m_borderSize + 2*m_paddingSize, XPos,    m_size.x - 2*m_paddingSize );
			XPosEnd = etk_avg(m_borderSize + 2*m_paddingSize, XPosEnd, m_size.x - 2*m_paddingSize );
			m_oObjectDecoration.SetColor(0x4444FFAA);
			m_oObjectDecoration.Rectangle( XPos, tmpTextOriginY, XPosEnd-XPos, maxSize.y);
			m_oObjectDecoration.clippingDisable();
		}
		if (true == m_displayCursor) {
			m_oObjectDecoration.SetColor(m_textColorFg);
			etk::UString tmpDisplay = m_data.Extract(0, m_displayCursorPos);
			etk::Vector2D<int32_t> minSize = m_oObjectText.GetSize(tmpDisplay);
			int32_t XCursorPos = minSize.x + m_borderSize + 2*m_paddingSize + m_displayStartPosition;
			if (XCursorPos >= m_borderSize + 2*m_paddingSize) {
				m_oObjectDecoration.Line(XCursorPos, tmpTextOriginY, XCursorPos, tmpTextOriginY + minSize.y, 1);
			}
		}
		*/
	}
}


void widget::Entry::UpdateCursorPosition(etk::Vector2D<float>& pos, bool selection)
{
	etk::Vector2D<float> relPos = RelativePosition(pos);
	relPos.x += -m_displayStartPosition - 2*m_paddingSize - m_borderSize;
	// try to find the new cursor position :
	etk::UString tmpDisplay = m_data.Extract(0, m_displayStartPosition);
	int32_t displayHidenSize = m_oObjectText.CalculateSize(tmpDisplay).x;
	//EWOL_DEBUG("hidenSize : " << displayHidenSize);
	int32_t newCursorPosition = -1;
	int32_t tmpTextOriginX = m_borderSize + 2*m_paddingSize;
	for (int32_t iii=0; iii<m_data.Size(); iii++) {
		tmpDisplay = m_data.Extract(0, iii);
		int32_t tmpWidth = m_oObjectText.CalculateSize(tmpDisplay).x - displayHidenSize;
		if (tmpWidth>=relPos.x-tmpTextOriginX) {
			newCursorPosition = iii;
			break;
		}
	}
	if (newCursorPosition == -1) {
		newCursorPosition = m_data.Size();
	}
	if (false == selection) {
		m_displayCursorPos = newCursorPosition;
		m_displayCursorPosSelection = m_displayCursorPos;
		MarkToRedraw();
	} else {
		if (m_displayCursorPos == m_displayCursorPosSelection) {
			m_displayCursorPosSelection = m_displayCursorPos;
		}
		m_displayCursorPos = newCursorPosition;
		MarkToRedraw();
	}
	UpdateTextPosition();
}


void widget::Entry::RemoveSelected(void)
{
	if (m_displayCursorPosSelection==m_displayCursorPos) {
		// nothing to cut ...
		return;
	}
	int32_t pos1 = m_displayCursorPosSelection;
	int32_t pos2 = m_displayCursorPos;
	if(m_displayCursorPosSelection>m_displayCursorPos) {
		pos2 = m_displayCursorPosSelection;
		pos1 = m_displayCursorPos;
	}
	// Remove data ...
	m_displayCursorPos = pos1;
	m_displayCursorPosSelection = pos1;
	m_data.Remove(pos1, pos2-pos1);
	MarkToRedraw();
}


void widget::Entry::CopySelectionToClipBoard(ewol::clipBoard::clipboardListe_te clipboardID)
{
	if (m_displayCursorPosSelection==m_displayCursorPos) {
		// nothing to cut ...
		return;
	}
	int32_t pos1 = m_displayCursorPosSelection;
	int32_t pos2 = m_displayCursorPos;
	if(m_displayCursorPosSelection>m_displayCursorPos) {
		pos2 = m_displayCursorPosSelection;
		pos1 = m_displayCursorPos;
	}
	// Copy
	etk::UString tmpData = m_data.Extract(pos1, pos2);
	ewol::clipBoard::Set(clipboardID, tmpData);
}


bool widget::Entry::OnEventInput(ewol::keyEvent::type_te type, int32_t IdInput, ewol::keyEvent::status_te typeEvent, etk::Vector2D<float> pos)
{
	//EWOL_DEBUG("Event on Entry ... type=" << (int32_t)type << " id=" << IdInput);
	if (1 == IdInput) {
		if (ewol::keyEvent::statusSingle == typeEvent) {
			KeepFocus();
			GenerateEventId(ewolEventEntryClick);
			//nothing to do ...
			return true;
		} else if (ewol::keyEvent::statusDouble == typeEvent) {
			KeepFocus();
			// select word
			m_displayCursorPosSelection = m_displayCursorPos-1;
			// search forward
			for (int32_t iii=m_displayCursorPos; iii<=m_data.Size(); iii++) {
				if(iii==m_data.Size()) {
					m_displayCursorPos = iii;
					break;
				}
				if(!(    (    m_data[iii] >= 'a'
				           && m_data[iii] <= 'z')
				      || (    m_data[iii] >= 'A'
				           && m_data[iii] <= 'Z')
				      || (    m_data[iii] >= '0'
				           && m_data[iii] <= '9')
				      || m_data[iii] == '_'
				      || m_data[iii] == '-'
				  ) ) {
					m_displayCursorPos = iii;
					break;
				}
			}
			// search backward
			for (int32_t iii=m_displayCursorPosSelection; iii>=-1; iii--) {
				if(iii==-1) {
					m_displayCursorPosSelection = 0;
					break;
				}
				if(!(    (    m_data[iii] >= 'a'
				           && m_data[iii] <= 'z')
				      || (    m_data[iii] >= 'A'
				           && m_data[iii] <= 'Z')
				      || (    m_data[iii] >= '0'
				           && m_data[iii] <= '9')
				      || m_data[iii] == '_'
				      || m_data[iii] == '-'
				  ) ) {
					m_displayCursorPosSelection = iii+1;
					break;
				}
			}
			// Copy to clipboard Middle ...
			CopySelectionToClipBoard(ewol::clipBoard::clipboardSelection);
			MarkToRedraw();
		} else if (ewol::keyEvent::statusTriple == typeEvent) {
			KeepFocus();
			m_displayCursorPosSelection = 0;
			m_displayCursorPos = m_data.Size();
		} else if (ewol::keyEvent::statusDown == typeEvent) {
			KeepFocus();
			UpdateCursorPosition(pos);
			MarkToRedraw();
		} else if (ewol::keyEvent::statusMove == typeEvent) {
			KeepFocus();
			UpdateCursorPosition(pos, true);
			MarkToRedraw();
		} else if (ewol::keyEvent::statusUp == typeEvent) {
			KeepFocus();
			UpdateCursorPosition(pos, true);
			// Copy to clipboard Middle ...
			CopySelectionToClipBoard(ewol::clipBoard::clipboardSelection);
			MarkToRedraw();
		}
	}
	else if(    ewol::keyEvent::typeMouse == type
	         && 2 == IdInput) {
		if(    typeEvent == ewol::keyEvent::statusDown
		    || typeEvent == ewol::keyEvent::statusMove
		    || typeEvent == ewol::keyEvent::statusUp) {
			KeepFocus();
			// updatethe cursor position : 
			UpdateCursorPosition(pos);
		}
		// Paste current selection only when up button
		if (typeEvent == ewol::keyEvent::statusUp) {
			KeepFocus();
			// middle button => past data...
			ewol::clipBoard::Request(ewol::clipBoard::clipboardSelection);
		}
	}
	return false;
}


bool widget::Entry::OnEventKb(ewol::keyEvent::status_te typeEvent, uniChar_t unicodeData)
{
	if( typeEvent == ewol::keyEvent::statusDown) {
		//EWOL_DEBUG("Entry input data ... : \"" << unicodeData << "\" " );
		//return GenEventInputExternal(ewolEventEntryEnter, -1, -1);
		// remove curent selected data ...
		RemoveSelected();
		if(    '\n' == unicodeData
		    || '\r' == unicodeData) {
			GenerateEventId(ewolEventEntryEnter, m_data);
			return true;
		} else if (0x7F == unicodeData) {
			// SUPPR :
			if (m_data.Size() > 0 && m_displayCursorPos<m_data.Size()) {
				m_data.Remove(m_displayCursorPos, 1);
				m_displayCursorPos = etk_max(m_displayCursorPos, 0);
				m_displayCursorPosSelection = m_displayCursorPos;
			}
		} else if (0x08 == unicodeData) {
			// DEL :
			if (m_data.Size() > 0 && m_displayCursorPos != 0) {
				m_data.Remove(m_displayCursorPos-1, 1);
				m_displayCursorPos--;
				m_displayCursorPos = etk_max(m_displayCursorPos, 0);
				m_displayCursorPosSelection = m_displayCursorPos;
			}
		} else if(unicodeData >= 20) {
			m_data.Add(m_displayCursorPos, unicodeData);
			m_displayCursorPos++;
			m_displayCursorPosSelection = m_displayCursorPos;
		}
		GenerateEventId(ewolEventEntryModify, m_data);
		MarkToRedraw();
		return true;
	}
	return false;
}


bool widget::Entry::OnEventKbMove(ewol::keyEvent::status_te typeEvent, ewol::keyEvent::keyboard_te moveTypeEvent)
{
	if(typeEvent == ewol::keyEvent::statusDown) {
		switch (moveTypeEvent)
		{
			case ewol::keyEvent::keyboardLeft:
				m_displayCursorPos--;
				break;
			case ewol::keyEvent::keyboardRight:
				m_displayCursorPos++;
				break;
			case ewol::keyEvent::keyboardStart:
				m_displayCursorPos = 0;
				break;
			case ewol::keyEvent::keyboardEnd:
				m_displayCursorPos = m_data.Size();
				break;
			default:
				return false;
		}
		m_displayCursorPos = etk_avg(0, m_displayCursorPos, m_data.Size());
		m_displayCursorPosSelection = m_displayCursorPos;
		MarkToRedraw();
		return true;
	}
	return false;
}


void widget::Entry::OnEventClipboard(ewol::clipBoard::clipboardListe_te clipboardID)
{
	// remove curent selected data ...
	RemoveSelected();
	// get current selection / Copy :
	etk::UString tmpData = Get(clipboardID);
	// add it on the current display :
	if (tmpData.Size() >= 0) {
		m_data.Add(m_displayCursorPos, &tmpData[0]);
		if (m_data.Size() == tmpData.Size()) {
			m_displayCursorPos = tmpData.Size();
		} else {
			m_displayCursorPos += tmpData.Size();
		}
		m_displayCursorPosSelection = m_displayCursorPos;
		MarkToRedraw();
	}
	GenerateEventId(ewolEventEntryModify, m_data);
}


void widget::Entry::OnReceiveMessage(ewol::EObject * CallerObject, const char * eventId, etk::UString data)
{
	ewol::Widget::OnReceiveMessage(CallerObject, eventId, data);
	if(eventId == ewolEventEntryClean) {
		m_data = "";
		m_displayStartPosition = 0;
		m_displayCursorPos = 0;
		m_displayCursorPosSelection = m_displayCursorPos;
		MarkToRedraw();
	} else if(eventId == ewolEventEntryCut) {
		CopySelectionToClipBoard(ewol::clipBoard::clipboardStd);
		RemoveSelected();
		GenerateEventId(ewolEventEntryModify, m_data);
	} else if(eventId == ewolEventEntryCopy) {
		CopySelectionToClipBoard(ewol::clipBoard::clipboardStd);
	} else if(eventId == ewolEventEntryPaste) {
		ewol::clipBoard::Request(ewol::clipBoard::clipboardStd);
	} else if(eventId == ewolEventEntrySelect) {
		if(data == "ALL") {
			m_displayCursorPosSelection = 0;
			m_displayCursorPos = m_data.Size();
		} else {
			m_displayCursorPosSelection = m_displayCursorPos;
		}
		MarkToRedraw();
	}
}


void widget::Entry::UpdateTextPosition(void)
{
	int32_t tmpSizeX = m_minSize.x;
	if (true==m_userFill.x) {
		tmpSizeX = m_size.x;
	}
	int32_t tmpUserSize = tmpSizeX - 2*(m_borderSize + 2*m_paddingSize);
	int32_t totalWidth = m_oObjectText.CalculateSize(m_data).x;
	// Check if the data inside the display can be contain in the entry box
	if (totalWidth < tmpUserSize) {
		// all can be display :
		m_displayStartPosition = 0;
	} else {
		// all can not be set :
		etk::UString tmpDisplay = m_data.Extract(0, m_displayCursorPos);
		int32_t pixelCursorPos = m_oObjectText.CalculateSize(tmpDisplay).x;
		// check if the Cussor is visible at 10px nearest the border :
		int32_t tmp1 = pixelCursorPos+m_displayStartPosition;
		EWOL_DEBUG("cursorPos=" << pixelCursorPos << "px maxSize=" << tmpUserSize << "px tmp1=" << tmp1);
		if (tmp1<10) {
			// set the cursor on le left
			m_displayStartPosition = etk_min(-pixelCursorPos+10, 0);
		} else if (tmp1>tmpUserSize-10) {
			// Set the cursor of the Right
			m_displayStartPosition = etk_min(-pixelCursorPos + tmpUserSize - 10, 0);
		}
		// else : the cursor is inside the display
		//m_displayStartPosition = -totalWidth + tmpUserSize;
	}
}


void widget::Entry::OnGetFocus(void)
{
	m_displayCursor = true;
	ewol::Keyboard(true);
	MarkToRedraw();
}


void widget::Entry::OnLostFocus(void)
{
	m_displayCursor = false;
	ewol::Keyboard(false);
	MarkToRedraw();
}
