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
#include <ewol/renderer/eContext.h>


const char * const ewolEventEntryCut    = "ewol-widget-entry-event-internal-cut";
const char * const ewolEventEntryCopy   = "ewol-widget-entry-event-internal-copy";
const char * const ewolEventEntryPaste  = "ewol-widget-entry-event-internal-paste";
const char * const ewolEventEntryClean  = "ewol-widget-entry-event-internal-clean";
const char * const ewolEventEntrySelect = "ewol-widget-entry-event-internal-select";


#undef __class__
#define __class__ "Entry"

// DEFINE for the shader display system :
#define STATUS_NORMAL    (0)
#define STATUS_HOVER     (1)
#define STATUS_SELECTED  (2)

static ewol::Widget* create(void) {
	return new widget::Entry();
}

void widget::Entry::init(ewol::WidgetManager& _widgetManager) {
	_widgetManager.addWidgetCreator(__class__,&create);
}

const char * const widget::Entry::eventClick  = "click";
const char * const widget::Entry::eventEnter  = "enter";
const char * const widget::Entry::eventModify = "modify";

const char* const widget::Entry::configMaxChar = "max";
const char* const widget::Entry::configRegExp  = "regExp";
const char* const widget::Entry::configColorFg = "color";
const char* const widget::Entry::configColorBg = "background";
const char* const widget::Entry::configEmptyMessage = "emptytext";

widget::Entry::Entry(std::string _newData) :
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
  m_textWhenNothing("") {
	addObjectType("widget::Entry");
	m_textColorBg.setA(0xAF);
	setCanHaveFocus(true);
	addEventId(eventClick);
	addEventId(eventEnter);
	addEventId(eventModify);
	shortCutAdd("ctrl+w", ewolEventEntryClean);
	shortCutAdd("ctrl+x", ewolEventEntryCut);
	shortCutAdd("ctrl+c", ewolEventEntryCopy);
	shortCutAdd("ctrl+v", ewolEventEntryPaste);
	shortCutAdd("ctrl+a", ewolEventEntrySelect, "ALL");
	shortCutAdd("ctrl+shift+a", ewolEventEntrySelect, "NONE");
	
	registerConfig(configMaxChar, "int", NULL, "Maximum cgar that can be set on the Entry");
	registerConfig(configRegExp, "string", NULL, "Control what it is write with a regular expression");
	registerConfig(configColorFg, "color", NULL, "Color of the text displayed");
	registerConfig(configColorBg, "color", NULL, "Color of the text selected");
	registerConfig(configEmptyMessage, "string", NULL, "Text that is displayed when the Entry is empty (decorated text)");
	
	setValue(_newData);
	markToRedraw();
}


widget::Entry::~Entry(void) {
	
}


void widget::Entry::setMaxChar(int32_t _nbMax) {
	if (_nbMax <= 0) {
		m_maxCharacter = 0x7FFFFFFF;
	} else {
		m_maxCharacter = _nbMax;
	}
}


void widget::Entry::calculateMinMaxSize(void) {
	// call main class
	ewol::Widget::calculateMinMaxSize();
	// get generic padding
	vec2 padding = m_shaper.getPadding();
	int32_t minHeight = m_oObjectText.calculateSize(char32_t('A')).y();
	vec2 minimumSizeBase(20, minHeight);
	// add padding :
	minimumSizeBase += padding*2.0f;
	m_minSize.setMax(minimumSizeBase);
	// verify the min max of the min size ...
	checkMinSize();
}


void widget::Entry::setValue(const std::string& _newData) {
	std::string newData = _newData;
	if (newData.size()>m_maxCharacter) {
		newData = std::string(_newData, 0, m_maxCharacter);
		EWOL_DEBUG("Limit entry set of data... " << std::string(_newData, m_maxCharacter));
	}
	// set the value with the check of the RegExp ...
	setInternalValue(newData);
	if (m_data == newData) {
		m_displayCursorPos = m_data.size();
		m_displayCursorPosSelection = m_displayCursorPos;
		EWOL_DEBUG("Set ... " << newData);
	}
	markToRedraw();
}


void widget::Entry::onDraw(void) {
	m_shaper.draw();
	m_oObjectText.draw();
}


void widget::Entry::onRegenerateDisplay(void) {
	if (true == needRedraw()) {
		m_shaper.clear();
		m_oObjectText.clear();
		updateTextPosition();
		vec2 padding = m_shaper.getPadding();
		
		vec2 tmpSizeShaper = m_minSize;
		if (true == m_userFill.x()) {
			tmpSizeShaper.setX(m_size.x());
		}
		if (true == m_userFill.y()) {
			tmpSizeShaper.setY(m_size.y());
		}
		
		vec2 tmpOriginShaper = (m_size - tmpSizeShaper) / 2.0f;
		vec2 tmpSizeText = tmpSizeShaper - padding * 2.0f;
		vec2 tmpOriginText = (m_size - tmpSizeText) / 2.0f;
		// sometimes, the user define an height bigger than the real size needed  == > in this case we need to center the text in the shaper ...
		int32_t minHeight = m_oObjectText.calculateSize(char32_t('A')).y();
		if (tmpSizeText.y()>minHeight) {
			tmpOriginText += vec2(0,(tmpSizeText.y()-minHeight)/2.0f);
		}
		// fix all the position in the int32_t class:
		tmpSizeShaper = vec2ClipInt32(tmpSizeShaper);
		tmpOriginShaper = vec2ClipInt32(tmpOriginShaper);
		tmpSizeText = vec2ClipInt32(tmpSizeText);
		tmpOriginText = vec2ClipInt32(tmpOriginText);
		
		m_oObjectText.setClippingWidth(tmpOriginText, tmpSizeText);
		m_oObjectText.setPos(tmpOriginText+vec2(m_displayStartPosition,0));
		if (m_displayCursorPosSelection != m_displayCursorPos) {
			m_oObjectText.setCursorSelection(m_displayCursorPos, m_displayCursorPosSelection);
		} else {
			m_oObjectText.setCursorPos(m_displayCursorPos);
		}
		if (0!=m_data.size()) {
			m_oObjectText.print(m_data);
		} else {
			if (0!=m_textWhenNothing.size()) {
				m_oObjectText.printDecorated(m_textWhenNothing);
			}
		}
		m_oObjectText.setClippingMode(false);
		
		m_shaper.setOrigin(tmpOriginShaper);
		m_shaper.setSize(tmpSizeShaper);
	}
}


void widget::Entry::updateCursorPosition(const vec2& _pos, bool _selection) {
	vec2 padding = m_shaper.getPadding();
	
	vec2 relPos = relativePosition(_pos);
	relPos.setX(relPos.x()-m_displayStartPosition - padding.x());
	// try to find the new cursor position :
	std::string tmpDisplay = std::string(m_data, 0, m_displayStartPosition);
	int32_t displayHidenSize = m_oObjectText.calculateSize(tmpDisplay).x();
	//EWOL_DEBUG("hidenSize : " << displayHidenSize);
	int32_t newCursorPosition = -1;
	int32_t tmpTextOriginX = padding.x();
	for (int32_t iii=0; iii<m_data.size(); iii++) {
		tmpDisplay = std::string(m_data, 0, iii);
		int32_t tmpWidth = m_oObjectText.calculateSize(tmpDisplay).x() - displayHidenSize;
		if (tmpWidth >= relPos.x()-tmpTextOriginX) {
			newCursorPosition = iii;
			break;
		}
	}
	if (newCursorPosition == -1) {
		newCursorPosition = m_data.size();
	}
	if (false == _selection) {
		m_displayCursorPos = newCursorPosition;
		m_displayCursorPosSelection = m_displayCursorPos;
		markToRedraw();
	} else {
		if (m_displayCursorPos == m_displayCursorPosSelection) {
			m_displayCursorPosSelection = m_displayCursorPos;
		}
		m_displayCursorPos = newCursorPosition;
		markToRedraw();
	}
	markToUpdateTextPosition();
}


void widget::Entry::removeSelected(void) {
	if (m_displayCursorPosSelection == m_displayCursorPos) {
		// nothing to cut ...
		return;
	}
	int32_t pos1 = m_displayCursorPosSelection;
	int32_t pos2 = m_displayCursorPos;
	if(m_displayCursorPosSelection>m_displayCursorPos) {
		pos2 = m_displayCursorPosSelection;
		pos1 = m_displayCursorPos;
	}
	// remove data ...
	m_displayCursorPos = pos1;
	m_displayCursorPosSelection = pos1;
	m_data.erase(pos1, pos2-pos1);
	markToRedraw();
}


void widget::Entry::copySelectionToClipBoard(enum ewol::clipBoard::clipboardListe _clipboardID) {
	if (m_displayCursorPosSelection == m_displayCursorPos) {
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
	std::string tmpData = std::string(m_data, pos1, pos2);
	ewol::clipBoard::set(_clipboardID, tmpData);
}


bool widget::Entry::onEventInput(const ewol::EventInput& _event) {
	//EWOL_DEBUG("Event on Entry ... type=" << (int32_t)type << " id=" << IdInput);
	if (1 == _event.getId()) {
		if (ewol::keyEvent::statusSingle == _event.getStatus()) {
			keepFocus();
			generateEventId(eventClick);
			//nothing to do ...
			return true;
		} else if (ewol::keyEvent::statusDouble == _event.getStatus()) {
			keepFocus();
			// select word
			m_displayCursorPosSelection = m_displayCursorPos-1;
			// search forward
			for (int32_t iii=m_displayCursorPos; iii <= m_data.size(); iii++) {
				if(iii == m_data.size()) {
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
			for (int32_t iii=m_displayCursorPosSelection; iii >= -1; iii--) {
				if(iii == -1) {
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
			copySelectionToClipBoard(ewol::clipBoard::clipboardSelection);
			markToRedraw();
		} else if (ewol::keyEvent::statusTriple == _event.getStatus()) {
			keepFocus();
			m_displayCursorPosSelection = 0;
			m_displayCursorPos = m_data.size();
		} else if (ewol::keyEvent::statusDown == _event.getStatus()) {
			keepFocus();
			updateCursorPosition(_event.getPos());
			markToRedraw();
		} else if (ewol::keyEvent::statusMove == _event.getStatus()) {
			keepFocus();
			updateCursorPosition(_event.getPos(), true);
			markToRedraw();
		} else if (ewol::keyEvent::statusUp == _event.getStatus()) {
			keepFocus();
			updateCursorPosition(_event.getPos(), true);
			// Copy to clipboard Middle ...
			copySelectionToClipBoard(ewol::clipBoard::clipboardSelection);
			markToRedraw();
		}
	}
	else if(    ewol::keyEvent::typeMouse == _event.getType()
	         && 2 == _event.getId()) {
		if(    _event.getStatus() == ewol::keyEvent::statusDown
		    || _event.getStatus() == ewol::keyEvent::statusMove
		    || _event.getStatus() == ewol::keyEvent::statusUp) {
			keepFocus();
			// updatethe cursor position : 
			updateCursorPosition(_event.getPos());
		}
		// Paste current selection only when up button
		if (_event.getStatus() == ewol::keyEvent::statusUp) {
			keepFocus();
			// middle button => past data...
			ewol::clipBoard::request(ewol::clipBoard::clipboardSelection);
		}
	}
	return false;
}


bool widget::Entry::onEventEntry(const ewol::EventEntry& _event) {
	if (_event.getType() == ewol::keyEvent::keyboardChar) {
		if(_event.getStatus() == ewol::keyEvent::statusDown) {
			//EWOL_DEBUG("Entry input data ... : \"" << unicodeData << "\" " );
			//return GenEventInputExternal(eventEnter, -1, -1);
			// remove curent selected data ...
			removeSelected();
			if(    _event.getChar() == '\n'
			    || _event.getChar() == '\r') {
				generateEventId(eventEnter, m_data);
				return true;
			} else if (_event.getChar() == 0x7F) {
				// SUPPR :
				if (m_data.size() > 0 && m_displayCursorPos<m_data.size()) {
					m_data.erase(m_displayCursorPos, 1);
					m_displayCursorPos = etk_max(m_displayCursorPos, 0);
					m_displayCursorPosSelection = m_displayCursorPos;
				}
			} else if (_event.getChar() == 0x08) {
				// DEL :
				if (m_data.size() > 0 && m_displayCursorPos != 0) {
					m_data.erase(m_displayCursorPos-1, 1);
					m_displayCursorPos--;
					m_displayCursorPos = etk_max(m_displayCursorPos, 0);
					m_displayCursorPosSelection = m_displayCursorPos;
				}
			} else if(_event.getChar() >= 20) {
				if (m_data.size() > m_maxCharacter) {
					EWOL_INFO("Reject data for entry : '" << _event.getChar() << "'");
				} else {
					std::string newData = m_data;
					newData.insert(newData.begin()+m_displayCursorPos, _event.getChar());
					setInternalValue(newData);
					if (m_data == newData) {
						m_displayCursorPos++;
						m_displayCursorPosSelection = m_displayCursorPos;
					}
				}
			}
			generateEventId(eventModify, m_data);
			markToRedraw();
			return true;
		}
		return false;
	} else {
		if(_event.getStatus() == ewol::keyEvent::statusDown) {
			switch (_event.getType())
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
					m_displayCursorPos = m_data.size();
					break;
				default:
					return false;
			}
			m_displayCursorPos = etk_avg(0, m_displayCursorPos, m_data.size());
			m_displayCursorPosSelection = m_displayCursorPos;
			markToRedraw();
			return true;
		}
	}
	return false;
}

void widget::Entry::setInternalValue(const std::string& _newData) {
	std::string previous = m_data;
	// check the RegExp :
	if (_newData.size()>0) {
		if (false == m_regExp.processOneElement(_newData,0,_newData.size()) ) {
			EWOL_INFO("the input data does not match with the regExp \"" << _newData << "\" RegExp=\"" << m_regExp.getRegExp() << "\" start=" << m_regExp.start() << " stop=" << m_regExp.stop() );
			return;
		}
		//EWOL_INFO("find regExp : \"" << m_data << "\" start=" << m_regExp.Start() << " stop=" << m_regExp.Stop() );
		if(    0 != m_regExp.start()
		    || _newData.size() != m_regExp.stop() ) {
			EWOL_INFO("The input data match not entirely with the regExp \"" << _newData << "\" RegExp=\"" << m_regExp.getRegExp() << "\" start=" << m_regExp.start() << " stop=" << m_regExp.stop() );
			return;
		}
	}
	m_data = _newData;
}

void widget::Entry::onEventClipboard(enum ewol::clipBoard::clipboardListe _clipboardID) {
	// remove curent selected data ...
	removeSelected();
	// get current selection / Copy :
	std::string tmpData = get(_clipboardID);
	// add it on the current display :
	if (tmpData.size() != 0) {
		std::string newData = m_data;
		newData.insert(m_displayCursorPos, &tmpData[0]);
		setInternalValue(newData);
		if (m_data == newData) {
			if (m_data.size() == tmpData.size()) {
				m_displayCursorPos = tmpData.size();
			} else {
				m_displayCursorPos += tmpData.size();
			}
			m_displayCursorPosSelection = m_displayCursorPos;
			markToRedraw();
		}
	}
	generateEventId(eventModify, m_data);
}


void widget::Entry::onReceiveMessage(const ewol::EMessage& _msg) {
	ewol::Widget::onReceiveMessage(_msg);
	if(_msg.getMessage() == ewolEventEntryClean) {
		m_data = "";
		m_displayStartPosition = 0;
		m_displayCursorPos = 0;
		m_displayCursorPosSelection = m_displayCursorPos;
		markToRedraw();
	} else if(_msg.getMessage() == ewolEventEntryCut) {
		copySelectionToClipBoard(ewol::clipBoard::clipboardStd);
		removeSelected();
		generateEventId(eventModify, m_data);
	} else if(_msg.getMessage() == ewolEventEntryCopy) {
		copySelectionToClipBoard(ewol::clipBoard::clipboardStd);
	} else if(_msg.getMessage() == ewolEventEntryPaste) {
		ewol::clipBoard::request(ewol::clipBoard::clipboardStd);
	} else if(_msg.getMessage() == ewolEventEntrySelect) {
		if(_msg.getData() == "ALL") {
			m_displayCursorPosSelection = 0;
			m_displayCursorPos = m_data.size();
		} else {
			m_displayCursorPosSelection = m_displayCursorPos;
		}
		markToRedraw();
	}
}

void widget::Entry::markToUpdateTextPosition(void) {
	m_needUpdateTextPos=true;
}

void widget::Entry::updateTextPosition(void) {
	if (false == m_needUpdateTextPos) {
		return;
	}
	vec2 padding = m_shaper.getPadding();
	
	int32_t tmpSizeX = m_minSize.x();
	if (true == m_userFill.x()) {
		tmpSizeX = m_size.x();
	}
	int32_t tmpUserSize = tmpSizeX - 2*(padding.x());
	int32_t totalWidth = m_oObjectText.calculateSize(m_data).x();
	// Check if the data inside the display can be contain in the entry box
	if (totalWidth < tmpUserSize) {
		// all can be display :
		m_displayStartPosition = 0;
	} else {
		// all can not be set :
		std::string tmpDisplay = std::string(m_data, 0, m_displayCursorPos);
		int32_t pixelCursorPos = m_oObjectText.calculateSize(tmpDisplay).x();
		// check if the Cussor is visible at 10px nearest the border :
		int32_t tmp1 = pixelCursorPos+m_displayStartPosition;
		EWOL_DEBUG("cursorPos=" << pixelCursorPos << "px maxSize=" << tmpUserSize << "px tmp1=" << tmp1);
		if (tmp1<10) {
			// set the cursor on le left
			m_displayStartPosition = etk_min(-pixelCursorPos+10, 0);
		} else if (tmp1>tmpUserSize-10) {
			// set the cursor of the Right
			m_displayStartPosition = etk_min(-pixelCursorPos + tmpUserSize - 10, 0);
		}
		// else : the cursor is inside the display
		//m_displayStartPosition = -totalWidth + tmpUserSize;
	}
}

void widget::Entry::onGetFocus(void) {
	m_displayCursor = true;
	changeStatusIn(STATUS_SELECTED);
	showKeyboard();
	markToRedraw();
}

void widget::Entry::onLostFocus(void) {
	m_displayCursor = false;
	changeStatusIn(STATUS_NORMAL);
	hideKeyboard();
	markToRedraw();
}

void widget::Entry::changeStatusIn(int32_t _newStatusId) {
	if (true == m_shaper.changeStatusIn(_newStatusId) ) {
		periodicCallEnable();
		markToRedraw();
	}
}

void widget::Entry::periodicCall(const ewol::EventTime& _event) {
	if (false == m_shaper.periodicCall(_event) ) {
		periodicCallDisable();
	}
	markToRedraw();
}

void widget::Entry::setRegExp(const std::string& _expression) {
	std::string previousRegExp = m_regExp.getRegExp();
	EWOL_DEBUG("change input regExp \"" << previousRegExp << "\"  == > \"" << _expression << "\"");
	m_regExp.setRegExp(_expression);
	if (m_regExp.getStatus() == false) {
		EWOL_ERROR("error when adding regExp ...  == > set the previous back ...");
		m_regExp.setRegExp(previousRegExp);
	}
}

void widget::Entry::setColorText(const etk::Color<>& _color) {
	m_textColorFg = _color;
	markToRedraw();
}

void widget::Entry::setColorTextSelected(const etk::Color<>& _color) {
	m_textColorBg = _color;
	markToRedraw();
}

void widget::Entry::setEmptyText(const std::string& _text) {
	m_textWhenNothing = _text;
	markToRedraw();
}

bool widget::Entry::onSetConfig(const ewol::EConfig& _conf) {
	if (true == ewol::Widget::onSetConfig(_conf)) {
		return true;
	}
	if (_conf.getConfig() == configMaxChar) {
		setMaxChar(stoi(_conf.getData()));
		return true;
	}
	if (_conf.getConfig() == configRegExp) {
		setRegExp(_conf.getData());
		return true;
	}
	if (_conf.getConfig() == configColorFg) {
		setColorText(_conf.getData());
		return true;
	}
	if (_conf.getConfig() == configColorBg) {
		setColorTextSelected(_conf.getData());
		return true;
	}
	if (_conf.getConfig() == configEmptyMessage) {
		setEmptyText(_conf.getData());
		return true;
	}
	return false;
}

bool widget::Entry::onGetConfig(const char* _config, std::string& _result) const {
	if (true == ewol::Widget::onGetConfig(_config, _result)) {
		return true;
	}
	if (_config == configMaxChar) {
		_result = std::to_string(getMaxChar());
		return true;
	}
	if (_config == configRegExp) {
		_result = getRegExp();
		return true;
	}
	if (_config == configColorFg) {
		_result = getColorText().getString();
		return true;
	}
	if (_config == configColorBg) {
		_result = getColorTextSelected().getString();
		return true;
	}
	if (_config == configEmptyMessage) {
		_result = getEmptyText();
		return true;
	}
	return false;
}



