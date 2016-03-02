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
  signalClick(this, "click", "the user Click on the Entry box"),
  signalEnter(this, "enter", "The cursor enter inside the button"),
  signalModify(this, "modify", "Entry box value change"),
  propertyShaper(this, "shaper",
                       "",
                       "Shaper to display the background",
                       &ewol::widget::Entry::onChangePropertyShaper),
  propertyValue(this, "value",
                      "",
                      "Value display in the entry (decorated text)",
                      &ewol::widget::Entry::onChangePropertyValue),
  propertyMaxCharacter(this, "max",
                             0x7FFFFFFF, 0, 0x7FFFFFFF,
                             "Maximum char that can be set on the Entry",
                             &ewol::widget::Entry::onChangePropertyMaxCharacter),
  propertyRegex(this, "regex",
                      ".*",
                      "Control what it is write with a regular expression",
                      &ewol::widget::Entry::onChangePropertyRegex),
  propertyTextWhenNothing(this, "empty-text",
                                "",
                                "Text when nothing is written",
                                &ewol::widget::Entry::onChangePropertyTextWhenNothing),
  m_needUpdateTextPos(true),
  m_displayStartPosition(0),
  m_displayCursor(false),
  m_displayCursorPos(0),
  m_displayCursorPosSelection(0) {
	addObjectType("ewol::widget::Entry");
}

void ewol::widget::Entry::init(const std::string& _newData, const std::string& _shaperName) {
	ewol::Widget::init();
	propertyValue.set(_newData);
	propertyShaper.set(_shaperName);
	propertyCanFocus.set(true);
	
	try {
		m_regex.assign(".*", std::regex_constants::optimize | std::regex_constants::ECMAScript);
	} catch (std::regex_error e) {
		EWOL_ERROR("can not parse regex : '" << e.what() << "' for : " << propertyRegex);
	}
	markToRedraw();
	
	shortCutAdd("ctrl+w", "clean");
	shortCutAdd("ctrl+x", "cut");
	shortCutAdd("ctrl+c", "copy");
	shortCutAdd("ctrl+v", "paste");
	shortCutAdd("ctrl+a", "select:all");
	shortCutAdd("ctrl+shift+a", "select:none");
	signalShortcut.connect(shared_from_this(), &ewol::widget::Entry::onCallbackShortCut);
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
	ewol::Padding padding = m_shaper.getPadding();
	int32_t minHeight = m_text.calculateSize(char32_t('A')).y();
	vec2 minimumSizeBase(20, minHeight);
	// add padding :
	minimumSizeBase += vec2(padding.x(), padding.y());
	m_minSize.setMax(minimumSizeBase);
	// verify the min max of the min size ...
	checkMinSize();
}


void ewol::widget::Entry::onDraw() {
	m_shaper.draw();
	m_text.draw();
}


void ewol::widget::Entry::onRegenerateDisplay() {
	if (needRedraw() == true) {
		m_shaper.clear();
		m_text.clear();
		if (m_colorIdTextFg >= 0) {
			m_text.setDefaultColorFg(m_shaper.getColor(m_colorIdTextFg));
			m_text.setDefaultColorBg(m_shaper.getColor(m_colorIdTextBg));
			m_text.setCursorColor(m_shaper.getColor(m_colorIdCursor));
			m_text.setSelectionColor(m_shaper.getColor(m_colorIdSelection));
		}
		updateTextPosition();
		ewol::Padding padding = m_shaper.getPadding();
		
		vec2 tmpSizeShaper = m_minSize;
		if (propertyFill->x() == true) {
			tmpSizeShaper.setX(m_size.x());
		}
		if (propertyFill->y() == true) {
			tmpSizeShaper.setY(m_size.y());
		}
		
		vec2 tmpOriginShaper = (m_size - tmpSizeShaper) / 2.0f;
		vec2 tmpSizeText = tmpSizeShaper - vec2(padding.x(), padding.y());
		vec2 tmpOriginText = (m_size - tmpSizeText) / 2.0f;
		// sometimes, the user define an height bigger than the real size needed  == > in this case we need to center the text in the shaper ...
		int32_t minHeight = m_text.calculateSize(char32_t('A')).y();
		if (tmpSizeText.y() > minHeight) {
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
		if (propertyValue->size() != 0) {
			m_text.print(propertyValue);
		} else {
			if (propertyTextWhenNothing->size() != 0) {
				m_text.printDecorated(propertyTextWhenNothing);
			}
		}
		m_text.setClippingMode(false);
		
		m_shaper.setShape(tmpOriginShaper, tmpSizeShaper, tmpOriginText, tmpSizeText);
	}
}


void ewol::widget::Entry::updateCursorPosition(const vec2& _pos, bool _selection) {
	ewol::Padding padding = m_shaper.getPadding();
	
	vec2 relPos = relativePosition(_pos);
	relPos.setX(relPos.x()-m_displayStartPosition - padding.xLeft());
	// try to find the new cursor position :
	std::string tmpDisplay = std::string(propertyValue, 0, m_displayStartPosition);
	int32_t displayHidenSize = m_text.calculateSize(tmpDisplay).x();
	//EWOL_DEBUG("hidenSize : " << displayHidenSize);
	int32_t newCursorPosition = -1;
	int32_t tmpTextOriginX = padding.xLeft();
	for (size_t iii=0; iii<propertyValue->size(); iii++) {
		tmpDisplay = std::string(propertyValue, 0, iii);
		int32_t tmpWidth = m_text.calculateSize(tmpDisplay).x() - displayHidenSize;
		if (tmpWidth >= relPos.x()-tmpTextOriginX) {
			newCursorPosition = iii;
			break;
		}
	}
	if (newCursorPosition == -1) {
		newCursorPosition = propertyValue->size();
	}
	if (_selection == false) {
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
	propertyValue.getDirect().erase(pos1, pos2-pos1);
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
	std::string tmpData = std::string(propertyValue, pos1, pos2);
	gale::context::clipBoard::set(_clipboardID, tmpData);
}


bool ewol::widget::Entry::onEventInput(const ewol::event::Input& _event) {
	//EWOL_DEBUG("Event on Entry ... type=" << (int32_t)type << " id=" << IdInput);
	if (_event.getId() == 1) {
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
			for (size_t iii=m_displayCursorPos; iii <= propertyValue->size(); iii++) {
				if(iii == propertyValue->size()) {
					m_displayCursorPos = iii;
					break;
				}
				if(!(    (    propertyValue.get()[iii] >= 'a'
				           && propertyValue.get()[iii] <= 'z')
				      || (    propertyValue.get()[iii] >= 'A'
				           && propertyValue.get()[iii] <= 'Z')
				      || (    propertyValue.get()[iii] >= '0'
				           && propertyValue.get()[iii] <= '9')
				      || propertyValue.get()[iii] == '_'
				      || propertyValue.get()[iii] == '-'
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
				if(!(    (    propertyValue.get()[iii] >= 'a'
				           && propertyValue.get()[iii] <= 'z')
				      || (    propertyValue.get()[iii] >= 'A'
				           && propertyValue.get()[iii] <= 'Z')
				      || (    propertyValue.get()[iii] >= '0'
				           && propertyValue.get()[iii] <= '9')
				      || propertyValue.get()[iii] == '_'
				      || propertyValue.get()[iii] == '-'
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
			m_displayCursorPos = propertyValue->size();
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
				signalEnter.emit(propertyValue);
				return true;
			} else if (_event.getChar() == 0x7F) {
				// SUPPR :
				if (propertyValue->size() > 0 && m_displayCursorPos < (int64_t)propertyValue->size()) {
					propertyValue.getDirect().erase(m_displayCursorPos, 1);
					m_displayCursorPos = std::max(m_displayCursorPos, 0);
					m_displayCursorPosSelection = m_displayCursorPos;
				}
			} else if (_event.getChar() == 0x08) {
				// DEL :
				if (propertyValue->size() > 0 && m_displayCursorPos != 0) {
					propertyValue.getDirect().erase(m_displayCursorPos-1, 1);
					m_displayCursorPos--;
					m_displayCursorPos = std::max(m_displayCursorPos, 0);
					m_displayCursorPosSelection = m_displayCursorPos;
				}
			} else if(_event.getChar() >= 20) {
				if ((int64_t)propertyValue->size() > propertyMaxCharacter) {
					EWOL_INFO("Reject data for entry : '" << _event.getChar() << "'");
				} else {
					std::string newData = propertyValue;
					newData.insert(newData.begin()+m_displayCursorPos, _event.getChar());
					setInternalValue(newData);
					if (propertyValue.get() == newData) {
						m_displayCursorPos++;
						m_displayCursorPosSelection = m_displayCursorPos;
					}
				}
			}
			signalModify.emit(propertyValue);
			markToRedraw();
			return true;
		}
		return false;
	} else {
		if(_event.getStatus() == gale::key::status_down) {
			switch (_event.getType()) {
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
					m_displayCursorPos = propertyValue->size();
					break;
				default:
					return false;
			}
			m_displayCursorPos = std::avg(0, m_displayCursorPos, (int32_t)propertyValue->size());
			m_displayCursorPosSelection = m_displayCursorPos;
			markToRedraw();
			return true;
		}
	}
	return false;
}

void ewol::widget::Entry::setInternalValue(const std::string& _newData) {
	std::string previous = propertyValue;
	// check the RegExp :
	if (_newData.size()>0) {
		std::smatch resultMatch;
		std::regex_search(_newData.begin(), _newData.end(), resultMatch, m_regex, std::regex_constants::match_continuous);
		if (resultMatch.size() <= 0) {
			EWOL_INFO("The input data does not match with the regExp '" << _newData << "' Regex='" << propertyRegex << "'" );
			return;
		}
		if (_newData.begin() != resultMatch[0].first) {
			EWOL_INFO("The input data does not match with the regExp '" << _newData << "' Regex='" << propertyRegex << "' (start position error)" );
			return;
		}
		if (_newData.end() !=  resultMatch[0].second) {
			EWOL_INFO("The input data does not match with the regExp '" << _newData << "' Regex='" << propertyRegex << "' (stop position error)" );
			return;
		}
	}
	propertyValue.setDirect(_newData);
	markToRedraw();
}

void ewol::widget::Entry::onEventClipboard(enum gale::context::clipBoard::clipboardListe _clipboardID) {
	// remove curent selected data ...
	removeSelected();
	// get current selection / Copy :
	std::string tmpData = get(_clipboardID);
	// add it on the current display :
	if (tmpData.size() != 0) {
		std::string newData = propertyValue;
		newData.insert(m_displayCursorPos, &tmpData[0]);
		setInternalValue(newData);
		if (propertyValue.get() == newData) {
			if (propertyValue->size() == tmpData.size()) {
				m_displayCursorPos = tmpData.size();
			} else {
				m_displayCursorPos += tmpData.size();
			}
			m_displayCursorPosSelection = m_displayCursorPos;
			markToRedraw();
		}
	}
	signalModify.emit(propertyValue);
}

void ewol::widget::Entry::onCallbackEntryClean() {
	propertyValue.setDirect("");
	m_displayStartPosition = 0;
	m_displayCursorPos = 0;
	m_displayCursorPosSelection = m_displayCursorPos;
	markToRedraw();
}

void ewol::widget::Entry::onCallbackCut() {
	copySelectionToClipBoard(gale::context::clipBoard::clipboardStd);
	removeSelected();
	signalModify.emit(propertyValue);
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
		m_displayCursorPos = propertyValue->size();
	} else {
		m_displayCursorPosSelection = m_displayCursorPos;
	}
	markToRedraw();
}

void ewol::widget::Entry::markToUpdateTextPosition() {
	m_needUpdateTextPos = true;
}

void ewol::widget::Entry::updateTextPosition() {
	if (m_needUpdateTextPos == false) {
		return;
	}
	ewol::Padding padding = m_shaper.getPadding();
	
	int32_t tmpSizeX = m_minSize.x();
	if (propertyFill->x() == true) {
		tmpSizeX = m_size.x();
	}
	int32_t tmpUserSize = tmpSizeX - padding.x();
	int32_t totalWidth = m_text.calculateSize(propertyValue).x();
	// Check if the data inside the display can be contain in the entry box
	if (totalWidth < tmpUserSize) {
		// all can be display :
		m_displayStartPosition = 0;
	} else {
		// all can not be set :
		std::string tmpDisplay = std::string(propertyValue, 0, m_displayCursorPos);
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
	if (m_shaper.changeStatusIn(_newStatusId) == true) {
		periodicCallEnable();
		markToRedraw();
	}
}

void ewol::widget::Entry::periodicCall(const ewol::event::Time& _event) {
	if (m_shaper.periodicCall(_event) == false) {
		periodicCallDisable();
	}
	markToRedraw();
}

void ewol::widget::Entry::onChangePropertyShaper() {
	m_shaper.setSource(propertyShaper.get());
	m_colorIdTextFg = m_shaper.requestColor("text-foreground");
	m_colorIdTextBg = m_shaper.requestColor("text-background");
	m_colorIdCursor = m_shaper.requestColor("text-cursor");
	m_colorIdSelection = m_shaper.requestColor("text-selection");
}

void ewol::widget::Entry::onChangePropertyValue() {
	std::string newData = propertyValue.get();
	if ((int64_t)newData.size() > propertyMaxCharacter) {
		newData = std::string(newData, 0, propertyMaxCharacter);
		EWOL_DEBUG("Limit entry set of data... " << std::string(newData, propertyMaxCharacter));
	}
	// set the value with the check of the RegExp ...
	setInternalValue(newData);
	if (newData == propertyValue.get()) {
		m_displayCursorPos = propertyValue->size();
		m_displayCursorPosSelection = m_displayCursorPos;
		EWOL_VERBOSE("Set : '" << newData << "'");
	}
	markToRedraw();
}

void ewol::widget::Entry::onChangePropertyMaxCharacter() {
	// TODO : check nomber of char in the data
}

void ewol::widget::Entry::onChangePropertyRegex() {
	try {
		m_regex.assign(propertyRegex.get(), std::regex_constants::optimize | std::regex_constants::ECMAScript);
	} catch (std::regex_error e) {
		EWOL_ERROR("can not parse regex : '" << e.what() << "' for : " << propertyRegex);
	}
	markToRedraw();
}

void ewol::widget::Entry::onChangePropertyTextWhenNothing() {
	markToRedraw();
}

