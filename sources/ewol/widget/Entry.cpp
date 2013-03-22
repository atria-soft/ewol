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

// DEFINE for the shader display system :
#define STATUS_NORMAL    (0)
#define STATUS_HOVER     (1)
#define STATUS_SELECTED  (2)


widget::Entry::Entry(etk::UString newData) :
	m_shaper("THEME:GUI:widgetEntry.conf"),
	m_data(""),
	m_textColorFg(draw::color::black),
	m_textColorBg(draw::color::white),
	m_userSize(50),
	m_displayStartPosition(0),
	m_displayCursor(false),
	m_displayCursorPos(0),
	m_displayCursorPosSelection(0)
{
	m_textColorBg.a = 0xAF;
	SetCanHaveFocus(true);
	AddEventId(ewolEventEntryClick);
	AddEventId(ewolEventEntryEnter);
	AddEventId(ewolEventEntryModify);
	ShortCutAdd("ctrl+w",       ewolEventEntryClean);
	ShortCutAdd("ctrl+x",       ewolEventEntryCut);
	ShortCutAdd("ctrl+c",       ewolEventEntryCopy);
	ShortCutAdd("ctrl+v",       ewolEventEntryPaste);
	ShortCutAdd("ctrl+a",       ewolEventEntrySelect, "ALL");
	ShortCutAdd("ctrl+shift+a", ewolEventEntrySelect, "NONE");
	SetValue(newData);
	UpdateTextPosition();
	MarkToRedraw();
}


widget::Entry::~Entry(void)
{
	
}


bool widget::Entry::CalculateMinSize(void)
{
	vec2 padding = m_shaper.GetPadding();
	
	int32_t minHeight = m_oObjectText.CalculateSize('A').y();
	m_minSize.setValue(m_userSize + 2*padding.x(),
	                   minHeight + 2*padding.y());
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


void widget::Entry::OnDraw(ewol::DrawProperty& displayProp)
{
	m_shaper.Draw();
	m_oObjectText.Draw();
}


void widget::Entry::OnRegenerateDisplay(void)
{
	if (true == NeedRedraw()) {
		m_shaper.Clear();
		m_oObjectText.Clear();
		UpdateTextPosition();
		vec2 padding = m_shaper.GetPadding();
		
		int32_t tmpSizeX = m_minSize.x();
		int32_t tmpSizeY = m_minSize.y();
		int32_t tmpOriginX = 0;
		int32_t tmpOriginY = (m_size.y() - tmpSizeY) / 2;
		// no change for the text orogin : 
		int32_t tmpTextOriginX = padding.x();
		int32_t tmpTextOriginY = tmpOriginY + padding.y();
		
		if (true==m_userFill.x()) {
			tmpSizeX = m_size.x();
		}
		if (true==m_userFill.y()) {
			//tmpSizeY = m_size.y;
			tmpOriginY = 0;
			tmpTextOriginY = tmpOriginY + padding.y();
		}
		tmpOriginX += padding.x();
		tmpOriginY += padding.y();
		tmpSizeX -= 2*padding.x();
		tmpSizeY -= 2*padding.y();
		
		
		vec3 textPos( tmpTextOriginX + m_displayStartPosition,
		              tmpTextOriginY,
		              0 );
		vec3 drawClippingPos( padding.x(),
		                      padding.y(),
		                      -1 );
		vec3 drawClippingSize( m_size.x() - 2*drawClippingPos.x(),
		                       m_size.y() - 2*drawClippingPos.y(),
		                       1 );
		m_oObjectText.SetClippingWidth(drawClippingPos, drawClippingSize);
		m_oObjectText.SetPos(textPos);
		if (m_displayCursorPosSelection != m_displayCursorPos) {
			m_oObjectText.SetCursorSelection(m_displayCursorPos, m_displayCursorPosSelection);
		} else {
			m_oObjectText.SetCursorPos(m_displayCursorPos);
		}
		m_oObjectText.Print(m_data);
		m_oObjectText.SetClippingMode(false);
		m_shaper.SetSize(m_size);
	}
}


void widget::Entry::UpdateCursorPosition(vec2& pos, bool selection)
{
	vec2 padding = m_shaper.GetPadding();
	
	vec2 relPos = RelativePosition(pos);
	relPos.setX(relPos.x()-m_displayStartPosition - padding.x());
	// try to find the new cursor position :
	etk::UString tmpDisplay = m_data.Extract(0, m_displayStartPosition);
	int32_t displayHidenSize = m_oObjectText.CalculateSize(tmpDisplay).x();
	//EWOL_DEBUG("hidenSize : " << displayHidenSize);
	int32_t newCursorPosition = -1;
	int32_t tmpTextOriginX = padding.x();
	for (int32_t iii=0; iii<m_data.Size(); iii++) {
		tmpDisplay = m_data.Extract(0, iii);
		int32_t tmpWidth = m_oObjectText.CalculateSize(tmpDisplay).x() - displayHidenSize;
		if (tmpWidth>=relPos.x()-tmpTextOriginX) {
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


bool widget::Entry::OnEventInput(ewol::keyEvent::type_te type, int32_t IdInput, ewol::keyEvent::status_te typeEvent, vec2 pos)
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


void widget::Entry::OnReceiveMessage(ewol::EObject * CallerObject, const char * eventId, const etk::UString& data)
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
	vec2 padding = m_shaper.GetPadding();
	
	int32_t tmpSizeX = m_minSize.x();
	if (true==m_userFill.x()) {
		tmpSizeX = m_size.x();
	}
	int32_t tmpUserSize = tmpSizeX - 2*(padding.x());
	int32_t totalWidth = m_oObjectText.CalculateSize(m_data).x();
	// Check if the data inside the display can be contain in the entry box
	if (totalWidth < tmpUserSize) {
		// all can be display :
		m_displayStartPosition = 0;
	} else {
		// all can not be set :
		etk::UString tmpDisplay = m_data.Extract(0, m_displayCursorPos);
		int32_t pixelCursorPos = m_oObjectText.CalculateSize(tmpDisplay).x();
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
	ChangeStatusIn(STATUS_SELECTED);
	ewol::Keyboard(true);
	MarkToRedraw();
}


void widget::Entry::OnLostFocus(void)
{
	m_displayCursor = false;
	ChangeStatusIn(STATUS_NORMAL);
	ewol::Keyboard(false);
	MarkToRedraw();
}


void widget::Entry::ChangeStatusIn(int32_t newStatusId)
{
	if (true == m_shaper.ChangeStatusIn(newStatusId) ) {
		PeriodicCallSet(true);
		MarkToRedraw();
	}
}


void widget::Entry::PeriodicCall(int64_t localTime)
{
	if (false == m_shaper.PeriodicCall(localTime) ) {
		PeriodicCallSet(false);
	}
	MarkToRedraw();
}
