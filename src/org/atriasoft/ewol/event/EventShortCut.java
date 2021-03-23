package org.atriasoft.ewol.event;

import org.atriasoft.gale.key.KeyKeyboard;
import org.atriasoft.gale.key.KeySpecial;

public class EventShortCut {
	public final String message; //!< data link with the event
	public final KeySpecial specialKey; //!< special board key
	public final Character unicodeValue; //!< 0 if not used
	public final KeyKeyboard keyboardMoveValue; //!< ewol::EVENT_KB_MOVE_TYPE_NONE if not used
	public boolean isActive; //!< If true, we need to filter the up key of ascii element (not control)
	
	public EventShortCut(final String message, final KeySpecial specialKey, final Character unicodeValue, final KeyKeyboard keyboardMoveValue, final boolean isActive) {
		super();
		this.message = message;
		this.specialKey = specialKey;
		this.unicodeValue = unicodeValue;
		this.keyboardMoveValue = keyboardMoveValue;
		this.isActive = isActive;
	}
}
