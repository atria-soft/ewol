package org.atriasoft.ewol.event;

import org.atriasoft.gale.key.KeyKeyboard;
import org.atriasoft.gale.key.KeySpecial;

public class EventShortCut{
	private final String message; //!< data link with the event
	private final KeySpecial specialKey; //!< special board key
	private final Character unicodeValue; //!< 0 if not used
	private final KeyKeyboard keyboardMoveValue; //!< ewol::EVENT_KB_MOVE_TYPE_NONE if not used
	public boolean isActive; //!< If true, we need to filter the up key of ascii element (not control)
	public EventShortCut(String message, KeySpecial specialKey, Character unicodeValue, KeyKeyboard keyboardMoveValue, boolean isActive) {
		this.message = message;
		this.specialKey = specialKey;
		this.unicodeValue = unicodeValue;
		this.keyboardMoveValue = keyboardMoveValue;
		this.isActive = isActive;
	}
	
	public String message() {
		return message;
	}
	
	public KeySpecial specialKey() {
		return specialKey;
	}
	
	public Character unicodeValue() {
		return unicodeValue;
	}
	
	public KeyKeyboard keyboardMoveValue() {
		return keyboardMoveValue;
	}
}
