package org.atriasoft.ewol.event;

import org.atriasoft.gale.key.KeyKeyboard;
import org.atriasoft.gale.key.KeySpecial;
import org.atriasoft.gale.key.KeyStatus;

public class EventEntry {
	private final KeySpecial specialKey; //!< input key status (prevent change in time..)
	private final KeyKeyboard type; //!< type of hardware event
	private final KeyStatus status; //!< status of hardware event
	private final Character unicodeData; //!< Unicode data (in some case)
	
	public EventEntry(final KeySpecial specialKey, final KeyKeyboard type, final KeyStatus status, final Character charValue) {
		this.type = type;
		this.status = status;
		this.specialKey = specialKey;
		this.unicodeData = charValue;
	}
	
	public Character getChar() {
		return this.unicodeData;
	}
	
	public KeySpecial getSpecialKey() {
		return this.specialKey;
	}
	
	public KeyStatus getStatus() {
		return this.status;
	}
	
	public KeyKeyboard getType() {
		return this.type;
	};
	
	@Override
	public String toString() {
		return "EventEntry [type=" + this.type + ", status=" + this.status + ", unicodeData=" + this.unicodeData + ", specialKey=" + this.specialKey + "]";
	}
}
