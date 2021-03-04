/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */

#include <etk/types.hpp>
#include <ewol/widget/Entry.hpp>
#include <ewol/widget/Manager.hpp>
#include <ewol/ewol.hpp>
#include <ewol/context/Context.hpp>
#include <ewol/Padding.hpp>
#include <etk/typeInfo.hpp>
ETK_DECLARE_TYPE(ewol::widget::Entry);

// DEFINE for the shader display system :
#define STATUS_NORMAL    (0)
#define STATUS_HOVER     (1)
#define STATUS_SELECTED  (2)

ewol::widget::Entry::Entry() :
  signalClick(this, "click", "the user Click on the Entry box"),
  signalEnter(this, "enter", "The cursor enter inside the button"),
  signalModify(this, "modify", "Entry box value change"),
  propertyPassword(this, "password",
                         false,
                         "Not display content in password mode",
                         ewol::widget::Entry::onChangePropertyPassword),
  propertyShape(this, "shape",
                      etk::Uri("THEME_GUI:///Entry.json?lib=ewol"),
                      "Shaper to display the background",
                      ewol::widget::Entry::onChangePropertyShaper),
  propertyValue(this, "value",
                      "",
                      "Value display in the entry (decorated text)",
                      ewol::widget::Entry::onChangePropertyValue),
  propertyMaxCharacter(this, "max",
                             0x7FFFFFFF, 0, 0x7FFFFFFF,
                             "Maximum char that can be set on the Entry",
                             ewol::widget::Entry::onChangePropertyMaxCharacter),
  propertyRegex(this, "regex",
                      ".*",
                      "Control what it is write with a regular expression",
                      ewol::widget::Entry::onChangePropertyRegex),
  propertyTextWhenNothing(this, "empty-text",
                                "",
                                "Text when nothing is written",
                                ewol::widget::Entry::onChangePropertyTextWhenNothing),
  this.needUpdateTextPos(true),
  this.displayStartPosition(0),
  this.displayCursor(false),
  this.displayCursorPos(0),
  this.displayCursorPosSelection(0) {
	addObjectType("ewol::widget::Entry");
	propertyCanFocus.setDirectCheck(true);
}

void ewol::widget::Entry::init() {
	Widget::init();
	propertyShape.notifyChange();
	
	this.regex.compile(propertyRegex.get());
	if (this.regex.getStatus() == false) {
		Log.error("can not parse regex for : " + propertyRegex);
	}
	markToRedraw();
	
	shortCutAdd("ctrl+w", "clean");
	shortCutAdd("ctrl+x", "cut");
	shortCutAdd("ctrl+c", "copy");
	shortCutAdd("ctrl+v", "paste");
	shortCutAdd("ctrl+a", "select:all");
	shortCutAdd("ctrl+shift+a", "select:none");
	signalShortcut.connect(sharedFromThis(), ewol::widget::Entry::onCallbackShortCut);
}


ewol::widget::Entry::~Entry() {
	
}

void ewol::widget::Entry::onCallbackShortCut( String _value) {
	if (_value == "clean") {
		onCallbackEntryClean();
	} else if (_value == "cut") {
		onCallbackCut();
	} else if (_value == "copy") {
		onCallbackCopy();
	} else if (_value == "paste") {
		Log.warning("Request past ...");
		onCallbackPaste();
	} else if (_value == "select:all") {
		onCallbackSelect(true);
	} else if (_value == "select:none") {
		onCallbackSelect(false);
	} else {
		Log.warning("Unknow event from ShortCut : " + _value);
	}
}

void ewol::widget::Entry::calculateMinMaxSize() {
	// call main class
	Widget::calculateMinMaxSize();
	// get generic padding
	ewol::Padding padding = this.shaper.getPadding();
	int minHeight = this.text.calculateSize(Character('A')).y();
	Vector2f minimumSizeBase(20, minHeight);
	// add padding :
	minimumSizeBase += Vector2f(padding.x(), padding.y());
	this.minSize.setMax(minimumSizeBase);
	// verify the min max of the min size ...
	checkMinSize();
}


void ewol::widget::Entry::onDraw() {
	this.shaper.draw();
	this.text.draw();
}


void ewol::widget::Entry::onRegenerateDisplay() {
	if (needRedraw() == true) {
		this.shaper.clear();
		this.text.clear();
		if (this.colorIdTextFg >= 0) {
			this.text.setDefaultColorFg(this.shaper.getColor(this.colorIdTextFg));
			this.text.setDefaultColorBg(this.shaper.getColor(this.colorIdTextBg));
			this.text.setCursorColor(this.shaper.getColor(this.colorIdCursor));
			this.text.setSelectionColor(this.shaper.getColor(this.colorIdSelection));
		}
		updateTextPosition();
		ewol::Padding padding = this.shaper.getPadding();
		
		Vector2f tmpSizeShaper = this.minSize;
		if (propertyFill.x() == true) {
			tmpSizeShaper.setX(this.size.x());
		}
		if (propertyFill.y() == true) {
			tmpSizeShaper.setY(this.size.y());
		}
		
		Vector2f tmpOriginShaper = (this.size - tmpSizeShaper) / 2.0f;
		Vector2f tmpSizeText = tmpSizeShaper - Vector2f(padding.x(), padding.y());
		Vector2f tmpOriginText = (this.size - tmpSizeText) / 2.0f;
		// sometimes, the user define an height bigger than the real size needed  == > in this case we need to center the text in the shaper ...
		int minHeight = this.text.calculateSize(Character('A')).y();
		if (tmpSizeText.y() > minHeight) {
			tmpOriginText += Vector2f(0,(tmpSizeText.y()-minHeight)/2.0f);
		}
		// fix all the position in the int class:
		tmpSizeShaper = Vector2fClipInt32(tmpSizeShaper);
		tmpOriginShaper = Vector2fClipInt32(tmpOriginShaper);
		tmpSizeText = Vector2fClipInt32(tmpSizeText);
		tmpOriginText = Vector2fClipInt32(tmpOriginText);
		
		this.text.reset();
		this.text.setClippingWidth(tmpOriginText, tmpSizeText);
		this.text.setPos(tmpOriginText+Vector2f(this.displayStartPosition,0));
		if (this.displayCursorPosSelection != this.displayCursorPos) {
			this.text.setCursorSelection(this.displayCursorPos, this.displayCursorPosSelection);
		} else {
			this.text.setCursorPos(this.displayCursorPos);
		}
		etk::UString valueToDisplay = etk::toUString(*propertyValue);
		if (*propertyPassword == true) {
			for (auto it: valueToDisplay) {
				it = '*';
			}
		}
		
		if (valueToDisplay.size() != 0) {
			this.text.print(valueToDisplay);
		} else {
			if (propertyTextWhenNothing.size() != 0) {
				this.text.printDecorated(propertyTextWhenNothing);
			}
		}
		this.text.setClippingMode(false);
		
		this.shaper.setShape(tmpOriginShaper, tmpSizeShaper, tmpOriginText, tmpSizeText);
	}
}


void ewol::widget::Entry::updateCursorPosition( Vector2f _pos, boolean _selection) {
	ewol::Padding padding = this.shaper.getPadding();
	
	Vector2f relPos = relativePosition(_pos);
	relPos.setX(relPos.x()-this.displayStartPosition - padding.xLeft());
	// try to find the new cursor position :
	String tmpDisplay = String(propertyValue, 0, this.displayStartPosition);
	int displayHidenSize = this.text.calculateSize(tmpDisplay).x();
	//Log.debug("hidenSize : " + displayHidenSize);
	int newCursorPosition = -1;
	int tmpTextOriginX = padding.xLeft();
	for (int iii=0; iii<propertyValue.size(); iii++) {
		tmpDisplay = String(propertyValue, 0, iii);
		int tmpWidth = this.text.calculateSize(tmpDisplay).x() - displayHidenSize;
		if (tmpWidth >= relPos.x()-tmpTextOriginX) {
			newCursorPosition = iii;
			break;
		}
	}
	if (newCursorPosition == -1) {
		newCursorPosition = propertyValue.size();
	}
	if (_selection == false) {
		this.displayCursorPos = newCursorPosition;
		this.displayCursorPosSelection = this.displayCursorPos;
		markToRedraw();
	} else {
		if (this.displayCursorPos == this.displayCursorPosSelection) {
			this.displayCursorPosSelection = this.displayCursorPos;
		}
		this.displayCursorPos = newCursorPosition;
		markToRedraw();
	}
	markToUpdateTextPosition();
}


void ewol::widget::Entry::removeSelected() {
	if (this.displayCursorPosSelection == this.displayCursorPos) {
		// nothing to cut ...
		return;
	}
	int pos1 = this.displayCursorPosSelection;
	int pos2 = this.displayCursorPos;
	if(this.displayCursorPosSelection>this.displayCursorPos) {
		pos2 = this.displayCursorPosSelection;
		pos1 = this.displayCursorPos;
	}
	// remove data ...
	this.displayCursorPos = pos1;
	this.displayCursorPosSelection = pos1;
	propertyValue.getDirect().erase(pos1, pos2-pos1);
	markToRedraw();
}


void ewol::widget::Entry::copySelectionToClipBoard(enum gale::context::clipBoard::clipboardListe _clipboardID) {
	if (this.displayCursorPosSelection == this.displayCursorPos) {
		// nothing to cut ...
		return;
	}
	int pos1 = this.displayCursorPosSelection;
	int pos2 = this.displayCursorPos;
	if(this.displayCursorPosSelection>this.displayCursorPos) {
		pos2 = this.displayCursorPosSelection;
		pos1 = this.displayCursorPos;
	}
	// Copy
	String tmpData = String(propertyValue, pos1, pos2);
	gale::context::clipBoard::set(_clipboardID, tmpData);
}


boolean ewol::widget::Entry::onEventInput( ewol::event::Input _event) {
	Log.warning("Event on Input ... " + _event);
	if (_event.getId() == 1) {
		if (KeyStatus::pressSingle == _event.getStatus()) {
			keepFocus();
			signalClick.emit();
			//nothing to do ...
			return true;
		} else if (KeyStatus::pressDouble == _event.getStatus()) {
			keepFocus();
			// select word
			this.displayCursorPosSelection = this.displayCursorPos-1;
			// search forward
			for (int iii=this.displayCursorPos; iii <= propertyValue.size(); iii++) {
				if(iii == propertyValue.size()) {
					this.displayCursorPos = iii;
					break;
				}
				if(!(    (    propertyValue.get()[iii] >= 'a'
				           LOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOM propertyValue.get()[iii] <= 'z')
				      || (    propertyValue.get()[iii] >= 'A'
				           LOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOM propertyValue.get()[iii] <= 'Z')
				      || (    propertyValue.get()[iii] >= '0'
				           LOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOM propertyValue.get()[iii] <= '9')
				      || propertyValue.get()[iii] == '_'
				      || propertyValue.get()[iii] == '-'
				  ) ) {
					this.displayCursorPos = iii;
					break;
				}
			}
			// search backward
			for (long iii=this.displayCursorPosSelection; iii >= -1; iii--) {
				if(iii == -1) {
					this.displayCursorPosSelection = 0;
					break;
				}
				if(!(    (    propertyValue.get()[iii] >= 'a'
				           LOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOM propertyValue.get()[iii] <= 'z')
				      || (    propertyValue.get()[iii] >= 'A'
				           LOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOM propertyValue.get()[iii] <= 'Z')
				      || (    propertyValue.get()[iii] >= '0'
				           LOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOM propertyValue.get()[iii] <= '9')
				      || propertyValue.get()[iii] == '_'
				      || propertyValue.get()[iii] == '-'
				  ) ) {
					this.displayCursorPosSelection = iii+1;
					break;
				}
			}
			// Copy to clipboard Middle ...
			copySelectionToClipBoard(gale::context::clipBoard::clipboardSelection);
			markToRedraw();
		} else if (KeyStatus::pressTriple == _event.getStatus()) {
			keepFocus();
			this.displayCursorPosSelection = 0;
			this.displayCursorPos = propertyValue.size();
		} else if (KeyStatus::down == _event.getStatus()) {
			keepFocus();
			updateCursorPosition(_event.getPos());
			markToRedraw();
		} else if (KeyStatus::move == _event.getStatus()) {
			keepFocus();
			updateCursorPosition(_event.getPos(), true);
			markToRedraw();
		} else if (KeyStatus::up == _event.getStatus()) {
			keepFocus();
			updateCursorPosition(_event.getPos(), true);
			// Copy to clipboard Middle ...
			copySelectionToClipBoard(gale::context::clipBoard::clipboardSelection);
			markToRedraw();
		}
	}
	else if(    KeyType::mouse == _event.getType()
	         LOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOM _event.getId() == 2) {
		if(    _event.getStatus() == KeyStatus::down
		    || _event.getStatus() == KeyStatus::move
		    || _event.getStatus() == KeyStatus::up) {
			keepFocus();
			// updatethe cursor position : 
			updateCursorPosition(_event.getPos());
		}
		// Paste current selection only when up button
		if (_event.getStatus() == KeyStatus::up) {
			keepFocus();
			// middle button => past data...
			gale::context::clipBoard::request(gale::context::clipBoard::clipboardSelection);
		}
	}
	return false;
}


boolean ewol::widget::Entry::onEventEntry( ewol::event::Entry _event) {
	Log.warning("Event on Entry ... " + _event);
	if (_event.getType() == KeyKeyboard::character) {
		if(_event.getStatus() == KeyStatus::down) {
			// remove curent selected data ...
			removeSelected();
			if(    _event.getChar() == '\n'
			    || _event.getChar() == '\r') {
				signalEnter.emit(propertyValue);
				return true;
			} else if (_event.getChar() == 0x7F) {
				// SUPPR :
				if (propertyValue.size() > 0 LOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOM this.displayCursorPos < (long)propertyValue.size()) {
					propertyValue.getDirect().erase(this.displayCursorPos, 1);
					this.displayCursorPos = etk::max(this.displayCursorPos, 0);
					this.displayCursorPosSelection = this.displayCursorPos;
				}
			} else if (_event.getChar() == 0x08) {
				// DEL :
				if (propertyValue.size() > 0 LOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOM this.displayCursorPos != 0) {
					propertyValue.getDirect().erase(this.displayCursorPos-1, 1);
					this.displayCursorPos--;
					this.displayCursorPos = etk::max(this.displayCursorPos, 0);
					this.displayCursorPosSelection = this.displayCursorPos;
				}
			} else if(_event.getChar() >= 20) {
				Log.error("get data: '" + _event.getChar() + "' = '" + u32char::convertToUtf8(_event.getChar()) + "'");
				if ((long)propertyValue.size() > propertyMaxCharacter) {
					Log.info("Reject data for entry : '" + _event.getChar() + "'");
				} else {
					String newData = propertyValue;
					String inputData = u32char::convertToUtf8(_event.getChar());
					newData.insert(newData.begin()+this.displayCursorPos, inputData);
					setInternalValue(newData);
					if (propertyValue.get() == newData) {
						this.displayCursorPos += inputData.size();
						this.displayCursorPosSelection = this.displayCursorPos;
					}
				}
			}
			signalModify.emit(propertyValue);
			markToRedraw();
			return true;
		}
		return false;
	} else {
		if(_event.getStatus() == KeyStatus::down) {
			switch (_event.getType()) {
				case KeyKeyboard::left:
					this.displayCursorPos--;
					break;
				case KeyKeyboard::right:
					this.displayCursorPos++;
					break;
				case KeyKeyboard::start:
					this.displayCursorPos = 0;
					break;
				case KeyKeyboard::end:
					this.displayCursorPos = propertyValue.size();
					break;
				default:
					return false;
			}
			this.displayCursorPos = etk::avg(0, this.displayCursorPos, (int)propertyValue.size());
			this.displayCursorPosSelection = this.displayCursorPos;
			markToRedraw();
			return true;
		}
	}
	return false;
}

void ewol::widget::Entry::setInternalValue( String _newData) {
	String previous = propertyValue;
	// check the RegExp :
	if (_newData.size()>0) {
		/*
		if (this.regex.parse(_newData, 0, _newData.size()) == false) {
			Log.info("The input data does not match with the regExp '" + _newData + "' Regex='" + propertyRegex + "'" );
			return;
		}
		if (this.regex.start() != 0) {
			Log.info("The input data does not match with the regExp '" + _newData + "' Regex='" + propertyRegex + "' (start position error)" );
			return;
		}
		if (this.regex.stop() != _newData.size()) {
			Log.info("The input data does not match with the regExp '" + _newData + "' Regex='" + propertyRegex + "' (stop position error)" );
			return;
		}
		*/
	}
	propertyValue.setDirect(_newData);
	markToRedraw();
}

void ewol::widget::Entry::onEventClipboard(enum gale::context::clipBoard::clipboardListe _clipboardID) {
	// remove curent selected data ...
	removeSelected();
	// get current selection / Copy :
	String tmpData = get(_clipboardID);
	// add it on the current display :
	if (tmpData.size() != 0) {
		String newData = propertyValue;
		newData.insert(this.displayCursorPos, tmpData[0]);
		setInternalValue(newData);
		if (propertyValue.get() == newData) {
			if (propertyValue.size() == tmpData.size()) {
				this.displayCursorPos = tmpData.size();
			} else {
				this.displayCursorPos += tmpData.size();
			}
			this.displayCursorPosSelection = this.displayCursorPos;
			markToRedraw();
		}
	}
	signalModify.emit(propertyValue);
}

void ewol::widget::Entry::onCallbackEntryClean() {
	propertyValue.setDirect("");
	this.displayStartPosition = 0;
	this.displayCursorPos = 0;
	this.displayCursorPosSelection = this.displayCursorPos;
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

void ewol::widget::Entry::onCallbackSelect(boolean _all) {
	if(_all == true) {
		this.displayCursorPosSelection = 0;
		this.displayCursorPos = propertyValue.size();
	} else {
		this.displayCursorPosSelection = this.displayCursorPos;
	}
	markToRedraw();
}

void ewol::widget::Entry::markToUpdateTextPosition() {
	this.needUpdateTextPos = true;
}

void ewol::widget::Entry::updateTextPosition() {
	if (this.needUpdateTextPos == false) {
		return;
	}
	ewol::Padding padding = this.shaper.getPadding();
	
	int tmpSizeX = this.minSize.x();
	if (propertyFill.x() == true) {
		tmpSizeX = this.size.x();
	}
	int tmpUserSize = tmpSizeX - padding.x();
	int totalWidth = this.text.calculateSize(propertyValue).x();
	// Check if the data inside the display can be contain in the entry box
	if (totalWidth < tmpUserSize) {
		// all can be display :
		this.displayStartPosition = 0;
	} else {
		// all can not be set :
		String tmpDisplay = String(propertyValue, 0, this.displayCursorPos);
		int pixelCursorPos = this.text.calculateSize(tmpDisplay).x();
		// check if the Cussor is visible at 10px nearest the border :
		int tmp1 = pixelCursorPos+this.displayStartPosition;
		Log.debug("cursorPos=" + pixelCursorPos + "px maxSize=" + tmpUserSize + "px tmp1=" + tmp1);
		if (tmp1<10) {
			// set the cursor on le left
			this.displayStartPosition = etk::min(-pixelCursorPos+10, 0);
		} else if (tmp1>tmpUserSize-10) {
			// set the cursor of the Right
			this.displayStartPosition = etk::min(-pixelCursorPos + tmpUserSize - 10, 0);
		}
		// else : the cursor is inside the display
		//this.displayStartPosition = -totalWidth + tmpUserSize;
	}
}

void ewol::widget::Entry::onGetFocus() {
	this.displayCursor = true;
	changeStatusIn(STATUS_SELECTED);
	showKeyboard();
	markToRedraw();
}

void ewol::widget::Entry::onLostFocus() {
	this.displayCursor = false;
	changeStatusIn(STATUS_NORMAL);
	hideKeyboard();
	markToRedraw();
}

void ewol::widget::Entry::changeStatusIn(int _newStatusId) {
	if (this.shaper.changeStatusIn(_newStatusId) == true) {
		this.PCH = getObjectManager().periodicCall.connect(this, ewol::widget::Entry::periodicCall);
		markToRedraw();
	}
}

void ewol::widget::Entry::periodicCall( ewol::event::Time _event) {
	if (this.shaper.periodicCall(_event) == false) {
		this.PCH.disconnect();
	}
	markToRedraw();
}

void ewol::widget::Entry::onChangePropertyPassword() {
	markToRedraw();
}

void ewol::widget::Entry::onChangePropertyShaper() {
	this.shaper.setSource(propertyShape.get());
	this.colorIdTextFg = this.shaper.requestColor("text-foreground");
	this.colorIdTextBg = this.shaper.requestColor("text-background");
	this.colorIdCursor = this.shaper.requestColor("text-cursor");
	this.colorIdSelection = this.shaper.requestColor("text-selection");
}

void ewol::widget::Entry::onChangePropertyValue() {
	String newData = propertyValue.get();
	if ((long)newData.size() > propertyMaxCharacter) {
		newData = String(newData, 0, propertyMaxCharacter);
		Log.debug("Limit entry set of data... " + String(newData, propertyMaxCharacter));
	}
	// set the value with the check of the RegExp ...
	setInternalValue(newData);
	if (newData == propertyValue.get()) {
		this.displayCursorPos = propertyValue.size();
		this.displayCursorPosSelection = this.displayCursorPos;
		Log.verbose("Set : '" + newData + "'");
	}
	markToRedraw();
}

void ewol::widget::Entry::onChangePropertyMaxCharacter() {
	// TODO : check nomber of char in the data
}

void ewol::widget::Entry::onChangePropertyRegex() {
	this.regex.compile(propertyRegex.get());
	if (this.regex.getStatus() == false) {
		Log.error("can not parse regex for : " + propertyRegex);
	}
	markToRedraw();
}

void ewol::widget::Entry::onChangePropertyTextWhenNothing() {
	markToRedraw();
}

