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
#include <ewol/renderer/os/eSystem.h>



const char * const ewolEventEntryCut    = "ewol-widget-entry-event-internal-cut";
const char * const ewolEventEntryCopy   = "ewol-widget-entry-event-internal-copy";
const char * const ewolEventEntryPaste  = "ewol-widget-entry-event-internal-paste";
const char * const ewolEventEntryClean  = "ewol-widget-entry-event-internal-clean";
const char * const ewolEventEntrySelect = "ewol-widget-entry-event-internal-select";


#undef __class__
#define __class__	"Entry"

// DEFINE for the shader display system :
#define STATUS_NORMAL    (0)
#define STATUS_HOVER     (1)
#define STATUS_SELECTED  (2)

static ewol::Widget* Create(void)
{
	return new widget::Entry();
}

void widget::Entry::Init(ewol::WidgetManager& _widgetManager)
{
	_widgetManager.AddWidgetCreator(__class__,&Create);
}

const char * const widget::Entry::eventClick  = "ewol-widget-entry-event-click";
const char * const widget::Entry::eventEnter  = "ewol-widget-entry-event-enter";
const char * const widget::Entry::eventModify = "ewol-widget-entry-event-modify";

const char* const widget::Entry::configMaxChar = "max";
const char* const widget::Entry::configRegExp  = "regExp";
const char* const widget::Entry::configColorFg = "color";
const char* const widget::Entry::configColorBg = "background";
const char* const widget::Entry::configEmptyMessage = "emptytext";

widget::Entry::Entry(etk::UString _newData) :
	m_shaper("THEME:GUI:widgetEntry.conf"),
	m_data(""),
	m_maxCharacter(0x7FFFFFFF),
	m_regExp(".*"),
	m_needUpdateTextPos(true),
	m_displayStartPosition(0),
	m_displayCursor(false),
	m_displayCursorPos(0),
	m_displayCursorPosSelection(0),
	m_textColorFg(etk::color::black),
	m_textColorBg(etk::color::white),
	m_textWhenNothing("")
{
	m_textColorBg.SetA(0xAF);
	SetCanHaveFocus(true);
	AddEventId(eventClick);
	AddEventId(eventEnter);
	AddEventId(eventModify);
	ShortCutAdd("ctrl+w", ewolEventEntryClean);
	ShortCutAdd("ctrl+x", ewolEventEntryCut);
	ShortCutAdd("ctrl+c", ewolEventEntryCopy);
	ShortCutAdd("ctrl+v", ewolEventEntryPaste);
	ShortCutAdd("ctrl+a", ewolEventEntrySelect, "ALL");
	ShortCutAdd("ctrl+shift+a", ewolEventEntrySelect, "NONE");
	
	RegisterConfig(configMaxChar, "int", NULL, "Maximum cgar that can be set on the Entry");
	RegisterConfig(configRegExp, "string", NULL, "Control what it is write with a regular expression");
	RegisterConfig(configColorFg, "color", NULL, "Color of the text displayed");
	RegisterConfig(configColorBg, "color", NULL, "Color of the text selected");
	RegisterConfig(configEmptyMessage, "string", NULL, "Text that is displayed when the Entry is empty (decorated text)");
	
	SetValue(_newData);
	MarkToRedraw();
}


widget::Entry::~Entry(void)
{
	
}


void widget::Entry::SetMaxChar(int32_t _nbMax)
{
	if (_nbMax<=0) {
		m_maxCharacter = 0x7FFFFFFF;
	} else {
		m_maxCharacter = _nbMax;
	}
}


void widget::Entry::CalculateMinMaxSize(void)
{
	// call main class
	ewol::Widget::CalculateMinMaxSize();
	// get generic padding
	vec2 padding = m_shaper.GetPadding();
	int32_t minHeight = m_oObjectText.CalculateSize(etk::UniChar('A')).y();
	vec2 minimumSizeBase(20, minHeight);
	// add padding :
	minimumSizeBase += padding*2.0f;
	m_minSize.setMax(minimumSizeBase);
	// verify the min max of the min size ...
	CheckMinSize();
}


void widget::Entry::SetValue(const etk::UString& _newData)
{
	etk::UString newData = _newData;
	if (newData.Size()>m_maxCharacter) {
		newData = _newData.Extract(0, m_maxCharacter);
		EWOL_DEBUG("Limit entry set of data... " << _newData.Extract(m_maxCharacter));
	}
	// set the value with the check of the RegExp ...
	SetInternalValue(newData);
	if (m_data==newData) {
		m_displayCursorPos = m_data.Size();
		m_displayCursorPosSelection = m_displayCursorPos;
		EWOL_DEBUG("Set ... " << newData);
	}
	MarkToRedraw();
}


void widget::Entry::OnDraw(void)
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
		
		vec2 tmpSizeShaper = m_minSize;
		if (true==m_userFill.x()) {
			tmpSizeShaper.setX(m_size.x());
		}
		if (true==m_userFill.y()) {
			tmpSizeShaper.setY(m_size.y());
		}
		
		vec2 tmpOriginShaper = (m_size - tmpSizeShaper) / 2.0f;
		vec2 tmpSizeText = tmpSizeShaper - padding * 2.0f;
		vec2 tmpOriginText = (m_size - tmpSizeText) / 2.0f;
		// sometimes, the user define an height bigger than the real size needed ==> in this case we need to center the text in the shaper ...
		int32_t minHeight = m_oObjectText.CalculateSize(etk::UniChar('A')).y();
		if (tmpSizeText.y()>minHeight) {
			tmpOriginText += vec2(0,(tmpSizeText.y()-minHeight)/2.0f);
		}
		// fix all the position in the int32_t class:
		tmpSizeShaper = vec2ClipInt32(tmpSizeShaper);
		tmpOriginShaper = vec2ClipInt32(tmpOriginShaper);
		tmpSizeText = vec2ClipInt32(tmpSizeText);
		tmpOriginText = vec2ClipInt32(tmpOriginText);
		
		m_oObjectText.SetClippingWidth(tmpOriginText, tmpSizeText);
		m_oObjectText.SetPos(tmpOriginText+vec2(m_displayStartPosition,0));
		if (m_displayCursorPosSelection != m_displayCursorPos) {
			m_oObjectText.SetCursorSelection(m_displayCursorPos, m_displayCursorPosSelection);
		} else {
			m_oObjectText.SetCursorPos(m_displayCursorPos);
		}
		if (0!=m_data.Size()) {
			m_oObjectText.Print(m_data);
		} else {
			if (0!=m_textWhenNothing.Size()) {
				m_oObjectText.PrintDecorated(m_textWhenNothing);
			}
		}
		m_oObjectText.SetClippingMode(false);
		
		m_shaper.SetOrigin(tmpOriginShaper);
		m_shaper.SetSize(tmpSizeShaper);
	}
}


void widget::Entry::UpdateCursorPosition(const vec2& _pos, bool _selection)
{
	vec2 padding = m_shaper.GetPadding();
	
	vec2 relPos = RelativePosition(_pos);
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
	if (false == _selection) {
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
	MarkToUpdateTextPosition();
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


void widget::Entry::CopySelectionToClipBoard(ewol::clipBoard::clipboardListe_te _clipboardID)
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
	ewol::clipBoard::Set(_clipboardID, tmpData);
}


bool widget::Entry::OnEventInput(const ewol::EventInput& _event)
{
	//EWOL_DEBUG("Event on Entry ... type=" << (int32_t)type << " id=" << IdInput);
	if (1 == _event.GetId()) {
		if (ewol::keyEvent::statusSingle == _event.GetStatus()) {
			KeepFocus();
			GenerateEventId(eventClick);
			//nothing to do ...
			return true;
		} else if (ewol::keyEvent::statusDouble == _event.GetStatus()) {
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
		} else if (ewol::keyEvent::statusTriple == _event.GetStatus()) {
			KeepFocus();
			m_displayCursorPosSelection = 0;
			m_displayCursorPos = m_data.Size();
		} else if (ewol::keyEvent::statusDown == _event.GetStatus()) {
			KeepFocus();
			UpdateCursorPosition(_event.GetPos());
			MarkToRedraw();
		} else if (ewol::keyEvent::statusMove == _event.GetStatus()) {
			KeepFocus();
			UpdateCursorPosition(_event.GetPos(), true);
			MarkToRedraw();
		} else if (ewol::keyEvent::statusUp == _event.GetStatus()) {
			KeepFocus();
			UpdateCursorPosition(_event.GetPos(), true);
			// Copy to clipboard Middle ...
			CopySelectionToClipBoard(ewol::clipBoard::clipboardSelection);
			MarkToRedraw();
		}
	}
	else if(    ewol::keyEvent::typeMouse == _event.GetType()
	         && 2 == _event.GetId()) {
		if(    _event.GetStatus() == ewol::keyEvent::statusDown
		    || _event.GetStatus() == ewol::keyEvent::statusMove
		    || _event.GetStatus() == ewol::keyEvent::statusUp) {
			KeepFocus();
			// updatethe cursor position : 
			UpdateCursorPosition(_event.GetPos());
		}
		// Paste current selection only when up button
		if (_event.GetStatus() == ewol::keyEvent::statusUp) {
			KeepFocus();
			// middle button => past data...
			ewol::clipBoard::Request(ewol::clipBoard::clipboardSelection);
		}
	}
	return false;
}


bool widget::Entry::OnEventEntry(const ewol::EventEntry& _event)
{
	if (_event.GetType() == ewol::keyEvent::keyboardChar) {
		if(_event.GetStatus() == ewol::keyEvent::statusDown) {
			//EWOL_DEBUG("Entry input data ... : \"" << unicodeData << "\" " );
			//return GenEventInputExternal(eventEnter, -1, -1);
			// remove curent selected data ...
			RemoveSelected();
			if(    _event.GetChar() == '\n'
			    || _event.GetChar() == '\r') {
				GenerateEventId(eventEnter, m_data);
				return true;
			} else if (_event.GetChar() == 0x7F) {
				// SUPPR :
				if (m_data.Size() > 0 && m_displayCursorPos<m_data.Size()) {
					m_data.Remove(m_displayCursorPos, 1);
					m_displayCursorPos = etk_max(m_displayCursorPos, 0);
					m_displayCursorPosSelection = m_displayCursorPos;
				}
			} else if (_event.GetChar() == 0x08) {
				// DEL :
				if (m_data.Size() > 0 && m_displayCursorPos != 0) {
					m_data.Remove(m_displayCursorPos-1, 1);
					m_displayCursorPos--;
					m_displayCursorPos = etk_max(m_displayCursorPos, 0);
					m_displayCursorPosSelection = m_displayCursorPos;
				}
			} else if(_event.GetChar() >= 20) {
				if (m_data.Size() > m_maxCharacter) {
					EWOL_INFO("Reject data for entry : '" << _event.GetChar() << "'");
				} else {
					etk::UString newData = m_data;
					newData.Add(m_displayCursorPos, _event.GetChar());
					SetInternalValue(newData);
					if (m_data==newData) {
						m_displayCursorPos++;
						m_displayCursorPosSelection = m_displayCursorPos;
					}
				}
			}
			GenerateEventId(eventModify, m_data);
			MarkToRedraw();
			return true;
		}
		return false;
	} else {
		if(_event.GetStatus() == ewol::keyEvent::statusDown) {
			switch (_event.GetType())
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
	}
	return false;
}

void widget::Entry::SetInternalValue(const etk::UString& _newData)
{
	etk::UString previous = m_data;
	// check the RegExp :
	if (_newData.Size()>0) {
		if (false==m_regExp.ProcessOneElement(_newData,0,_newData.Size()) ) {
			EWOL_INFO("the input data does not match with the regExp \"" << _newData << "\" RegExp=\"" << m_regExp.GetRegExp() << "\" start=" << m_regExp.Start() << " stop=" << m_regExp.Stop() );
			return;
		}
		//EWOL_INFO("find regExp : \"" << m_data << "\" start=" << m_regExp.Start() << " stop=" << m_regExp.Stop() );
		if(    0 != m_regExp.Start()
		    || _newData.Size() != m_regExp.Stop() ) {
			EWOL_INFO("The input data match not entirely with the regExp \"" << _newData << "\" RegExp=\"" << m_regExp.GetRegExp() << "\" start=" << m_regExp.Start() << " stop=" << m_regExp.Stop() );
			return;
		}
	}
	m_data = _newData;
}

void widget::Entry::OnEventClipboard(ewol::clipBoard::clipboardListe_te _clipboardID)
{
	// remove curent selected data ...
	RemoveSelected();
	// get current selection / Copy :
	etk::UString tmpData = Get(_clipboardID);
	// add it on the current display :
	if (tmpData.Size() >= 0) {
		etk::UString newData = m_data;
		newData.Add(m_displayCursorPos, &tmpData[0]);
		SetInternalValue(newData);
		if (m_data == newData) {
			if (m_data.Size() == tmpData.Size()) {
				m_displayCursorPos = tmpData.Size();
			} else {
				m_displayCursorPos += tmpData.Size();
			}
			m_displayCursorPosSelection = m_displayCursorPos;
			MarkToRedraw();
		}
	}
	GenerateEventId(eventModify, m_data);
}


void widget::Entry::OnReceiveMessage(const ewol::EMessage& _msg)
{
	ewol::Widget::OnReceiveMessage(_msg);
	if(_msg.GetMessage() == ewolEventEntryClean) {
		m_data = "";
		m_displayStartPosition = 0;
		m_displayCursorPos = 0;
		m_displayCursorPosSelection = m_displayCursorPos;
		MarkToRedraw();
	} else if(_msg.GetMessage() == ewolEventEntryCut) {
		CopySelectionToClipBoard(ewol::clipBoard::clipboardStd);
		RemoveSelected();
		GenerateEventId(eventModify, m_data);
	} else if(_msg.GetMessage() == ewolEventEntryCopy) {
		CopySelectionToClipBoard(ewol::clipBoard::clipboardStd);
	} else if(_msg.GetMessage() == ewolEventEntryPaste) {
		ewol::clipBoard::Request(ewol::clipBoard::clipboardStd);
	} else if(_msg.GetMessage() == ewolEventEntrySelect) {
		if(_msg.GetData() == "ALL") {
			m_displayCursorPosSelection = 0;
			m_displayCursorPos = m_data.Size();
		} else {
			m_displayCursorPosSelection = m_displayCursorPos;
		}
		MarkToRedraw();
	}
}

void widget::Entry::MarkToUpdateTextPosition(void)
{
	m_needUpdateTextPos=true;
}

void widget::Entry::UpdateTextPosition(void)
{
	if (false==m_needUpdateTextPos) {
		return;
	}
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
	GetSystem().KeyboardShow();
	MarkToRedraw();
}


void widget::Entry::OnLostFocus(void)
{
	m_displayCursor = false;
	ChangeStatusIn(STATUS_NORMAL);
	GetSystem().KeyboardHide();
	MarkToRedraw();
}


void widget::Entry::ChangeStatusIn(int32_t _newStatusId)
{
	if (true == m_shaper.ChangeStatusIn(_newStatusId) ) {
		PeriodicCallEnable();
		MarkToRedraw();
	}
}


void widget::Entry::PeriodicCall(const ewol::EventTime& _event)
{
	if (false == m_shaper.PeriodicCall(_event) ) {
		PeriodicCallDisable();
	}
	MarkToRedraw();
}



void widget::Entry::SetRegExp(const etk::UString& _expression)
{
	etk::UString previousRegExp = m_regExp.GetRegExp();
	EWOL_DEBUG("change input regExp \"" << previousRegExp << "\" ==> \"" << _expression << "\"");
	m_regExp.SetRegExp(_expression);
	if (m_regExp.GetStatus()==false) {
		EWOL_ERROR("error when adding regExp ... ==> set the previous back ...");
		m_regExp.SetRegExp(previousRegExp);
	}
}


void widget::Entry::SetColorText(const etk::Color<>& _color)
{
	m_textColorFg = _color;
	MarkToRedraw();
}

void widget::Entry::SetColorTextSelected(const etk::Color<>& _color)
{
	m_textColorBg = _color;
	MarkToRedraw();
}

void widget::Entry::SetEmptyText(const etk::UString& _text)
{
	m_textWhenNothing = _text;
	MarkToRedraw();
}

bool widget::Entry::OnSetConfig(const ewol::EConfig& _conf)
{
	if (true == ewol::Widget::OnSetConfig(_conf)) {
		return true;
	}
	if (_conf.GetConfig() == configMaxChar) {
		SetMaxChar(_conf.GetData().ToInt32());
		return true;
	}
	if (_conf.GetConfig() == configRegExp) {
		SetRegExp(_conf.GetData());
		return true;
	}
	if (_conf.GetConfig() == configColorFg) {
		SetColorText(_conf.GetData());
		return true;
	}
	if (_conf.GetConfig() == configColorBg) {
		SetColorTextSelected(_conf.GetData());
		return true;
	}
	if (_conf.GetConfig() == configEmptyMessage) {
		SetEmptyText(_conf.GetData());
		return true;
	}
	return false;
}

bool widget::Entry::OnGetConfig(const char* _config, etk::UString& _result) const
{
	if (true == ewol::Widget::OnGetConfig(_config, _result)) {
		return true;
	}
	if (_config == configMaxChar) {
		_result = etk::UString(GetMaxChar());
		return true;
	}
	if (_config == configRegExp) {
		_result = GetRegExp();
		return true;
	}
	if (_config == configColorFg) {
		_result = GetColorText().GetString();
		return true;
	}
	if (_config == configColorBg) {
		_result = GetColorTextSelected().GetString();
		return true;
	}
	if (_config == configEmptyMessage) {
		_result = GetEmptyText();
		return true;
	}
	return false;
}



