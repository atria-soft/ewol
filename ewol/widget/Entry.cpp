/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license APACHE v2.0 (see license file)
 */

#include <etk/types.h>
#include <ewol/widget/Entry.h>
#include <ewol/widget/Manager.h>
#include <ewol/ewol.h>
#include <ewol/context/Context.h>
#include <ewol/Padding.h>

#undef __class__
#define __class__ "Entry"

// DEFINE for the shader display system :
#define STATUS_NORMAL    (0)
#define STATUS_HOVER     (1)
#define STATUS_SELECTED  (2)

ewol::widget::Entry::Entry() :
  signalClick(*this, "click", "the user Click on the Entry box"),
  signalEnter(*this, "enter", "The cursor enter inside the button"),
  signalModify(*this, "modify", "Entry box value change"),
  m_shaper(*this, "shaper", "Shaper to display the background"),
  m_data(*this, "value", "", "Value display in the entry (decorated text)"),
  m_maxCharacter(*this, "max", 0x7FFFFFFF, 0, 0x7FFFFFFF, "Maximum cgar that can be set on the Entry"),
  m_regexValue(*this, "regex", ".*", "Control what it is write with a regular expression"),
  m_needUpdateTextPos(true),
  m_displayStartPosition(0),
  m_displayCursor(false),
  m_displayCursorPos(0),
  m_displayCursorPosSelection(0),
  m_textWhenNothing(*this, "emptytext", "", "Text that is displayed when the Entry is empty (decorated text)") {
	addObjectType("ewol::widget::Entry");
}

void ewol::widget::Entry::init(const std::string& _newData) {
	ewol::Widget::init();
	m_data.set(_newData);
	m_shaper.setString("{ewol}THEME:GUI:Entry.json");
	setCanHaveFocus(true);
	
	try {
		m_regex.assign(".*", std::regex_constants::optimize | std::regex_constants::ECMAScript);
	} catch (std::regex_error e) {
		EWOL_ERROR("can not parse regex : '" << e.what() << "' for : " << m_regexValue);
	}
	markToRedraw();
	
	shortCutAdd("ctrl+w", "clean");
	shortCutAdd("ctrl+x", "cut");
	shortCutAdd("ctrl+c", "copy");
	shortCutAdd("ctrl+v", "paste");
	shortCutAdd("ctrl+a", "select:all");
	shortCutAdd("ctrl+shift+a", "select:none");
	signalShortcut.bind(shared_from_this(), &ewol::widget::Entry::onCallbackShortCut);
}


ewol::widget::Entry::~Entry() {
	
}

void ewol::widget::Entry::onCallbackShortCut(const std::string& _value) {
	if (_value == "clean") {
		onCallbackEntryClean();
	} else if (_value == "cut") {
		onCallbackCut();
	} else if (_value == "copy") {
		onCallbackCopy();
	} else if (_value == "paste") {
		onCallbackPaste();
	} else if (_value == "select:all") {
		onCallbackSelect(true);
	} else if (_value == "select:none") {
		onCallbackSelect(false);
	} else {
		EWOL_WARNING("Unknow event from ShortCut : " << _value);
	}
}

void ewol::widget::Entry::calculateMinMaxSize() {
	// call main class
	ewol::Widget::calculateMinMaxSize();
	// get generic padding
	ewol::Padding padding = m_shaper->getPadding();
	int32_t minHeight = m_text.calculateSize(char32_t('A')).y();
	vec2 minimumSizeBase(20, minHeight);
	// add padding :
	minimumSizeBase += vec2(padding.x(), padding.y());
	m_minSize.setMax(minimumSizeBase);
	// verify the min max of the min size ...
	checkMinSize();
}

// TODO : ... Set it a a generic parameter...
void ewol::widget::Entry::setValue(const std::string& _newData) {
	std::string newData = _newData;
	if ((int64_t)newData.size() > m_maxCharacter) {
		newData = std::string(_newData, 0, m_maxCharacter);
		EWOL_DEBUG("Limit entry set of data... " << std::string(_newData, m_maxCharacter));
	}
	// set the value with the check of the RegExp ...
	setInternalValue(newData);
	if (newData == m_data.get()) {
		m_displayCursorPos = m_data->size();
		m_displayCursorPosSelection = m_displayCursorPos;
		EWOL_VERBOSE("Set : '" << newData << "'");
	}
	markToRedraw();
}


void ewol::widget::Entry::onDraw() {
	m_shaper->draw();
	m_text.draw();
}


void ewol::widget::Entry::onRegenerateDisplay() {
	if (true == needRedraw()) {
		m_shaper->clear();
		m_text.clear();
		if (m_colorIdTextFg >= 0) {
			m_text.setDefaultColorFg(m_shaper->getColor(m_colorIdTextFg));
			m_text.setDefaultColorBg(m_shaper->getColor(m_colorIdTextBg));
			m_text.setCursorColor(m_shaper->getColor(m_colorIdCursor));
			m_text.setSelectionColor(m_shaper->getColor(m_colorIdSelection));
		}
		updateTextPosition();
		ewol::Padding padding = m_shaper->getPadding();
		
		vec2 tmpSizeShaper = m_minSize;
		if (true == m_userFill->x()) {
			tmpSizeShaper.setX(m_size.x());
		}
		if (true == m_userFill->y()) {
			tmpSizeShaper.setY(m_size.y());
		}
		
		vec2 tmpOriginShaper = (m_size - tmpSizeShaper) / 2.0f;
		vec2 tmpSizeText = tmpSizeShaper - vec2(padding.x(), padding.y());
		vec2 tmpOriginText = (m_size - tmpSizeText) / 2.0f;
		// sometimes, the user define an height bigger than the real size needed  == > in this case we need to center the text in the shaper ...
		int32_t minHeight = m_text.calculateSize(char32_t('A')).y();
		if (tmpSizeText.y()>minHeight) {
			tmpOriginText += vec2(0,(tmpSizeText.y()-minHeight)/2.0f);
		}
		// fix all the position in the int32_t class:
		tmpSizeShaper = vec2ClipInt32(tmpSizeShaper);
		tmpOriginShaper = vec2ClipInt32(tmpOriginShaper);
		tmpSizeText = vec2ClipInt32(tmpSizeText);
		tmpOriginText = vec2ClipInt32(tmpOriginText);
		
		m_text.reset();
		m_text.setClippingWidth(tmpOriginText, tmpSizeText);
		m_text.setPos(tmpOriginText+vec2(m_displayStartPosition,0));
		if (m_displayCursorPosSelection != m_displayCursorPos) {
			m_text.setCursorSelection(m_displayCursorPos, m_displayCursorPosSelection);
		} else {
			m_text.setCursorPos(m_displayCursorPos);
		}
		if (0!=m_data->size()) {
			m_text.print(m_data);
		} else {
			if (0!=m_textWhenNothing->size()) {
				m_text.printDecorated(m_textWhenNothing);
			}
		}
		m_text.setClippingMode(false);
		
		m_shaper->setShape(tmpOriginShaper, tmpSizeShaper, tmpOriginText, tmpSizeText);
	}
}


void ewol::widget::Entry::updateCursorPosition(const vec2& _pos, bool _selection) {
	ewol::Padding padding = m_shaper->getPadding();
	
	vec2 relPos = relativePosition(_pos);
	relPos.setX(relPos.x()-m_displayStartPosition - padding.xLeft());
	// try to find the new cursor position :
	std::string tmpDisplay = std::string(m_data, 0, m_displayStartPosition);
	int32_t displayHidenSize = m_text.calculateSize(tmpDisplay).x();
	//EWOL_DEBUG("hidenSize : " << displayHidenSize);
	int32_t newCursorPosition = -1;
	int32_t tmpTextOriginX = padding.xLeft();
	for (size_t iii=0; iii<m_data->size(); iii++) {
		tmpDisplay = std::string(m_data, 0, iii);
		int32_t tmpWidth = m_text.calculateSize(tmpDisplay).x() - displayHidenSize;
		if (tmpWidth >= relPos.x()-tmpTextOriginX) {
			newCursorPosition = iii;
			break;
		}
	}
	if (newCursorPosition == -1) {
		newCursorPosition = m_data->size();
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


void ewol::widget::Entry::removeSelected() {
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
	m_data->erase(pos1, pos2-pos1);
	markToRedraw();
}


void ewol::widget::Entry::copySelectionToClipBoard(enum gale::context::clipBoard::clipboardListe _clipboardID) {
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
	gale::context::clipBoard::set(_clipboardID, tmpData);
}


bool ewol::widget::Entry::onEventInput(const ewol::event::Input& _event) {
	//EWOL_DEBUG("Event on Entry ... type=" << (int32_t)type << " id=" << IdInput);
	if (1 == _event.getId()) {
		if (gale::key::status_single == _event.getStatus()) {
			keepFocus();
			signalClick.emit();
			//nothing to do ...
			return true;
		} else if (gale::key::status_double == _event.getStatus()) {
			keepFocus();
			// select word
			m_displayCursorPosSelection = m_displayCursorPos-1;
			// search forward
			for (size_t iii=m_displayCursorPos; iii <= m_data->size(); iii++) {
				if(iii == m_data->size()) {
					m_displayCursorPos = iii;
					break;
				}
				if(!(    (    m_data.get()[iii] >= 'a'
				           && m_data.get()[iii] <= 'z')
				      || (    m_data.get()[iii] >= 'A'
				           && m_data.get()[iii] <= 'Z')
				      || (    m_data.get()[iii] >= '0'
				           && m_data.get()[iii] <= '9')
				      || m_data.get()[iii] == '_'
				      || m_data.get()[iii] == '-'
				  ) ) {
					m_displayCursorPos = iii;
					break;
				}
			}
			// search backward
			for (int64_t iii=m_displayCursorPosSelection; iii >= -1; iii--) {
				if(iii == -1) {
					m_displayCursorPosSelection = 0;
					break;
				}
				if(!(    (    m_data.get()[iii] >= 'a'
				           && m_data.get()[iii] <= 'z')
				      || (    m_data.get()[iii] >= 'A'
				           && m_data.get()[iii] <= 'Z')
				      || (    m_data.get()[iii] >= '0'
				           && m_data.get()[iii] <= '9')
				      || m_data.get()[iii] == '_'
				      || m_data.get()[iii] == '-'
				  ) ) {
					m_displayCursorPosSelection = iii+1;
					break;
				}
			}
			// Copy to clipboard Middle ...
			copySelectionToClipBoard(gale::context::clipBoard::clipboardSelection);
			markToRedraw();
		} else if (gale::key::status_triple == _event.getStatus()) {
			keepFocus();
			m_displayCursorPosSelection = 0;
			m_displayCursorPos = m_data->size();
		} else if (gale::key::status_down == _event.getStatus()) {
			keepFocus();
			updateCursorPosition(_event.getPos());
			markToRedraw();
		} else if (gale::key::status_move == _event.getStatus()) {
			keepFocus();
			updateCursorPosition(_event.getPos(), true);
			markToRedraw();
		} else if (gale::key::status_up == _event.getStatus()) {
			keepFocus();
			updateCursorPosition(_event.getPos(), true);
			// Copy to clipboard Middle ...
			copySelectionToClipBoard(gale::context::clipBoard::clipboardSelection);
			markToRedraw();
		}
	}
	else if(    gale::key::type_mouse == _event.getType()
	         && _event.getId() == 2) {
		if(    _event.getStatus() == gale::key::status_down
		    || _event.getStatus() == gale::key::status_move
		    || _event.getStatus() == gale::key::status_up) {
			keepFocus();
			// updatethe cursor position : 
			updateCursorPosition(_event.getPos());
		}
		// Paste current selection only when up button
		if (_event.getStatus() == gale::key::status_up) {
			keepFocus();
			// middle button => past data...
			gale::context::clipBoard::request(gale::context::clipBoard::clipboardSelection);
		}
	}
	return false;
}


bool ewol::widget::Entry::onEventEntry(const ewol::event::Entry& _event) {
	if (_event.getType() == gale::key::keyboard_char) {
		if(_event.getStatus() == gale::key::status_down) {
			// remove curent selected data ...
			removeSelected();
			if(    _event.getChar() == '\n'
			    || _event.getChar() == '\r') {
				signalEnter.emit(m_data);
				return true;
			} else if (_event.getChar() == 0x7F) {
				// SUPPR :
				if (m_data->size() > 0 && m_displayCursorPos < (int64_t)m_data->size()) {
					m_data->erase(m_displayCursorPos, 1);
					m_displayCursorPos = std::max(m_displayCursorPos, 0);
					m_displayCursorPosSelection = m_displayCursorPos;
				}
			} else if (_event.getChar() == 0x08) {
				// DEL :
				if (m_data->size() > 0 && m_displayCursorPos != 0) {
					m_data->erase(m_displayCursorPos-1, 1);
					m_displayCursorPos--;
					m_displayCursorPos = std::max(m_displayCursorPos, 0);
					m_displayCursorPosSelection = m_displayCursorPos;
				}
			} else if(_event.getChar() >= 20) {
				if ((int64_t)m_data->size() > m_maxCharacter) {
					EWOL_INFO("Reject data for entry : '" << _event.getChar() << "'");
				} else {
					std::string newData = m_data;
					newData.insert(newData.begin()+m_displayCursorPos, _event.getChar());
					setInternalValue(newData);
					if (m_data.get() == newData) {
						m_displayCursorPos++;
						m_displayCursorPosSelection = m_displayCursorPos;
					}
				}
			}
			signalModify.emit(m_data);
			markToRedraw();
			return true;
		}
		return false;
	} else {
		if(_event.getStatus() == gale::key::status_down) {
			switch (_event.getType())
			{
				case gale::key::keyboard_left:
					m_displayCursorPos--;
					break;
				case gale::key::keyboard_right:
					m_displayCursorPos++;
					break;
				case gale::key::keyboard_start:
					m_displayCursorPos = 0;
					break;
				case gale::key::keyboard_end:
					m_displayCursorPos = m_data->size();
					break;
				default:
					return false;
			}
			m_displayCursorPos = std::avg(0, m_displayCursorPos, (int32_t)m_data->size());
			m_displayCursorPosSelection = m_displayCursorPos;
			markToRedraw();
			return true;
		}
	}
	return false;
}

void ewol::widget::Entry::setInternalValue(const std::string& _newData) {
	std::string previous = m_data;
	// check the RegExp :
	if (_newData.size()>0) {
		std::smatch resultMatch;
		std::regex_search(_newData.begin(), _newData.end(), resultMatch, m_regex, std::regex_constants::match_continuous);
		if (resultMatch.size() <= 0) {
			EWOL_INFO("The input data does not match with the regExp '" << _newData << "' Regex='" << m_regexValue << "'" );
			return;
		}
		if (_newData.begin() != resultMatch[0].first) {
			EWOL_INFO("The input data does not match with the regExp '" << _newData << "' Regex='" << m_regexValue << "' (start position error)" );
			return;
		}
		if (_newData.end() !=  resultMatch[0].second) {
			EWOL_INFO("The input data does not match with the regExp '" << _newData << "' Regex='" << m_regexValue << "' (stop position error)" );
			return;
		}
	}
	m_data = _newData;
	markToRedraw();
}

void ewol::widget::Entry::onEventClipboard(enum gale::context::clipBoard::clipboardListe _clipboardID) {
	// remove curent selected data ...
	removeSelected();
	// get current selection / Copy :
	std::string tmpData = get(_clipboardID);
	// add it on the current display :
	if (tmpData.size() != 0) {
		std::string newData = m_data;
		newData.insert(m_displayCursorPos, &tmpData[0]);
		setInternalValue(newData);
		if (m_data.get() == newData) {
			if (m_data->size() == tmpData.size()) {
				m_displayCursorPos = tmpData.size();
			} else {
				m_displayCursorPos += tmpData.size();
			}
			m_displayCursorPosSelection = m_displayCursorPos;
			markToRedraw();
		}
	}
	signalModify.emit(m_data);
}

void ewol::widget::Entry::onCallbackEntryClean() {
	m_data = "";
	m_displayStartPosition = 0;
	m_displayCursorPos = 0;
	m_displayCursorPosSelection = m_displayCursorPos;
	markToRedraw();
}

void ewol::widget::Entry::onCallbackCut() {
	copySelectionToClipBoard(gale::context::clipBoard::clipboardStd);
	removeSelected();
	signalModify.emit(m_data);
}

void ewol::widget::Entry::onCallbackCopy() {
	copySelectionToClipBoard(gale::context::clipBoard::clipboardStd);
}

void ewol::widget::Entry::onCallbackPaste() {
	gale::context::clipBoard::request(gale::context::clipBoard::clipboardStd);
}

void ewol::widget::Entry::onCallbackSelect(bool _all) {
	if(_all == true) {
		m_displayCursorPosSelection = 0;
		m_displayCursorPos = m_data->size();
	} else {
		m_displayCursorPosSelection = m_displayCursorPos;
	}
	markToRedraw();
}

void ewol::widget::Entry::markToUpdateTextPosition() {
	m_needUpdateTextPos=true;
}

void ewol::widget::Entry::updateTextPosition() {
	if (false == m_needUpdateTextPos) {
		return;
	}
	ewol::Padding padding = m_shaper->getPadding();
	
	int32_t tmpSizeX = m_minSize.x();
	if (true == m_userFill->x()) {
		tmpSizeX = m_size.x();
	}
	int32_t tmpUserSize = tmpSizeX - padding.x();
	int32_t totalWidth = m_text.calculateSize(m_data).x();
	// Check if the data inside the display can be contain in the entry box
	if (totalWidth < tmpUserSize) {
		// all can be display :
		m_displayStartPosition = 0;
	} else {
		// all can not be set :
		std::string tmpDisplay = std::string(m_data, 0, m_displayCursorPos);
		int32_t pixelCursorPos = m_text.calculateSize(tmpDisplay).x();
		// check if the Cussor is visible at 10px nearest the border :
		int32_t tmp1 = pixelCursorPos+m_displayStartPosition;
		EWOL_DEBUG("cursorPos=" << pixelCursorPos << "px maxSize=" << tmpUserSize << "px tmp1=" << tmp1);
		if (tmp1<10) {
			// set the cursor on le left
			m_displayStartPosition = std::min(-pixelCursorPos+10, 0);
		} else if (tmp1>tmpUserSize-10) {
			// set the cursor of the Right
			m_displayStartPosition = std::min(-pixelCursorPos + tmpUserSize - 10, 0);
		}
		// else : the cursor is inside the display
		//m_displayStartPosition = -totalWidth + tmpUserSize;
	}
}

void ewol::widget::Entry::onGetFocus() {
	m_displayCursor = true;
	changeStatusIn(STATUS_SELECTED);
	showKeyboard();
	markToRedraw();
}

void ewol::widget::Entry::onLostFocus() {
	m_displayCursor = false;
	changeStatusIn(STATUS_NORMAL);
	hideKeyboard();
	markToRedraw();
}

void ewol::widget::Entry::changeStatusIn(int32_t _newStatusId) {
	if (true == m_shaper->changeStatusIn(_newStatusId) ) {
		periodicCallEnable();
		markToRedraw();
	}
}

void ewol::widget::Entry::periodicCall(const ewol::event::Time& _event) {
	if (false == m_shaper->periodicCall(_event) ) {
		periodicCallDisable();
	}
	markToRedraw();
}

void ewol::widget::Entry::onParameterChangeValue(const ewol::parameter::Ref& _paramPointer) {
	ewol::Widget::onParameterChangeValue(_paramPointer);
	if (_paramPointer == m_shaper) {
		m_colorIdTextFg = m_shaper->requestColor("text-foreground");
		m_colorIdTextBg = m_shaper->requestColor("text-background");
		m_colorIdCursor = m_shaper->requestColor("text-cursor");
		m_colorIdSelection = m_shaper->requestColor("text-selection");
	} else if (_paramPointer == m_data) {
		// to late to update data ... with control.
		markToRedraw();
	} else if (_paramPointer == m_maxCharacter) {
		// nothing to do ...
	} else if (_paramPointer == m_regexValue) {
		try {
			m_regex.assign(m_regexValue.get(), std::regex_constants::optimize | std::regex_constants::ECMAScript);
		} catch (std::regex_error e) {
			EWOL_ERROR("can not parse regex : '" << e.what() << "' for : " << m_regexValue);
		}
		markToRedraw();
	} else if (_paramPointer == m_textWhenNothing) {
		markToRedraw();
	}
}


